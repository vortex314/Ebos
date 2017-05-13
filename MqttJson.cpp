/*
 * Router.cpp
 *
 *  Created on: 3-jan.-2017
 *      Author: lieven2
 */
/*#include <LinkedList.h>
#include <LinkedList.cpp>*/
#include <MqttJson.h>
#include <malloc.h>


#define TOPIC_LENGTH 40
/*
Router - events
- non-local dst message :
	- Request or Reply :
		- topic : add "dst/" S(dst) - where dst should be the hash of already available key
		- message to json :
			+ src field = <local_device> + S(src) ,
			+ other fields follow naming convention depending on following types,
				. #field for UID types
				. $field for hex Bytes
				. @field for BASE64 fields
- event and public Object
	- Event src -> "event/<local_device>/" + S(src) + S(event)

- incoming MQTT message
	- topic-> dst : remove "dst/device/" ; H(topic)
	- message from json
			. src field -> h() en store it
			. other fields use first char and payload to convert to h(field) & addKeyValue(h(field),type)
-
- outgoing reply add "dst/"
- ougoing event add "event/"


 */

#include "ebos_labels.h"

MqttJson::MqttJson(const char* name,uint32_t jsonSize) :
    Actor(name), _topic(TOPIC_LENGTH), _message(jsonSize)//, _name(30)
{
    _actor = 0;
}

void MqttJson::setMqttId(uid_t mqttId)
{
    _mqttId=mqttId;
}
//----------------------------------------------------------------------------------
void MqttJson::setup()
{
    timeout(1);
    eb.onDst(id()).call(this);
    eb.onRemote().call(this, (MethodHandler) &MqttJson::ebToMqtt);
//	eb.onEvent(H("Relay"), 0).call(this,
//	                                    (MethodHandler) &Router::ebToMqtt);
    eb.onEvent(_mqttId, H("published")).call(this,
            (MethodHandler) &MqttJson::mqttToEb);
    eb.onEvent(_mqttId, H("disconnected")).call(this,
            (MethodHandler) &MqttJson::onEvent);
    eb.onEvent(EB_UID_IGNORE,EB_UID_ANY).call(this,(MethodHandler) &MqttJson::sendPublicEvents);

//    eb.onEvent(0,1).call(this,(MethodHandler) &MqttJson::sendPublicEvents);
    uid.add(labels,LABEL_COUNT);
}
//----------------------------------------------------------------------------------
void MqttJson::sendPublicEvents(Cbor& msg)
{
    bool isPublic;
    if ( msg.getKeyValue(H("public"),isPublic) && isPublic==true) {  // src actor is public
        this->ebToMqtt(msg);
    }
}
//----------------------------------------------------------------------------------

#define CNT 100
bool MqttJson::addHeader(Json& json, Cbor& cbor, uid_t key)
{
    uid_t v;
    if (cbor.getKeyValue(key, v)) {
        json.addKey(uid.label(key));
        json.add(uid.label(v));
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------------

bool MqttJson::addTopic(Str& topic, Cbor& cbor, uid_t key)
{
    uid_t v;
    if (cbor.getKeyValue(key, v)) {
        const char* nm = uid.label(v);
        if (nm)
            topic.append(nm);
        else
            topic.append((uint64_t) v);
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------------

bool MqttJson::isHeaderField(uid_t key)
{
    if (key == EB_SRC || key == EB_DST || key == EB_REQUEST || key == EB_REPLY
        || key == EB_EVENT)
        return true;
    return false;
}
//----------------------------------------------------------------------------------

int MqttJson::nextHash(Str& str)
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
//    LOGF(" %s : %d %d",field.c_str(),hsh,H("event"));
    return hsh;
}
//----------------------------------------------------------------------------------

void MqttJson::jsonToCbor(Cbor& cbor, Json& json)
{
    if (json.parse() != E_OK) {
        WARN(" invalid JSON ");
        return;
    }
    Str key(30), value(30);
    json.rewind();
    if ( json.getMap()) {
        while (true) {
            if (json.get(key)) {
                uid_t hkey=uid.hash(key);
                cbor.addKey(hkey);
                if (key.startsWith("#")) { // ENUM hash field
                    json.get(value);
                    cbor.add(uid.hash(value));
                } else if (key.startsWith("$")) { // HEX string field
                    Bytes bytes(1024);
                    json.get(bytes);
                    cbor.add(bytes);
                } else if (json.getType() == Json::JSON_STRING) {
                    Str str(1024);
                    json.get(str);
                    cbor.add(str);
                } else if (json.getType() == Json::JSON_NUMBER) {
                    int64_t d;
                    json.get(d);
                    cbor.add(d);
                } else if (json.getType() == Json::JSON_BOOL) {
                    bool flag;
                    json.get(flag);
                    cbor.add(flag);
                } else {
                    cbor.addNull(); // just to avoid map index lost
                }
            } else
                break;
        }
    } else {
        WARN(" no JSON OBject found ");
    }
}
//------------------------------------------------------------------------------------------------
//

void MqttJson::mqttToEb(Cbor& msg)
{
    if (msg.getKeyValue(H("topic"), _topic)
        && msg.getKeyValue(H("message"), (Bytes&) _message)) {
        uid_t field[4]= {0,0,0,0};
        int i = 0;
        uid_t v;
        _topic.offset(0);
        while ((v = nextHash(_topic)) && i < 3) {
            field[i++] = v;
        }
        if ( Actor::findById(field[1]) || ( field[0]==H("src")) ) {	// check actor
            Cbor& cbor = eb.empty();
            if ( field[0]==H("dst")) {
                cbor.addKeyValue(EB_DST,field[1]);

            } else if ( field[0]==H("src")) {
                cbor.addKeyValue(EB_SRC,field[1]);
                cbor.addKeyValue(EB_EVENT,field[2]);
            }
            jsonToCbor(cbor, _message);
            eb.send();
        } else {
            WARN(" wrong actor ");
        }
    } else {
        WARN(" wrong mqtt layout ");
    }
}


//__________________________________________________________________________________________________
//
void MqttJson::addData(Json& json,Cbor& cbor)
{
    //	Cbor::PackType type;
//        Cbor::CborVariant variant;
    cbor.offset(0);
    uid_t key,value;


//    json.addKey("length").add(cbor.length());
    if ( cbor.gotoKey(H("data"))) {
        cbor.tokenToString(json);
    } else if ( cbor.gotoKey(H("$data"))) {
        cbor.tokenToString(json);
    } else if ( cbor.gotoKey(H("#data"))) {
        if(cbor.get(value)) {
            if (uid.label(value)) {
                json.add(uid.label(value));
            } else {
                json.add("enum no label");
            }
        } else {
            json.add(" expected enum ");
        }
    } else if ( cbor.gotoKey(H("@data"))) {
        json.add(" Base64 data top implement ");
    } else {
        json.add(" no data field ");
    }
}

//__________________________________________________________________________________________________
//
void MqttJson::addMessageData(Json& json,Cbor& cbor)
{
    //	Cbor::PackType type;
//        Cbor::CborVariant variant;
    cbor.offset(0);
    uid_t key,value;


//    json.addKey("length").add(cbor.length());

    while (cbor.hasData()) {
        if (cbor.get(key)) {
            const char* name = uid.label(key);
            json.addKey(name);
            if (name[0]=='#' ) {
                if(cbor.get(value)) {
                    if (uid.label(value)) {
                        json.add(uid.label(value));
                    } else {
                        json.add("enum no label");
                    }
                } else {
                    json.add(" expected enum ");
                }

            } else if ( name[0]=='@') { // add bytes in Base64
                json.add(" @ not implemented yet ");
                cbor.skipToken();
            } else {// default cbor behaviour
                json.addComma();
                cbor.tokenToString(json);
            }
        } else {
            json.add("expected int key");
        }
    }

}

//__________________________________________________________________________________________________
//
void MqttJson::cborToMqtt(Str& topic, Json& json, Cbor& cbor)
{
    topic.clear();
    json.clear();

    if (cbor.gotoKey(EB_REQUEST) || cbor.gotoKey(EB_REPLY)) {
        topic="dst/";
        addTopic(topic, cbor, EB_DST);
        json.addMap();
        addMessageData(json,cbor);
        json.addBreak();
    } else if (cbor.gotoKey(EB_EVENT)) {
        topic="src/";
        addTopic(topic, cbor, EB_SRC);
        topic += "/";
        addTopic(topic, cbor, EB_EVENT);
        addData(json,cbor);
//        cbor.getKeyValue(H("data"),json);
    }
}
//--------------------------------------------------------------------------------------------------
//
void MqttJson::ebToMqtt(Cbor& msg)
{
    uid_t dst,uid;
    if (msg.getKeyValue(EB_DST, dst) && dst == _mqttId )
        return;
    cborToMqtt(_topic, _message, msg);

    eb.request(_mqttId, H("publish"), id()).addKeyValue(H("topic"), _topic).addKeyValue(H("message"), _message);
    eb.send();
}
//__________________________________________________________________________________________________
//
void MqttJson::onEvent(Cbor& msg)
{

    Str willTopic(TOPIC_LENGTH);
    static Actor* currentActor;
    PT_BEGIN()
    ;
    goto CONNECTING;
DISCONNECTING: {
        while (true) {
            timeout(2000);
            PT_YIELD_UNTIL(timeout() );
            eb.request(_mqttId, H("disconnect"), id());
            eb.send();
            timeout(2000);
            PT_YIELD_UNTIL(timeout() || eb.isReplyCorrect(_mqttId,H("disconnect")) );
            if ( eb.isReplyCorrect(_mqttId,H("disconnect")) ) goto CONNECTING;
        }
    }
CONNECTING : {
        while(true) {

            willTopic="src/";
            willTopic+=Sys::hostname();
            willTopic+=".system/alive";

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
                currentActor = Actor::first();
                goto SUBSCRIBE_REQUEST;
            }
        }
    }
SUBSCRIBE_REQUEST: {
        while ( currentActor != 0 ) {
            _topic = "dst/";
            _topic += currentActor->name();
            eb.request(_mqttId, H("subscribe"), id()).addKeyValue(H("topic"), _topic);
            eb.send();
            timeout(3000);
            PT_YIELD_UNTIL(
                eb.isReply(_mqttId, H("subscribe")) || timeout() || eb.isEvent(_mqttId, H("disconnected")));
            if (!eb.isReplyCorrect(_mqttId, H("subscribe")) ) {
                goto DISCONNECTING;
            }
            currentActor = currentActor->next();
        }
        currentActor = Actor::first();
        goto ALIVE;
    }

ALIVE: {
        while ( currentActor != 0 ) {
            _topic = "src/";
            _topic += currentActor->name();
            _topic += "/alive";
            eb.request(_mqttId, H("publish"), id()).addKeyValue(H("topic"),_topic).addKeyValue(H("message"), "true");
            eb.send();
            timeout(3000);
            PT_YIELD_UNTIL(
                eb.isReply(_mqttId, H("publish")) || timeout() || eb.isEvent(_mqttId, H("disconnected")));
            if (!eb.isReplyCorrect(_mqttId, H("publish")) ) {
                goto DISCONNECTING;
            }
            timeout(2000);
            PT_YIELD_UNTIL(timeout() );
 //           currentActor = currentActor->next();
        }
        currentActor = Actor::first();
        goto ALIVE;
    }
    PT_END()
    ;
}
