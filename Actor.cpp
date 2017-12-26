/*
 * Actor.cpp
 *
 *  Created on: Jun 27, 2016
 *      Author: lieven
 */

#include "Actor.h"
#include <EventBus.h>

Actor* Actor::_first = 0;
#ifndef UINT_LEAST64_MAX
#define UINT_LEAST64_MAX 0xFFFFFFFFFFFFFFFFL
#endif  // UINT_LEAST64_MAX
extern constexpr uint16_t H(const char* s);

Actor::Actor(const char* name) {
  _timeout = UINT_LEAST64_MAX;
  _state = 0;
  _ptLine = 0;
  _next = 0;
  _public = false;
  _name = name;
  //	setName(name);
  if (first() == 0) {
    setFirst(this);
  } else {
    last()->setNext(this);
  }
}

void Actor::setName(const char* name) {
  _name = name;
  _id = uid.hash(name);
}

uid_t Actor::id() {
  if (_id == 0) _id = uid.hash(_name);
  return _id;
}

Actor* Actor::last() {
  Actor* cursor = first();
  while (cursor->_next) {
    cursor = cursor->next();
  }
  return cursor;
}

Actor* Actor::first() { return Actor::_first; }
Actor* Actor::next() { return _next; }

void Actor::setNext(Actor* a) { _next = a; }

void Actor::setFirst(Actor* f) { _first = f; }

Actor::~Actor() {}

void Actor::onEvent(Cbor& cbor) { ASSERT(false); }

uint64_t Actor::lowestTimeout() {
  uint64_t lt = UINT64_MAX;
  Actor* la = 0;
  for (Actor* cur = first(); cur; cur = cur->next()) {
    if (cur->_timeout < lt) {
      lt = cur->_timeout;
      la = cur;
    }
  }
  if (la == 0) return 0;
  if (lt < (Sys::_upTime - 2000)) {
    WARN(" timeout 2 sec in the past for Actor : %s ", la->name());
  }
  //    DEBUG(" lowest timeout actor : %s ",lowest->_name);
  return lt;
}

Actor* Actor::findById(uint16_t id) {
  for (Actor* cur = first(); cur; cur = cur->next()) {
    if (cur->id() == id) return cur;
  }
  return 0;
}

bool Actor::state(uint32_t st) {
  if (_state != st) {
    DEBUG(" Actor '%s' state change '%s' => '%s'", name(), uid.label(_state),
          uid.label(st));
    _state = st;
    return true;
  }
  return false;
}
