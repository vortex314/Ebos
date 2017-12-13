/*
 * Router.h
 *
 *  Created on: 3-jan.-2017
 *      Author: lieven2
 */

#ifndef MQTT_JSON_H_
#define MQTT_JSON_H_

#include <Cbor.h>
#include <EventBus.h>
#include <Json.h>
#include <Uid.h>
#define ARDUINOJSON_USE_LONG_LONG 1
//#define ARDUINOJSON_USE_INT64 1
#include <ArduinoJson.h>

//_______________________________________________________________________________________________________________
//
class MqttJson : public Actor
{
    Str _topic;
    Str _message;
    Actor* _actor;
    //    Str _name;
    uid_t _mqttId;
    StaticJsonBuffer<512> _jsonBuffer;

public:
    MqttJson(const char* name, uint32_t jsonSize);
    void setup();
    void init() {};
    bool addHeader(Json& json, Cbor& cbor, uint16_t key);
    bool addTopic(Str& topic, Cbor& cbor, uint16_t key);
    void addMessageObject(Str& json, Cbor& cbor);
    void addMessageData(Str& json, Cbor& cbor);

    bool isHeaderField(uint16_t key);
    int nextHash(Str& str);
    uid_t getRemoteSrcUid(Str&);
    void jsonVariantToCbor(Cbor& cbor, uid_t , const JsonVariant& json);
    void jsonObjectToCbor(Cbor& cbor,  JsonObject& json);
    void cborFieldToJsonObject(JsonObject& ,const char* name,Cbor& cbor);
    void cborObjectToJsonObject(JsonObject& ,Cbor& cbor);
    void cborToMqtt(Str& topic, Str& message, Cbor& cbor);
    void onEvent(Cbor& msg);
    void ebToMqtt(Cbor& msg);
    void mqttToEb(Cbor& msg);
    void setMqttId(uid_t mqttId);
    void sendPublicEvents(Cbor& msg);
    int fd();
};

#endif /* MQTT_JSON_H_ */
