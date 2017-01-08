/*
 * Actor.h
 *
 *  Created on: Jun 27, 2016
 *      Author: lieven
 */

#ifndef ACTOR_H_
#define ACTOR_H_

#include <stdint.h>
#include <Sys.h>
#include <Cbor.h>
/*
template<class T> class LinkedList {
	T* _next;
	static T* _first;
public:
	LinkedList() {
		_next = 0;
	}
	static T* next(T* t) {
		return t->_next;
	}
	T* next() {
		return _next;
	}
	void setNext(T* t) {
		_next = t;
	}
	static T* first() {
		return _first;
	}
	void setFirst(T* f) {
		_first = f;
	}
	static T* last() {
		for (T* t = first(); t != 0; t = next(t)) {
			if (t->_next == 0)
				return t;
		}
		return 0;
	}

};
template<class T> T* LinkedList<T>::_first = 0;
*/

//#define LOGF(fmt,...) PrintHeader(__FILE__,__LINE__,__FUNCTION__);Serial.printf(fmt,##__VA_ARGS__);Serial.println();
//extern void PrintHeader(const char* file, uint32_t line, const char *function);

#define PT_BEGIN() bool ptYielded = true; switch (_ptLine) { case 0: // Declare start of protothread (use at start of Run() implementation).
#define PT_END() default: ; } ; return ; // Stop protothread and end it (use at end of Run() implementation).
// Cause protothread to wait until given condition is true.
#define PT_WAIT_UNTIL(condition) \
    do { _ptLine = __LINE__; case __LINE__: \
    if (!(condition)) return ; } while (0)

#define PT_WAIT_WHILE(condition) PT_WAIT_UNTIL(!(condition)) // Cause protothread to wait while given condition is true.
#define PT_WAIT_THREAD(child) PT_WAIT_WHILE((child).dispatch(msg)) // Cause protothread to wait until given child protothread completes.
// Restart and spawn given child protothread and wait until it completes.
#define PT_SPAWN(child) \
    do { (child).restart(); PT_WAIT_THREAD(child); } while (0)

// Restart protothread's execution at its PT_BEGIN.
#define PT_RESTART() do { restart(); return ; } while (0)

// Stop and exit from protothread.
#define PT_EXIT() do { stop(); return ; } while (0)

// Yield protothread till next call to its Run().
#define PT_YIELD() \
    do { ptYielded = false; _ptLine = __LINE__; case __LINE__: \
    if (!ptYielded) return ; } while (0)

// Yield protothread until given condition is true.
#define PT_YIELD_UNTIL(condition) \
    do { ptYielded = false; _ptLine = __LINE__; case __LINE__: \
    if (!ptYielded || !(condition)) return ; } while (0)
// Used to store a protothread's position (what Dunkels calls a
// "local continuation").
typedef unsigned short LineNumber;
// An invalid line number, used to mark the protothread has ended.
static const LineNumber LineNumberInvalid = (LineNumber) (-1);
// Stores the protothread's position (by storing the line number of
// the last PT_WAIT, which is then switched on at the next Run).

class Actor;

// #include <functional>
// typedef std::function<void(Header)> EventHandler;

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

class Actor
{
private:
    friend class EventBus; // A is a friend of B

    const char* _name;
    uint16_t _id;
    uint64_t _timeout;
    uint32_t _state;

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
    virtual void setup()=0;
    virtual void init() {};
    uint64_t nextTimeout()
    {
        return _timeout;
    };


    void timeout(uint32_t time)
    {
        _timeout = Sys::millis() + time;
    }
    bool timeout()
    {
        return Sys::millis() > _timeout;
    }
    static uint64_t lowestTimeout();

    inline void state(int st)
    {
        DEBUG(" state change %d => %d", _state, st);
        _state = st;
    }
    inline int state()
    {
        return _state;
    }
    inline uint16_t id()
    {
        return _id;
    }
    inline void id(uint16_t id)
    {
        _id=id;
    }
    inline const char* name()
    {
        return _name;
    }
    void setName(const char* name);
    static Actor* findById(uint16_t id);
};

#endif /* ACTOR_H_ */

