/*
 * Router.cpp
 *
 *  Created on: 3-jan.-2017
 *      Author: lieven2
 */
/*#include <LinkedList.h>
#include <LinkedList.cpp>*/
#include <Router.h>
#include <malloc.h>


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
 *
 * #define EB_DST H("#dst")
#define EB_SRC H("#src")
#define EB_REQUEST H("#request")
#define EB_REPLY H("#reply")
#define EB_EVENT H("#event")
#define EB_ERROR H("error")
#define EB_REGISTER H("register")
#define EB_SRC_DEVICE H("#src_device")
#define EB_DST_DEVICE	H("#dst_device")
H("state")
 * H("timeout"),
 * H("id")
 * H("line")
 * H("init")
 * H("state")
 * H("timeout")
 * H("id")
 * H("line")
 * H("error_msg")
 * H("Actor")
 * H("#from")

 */
Router::Router() :
	Actor("Router"), _topic(30), _message(300), _name(30)
{
	_actor = 0;
}

void Router::setup()
{
	timeout(20000);
	eb.onDst(id()).subscribe(this);
	eb.onRemote().subscribe(this, (MethodHandler) &Router::ebToMqtt);
//	eb.onEvent(H("Relay"), 0).subscribe(this,
//	                                    (MethodHandler) &Router::ebToMqtt);
	eb.onEvent(H("mqtt"), H("published")).subscribe(this,
	        (MethodHandler) &Router::mqttToEb);
	eb.onEvent(H("mqtt"), H("disconnected")).subscribe(this,
	        (MethodHandler) &Router::onEvent);
}
#define CNT 100
bool Router::addHeader(Json& json, Cbor& cbor, uid_t key)
{
	uid_t v;
	if (cbor.getKeyValue(key, v)) {
		json.addKey(uid.label(key));
		json.add(uid.label(v));
		return true;
	}
	return false;
}

bool Router::addTopic(Str& topic, Cbor& cbor, uid_t key)
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

bool Router::isHeaderField(uid_t key)
{
	if (key == EB_SRC || key == EB_DST || key == EB_REQUEST || key == EB_REPLY
	    || key == EB_EVENT)
		return true;
	return false;
}
int Router::nextHash(Str& str)
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

void Router::jsonToCbor(Cbor& cbor, Json& json)
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
					Bytes bytes(100);
					json.get(bytes);
					cbor.add(bytes);
				} else if (json.getType() == Json::JSON_STRING) {
					Str str(100);
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
//__________________________________________________________________________________________________
//
void Router::cborToMqtt(Str& topic, Json& json, Cbor& cbor)
{
	topic.clear();
	json.clear();
	if (cbor.gotoKey(EB_REQUEST) || cbor.gotoKey(EB_REPLY)) {
		topic="dst";
		addTopic(topic,cbor,EB_DST_DEVICE);
		addTopic(topic, cbor, EB_DST);
	}
	if (cbor.gotoKey(EB_EVENT)) {
		topic="event/";
		topic += Sys::hostname();
		addTopic(topic, cbor, EB_SRC);
		addTopic(topic, cbor, EB_EVENT);
	}
	Cbor::PackType type;
	//        Cbor::CborVariant variant;
	cbor.offset(0);
	uid_t key,value;
	json.addMap();

	json.addKey("length").add(cbor.length());
	_message.addKey("#src_device").add(Sys::hostname());

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
				type = cbor.tokenToString(json);
			}
		} else {
			json.add("expected int key");
		}
	}
	json.addBreak();

}
//__________________________________________________________________________________________________
//
void Router::onEvent(Cbor& msg)
{
	Str willTopic(30);
	PT_BEGIN()
	;
	goto CONNECTING;
DISCONNECTING: {
		while (true) {
			eb.request(H("mqtt"), H("disconnect"), id());
			eb.send();
			timeout(2000);
			PT_YIELD_UNTIL(timeout() );
		}
	}
CONNECTING : {
		while(true) {

			willTopic="event/";
			willTopic+=Sys::hostname();
			willTopic+="/system/alive";

			eb.request(H("mqtt"), H("connect"), id())
			.addKeyValue(H("clientId"),Sys::hostname())
			.addKeyValue(H("will_topic"),willTopic)
			.addKeyValue(H("will_message"),"false")
			.addKeyValue(H("keep_alive"),120)
			.addKeyValue(H("prefix"),Sys::hostname());
			eb.send();
			timeout(2000);

			PT_YIELD_UNTIL(timeout() || eb.isEvent(H("mqtt"), H("connected")) || eb.isReplyCorrect(H("mqtt"),H("connect")));
			if (!timeout()) {
				goto SUBSCRIBING;
			}
		}
	}
SUBSCRIBING: {
		_topic = "dst/";
		_topic += Sys::hostname();
		_topic += "/#";
		eb.request(H("mqtt"), H("subscribe"), H("Router")).addKeyValue(
		    H("topic"), _topic);
		eb.send();
		timeout(3000);
		PT_YIELD_UNTIL(
		    eb.isReply(H("mqtt"), H("subscribe")) || timeout() || eb.isEvent(H("mqtt"), H("disconnected")));
		if (eb.isReplyCorrect(H("mqtt"), H("subscribe")) )
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
				eb.request(H("mqtt"), H("publish"), H("Router")).addKeyValue(
				    H("topic"), _topic).addKeyValue(H("message"), _message);
				eb.send();
				timeout(2000);
				PT_YIELD_UNTIL(
				    eb.isReplyCorrect(H("mqtt"), H("publish"))
				    || timeout());
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
void Router::ebToMqtt(Cbor& msg)
{
	uid_t dst;
	if (msg.getKeyValue(EB_DST, dst) && dst == H("mqtt"))
		return;
	cborToMqtt(_topic, _message, msg);

	eb.request(H("mqtt"), H("publish"), H("Router")).addKeyValue(H("topic"), _topic).addKeyValue(
	    H("message"), _message);
	eb.send();
}

void Router::mqttToEb(Cbor& msg)
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
				Cbor& cbor = eb.empty();
				cbor.addKeyValue(EB_DST,field[2]);
				jsonToCbor(cbor, _message);
				eb.send();
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
