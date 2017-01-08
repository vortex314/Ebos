#include "System.h"
#include "Sys.h"

System::System() : Actor("system")
{
}

System::~System()
{
}

void System::init()
{
}
void System::setup()
{
	eb.onDst(H("system")).subscribe(this);
}

void System::onEvent(Cbor& msg)
{
	if (eb.isRequest(H("props"))) {
		eb.reply()
		.addKeyValue(H("hostname"),Sys::hostname())
		.addKeyValue(H("upTime"),Sys::millis())
		.addKeyValue(H("bootTime"),Sys::now()-Sys::millis());
		eb.send();

	} else if (eb.isRequest(H("set"))) {

		uint64_t now;
		Str hostname(20);
		if ( msg.getKeyValue(H("now"),now)) {
			Sys::setNow(now);
		} else if ( msg.getKeyValue(H("hostname"),hostname)) {
			Sys::hostname(hostname.c_str());
		}
		eb.reply().addKeyValue(EB_ERROR,E_OK);
		eb.send();

	} else if (eb.isRequest(H("reset"))) {

		reset();

	} else

		eb.defaultHandler(this, msg);
}

#ifdef STM32F1
void System::reset(){

}
#endif

#ifdef ESP8266
extern "C" void __real_system_restart_local();

void System::reset()
{
	__real_system_restart_local();
}
#endif
