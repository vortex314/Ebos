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
#endif // UINT_LEAST64_MAX
extern constexpr uint16_t H(const char* s);

Actor::Actor(const char* name) {
	_timeout = UINT_LEAST64_MAX;
	_state = 0;
	_ptLine = 0;
	_next = 0;
	setName(name);
	if (first() == 0) {
		setFirst(this);
	} else {
		last()->setNext(this);
	}
}

void Actor::setName(const char* name) {
	_id = H(name);
	_name = name;
}

/*Actor::Actor(uint16_t id) {
 _timeout = UINT_LEAST64_MAX;
 _state = 0;
 _ptLine = 0;
 _next = 0;
 _id = id;
 if (first() == 0) {
 setFirst(this);
 } else {
 last()->setNext(this);
 }
 }*/

Actor* Actor::last() {
	Actor* cursor = first();
	while (cursor->_next) {
		cursor = cursor->next();
	}
	return cursor;
}

Actor* Actor::first() {
	return Actor::_first;
}
Actor* Actor::next() {
	return _next;
}

void Actor::setNext(Actor* a) {
	_next = a;
}

void Actor::setFirst(Actor* f) {
	_first = f;
}

Actor::~Actor() {
}


void Actor::onEvent(Cbor& cbor) {
    ASSERT(false);
}

uint64_t Actor::lowestTimeout() {
	uint64_t lt = UINT64_MAX;
	Actor* la;
	for (Actor* cur = first(); cur; cur = cur->next()) {
		if (cur->_timeout < lt) {
			lt = cur->_timeout;
			la = cur;
		}
	}
	if ( lt< (Sys::_upTime - 2000)) {
        WARN(" timeout 2 sec in the past for Actor : %s ",la->name());
	}
//    DEBUG(" lowest timeout actor : %s ",lowest->_name);
	return lt;
}

Actor* Actor::findById(uint16_t id) {
	for (Actor* cur = first(); cur; cur = cur->next()) {
		if (cur->id() == id)
			return cur;
	}
	return 0;
}
