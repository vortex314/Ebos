/*
 * Property.h
 *
 *  Created on: 23-jun.-2013
 *      Author: lieven2
 */

#ifndef PROPERTY_H_
#define PROPERTY_H_
#include <Cbor.h>
#include <Uid.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include "Erc.h"
#include "EventBus.h"
#include "Str.h"
#include "Sys.h"

enum Mode { M_READ, M_WRITE };

class PropertyBase {
  static PropertyBase* _first;
  PropertyBase* _next;
  static PropertyBase* _current;  // next to snend
  uid_t _service;
  uid_t _property;
  uint32_t _interval;
  uint64_t _timeout;
  bool _isReady;

 public:
  PropertyBase(uid_t service, uid_t property, uint32_t interval);
  void add(PropertyBase* p);
  void setReady(bool b);
  bool isReady();
  uid_t service();
  uid_t property();
  static PropertyBase* nextReady();
  static PropertyBase* find(uid_t service, uid_t property);

  virtual void addEventCbor(Cbor& cbor) = 0;
  virtual void doSet(Cbor& cbor) = 0;
};

template <typename T>
class Prop : public PropertyBase {
  typedef T (*Getter)(void);
  T& _var;

 public:
  Prop(T& var, uid_t service, const char* name, uint32_t interval)
      : PropertyBase(service, uid.hash(name), interval), _var(var){};

  void addEventCbor(Cbor& cbor) {
    cbor.addKeyValue(EB_SRC, service())
        .addKeyValue(EB_EVENT, property())
        .addKeyValue(property(), _var)
        .addKeyValue(H("public"), true);
  }
  void doSet(Cbor& cbor){};
};

template <typename T>
class PropGetter : public PropertyBase {
  typedef T (*Getter)(void);
  Getter _getter;

 public:
  PropGetter(Getter getter, uid_t service, const char* name, uint32_t interval)
      : PropertyBase(service, uid.hash(name), interval), _getter(getter){};

  void addEventCbor(Cbor& cbor) {
    cbor.addKeyValue(EB_SRC, service())
        .addKeyValue(EB_EVENT, property())
        .addKeyValue(property(), _getter())
        .addKeyValue(H("public"), true);
  }
  void doSet(Cbor& cbor){};
};

template <typename T>
class Property : public PropertyBase {
  typedef T (*Getter)(void);
  typedef void (*Setter)(T&);
  typedef enum { PROP_STATIC, PROP_FUNC } PropertyType;

  //  union {
  T* _var;
  Getter _getter;
  //    };

  Setter _setter;
  PropertyType _type;

 public:
  Property(T& var, uid_t service, const char* property, uint32_t interval)
      : PropertyBase(service, uid.hash(property), interval),
        _var(&var),
        _type(PROP_STATIC){};
  Property(T& var, uid_t service, uid_t property, uint32_t interval)
      : PropertyBase(service, property, interval),
        _var(&var),
        _type(PROP_STATIC){};
  Property(Getter getter, uid_t service, uid_t property, uint32_t interval)
      : PropertyBase(service, property, interval),

        _getter(getter),
        _type(PropertyType::PROP_FUNC) {
    _var = (T*)NULL;
  };
  Property(Getter getter, Setter setter, uid_t service, uid_t property,
           uint32_t interval)
      : PropertyBase(service, property, interval),
        _var((T&)NULL),
        _getter(getter),
        _setter(setter),
        _type(PROP_FUNC) {
    _var = ((T)NULL);
  };

 public:
  static Property* build(T& var, uid_t service, uid_t property,
                         uint32_t interval) {
    return new Property(var, service, property, interval);
  }
  static Property* build(T& var, uid_t service, const char* nameProperty,
                         uint32_t interval) {
    return new Property(var, service, uid.hash(nameProperty), interval);
  }

  static Property* build(Getter getter, uid_t service, uid_t property,
                         uint32_t interval) {
    return new Property(getter, service, property, interval);
  }

  static Property* build(Getter getter, uid_t service, const char* nameProperty,
                         uint32_t interval) {
    return new Property(getter, service, uid.hash(nameProperty), interval);
  }
  static Property* build(Getter getter, Setter setter, uid_t service,
                         uid_t property, uint32_t interval) {
    return new Property(getter, setter, service, property, interval);
  }
  virtual ~Property() {}

  void addEventCbor(Cbor& cbor) {
    if (_type == PROP_FUNC) {
      cbor.addKeyValue(EB_SRC, service())
          .addKeyValue(EB_EVENT, property())
          .addKeyValue(property(), _getter())
          .addKeyValue(H("public"), true);
    } else {
      cbor.addKeyValue(EB_SRC, service())
          .addKeyValue(EB_EVENT, property())
          .addKeyValue(property(), *_var)
          .addKeyValue(H("public"), true);
    }
    setReady(false);
  }
  void doSet(Cbor& cbor) {
    if (_type == PROP_STATIC) {
      cbor.getKeyValue(property(), *_var);
    }
    setReady(true);
  }
};

#endif /* PROPERTY_H_ */
