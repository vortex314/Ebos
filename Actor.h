/*
 * Actor.h
 *
 *  Created on: Jun 27, 2016
 *      Author: lieven
 */

#ifndef ACTOR_H_
#define ACTOR_H_

#include <Cbor.h>
#include <Log.h>
#include <Sys.h>
#include <Uid.h>
#include <stdint.h>

#define PT_BEGIN()       \
  bool ptYielded = true; \
  switch (_ptLine) {     \
    case 0:  // Declare start of protothread (use at start of Run()
             // implementation).
#define PT_END() \
  default:;      \
    }            \
    ;            \
    return;  // Stop protothread and end it (use at end of Run()
             // implementation).
// Cause protothread to wait until given condition is true.
#define PT_WAIT_UNTIL(condition) \
  do {                           \
    _ptLine = __LINE__;          \
    case __LINE__:               \
      if (!(condition)) return;  \
  } while (0)

#define PT_WAIT_WHILE(condition) \
  PT_WAIT_UNTIL(!(               \
      condition))  // Cause protothread to wait while given condition is true.
#define PT_WAIT_THREAD(child) \
  PT_WAIT_WHILE((child).dispatch(msg))  // Cause protothread to wait until given
                                        // child protothread completes.
// Restart and spawn given child protothread and wait until it completes.
#define PT_SPAWN(child)    \
  do {                     \
    (child).restart();     \
    PT_WAIT_THREAD(child); \
  } while (0)

// Restart protothread's execution at its PT_BEGIN.
#define PT_RESTART() \
  do {               \
    restart();       \
    return;          \
  } while (0)

// Stop and exit from protothread.
#define PT_EXIT() \
  do {            \
    stop();       \
    return;       \
  } while (0)

// Yield protothread till next call to its Run().
#define PT_YIELD()            \
  do {                        \
    ptYielded = false;        \
    _ptLine = __LINE__;       \
    case __LINE__:            \
      if (!ptYielded) return; \
  } while (0)

// Yield protothread until given condition is true.
#define PT_YIELD_UNTIL(condition)             \
  do {                                        \
    ptYielded = false;                        \
    _ptLine = __LINE__;                       \
    case __LINE__:                            \
      if (!ptYielded || !(condition)) return; \
  } while (0)
// Used to store a protothread's position (what Dunkels calls a
// "local continuation").
typedef unsigned short LineNumber;
// An invalid line number, used to mark the protothread has ended.
static const LineNumber LineNumberInvalid = (LineNumber)(-1);
// Stores the protothread's position (by storing the line number of
// the last PT_WAIT, which is then switched on at the next Run).

class Actor;

// #include <functional>
// typedef std::function<void(Header)> EventHandler;

#define CALL_MEMBER_FN(object, ptrToMember) ((object).*(ptrToMember))

class Actor {
 private:
  friend class EventBus;  // A is a friend of B

  const char* _name;
  uint16_t _id;
  uint64_t _timeout;
  uint32_t _state;
  bool _public;

  static Actor* _first;
  Actor* _next;

 protected:
  LineNumber _ptLine;

 public:
  Actor(const char* name);
  Actor(uint16_t id);
  virtual ~Actor();

  void setNext(Actor*);
  Actor* next();
  static Actor* last();
  static Actor* first();
  static void setFirst(Actor*);

  virtual void onEvent(Cbor& cbor);
  virtual void setup() = 0;
  virtual void init(){};
  uint64_t nextTimeout() { return _timeout; };

  void timeout(uint32_t time) { _timeout = Sys::millis() + time; }
  bool timeout() {
    //     INFO(" Actor %s millis %lld : _timeout %lld
    //     ",name(),Sys::millis(),_timeout);
    return Sys::millis() >= _timeout;
  }
  static uint64_t lowestTimeout();

  bool state(uint32_t state);
  inline int state() { return _state; }
  uint16_t id();

  inline bool isPublic() { return _public; }

  inline void setPublic(bool isPublic) { _public = isPublic; }
  inline const char* name() { return _name; }
  void setName(const char* name);
  static Actor* findById(uint16_t id);
};

#endif /* ACTOR_H_ */
