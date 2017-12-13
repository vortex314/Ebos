#include "EventBus.h"

Cbor* timeoutEvent;

const char* eventbus_uids[] = {"dst",        "src",           "request",
                               "reply",      "event",         "error",
                               "%dst",       "%dst_device",   "%event",
                               "%from",      "%reply",        "%request",
                               "%src",       "%src_device",   "Actor",
                               "Echo",       "Logger",        "Relay",
                               "Router",     "Sonar",         "Tester",
                               "bootTime",   "clean_session", "clientId",
                               "client_id",  "closed",        "connect",
                               "connected",  "connected)",    "data",
                               "disconnect", "disconnected",  "err",
                               "error",      "error_detail",  "error_msg",
                               "event",      "host",          "hostname",
                               "id",         "init",          "keep_alive",
                               "line",       "log",           "message",
                               "method",     "motor",         "mqtt",
                               "name",       "now",           "nr",
                               "object",     "opened",        "password",
                               "ping",       "port",          "prefix",
                               "props",      "publish",       "published",
                               "qos",        "register",      "reply",
                               "request",    "reset",         "retain",
                               "retained",   "rxd",           "serial",
                               "set",        "setup",         "slip",
                               "src",        "state",         "status",
                               "subscribe",  "sys",           "system",
                               "tcp",        "tick",          "time",
                               "timeout",    "topic",         "uint32_t",
                               "upTime",     "user",          "will_message",
                               "will_qos",   "will_retain",   "will_topic"
                              };

//____________________________________________________________________
//
static uint64_t startTime = 0;
void startTimer()
{
    startTime= Sys::millis();
}
void stopTimer(const char* actor)
{
    uint32_t delta = Sys::millis() - startTime;
    if (delta > 10) {
        WARN(" slow Actor %s : %d msec on timeout.", actor, delta);
    }
    startTime = Sys::millis();
}

EventBus::EventBus(uint32_t size, uint32_t msgSize)
    : _queue(size), _firstFilter(0), _txd(msgSize), _rxd(msgSize), _id(0)
{
    timeoutEvent = new Cbor(12);
    timeoutEvent->addKeyValue(EB_SRC, H("system"));
    timeoutEvent->addKeyValue(EB_EVENT, H("timeout"));
}

void EventBus::setup()
{
    uid.add(eventbus_uids, sizeof(eventbus_uids) / sizeof(const char*));
    publish(H("system"), H("setup"));
}

void EventBus::publish(uid_t header, Cbor& cbor)
{
    Cbor msg(0);
    _queue.putMap(msg);
    msg.addKey(0).add(header);
    msg.append(cbor);
    _queue.putRelease(msg);
}

void EventBus::publish(uid_t src, uid_t ev)
{
    event(src, ev);
    send();
}

Cbor& EventBus::empty()
{
    if (_txd.length() != 0) WARN(" EB.txd not cleared ");
    _txd.clear();
    return _txd;
}

Cbor& EventBus::clear()
{
    _txd.clear();
    return _txd;
}

Cbor& EventBus::event(uid_t src, uid_t event)
{
    empty();
    _txd.addKeyValue(EB_SRC, src);
    _txd.addKeyValue(EB_EVENT, event);
    _txd.addKeyValue(EB_ID, _id++);
    return _txd;
}

Cbor& EventBus::requestRemote(uid_t dev, uid_t dst, uid_t req, uid_t src)
{
    empty();
    _txd.addKeyValue(EB_DST, dst);
    _txd.addKeyValue(EB_REQUEST, req);
    _txd.addKeyValue(EB_SRC, src);
    _txd.addKeyValue(EB_ID, _id++);
    return _txd;
}

Cbor& EventBus::request(uid_t dst, uid_t req, uid_t src)
{
    empty();
    _txd.addKeyValue(EB_DST, dst);
    _txd.addKeyValue(EB_REQUEST, req);
    _txd.addKeyValue(EB_SRC, src);
    _txd.addKeyValue(EB_ID, _id++);
    return _txd;
}
Cbor& EventBus::reply(uid_t dst, uid_t repl, uid_t src)
{
    empty();
    _txd.addKeyValue(EB_DST, dst);
    _txd.addKeyValue(EB_REPLY, repl);
    _txd.addKeyValue(EB_SRC, src);
    return _txd;
}

Cbor& EventBus::reply()
{
    empty();
    uid_t dst, src, repl, id;
    if (_rxd.getKeyValue(EB_SRC, dst)) _txd.addKeyValue(EB_DST, dst);
    if (_rxd.getKeyValue(EB_REQUEST, repl)) _txd.addKeyValue(EB_REPLY, repl);
    if (_rxd.getKeyValue(EB_DST, src)) _txd.addKeyValue(EB_SRC, src);
    if (_rxd.getKeyValue(EB_ID, id)) _txd.addKeyValue(EB_ID, id);
    return _txd;
}

void EventBus::send()
{
    if (_queue.put(_txd) != E_OK) WARN(" Eventbus full !!");
    _txd.clear();
}

void EventBus::publish(Cbor& cbor)
{
    Cbor msg(0);
    if (_queue.putMap(msg)) WARN(" Eventbus full !!");
    msg.append(cbor);
    if (_queue.putRelease(msg)) WARN(" Eventbus full !!");
}

EventFilter& EventBus::onAny()
{
    Header h = {};
    return addFilter(h);
}
//_______________________________________________________________________________________________
//
/*EventFilter& EventBus::filter(uid_t key,uid_t value)
{
    Header h= {};
    return addFilter(EventFilter::EF_KV,key,value);
}*/
//_______________________________________________________________________________________________
//
EventFilter& EventBus::onRequest(uid_t dst)
{
    Header h = {};
    h.dst = dst;
    return addFilter(h);
}
//_______________________________________________________________________________________________
//
EventFilter& EventBus::onRequest(uid_t dst, uid_t req)
{
    Header h = {};
    h.dst = dst;
    h.request = req;
    return addFilter(h);
}
//_______________________________________________________________________________________________
//
EventFilter& EventBus::onReply(uid_t dst, uid_t repl)
{
    Header h = {};
    h.dst = dst;
    h.reply = repl;
    return addFilter(h);
}
//_______________________________________________________________________________________________
//
EventFilter& EventBus::onEvent(uid_t src, uid_t ev)
{
    Header h = {};
    h.src = src;
    h.event = ev;
    return addFilter(h);
}

//_______________________________________________________________________________________________
//
EventFilter& EventBus::onDst(uid_t dst)
{
    Header h = {};
    h.dst = dst;
    return addFilter(h);
}
//_______________________________________________________________________________________________
//
EventFilter& EventBus::onRemote()
{
    Header h = {};
    h.dst = EB_UID_REMOTE;
    return addFilter(h);
}
//_______________________________________________________________________________________________
//
/*EventFilter& EventBus::onRemoteSrc(uid_t src)
{
    Header h= {};
    h.src=EB_UID_REMOTE;
    return addFilter(h);
}
//_______________________________________________________________________________________________
//
EventFilter& EventBus::onRemoteDst(uid_t dst)
{
    Header h= {};
    h.dst=EB_UID_REMOTE;
    return addFilter(h);

}*/
//__________________________________log_____________________________________________________________
//
EventFilter& EventBus::onSrc(uid_t src)
{
    Header h = {};
    h.src = src;
    return addFilter(h);
}
//_______________________________________________________________________________________________
//
bool EventBus::isEvent(uid_t src, uid_t ev)
{
    return (src == 0 || _rxdHeader.src == src) &&
           (ev == 0 || _rxdHeader.event == ev);
}
//_______________________________________________________________________________________________
//
bool EventBus::isPublicEvent()
{
    if (_rxdHeader.dst == EB_UID_REMOTE && _rxdHeader.src && _rxdHeader.event) {
        Actor* pActor = Actor::findById(_rxdHeader.src);
        if (pActor && pActor->isPublic()) {
            return true;
        }
    }
    return false;
}
//_______________________________________________________________________________________________
//
bool EventBus::isReply(uid_t src, uid_t req)
{
    return (src == 0 || _rxdHeader.src == src) &&
           (req == 0 || _rxdHeader.reply == req);
}
//_______________________________________________________________________________________________
//
bool EventBus::isReplyCorrect(uid_t src, uid_t req)
{
    return isReply(src, req) && _rxdHeader.error == 0;
}
//_______________________________________________________________________________________________
//
bool EventBus::isRequest(uid_t dst, uid_t req)
{
    return (dst == 0 || _rxdHeader.dst == dst) &&
           (req == 0 || _rxdHeader.request == req);
}
//_______________________________________________________________________________________________
//
bool EventBus::isRequest(uid_t req)
{
    return isRequest((uid_t)EB_UID_IGNORE, req);
}

bool EventBus::isHeader(uid_t id)
{
    return id == EB_REQUEST || id == EB_REPLY || id == EB_DST || id == EB_SRC ||
           id == EB_EVENT;
}

extern void usart_send_string(const char* s);

//____________________________________________________________________
//
EventFilter* EventBus::firstFilter()
{
    return _firstFilter;
}
//____________________________________________________________________
//

void EventFilter::invokeAllSubscriber(Cbor& cbor)
{
    for (Subscriber* sub = firstSubscriber(); sub != 0; sub = sub->next()) {
        startTimer();
        if (sub->_actor == 0) {
            sub->_staticHandler(cbor);
            stopTimer("static function");
        } else {
            if (sub->_methodHandler == 0) {
                sub->_actor->onEvent(cbor);
            } else {
                CALL_MEMBER_FUNC(sub->_actor, sub->_methodHandler)(cbor);
            }
            stopTimer(sub->_actor->name());
        }
    }
}

//____________________________________________________________________
//
void EventBus::defaultHandler(Actor* actor, Cbor& msg)
{
    if (isRequest(actor->id(), H("ping"))) {
        eb.reply().addKeyValue(H("error"), E_OK);
        eb.send();
    } else if (isRequest(actor->id(), H("status"))) {
        eb.reply()
        .addKeyValue(H("error"), 0)
        .addKeyValue(H("name"), actor->_name)
        .addKeyValue(H("state"), uid.label(actor->_state))
        .addKeyValue(H("timeout"), actor->_timeout)
        .addKeyValue(H("id"), actor->_id)
        .addKeyValue(H("line"), actor->_ptLine);
        eb.send();
    } else if (isRequest(actor->id(), H("init"))) {
        actor->init();
        eb.reply()
        .addKeyValue(H("error"), 0)
        .addKeyValue(H("line"), actor->_ptLine);
        eb.send();
    } else {
        uid_t src = 0;
        msg.getKeyValue(EB_SRC, src);
        WARN(" unknown event from %s to %s ", uid.label(src), actor->name());
        return;
        eb.reply()
        .addKeyValue(EB_ERROR, EBADMSG)
        .addKeyValue(H("error_msg"), "unknown event")
        .addKeyValue(H("Actor"), actor->name())
        .addKeyValue(EB_SRC, src);
        eb.send();
    }
}
//____________________________________________________________________
//

Str& tupple(Str& str, uid_t dev, uid_t srv)
{
    str.append("(");
    //    str.append(dev ? uid.label(dev) : "-");
    //    str.append(",");
    str.append(srv ? uid.label(srv) : "-");
    str.append(")");
    return str;
}

void EventBus::log(Str& str, Cbor& cbor)
{
    cbor.offset(0);
    uid_t key, value, dst, src, op;
    key = value = dst = src = op = 0;
    str.clear();
    Cbor::PackType ct;
    cbor.offset(0);
    cbor.getKeyValue(EB_DST, dst);
    cbor.getKeyValue(EB_SRC, src);
    if (cbor.getKeyValue(EB_REQUEST, op)) {
        tupple(str, 0, _rxdHeader.src);
        str.append("---").append(uid.label(op)).append("-->");
        tupple(str, 0, _rxdHeader.dst);
    } else if (cbor.getKeyValue(EB_REPLY, op)) {
        tupple(str, 0, _rxdHeader.dst);
        str.append("<--").append(uid.label(op)).append("---");
        tupple(str, 0, _rxdHeader.src);
    } else if (cbor.getKeyValue(EB_EVENT, op)) {
        tupple(str, 0, _rxdHeader.src);
        str.append("---").append(uid.label(op)).append(" >> ");
    }
    cbor.offset(0);

    while (cbor.hasData()) {
        cbor.get(key);
        if (isHeader(key)) {
            cbor.skipToken();
        } else {
            const char* label = uid.label(key);
            str.append(" | ").append(label).append(":");
            if (label[0] == '%') {
                cbor.get(value);
                str.append("").append(uid.label(value));
            } else {
                ct = cbor.tokenToString(str);
                if (ct == Cbor::P_BREAK || ct == Cbor::P_ERROR) break;
            }
            str.append("");
            if (cbor.hasData()) str << "";
        }
    };
}

void EventBus::getHeader(Header& header)
{
    Cbor& msg = _rxd;
    bzero(&header, sizeof(header));
    msg.getKeyValue(EB_DST, header.dst);
    msg.getKeyValue(EB_SRC, header.src);
    msg.getKeyValue(EB_REQUEST, header.request);
    msg.getKeyValue(EB_REPLY, header.reply);
    msg.getKeyValue(EB_EVENT, header.event);
    msg.getKeyValue(EB_ERROR, header.error);
    msg.getKeyValue(EB_ID, header.id);
}
uint32_t lastHeapSize=2000000;
void startHeap()
{
   if (   Sys::getFreeHeap()  < lastHeapSize)
       lastHeapSize = Sys::getFreeHeap();
}

void stopHeap(Actor* actor,const char* s)
{
    uint32_t heapSize = Sys::getFreeHeap();
    if ( heapSize < lastHeapSize ) {
        if ( actor)  {
            INFO ( " HEAP : %d:%d in actor : %s : %s " ,heapSize,heapSize-lastHeapSize,actor->name() ,s);
        }

        else {
            INFO ( " HEAP :  %d:%d in static function :%s " ,heapSize,heapSize-lastHeapSize,s);
        }
    }
}


void EventBus::eventLoop()
{

    static uint32_t lastHeap=0;

    lastHeap = Sys::getFreeHeap();
    if ((_queue.get(_rxd) == 0)) {  // handle all events
        uint32_t subscribers = 0;
        getHeader(_rxdHeader);
        for (EventFilter* filter = firstFilter(); filter;
             filter = filter->next()) {  // handle all matching filters
            if (filter->match(_rxdHeader)) {
                for (Subscriber* sub = filter->firstSubscriber(); sub != 0; sub = sub->next()) {
                    startTimer();
                    if (sub->_actor == 0) {
                        startHeap();
                        sub->_staticHandler(_rxd);
                        stopHeap(0,"static");
                        stopTimer("static function");
                    } else {
                        if (sub->_methodHandler == 0) {
                            startHeap();
                            sub->_actor->onEvent(_rxd);
                            stopHeap(sub->_actor,"method");
                        } else {
                            startHeap();
                            CALL_MEMBER_FUNC(sub->_actor, sub->_methodHandler)(_rxd);
                            stopHeap(sub->_actor,"member");
                        }
                        stopTimer(sub->_actor->name());
                    }
                }
                subscribers++;
            }
        }
        if (subscribers == 0) DEBUG(" no subscribers for this event ");
    }

    startTimer();
    for (Actor* actor = Actor::first(); actor;
         actor = actor->next()) {  // handle all actor timeouts

        if (actor->timeout()) {
            _rxd = *timeoutEvent;
            startHeap();
            actor->onEvent(_rxd);
            stopHeap(actor,"timeout");
            stopTimer(actor->name());
        }

    }
}
//____________________________________________________________________
//
EventFilter& EventBus::addFilter(Header& h)
{
    if (_firstFilter == 0) {
        _firstFilter = new EventFilter(h);
        return *_firstFilter;
    } else {
        EventFilter* cursorFilter = findFilter(h);
        if (cursorFilter == 0) {
            cursorFilter = lastFilter()->_nextFilter = new EventFilter(h);
        }
        return *cursorFilter;
    }
}
//____________________________________________________________________
//
EventFilter* EventBus::findFilter(Header& h)
{
    for (EventFilter* ef = firstFilter(); ef; ef = ef->next()) {
        if (memcmp(&h, &ef->_pattern, sizeof(Header)) == 0) return ef;
    }
    return 0;
}
//_______________________________________________________________________E V E
// NT F I L T E R _______________________________________

EventFilter::EventFilter(Header& h) : _firstSubscriber(0), _nextFilter(0)
{
    memcpy(&_pattern, &h, sizeof(Header));
}
//_______________________________________________________________________________________________
//
bool EventFilter::match(Header& header)
{
    for (int i = 0; i < HEADER_COUNT; i++) {
        if ((_pattern.uid[i] == EB_UID_REMOTE) && (header.uid[i] != 0) &&
            (Actor::findById(header.uid[i]) == 0))
            DEBUG("remote event [%d] pattern:%d, header:%d, id : %d", i,
                  _pattern.uid[i], header.uid[i], header.id);
        if ((_pattern.uid[i] == EB_UID_IGNORE) ||
            (_pattern.uid[i] == header.uid[i]) ||
            (_pattern.uid[i] == EB_UID_ANY && header.uid[i] != 0) ||
            (_pattern.uid[i] == EB_UID_LOCAL &&
             (Actor::findById(header.uid[i]) != 0)) ||
            (_pattern.uid[i] == EB_UID_REMOTE && (header.uid[i] != 0) &&
             (Actor::findById(header.uid[i]) == 0)))
            continue;
        return false;
    }
    return true;
}
/*
bool EventFilter::match(Cbor& cbor)
{
    if ( _type == EF_ANY ) return true;
    if ( _type == EF_EVENT ) {
        return isEvent(cbor,_object,_value);
    } else if ( _type == EF_REPLY ) {
        return isReply(cbor,_object,_value);
    } else if ( _type == EF_REQUEST ) {
        return isRequest(cbor,_object,_value);
    } else if ( _type == EF_KV ) {
        uid_t v;
        if ( cbor.getKeyValue(_object,v) &&  (_value==v || _value==0 )   )
            return true;
        return false;
    } else if ( _type==EF_REMOTE ) {
        uid_t dst;
        if ( cbor.getKeyValue(EB_DST_DEVICE,dst) && dst != H(Sys::hostname()))
            return true;
    } else if ( _type==EF_REMOTE_DST ) {
        uid_t dst_dev,dst;
        if ( cbor.getKeyValue(EB_DST_DEVICE,dst_dev) && dst_dev == _object &&
cbor.getKeyValue(EB_DST,dst) && dst == _value)
            return true;
        } else if ( _type==EF_REMOTE_SRC ) {
        uid_t src_dev,src;
        if ( cbor.getKeyValue(EB_SRC_DEVICE,src_dev) && src_dev == _object &&
cbor.getKeyValue(EB_SRC,src) && src == _value)
            return true;
        }
    return false;
}
 * */
//_______________________________________________________________________________________________
//

//_______________________________________________________________________________________________
//
bool EventFilter::isEvent(Cbor& cbor, uid_t src, uid_t ev)
{
    uid_t _src, _event;
    if (cbor.getKeyValue(EB_EVENT, _event) && cbor.getKeyValue(EB_SRC, _src)) {
        if ((_event == ev || _event == 0 || ev == 0) &&
            (_src == src || _src == 0 || src == 0))
            return true;
    }
    return false;
}
//_______________________________________________________________________________________________
//
bool EventFilter::isReply(Cbor& cbor, uid_t src, uid_t req)
{
    uid_t _src, _req;
    if (cbor.getKeyValue(EB_REPLY, _req) && cbor.getKeyValue(EB_SRC, _src)) {
        if ((_req == req || _req == 0 || req == 0) &&
            (_src == src || _src == 0 || src == 0))
            return true;
    }
    return false;
}
//_______________________________________________________________________________________________
//
bool EventFilter::isReplyCorrect(Cbor& cbor, uid_t src, uid_t req)
{
    uid_t _src, _req;
    uint32_t error;
    if (cbor.getKeyValue(EB_REPLY, _req) && cbor.getKeyValue(EB_SRC, _src) &&
        cbor.getKeyValue(EB_ERROR, error)) {
        if ((_req == req || _req == 0 || req == 0) &&
            (_src == src || _src == 0 || src == 0) && (error == 0))
            return true;
    }
    return false;
}
//_______________________________________________________________________________________________
//
bool EventFilter::isRequest(Cbor& cbor, uid_t dst, uid_t req)
{
    uid_t _dst, _req;
    if (cbor.getKeyValue(EB_REQUEST, _req) && cbor.getKeyValue(EB_DST, _dst)) {
        if ((_req == req || _req == 0 || req == 0) &&
            (_dst == dst || _dst == 0 || dst == 0))
            return true;
    }
    return false;
}
//________________________________________________________ S U B S C R I B E R
//_______________________________________
//

Subscriber* EventFilter::addSubscriber()
{
    Subscriber* cursorSubscriber;

    if (_firstSubscriber == 0) {
        cursorSubscriber = _firstSubscriber = new Subscriber();
    } else {
        cursorSubscriber = lastSubscriber()->_nextSubscriber = new Subscriber();
    }
    return cursorSubscriber;
}
//_______________________________________________________________________________________________
//

//____________________________________________________________________
//
void EventFilter::call(Actor* instance, MethodHandler handler)
{
    Subscriber* sub = addSubscriber();
    sub->_actor = instance;
    sub->_methodHandler = handler;
}
//____________________________________________________________________
//
void EventFilter::call(StaticHandler handler)
{
    Subscriber* sub = addSubscriber();
    sub->_staticHandler = handler;
    sub->_actor = 0;
}
//____________________________________________________________________
//
void EventFilter::call(Actor* instance)
{
    call(instance, 0);
}

EventFilter* EventFilter::next()
{
    return _nextFilter;
}

//____________________________________________________________________
//
EventFilter* EventBus::lastFilter()
{
    for (EventFilter* ef = firstFilter(); ef != 0; ef = ef->next()) {
        if (ef->_nextFilter == 0) return ef;
    }
    ASSERT(false);  // shouldn't arrive here
    return 0;
}
//____________________________________________________________________
//
Subscriber* EventFilter::firstSubscriber()
{
    return _firstSubscriber;
}
Subscriber* Subscriber::next()
{
    return _nextSubscriber;
}
//____________________________________________________________________
//
Subscriber* EventFilter::lastSubscriber()
{
    for (Subscriber* sub = firstSubscriber(); sub; sub = sub->next()) {
        if (sub->next() == 0) return sub;
    }
    ASSERT(false);  // shouldn't come here
    return 0;
}

//____________________________________________________________________
//

Subscriber::Subscriber() : _nextSubscriber(0) {}
