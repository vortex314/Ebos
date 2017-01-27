#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <Actor.h>
#include <CborQueue.h>
#include <Uid.h>

typedef void (Actor::*MethodHandler)(Cbor&);
typedef void (*StaticHandler)(Cbor&);
#define CALL_MEMBER_FUNC(object,ptrToMember)  ((*object).*(ptrToMember))
#define HEADER_COUNT 9
typedef struct {
    union {
        struct {
            uid_t dst,dst_device,src,src_device,request,reply,event,id,error ;
        };
        uid_t uid[HEADER_COUNT];
    };
} Header;

typedef Cbor Em;

class Subscriber
{
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

class EventFilter
{

public:
    typedef enum  { EF_ANY,EF_REQUEST,EF_EVENT,EF_REPLY,EF_KV,EF_REMOTE,EF_REMOTE_SRC,EF_REMOTE_DST } type;
    Header _pattern;


    Subscriber* _firstSubscriber;
    EventFilter* _nextFilter;
    EventFilter(Header& h);
    Subscriber* firstSubscriber();
    Subscriber* lastSubscriber();
    Subscriber* addSubscriber();
    void subscribe( Actor* instance, MethodHandler handler);
    void subscribe( StaticHandler handler);
    void subscribe(Actor* actor);
    void invokeAllSubscriber(Cbor& );

    EventFilter* next();
    bool match(Cbor& cbor);
    bool match(Header& h);

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
#define EB_ID H("id")

class EventBus
{
private:
    CborQueue _queue;
    EventFilter* _firstFilter;

    EventFilter* firstFilter();
    EventFilter* findFilter(EventFilter::type ,uid_t o,uid_t v);
    EventFilter* lastFilter();
    EventFilter& addFilter(EventFilter::type ,uid_t o,uid_t v);
    EventFilter* findFilter(Header& h);
    EventFilter& addFilter(Header& h);



    Cbor _txd;
    Cbor _rxd;
    Header _rxdHeader;
    uint16_t _id;


public:
    EventBus(uint32_t size,uint32_t msgSize);
    void setup();
    void log(Str& str,Cbor& cbor);
//    Erc initAll();
    bool match(uint32_t header,uid_t dst,uid_t src,uid_t op);
    void getHeader(Header& header);

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
    EventFilter& onRemoteSrc(uid_t src_dev,uid_t src);
    EventFilter& onRemoteDst(uid_t dst_dev,uid_t dst);

    void eventLoop();
//   EventFilter* findFilter(EventFilter::type ,uid_t o,uid_t v);

    Cbor& request(uid_t dst,uid_t req,uid_t src);
    Cbor& requestRemote(uid_t dev,uid_t dst,uid_t req,uid_t src);
    Cbor& reply(uid_t dst,uid_t repl,uid_t src);
    Cbor& reply();
    Cbor& event(uid_t src,uid_t ev);
    Cbor& empty();
    Cbor& clear();
    void defaultHandler(Actor* actor,Cbor& msg);
    // Cbor& data();                  //  eb.request(H("mqtt"),H("connect"),H("motor")).addKeyValue(H("host"),"test.mosquitto.org");eb.send(); eb.
    bool isEvent(uid_t ev,uid_t src);
    bool isRequest(uid_t dst,uid_t req);
    bool isRequest(uid_t req);
    bool isReply(uid_t src,uid_t req);
    bool isReplyCorrect(uid_t src,uid_t req);
    bool isHeader(uid_t id);
    void send();
};

extern EventBus eb;

#endif // EVENTBUS_H
