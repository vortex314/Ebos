#ifndef SYSTEM_H
#define SYSTEM_H

#include <EventBus.h>

class System : public Actor
{
    uint32_t _idxProps;
public:
	System(const char* name);
	~System();
	void setup();
	void init();
	void onEvent(Cbor& msg);
	void reset();
	void setBootTime(uint64_t t);
    void publishProps();
};

#endif // SYSTEM_H
