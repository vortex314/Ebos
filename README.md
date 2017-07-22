# Eventbus Operating System aka EBOS
Ebos is an eventbus that can replace most of what an RTOS is offering today.
Lots of issues arise when using an RTOS, mostly related to modifying data shared between threads. An RTOS also comes with an higher consumption of RAM to assure stackspace for all the different tasks.

## Principles
Eventbus is nothing else then a list of callbacks triggered depending on the events that have been generated. A single thread is calling all these callback functions.
Events can be purely aligned with a publish/subscribe pattern , with listeners waiting for some topics to be published. 
Events can be purely aligned to request/reply pattern, where one actor is sending requests to another and waits asynchronously for an answer.
The interface for an actor/service to send/receive messages for a local or remote communication is identical. Except for the public events
Each Actor decides to what it subscribes.
An actor can subscribe specific methods to specific events.
Events can be subscribed to based on the below fields.
## Design
Messages contain the following data depending on the use case :
- mandatory : eventType #request|#reply|#event 
- mandatory : source field #src
- #dst, error ( int errno.h ) , id ( unique id of message for correlation )

Fields have been enriched with a leading character to ease translation from JSON to CBOR.
Fields starting with a '%' : indicate that an string enum field is following that can be hashed to pass on the EB.
Fields starting with a '$' : indicate that a string hex field is following, will be transformed to a ByteArray.
Fields starting with a '@' : indicate that a string in base64 is following, will be transformed to a ByteArray
To ease state maintenance the services can be implemented with ProtoThreads
Each actor has a unique address indicated by a 16bit word.
Each actor has a unque name which consists of a logical name concatenated to the local device name.
The name and unique id are linked as the id is calculated as an hash on the string. Util functions support the 2-way translation.
The String name is used to communicate to/from mqtt.
The Eventbus message layout is a CBOR serialization, while using KeyValue pairs. The Key is a hash on the name of the fields. This avoids having a central symbol table.

 
# EventBus - JSON MQTT Bridge

Eventbusses will be bridged by the use of MQTT.
In Request/reply pattern full JSON objects are used.


## MQTT request - reply
dst/<service> : { json_request }
mqtt.subscribe("dst/<service>")
actor.setPublic(true);

## service discovery
src/services/<interface> : <service>
eb.event(H("services"),H("memory")).addKeyValue(H("#data"),id());

## property published
src/<service>/<property> : value

property : #name : enum , hash for value
 		| $name : string containing hex
		| @name : string base64 for hex

property value : boolean
	| number
	| string
	| null
router.publish(H("src"),H("event"))


## eb to mqtt
- request / reply transparent
- publish(<service>).addKV(property,value)
- subscribe(me()).onDst(), onSrc()

eb.event(Id(),H("euid")).addKV(H("$data"),&euid);

eb.onEvent(H("services"),H("memory"))-> src/services/memory. 

eb.onEvent(H("world"),H("UTC"))-> src/world/UTC. 

src=world,event=UTC,data=utc

eb.onEvent(H("local_actor")) -> (local_actor,ANY). 

eb.onEvent(H("remote_actor")) -> src/remote_actor/#. 
		-> H("remote_Actor"),H("event");  
		
router
	- setPublic(H("src"),H("event"))) 
		-> onSrc(s,e).call(ebToMqtt);
	-  
setup
	- onRemote().call(ebToMqtt);
	
loop
	- establish connection
	- register all subscribers for external services
		- check external subscriptions in eb tables and subscribe
		- check all public actors and subscribe : dst/actor
ebToMqtt  
	- push remote events
	- route external events
mqttToEb 
	- external events to internal serialization
	- 




