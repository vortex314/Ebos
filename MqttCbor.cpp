/*
 * MqttCbor.cpp
 *
 *  Created on: 3-jan.-2017
 *      Author: lieven2
 */
/*#include <LinkedList.h>
#include <LinkedList.cpp>*/
#include <MqttCbor.h>
#include <malloc.h>


/*
MqttCbor - events
- non-local dst message :
	- Request or Reply :
		- topic : add "dst/" S(dst) - where dst should be the hash of already available key
		- message to msg :
			+ src field = <local_device> + S(src) ,
			+ other fields follow naming convention depending on following types,
				. #field for UID types
				. $field for hex Bytes
				. @field for BASE64 fields
- event and public Object
	- Event src -> "event/<local_device>/" + S(src) + S(event)

- incoming MQTT message
	- topic-> dst : remove "dst/device/" ; H(topic)
	- message from msg
			. src field -> h() en store it
			. other fields use first char and payload to convert to h(field) & addKeyValue(h(field),type)
-
- outgoing reply add "dst/"
- ougoing event add "event/"


 */

#include "ebos_labels.h"

MqttCbor::MqttCbor(const char* name) :
    Actor(name), _topic(30), _message(300), _name(30),_mqttId(H("mqtt"))
{
    _actor = 0;
}

void MqttCbor::setMqttId(uid_t mqttId)
{
    _mqttId=mqttId;
}

void MqttCbor::setup()
{
    timeout(1);
    eb.onDst(id()).call(this);
    eb.onRemote().call(this, (MethodHandler) &MqttCbor::ebToMqtt);
//	eb.onEvent(H("Relay"), 0).call(this,
//	                                    (MethodHandler) &MqttCbor::ebToMqtt);
    eb.onEvent(_mqttId, H("published")).call(this,
            (MethodHandler) &MqttCbor::mqttToEb);
    eb.onEvent(_mqttId, H("disconnected")).call(this,
            (MethodHandler) &MqttCbor::onEvent);
    uid.add(labels,LABEL_COUNT);
}

//__________________________________________________________________________________________________
//
void MqttCbor::onEvent(Cbor& msg)
{
    Str willTopic(30);
    PT_BEGIN()
    ;
    goto CONNECTING;
DISCONNECTING: {
        while (true) {
            eb.request(_mqttId, H("disconnect"), id());
            eb.send();
            timeout(2000);
            
            PT_YIELD_UNTIL(timeout() || eb.isReplyCorrect(_mqttId,H("disconnect")) );
            if ( eb.isReplyCorrect(_mqttId,H("disconnect")) ) 
                goto CONNECTING;
        }
    }
CONNECTING : {
        while(true) {

            willTopic="event/";
            willTopic+=Sys::hostname();
            willTopic+="/system/alive";

            eb.request(_mqttId, H("connect"), id())
            .addKeyValue(H("clientId"),Sys::hostname())
            .addKeyValue(H("will_topic"),willTopic)
            .addKeyValue(H("will_message"),"false")
            .addKeyValue(H("keep_alive"),120)
            .addKeyValue(H("prefix"),Sys::hostname());
            eb.send();
            timeout(2000);

            PT_YIELD_UNTIL(timeout() || eb.isEvent(_mqttId, H("connected")) || eb.isReplyCorrect(_mqttId,H("connect")));
            if (!timeout()) {
                goto SUBSCRIBING;
            }
        }
    }
SUBSCRIBING: {
        _topic = "dst/";
        _topic += Sys::hostname();
        _topic += "/#";
        eb.request(_mqttId, H("subscribe"), id())
            .addKeyValue(H("topic"), _topic);
        eb.send();
        timeout(3000);
        
        PT_YIELD_UNTIL(
            eb.isReply(_mqttId, H("subscribe")) || timeout() || eb.isEvent(_mqttId, H("disconnected")));
        if (eb.isReplyCorrect(_mqttId, H("subscribe")) )
            goto SLEEPING;
        goto DISCONNECTING;
    }
SLEEPING: {
        while (true) {
            for (_actor = Actor::first(); _actor; _actor = _actor->next()) {
                _name = _actor->name();
                _topic = "event/";
                _topic += Sys::hostname();
                _topic += "/";
                _topic += _name;
                _topic += "/alive";
                _message.clear();
                _message.add(true);
                eb.request(_mqttId, H("publish"), id())
                    .addKeyValue(H("topic"), _topic)
                    .addKeyValue(H("message"), _message);
                eb.send();
                timeout(2000);
                
                PT_YIELD_UNTIL(eb.isReplyCorrect(_mqttId, H("publish")) || timeout());
                if (timeout())
                    goto DISCONNECTING;
                timeout(5000);
                
                PT_YIELD_UNTIL(timeout());
            }
        }
    }
    PT_END()
    ;
}

//__________________________________________________________________________________________________
//
bool MqttCbor::addTopic(Str& topic, Cbor& cbor, uid_t key)
{
    uid_t v;
    if (cbor.getKeyValue(key, v)) {
        if (topic.length())
            topic.append('/');
        const char* nm = uid.label(v);
        if (nm)
            topic.append(nm);
        else
            topic.append((uint64_t) v);
        return true;
    }
    return false;
}

void MqttCbor::cborToMqtt(Str& topic, Cbor& msg, Cbor& cbor)
{
    topic.clear();
    msg.clear();
    if (cbor.gotoKey(EB_REQUEST)  || cbor.gotoKey(EB_REPLY)) {
        topic="dst";
        addTopic(topic,cbor,EB_DST_DEVICE);
        addTopic(topic, cbor, EB_DST);
        if ( cbor.gotoKey(EB_REQUEST))
            addTopic(topic,cbor,EB_REQUEST);
        else
            addTopic(topic,cbor,EB_REPLY);
    } else if (cbor.gotoKey(EB_EVENT)) {
        topic="event/";
        topic += Sys::hostname();
        addTopic(topic, cbor, EB_SRC);
        addTopic(topic, cbor, EB_EVENT);
    }
    msg=cbor;
}

void MqttCbor::ebToMqtt(Cbor& msg)
{
    uid_t dst;
    if (msg.getKeyValue(EB_DST, dst) && dst == id())
        return;
    cborToMqtt(_topic, _message, msg);

    eb.request(id(), H("publish"), id()).addKeyValue(H("topic"), _topic).addKeyValue(
        H("message"), _message);
    eb.send();
}

//_______________________________________________________________________________________
//

int MqttCbor::nextHash(Str& str)
{
    Str field(30);
    while (str.hasData()) {
        uint8_t ch = str.read();
        if (ch == '/')
            break;
        field.append((char) ch);
    }
    if (field.length() == 0)
        return 0;
    uid_t hsh = uid.hash(field);	// generates new label if necessary
    return hsh;
}

void MqttCbor::mqttToEb(Cbor& msg)
{
    if (msg.getKeyValue(H("topic"), _topic)
        && msg.getKeyValue(H("message"), (Bytes&) _message)) {
        uid_t field[4]= {0,0,0,0};
        int i = 0;
        uid_t v;
        _topic.offset(0);
        while ((v = nextHash(_topic)) && i < 4) {
            field[i++] = v;
        }
        if ( field[1]==H(Sys::hostname())) {	// check device
            if ( Actor::findById(field[2])) {	// check actor
                eb.publish(_message);
            } else {
                WARN(" wrong actor ");
            }
        } else {
            WARN(" wrong device ");	//TODO could try CBOR
        }
    } else {
        WARN(" wrong mqtt layout ");
    }
}
