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
#include "ebos_labels.h"

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

void MqttJson::jsonToCbor(Cbor& cbor, cJSON* object) {
  cJSON* field = object->child;
  while (field) {
    uid_t ui = uid.create(field->string);
    char* label = field->string;
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
    }
    field = field->next;
  }
}

  void MqttJson::mqttToCbor(Cbor & cbor, uid_t ui, Json & json) {
    const char* label = uid.label(ui);
    cJSON* field = cJSON_Parse(json.c_str());
    //  INFO(" label :%s json:%s cjson x%X",label,json.c_str(),field);
    if (field) {
      //    INFO(" cjson.type : %d ",field->type);
    }
    // if no valid type found assume string
    if (field == 0) field = cJSON_CreateString(json.c_str());

    if (field->type == cJSON_Object) {
      cbor.addKeyValue(EB_REQUEST, ui);
      jsonToCbor(cbor, field);
    } else {
      cbor.addKeyValue(EB_REQUEST, H("set"));
      if (label[0] == '$' && field->type == cJSON_String) {
        Bytes bytes(1024);
        Str str(0);
        str.map((uint8_t*)field->valuestring, strlen(field->valuestring));
        str.parseHex(bytes);
        cbor.addKeyValue(ui, bytes);
      } else if (label[0] == '%' && field->type == cJSON_String) {
        cbor.addKeyValue(ui, H(field->valuestring));
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
    cJSON_Delete(field);
    return;
  }
  //------------------------------------------------------------------------------------------------
  //

  void MqttJson::mqttToEb(Cbor & msg) {
    if (msg.getKeyValue(H("topic"), _topic) &&
        msg.getKeyValue(H("message"), (Bytes&)_message)) {
      uid_t field[4] = {0, 0, 0, 0};  // dst/device/service/property
      int i = 0;
      uid_t v;
      _topic.offset(0);
      while ((v = nextHash(_topic)) && i < 4) {
        field[i++] = v;
      }
//      INFO(" fields : %d %d %d %d ", field[0], field[1], field[2], field[3]);
//      INFO(" fields : %s %s %s %s ", uid.label(field[0]), uid.label(field[1]),
//           uid.label(field[2]), uid.label(field[3]));

      if ((field[0] == H("dst") && (field[1] == H(Sys::hostname()))) &&
          Actor::findById(field[2])) {
        Cbor& cbor = eb.empty();
        cbor.addKeyValue(EB_DST, field[2]);
        mqttToCbor(cbor, field[3], _message);
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
  void MqttJson::addData(Json & json, Cbor & cbor) {
    //	Cbor::PackType type;
    //        Cbor::CborVariant variant;
    cbor.offset(0);
    uid_t value;

    //    json.addKey("length").add(cbor.length());
    if (cbor.gotoKey(H("data"))) {
      cbor.tokenToString(json);
    } else if (cbor.gotoKey(H("$data"))) {
      cbor.tokenToString(json);
    } else if (cbor.gotoKey(H("#data"))) {
      if (cbor.get(value)) {
        if (uid.label(value)) {
          json.add(uid.label(value));
        } else {
          json.add("enum no label");
        }
      } else {
        json.add(" expected enum ");
      }
    } else if (cbor.gotoKey(H("@data"))) {
      json.add(" Base64 data top implement ");
    } else {
      json.add(" no data field ");
    }
  }

  //__________________________________________________________________________________________________
  //
  void MqttJson::addMessageData(Json & json, Cbor & cbor) {
    //	Cbor::PackType type;
    //        Cbor::CborVariant variant;
    cbor.offset(0);
    uid_t key, value;

    //    json.addKey("length").add(cbor.length());

    while (cbor.hasData()) {
      if (cbor.get(key)) {
        const char* name = uid.label(key);
        json.addKey(name);
        if (name[0] == '%') {
          if (cbor.get(value)) {
            if (uid.label(value)) {
              json.add(uid.label(value));
            } else {
              json.add("enum no label");
            }
          } else {
            json.add(" expected enum ");
          }

        } else if (name[0] == '@') {  // add bytes in Base64
          json.add(" @ not implemented yet ");
          cbor.skipToken();
        } else {  // default cbor behaviour
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
  void MqttJson::cborToMqtt(Str & topic, Json & json, Cbor & cbor) {
    topic.clear();
    json.clear();

    if (cbor.gotoKey(EB_REQUEST) || cbor.gotoKey(EB_REPLY)) {
      topic = "dst/";
      addTopic(topic, cbor, EB_DST);
      json.addMap();
      addMessageData(json, cbor);
      json.addBreak();
    } else if (cbor.gotoKey(EB_EVENT)) {
      topic = "src/";
      addTopic(topic, cbor, EB_SRC);
      topic += "/";
      addTopic(topic, cbor, EB_EVENT);
      addData(json, cbor);
      //        cbor.getKeyValue(H("data"),json);
    }
  }
  //--------------------------------------------------------------------------------------------------
  //
  void MqttJson::ebToMqtt(Cbor & msg) {
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
  void MqttJson::onEvent(Cbor & msg) {
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
