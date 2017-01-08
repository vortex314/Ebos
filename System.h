#ifndef SYSTEM_H
#define SYSTEM_H

#include <EventBus.h>

class System : public Actor
{
public:
	System();
	~System();
	void setup();
	void init();
	void onEvent(Cbor& msg);
	void reset();
	void setBootTime(uint64_t t);
};

#endif // SYSTEM_H
