/*
 * Router.cpp
 *
 *  Created on: 3-jan.-2017
 *      Author: lieven2
 */
/*#include <LinkedList.h>
#include <MqttJson.h>
#include <malloc.h>
#include <LinkedList.cpp>*/

#define TOPIC_LENGTH 40
/*
Router - events
- non-local dst message :
        - Request or Reply :
                - topic : add "dst/" S(dst) - where dst should be the hash of
already available key
                - message to json :
                        + src field = <local_device> + S(src) ,
                        + other fields follow naming convention depending on
following types,
                                . #field for UID types
                                . $field for hex Bytes
                                . @field for BASE64 fields
- event and public Object
        - Event src -> "event/<local_device>/" + S(src) + S(event)

- incoming MQTT message
        - topic-> dst : remove "dst/device/" ; H(topic)
        - message from json
                        . src field -> h() en store it
                        . other fields use first char and payload to convert to
h(field) & addKeyValue(h(field),type)
-
- outgoing reply add "dst/"
- ougoing event add "event/"


 */

#include <MqttJson.h>
#include <Property.h>
#include "ebos_labels.h"

static uint32_t rcvCounter = 0;

MqttJson::MqttJson(const char* name, uint32_t jsonSize)
    : Actor(name),
      _topic(TOPIC_LENGTH),
      _message(jsonSize)  //, _name(30)
{
  _actor = 0;
}

void MqttJson::setMqttId(uid_t mqttId) { _mqttId = mqttId; }
//----------------------------------------------------------------------------------
void MqttJson::setup() {
  timeout(1);
  eb.onDst(id()).call(this);
  eb.onRemote().call(this, (MethodHandler)&MqttJson::ebToMqtt);
  //	eb.onEvent(H("Relay"), 0).call(this,
  //	                                    (MethodHandler) &Router::ebToMqtt);
  eb.onEvent(_mqttId, H("published"))
      .call(this, (MethodHandler)&MqttJson::mqttToEb);
  eb.onEvent(_mqttId, EB_UID_ANY).call(this);
  eb.onEvent(EB_UID_IGNORE, EB_UID_ANY)
      .call(this, (MethodHandler)&MqttJson::sendPublicEvents);

  //    eb.onEvent(0,1).call(this,(MethodHandler) &MqttJson::sendPublicEvents);
  uid.add(labels, LABEL_COUNT);
  Property<uint32_t>::build(rcvCounter, id(), H("rcv"), 2000);
}
//----------------------------------------------------------------------------------
void MqttJson::sendPublicEvents(Cbor& msg) {
  bool isPublic;
  if (msg.getKeyValue(H("public"), isPublic) &&
      isPublic == true) {  // src actor is public
    this->ebToMqtt(msg);
  }
}
//----------------------------------------------------------------------------------

#define CNT 100
bool MqttJson::addHeader(Json& json, Cbor& cbor, uid_t key) {
  uid_t v;
  if (cbor.getKeyValue(key, v)) {
    json.addKey(uid.label(key));
    json.add(uid.label(v));
    return true;
  }
  return false;
}
//----------------------------------------------------------------------------------

bool MqttJson::addTopic(Str& topic, Cbor& cbor, uid_t key) {
  uid_t v;
  if (cbor.getKeyValue(key, v)) {
    const char* nm = uid.label(v);
    if (nm)
      topic.append(nm);
    else
      topic.append((uint64_t)v);
    return true;
  }
  return false;
}
//----------------------------------------------------------------------------------

bool MqttJson::isHeaderField(uid_t key) {
  if (key == EB_SRC || key == EB_DST || key == EB_REQUEST || key == EB_REPLY ||
      key == EB_EVENT)
    return true;
  return false;
}
//----------------------------------------------------------------------------------

int MqttJson::nextHash(Str& str) {
  Str field(30);
  while (str.hasData()) {
    uint8_t ch = str.read();
    if (ch == '/') break;
    field.append((char)ch);
  }
  if (field.length() == 0) return 0;

  uid_t hsh = uid.create(field);  // generates new label if necessary
  //  INFO(" %s : %d : %s ",field.c_str(),hsh,uid.label(hsh));

  return hsh;
}
//----------------------------------------------------------------------------------
#include <cJSON.h>

void MqttJson::jsonObjectToCbor(Cbor& cbor, cJSON* object) {
  cJSON* field = object->child;
  while (field) {
    uid_t ui = uid.create(field->string);
    // char* label = field->string;
    jsonToCbor(cbor, ui, field);
    /*    if (label[0] == '$' && field->type == cJSON_String) {
          Bytes bytes(1024);
          Str str(0);
          str.map((uint8_t*)field->valuestring, strlen(field->valuestring));
          str.parseHex(bytes);
          cbor.addKeyValue(ui, bytes);
        } else if (label[0] == '%' && field->type == cJSON_String) {
          cbor.addKeyValue(ui, uid.create(field->valuestring));
        } else if (field->type == cJSON_Number) {
          cbor.addKeyValue(ui, field->valuedouble);
        } else if (field->type == cJSON_False) {
          cbor.addKeyValue(ui, false);
        } else if (field->type == cJSON_True) {
          cbor.addKeyValue(ui, true);
        } else if (field->type == cJSON_String) {
          Str str(0);
          str.map((uint8_t*)field->valuestring, strlen(field->valuestring));
          cbor.addKeyValue(ui, str);
        } else {
        }*/
    field = field->next;
  }
}

void MqttJson::jsonToCbor(Cbor& cbor, uid_t ui, cJSON* field) {
  const char* label = uid.label(ui);
  if (label[0] == '$' && field->type == cJSON_String) {
    Bytes bytes(1024);
    Str str(0);
    str.map((uint8_t*)field->valuestring, strlen(field->valuestring));
    str.parseHex(bytes);
    cbor.addKeyValue(ui, bytes);
  } else if (label[0] == '%' && field->type == cJSON_String) {
    cbor.addKeyValue(ui, uid.create(field->valuestring));
  } else if (field->type == cJSON_Number) {
    cbor.addKeyValue(ui, field->valuedouble);
  } else if (field->type == cJSON_False) {
    cbor.addKeyValue(ui, false);
  } else if (field->type == cJSON_True) {
    cbor.addKeyValue(ui, true);
  } else if (field->type == cJSON_String) {
    Str str(0);
    str.map((uint8_t*)field->valuestring, strlen(field->valuestring));
    cbor.addKeyValue(ui, str);
  } else {
    WARN(" unknown combination json %d , %s ", field->type, label);
  }
}

//------------------------------------------------------------------------------------------------
//

uid_t MqttJson::getRemoteSrcUid(Str& topic) {
  int pos[] = {0, 0, 0};
  int posIndex = 0;
  int index = 0;
  topic.offset(0);
  while (topic.hasData()) {
    char ch = topic.read();
    if (ch == '/') {
      pos[posIndex++] = index;
      if (posIndex == 3) break;
    }
    index++;
  }
  if (pos[2] == 0) return 0;
  Str srcService(40);
  for (int i = pos[0] + 1; i < pos[2]; i++) {
    srcService.write(topic.peek(i));
  }
  return uid.create(srcService);
}

void MqttJson::mqttToEb(Cbor& msg) {
    if (msg.getKeyValue(H("topic"), _topic) &&
      msg.getKeyValue(H("message"), (Bytes&)_message)) {
    uid_t field[4] = {0, 0, 0, 0};  // dst/device/service/property
    int i = 0;
    uid_t v;
    _topic.offset(0);
    while ((v = nextHash(_topic)) && i < 4) {
      field[i++] = v;
    }
    cJSON* json = cJSON_Parse(_message.c_str());
    if (json == 0) json = cJSON_CreateString(_message.c_str());
    //      INFO(" fields : %d %d %d %d ", field[0], field[1], field[2],
    //      field[3]);
    //      INFO(" fields : %s %s %s %s ", uid.label(field[0]),
    //      uid.label(field[1]),
    //           uid.label(field[2]), uid.label(field[3]));

    if ((field[0] == H("dst")) && Actor::findById(field[2])) {
      if (json->type == cJSON_Object) {
        rcvCounter++;
        Cbor& cbor = eb.empty()
                         .addKeyValue(EB_DST, field[2])
                         .addKeyValue(EB_REQUEST, field[3]);
        jsonObjectToCbor(cbor, json);
        eb.send();
      } else {
        Cbor& cbor = eb.empty()
                         .addKeyValue(EB_DST, field[2])
                         .addKeyValue(EB_REQUEST, H("set"));
        jsonToCbor(cbor, field[3], json);
        eb.send();
      }

    } else if (field[0] == H("src")) {
      // extract field 1 and field 2 for src address
      uid_t srcUid = getRemoteSrcUid(_topic);
      Cbor& cbor = eb.event(srcUid, field[3]);
      jsonToCbor(cbor, field[3], json);
      eb.send();

    } else {
      WARN(" wrong actor ");
    }
    cJSON_Delete(json);
  } else {
    WARN(" wrong mqtt layout ");
  }
}

//__________________________________________________________________________________________________
//

cJSON* cborToJson(const char* name, Cbor& cbor) {
  cJSON* field = 0;
  Cbor::PackType type;
  cbor.peekToken(type);
  if (type == Cbor::P_PINT && name[0] == '%') {
    int value;
    cbor.get(value);
    field = cJSON_CreateString(uid.label(value));
  } else if (type == Cbor::P_BYTES &&
             name[0] == '$') {  // default cbor behaviour
    Str str(1024);
    Bytes bytes(1024);
    cbor.get(bytes);
    str.appendHex(bytes);
    field = cJSON_CreateString(str.c_str());
  } else if (type == Cbor::P_DOUBLE) {
    double d;
    cbor.get(d);
    field = cJSON_CreateNumber(d);
  } else if (type == Cbor::P_BOOL) {
    bool b;
    cbor.get(b);
    field = cJSON_CreateBool(b);
  } else if (type == Cbor::P_PINT) {
    uint64_t d;
    cbor.get(d);
    field = cJSON_CreateNumber(d);
  } else if (type == Cbor::P_NINT) {
    int d;
    cbor.get(d);
    field = cJSON_CreateNumber(d);
  } else if (type == Cbor::P_STRING) {
    Str str(1024);
    cbor.get(str);
    field = cJSON_CreateString(str.c_str());
  }
  return field;
}
//=================================================================================================
void MqttJson::addMessageData(Str& json, Cbor& cbor) {
  //	Cbor::PackType type;
  //        Cbor::CborVariant variant;
  cbor.offset(0);
  cJSON* data = 0;
  uid_t key;
  cbor.getKeyValue(EB_EVENT, key);
  const char* label = uid.label(key);
  //    json.addKey("length").add(cbor.length());
  if (cbor.gotoKey(key)) {
    data = cborToJson(label, cbor);
  }
  if (data) {
    char* buffer = cJSON_PrintUnformatted(data);
    json = buffer;
    cJSON_Delete(data);
    free(buffer);
  }
}

//__________________________________________________________________________________________________
//
void MqttJson::addMessageObject(Str& json, Cbor& cbor) {
  //	Cbor::PackType type;
  //        Cbor::CborVariant variant;
  cbor.offset(0);
  uid_t key;
  cJSON* object = cJSON_CreateObject();

  //    json.addKey("length").add(cbor.length());

  while (cbor.hasData()) {
    if (cbor.get(key)) {
      const char* name = uid.label(key);
      if (key == EB_SRC) {  // extend src with device name
        uid_t src;
        cbor.get(src);
        Str srcLabel(100);
        srcLabel = Sys::hostname();
        srcLabel += "/";
        srcLabel += uid.label(src);
        cJSON_AddItemToObject(object, name,
                              cJSON_CreateString(srcLabel.c_str()));
      } else {
        cJSON_AddItemToObject(object, name, cborToJson(name, cbor));
      }
    } else {
      WARN("expected int key");
    }
  }
  char* print = cJSON_PrintUnformatted(object);
  cJSON_Delete(object);
  json = print;
  free(print);
}

//__________________________________________________________________________________________________
//
void MqttJson::cborToMqtt(Str& topic, Str& message, Cbor& cbor) {
  topic.clear();
  message.clear();

  if (cbor.gotoKey(EB_REQUEST) || cbor.gotoKey(EB_REPLY)) {
    topic = "dst/";
    addTopic(topic, cbor, EB_DST);
    addMessageObject(message, cbor);
  } else if (cbor.gotoKey(EB_EVENT)) {
    topic = "src/";
    topic += Sys::hostname();
    topic += "/";
    addTopic(topic, cbor, EB_SRC);
    topic += "/";
    addTopic(topic, cbor, EB_EVENT);
    addMessageData(message, cbor);
    //        cbor.getKeyValue(H("data"),json);
  }
}
//--------------------------------------------------------------------------------------------------
//

void MqttJson::ebToMqtt(Cbor& msg) {
  uid_t dst;
  if (msg.getKeyValue(EB_DST, dst) && dst == _mqttId) return;
  cborToMqtt(_topic, _message, msg);

  eb.request(_mqttId, H("publish"), id())
      .addKeyValue(H("topic"), _topic)
      .addKeyValue(H("message"), _message);
  eb.send();
}
//__________________________________________________________________________________________________
//
void MqttJson::onEvent(Cbor& msg) {
  Str willTopic(TOPIC_LENGTH);

  PT_BEGIN();
  goto WAIT_CONNECT;
DISCONNECTING : {
  while (true) {
    timeout(2000);
    PT_YIELD_UNTIL(timeout());
    eb.request(_mqttId, H("disconnect"), id());
    eb.send();
    timeout(2000);
    PT_YIELD_UNTIL(timeout() || eb.isReplyCorrect(_mqttId, H("disconnect")));
    if (eb.isReplyCorrect(_mqttId, H("disconnect"))) goto WAIT_CONNECT;
  }
}
WAIT_CONNECT : {
  timeout(UINT32_MAX);
  PT_YIELD_UNTIL(eb.isEvent(_mqttId, H("connected")));
  goto CONNECTED;
}

CONNECTED : {
  while (true) {
    _topic = "dst/";
    _topic += Sys::hostname();
    _topic += "/#";
    eb.request(_mqttId, H("subscribe"), id()).addKeyValue(H("topic"), _topic);
    eb.send();
    _topic = "src/global/clock/epoch";
    eb.request(_mqttId, H("subscribe"), id()).addKeyValue(H("topic"), _topic);
    eb.send();
    timeout(3000);
    PT_YIELD_UNTIL(eb.isReply(_mqttId, H("subscribe")) || timeout() ||
                   eb.isEvent(_mqttId, H("disconnected")));
    if (eb.isReplyCorrect(_mqttId, H("subscribe"))) {
      goto ALIVE;
    }
    goto DISCONNECTING;
  }
}

ALIVE : {
  while (true) {
    timeout(UINT32_MAX);
    PT_YIELD_UNTIL(eb.isEvent(_mqttId, H("disconnected")));
    if (eb.isEvent(_mqttId, H("disconnected"))) {
      goto WAIT_CONNECT;
    }
  }
}
  PT_END();
}
