#include "System.h"
#include "Sys.h"

System::System(const char* name) : Actor(name) { _idxProps = 0; }

System::~System() {}

void System::reset() { esp_restart(); }

void System::init() {}
void System::setup() {
  eb.onDst(H("system")).call(this);
  timeout(5000);
}

void System::publishProps() {
  switch (_idxProps) {
    case 0: {
      eb.event(id(), H("hostname"))
          .addKeyValue(H("data"), Sys::hostname())
          .addKeyValue(H("public"), true);
      break;
    }
    case 1: {
      eb.event(id(), H("upTime"))
          .addKeyValue(H("data"), Sys::millis())
          .addKeyValue(H("public"), true);
      break;
    }
    case 2: {
#ifdef ESP8266
      uint32_t nr = ESP.getChipId();
      uint8_t data[4];
      Bytes bytes(data, 4);
      for (int i = 0; i < 4; i++) {
        data[3 - i] = nr & 0xFF;
        nr = nr >> 8;
      }
      eb.event(id(), H("$chipId"))
          .addKeyValue(H("$data"), bytes)
          .addKeyValue(H("public"), true);
#endif
      break;
    }
    case 3: {
      eb.event(id(), H("heap"))
          .addKeyValue(H("data"), ESP.getFreeHeap())
          .addKeyValue(H("public"), true);
      break;
    }
  }
  eb.send();
  if (++_idxProps == 4) _idxProps = 0;
}

void System::onEvent(Cbor& msg) {
  if (eb.isRequest(H("props"))) {
    eb.reply()
        .addKeyValue(H("hostname"), Sys::hostname())
        .addKeyValue(H("upTime"), Sys::millis())
        .addKeyValue(H("bootTime"), Sys::now() - Sys::millis())
#ifdef ESP8266
        .addKeyValue(H("chip_id"), ESP.getChipId())
        .addKeyValue(H("heap"), ESP.getFreeHeap())
#endif
        ;
    eb.send();

  } else if (eb.isRequest(H("set"))) {
    uint64_t now;
    Str hostname(20);
    uint32_t level;
    if (msg.getKeyValue(H("now"), now)) {
      Sys::setNow(now);
      eb.publicEvent(id(), H("now")).addKeyValue(H("now"), Sys::now());
    };
    if (msg.getKeyValue(H("hostname"), hostname)) {
      Sys::hostname(hostname.c_str());
    };
    if (msg.getKeyValue(H("log_level"), level) && level <= Log::LOG_NONE) {
      logger.level((Log::LogLevel)level);
    };

    eb.reply().addKeyValue(EB_ERROR, E_OK);
    eb.send();

  } else if (eb.isRequest(H("reset"))) {
    reset();

  } else if (timeout()) {
    publishProps();
    timeout(20000);
  } else

    eb.defaultHandler(this, msg);
}

#ifdef STM32F1
void System::reset() {}
#endif

#ifdef ESP8266
extern "C" void __real_system_restart_local();

void System::reset() { __real_system_restart_local(); }
#endif

#ifdef __linux__
void System::reset() {}
#endif
