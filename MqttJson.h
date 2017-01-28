/*
 * Router.h
 *
 *  Created on: 3-jan.-2017
 *      Author: lieven2
 */

#ifndef MQTT_JSON_H_
#define MQTT_JSON_H_

#include <Json.h>
#include <Cbor.h>
#include <EventBus.h>
#include <Uid.h>


//_______________________________________________________________________________________________________________
//
class MqttJson: public Actor {
	Str _topic;
	Json _message;
	Actor* _actor;
	Str _name;
        uid_t _mqttId;

public:
	MqttJson(const char* name,uint32_t jsonSize);
	void setup();
	void init(){};
	bool addHeader(Json& json, Cbor& cbor, uint16_t key);
	bool addTopic(Str& topic, Cbor& cbor, uint16_t key);
	bool isHeaderField(uint16_t key);
	int nextHash(Str& str);
	void jsonToCbor(Cbor& cbor, Json& json);
	void cborToMqtt(Str& topic, Json& json, Cbor& cbor);
	void onEvent(Cbor& msg);
	void ebToMqtt(Cbor& msg);
	void mqttToEb(Cbor& msg);
    void setMqttId(uid_t mqttId);
    void sendPublicEvents(Cbor& msg);
        int fd();
};

#endif /* MQTT_JSON_H_ */
