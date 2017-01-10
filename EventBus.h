#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <Actor.h>
#include <CborQueue.h>
#include <Uid.h>

typedef void (Actor::*MethodHandler)(Cbor&);
typedef void (*StaticHandler)(Cbor&);
#define CALL_MEMBER_FUNC(object,ptrToMember)  ((*object).*(ptrToMember))

class Subscriber {
public:
    Actor* _actor;
    union {
        StaticHandler _staticHandler;
        MethodHandler _methodHandler;
    };
    Subscriber* _nextSubscriber;
    Subscriber();
    Subscriber* next();
};

class EventFilter {

public:
    typedef enum  { EF_ANY,EF_REQUEST,EF_EVENT,EF_REPLY,EF_KV,EF_REMOTE } type;
    type _type;
    uid_t _object;
    uid_t _value;

    Subscriber* _firstSubscriber;
    EventFilter* _nextFilter;
    EventFilter(type t,uid_t o,uid_t v);
    Subscriber* firstSubscriber();
    Subscriber* lastSubscriber();
    Subscriber* addSubscriber();
    void subscribe( Actor* instance, MethodHandler handler);
    void subscribe( StaticHandler handler);
    void subscribe(Actor* actor);
    void invokeAllSubscriber(Cbor& );

    EventFilter* next();
    bool match(Cbor& cbor);

    static bool isEvent(Cbor& cbor,uid_t src,uid_t ev);
    static bool isRequest(Cbor& cbor,uid_t dst,uid_t req);
    static bool isReply(Cbor& cbor,uid_t src,uid_t req);
    static bool isReplyCorrect(Cbor& cbor,uid_t src,uid_t req);
	static bool isRemote(Cbor& cbor);

} ;

#define EB_DST H("#dst")
#define EB_SRC H("#src")
#define EB_REQUEST H("#request")
#define EB_REPLY H("#reply")
#define EB_EVENT H("#event")
#define EB_ERROR H("error")
#define EB_REGISTER H("register")
#define EB_SRC_DEVICE H("#src_device")
#define EB_DST_DEVICE	H("#dst_device")

class EventBus {
private:
    CborQueue _queue;
    EventFilter* _firstFilter;

    EventFilter* firstFilter();
    EventFilter* findFilter(EventFilter::type ,uid_t o,uid_t v);
    EventFilter* lastFilter();
    EventFilter& addFilter(EventFilter::type ,uid_t o,uid_t v);



    Cbor _txd;
    Cbor _rxd;

public:
    EventBus(uint32_t size,uint32_t msgSize);
	void setup();
//    Erc initAll();
    bool match(uint32_t header,uid_t dst,uid_t src,uid_t op);

    void publish(uid_t header, Cbor& cbor);
    void publish(uid_t src,uid_t event);
    void publish(Cbor& cbor);

    EventFilter& filter(uid_t key,uid_t value);
    EventFilter& onRequest(uid_t dst);
    EventFilter& onRequest(uid_t dst,uid_t req);
    EventFilter& onReply(uid_t dst,uid_t repl);
    EventFilter& onEvent(uid_t src,uid_t ev);
    EventFilter& onAny();
    EventFilter& onDst(uid_t dst);
    EventFilter& onSrc(uid_t src);
    EventFilter& onRemote();

    void eventLoop();
//   EventFilter* findFilter(EventFilter::type ,uid_t o,uid_t v);

    Cbor& request(uid_t dst,uid_t req,uid_t src);
    Cbor& reply(uid_t dst,uid_t repl,uid_t src);
    Cbor& reply();
    Cbor& event(uid_t src,uid_t ev);
	Cbor& empty();
    void defaultHandler(Actor* actor,Cbor& msg);
    // Cbor& data();                  //  eb.request(H("mqtt"),H("connect"),H("motor")).addKeyValue(H("host"),"test.mosquitto.org");eb.send(); eb.
    bool isEvent(uid_t ev,uid_t src);
    bool isRequest(uid_t dst,uid_t req);
	bool isRequest(uid_t req);
    bool isReply(uid_t src,uid_t req);
    bool isReplyCorrect(uid_t src,uid_t req);
    void send();
};

extern EventBus eb;

#endif // EVENTBUS_H
