#include "System.h"
#include <Property.h>
#include "Sys.h"
//============================================================   STM32

#ifdef STM32F1
void System::reset() {}
#endif
  //============================================================   ESP8266

#ifdef ESP8266
extern "C" void __real_system_restart_local();

void System::reset() { __real_system_restart_local(); }
#endif
  //============================================================   LINUX

#ifdef __linux__
void System::reset() {}
#endif
  //============================================================   ESP32 IDF

#ifdef ESP32_IDF

const char* getCpu() { return "ESP32"; }
uint32_t getFreeHeap() { return 0; }

const char* getSdk() { return "1.2.3"; }
const char* getHardware() { return "SDK-xxx"; }
void System::reset() { FATAL("Resetting.... ");while(true); };

#endif
  //============================================================   ESP32 ARDUINO

#ifdef ESP32_ARDUINO

uint32_t getFreeHeap() { return ESP.getFreeHeap(); };

void System::reset() { esp_restart(); }

const char* getCpu() {
  esp_chip_info_t chip;
  esp_chip_info(&chip);

  sprintf(info, "ESP32 %s %s %s cores : %d revision : 0x%X ",
          chip.features & CHIP_FEATURE_WIFI_BGN ? "WIFI" : "",
          chip.features & CHIP_FEATURE_BLE ? "/BLE" : "",
          chip.features & CHIP_FEATURE_BT ? "/BT" : "", chip.cores,
          chip.revision);
  return info;
}

const char* getSdk() { return ESP.getSdkVersion(); }

const char* getHardware() {
  sprintf(info, "Flash %dMB %dMhz, CPU %dMHz", ESP.getFlashChipSize() / 1048576,
          ESP.getFlashChipSpeed() / 1000000, ESP.getCpuFreqMHz());
  return info;
}

#endif
//=====================================================================================

System::System(const char* name) : Actor(name) { _idxProps = 0; }

System::~System() {}

static const char* labels[] = {"hostname",  "upTime",   "heap",
                               "processor", "hardware", "version",
                               "alive",     "cpu",      "sdk"};

char info[100];
const char* getVersion() { return __FILE__ " " __DATE__ " " __TIME__; };

const char* getHostname(void) { return Sys::hostname(); };
uint64_t getUpTime() { return Sys::millis(); };

bool alive = true;

void System::init() {}
void System::setup() {
  eb.onDst(H("system")).call(this);
  uid.add(labels, sizeof(labels) / sizeof(const char*));

  Property<bool>::build(alive, id(), H("alive"), 5000);
  Property<const char*>::build(getHostname, id(), H("hostname"), 20000);
  Property<uint64_t>::build(getUpTime, id(), H("upTime"), 5000);
  Property<const char*>::build(getCpu, id(), H("cpu"), 20000);
  Property<uint32_t>::build(Sys::getFreeHeap, id(), H("heap"), 5000);
  Property<const char*>::build(getSdk, id(), H("sdk"), 20000);
  Property<const char*>::build(getHardware, id(), H("hardware"), 20000);

  timeout(5000);
}

void System::publishProps() {
  PropertyBase* readyProperty = PropertyBase::nextReady();
  if (readyProperty) {
    readyProperty->addEventCbor(eb.empty());
    eb.send();
  }
}

void System::onEvent(Cbor& msg) {
  if (timeout()) {
    publishProps();
    timeout(100);
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
  } else
    eb.defaultHandler(this, msg);
}
