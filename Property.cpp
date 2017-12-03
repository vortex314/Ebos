#include "Property.h"

PropertyBase* PropertyBase::_first = 0;
PropertyBase* PropertyBase::_current = 0;

PropertyBase::PropertyBase(uid_t service, uid_t property, uint32_t interval) {
  //    _var = var;
  _service = service;
  _property = property;
  _interval = interval;
  _timeout = 0;
  _isReady = false;
  _next = 0;
  add(this);
}
void PropertyBase::add(PropertyBase* p) {
  if (_first == 0) {
    _first = p;
  } else {
    PropertyBase* cursor;
    for (cursor = _first; cursor; cursor = cursor->_next) {
      if (cursor->_next == 0) {
        cursor->_next = p;
        break;
      }
    };
  }
}

PropertyBase* PropertyBase::find(uid_t service, uid_t property) {
  PropertyBase* cursor;
  for (cursor = _first; cursor; cursor = cursor->_next) {
    if (cursor->_service == service && cursor->_property == property) break;
  }
  return cursor;
}

void PropertyBase::setReady(bool b) { _isReady = b; }

bool PropertyBase::isReady() {
  if (_isReady == true) {
    return true;
  }
  if (Sys::millis() > _timeout) {
    _timeout = Sys::millis() + _interval;
    return true;
  }
  return false;
}

uid_t PropertyBase::service() { return _service; }

uid_t PropertyBase::property() { return _property; }

PropertyBase* PropertyBase::nextReady() {
  if (_current == 0) {
    _current = _first;
  } else {
    _current = _current->_next;
  };
  while (_current) {
    if (_current && (_current->isReady())) return _current;
    _current = _current->_next;
  }
  return _current;
}