/*
 * MqttCbor.h
 *
 *  Created on: 3-jan.-2017
 *      Author: lieven2
 */

#ifndef MQTT_CBOR_H_
#define MQTT_CBOR_H_

#include <Cbor.h>
#include <Cbor.h>
#include <EventBus.h>
#include <Uid.h>


//_______________________________________________________________________________________________________________
//
class MqttCbor: public Actor {
	Str _topic;
	Cbor _message;
	Actor* _actor;
	Str _name;
    int _fd;
    uid_t _mqttId;

public:
	MqttCbor(const char* name);
    void setMqttId(uid_t mqttId);
	void setup();
	void init(){};
	bool addHeader(Cbor& json, Cbor& cbor, uint16_t key);
	bool addTopic(Str& topic, Cbor& cbor, uint16_t key);
	bool isHeaderField(uint16_t key);
	int nextHash(Str& str);
	void jsonToCbor(Cbor& cbor, Cbor& json);
	void cborToMqtt(Str& topic, Cbor& json, Cbor& cbor);
	void onEvent(Cbor& msg);
	void ebToMqtt(Cbor& msg);
	void mqttToEb(Cbor& msg);
    int fd();
};

#endif /* MQTT_CBOR_H_ */
