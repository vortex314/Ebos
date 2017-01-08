/*
 * SlipStream.h
 *
 *  Created on: 24-sep.-2016
 *      Author: lieven2
 */

#ifndef SLIPSTREAM_H_
#define SLIPSTREAM_H_

#include <stdint.h>
#include <Bytes.h>
#include <ByteStream.h>
#include <EventBus.h>

typedef uint8_t byte;
typedef struct {
	byte b[2];
} Crc;

class SlipStream: public Bytes, public Actor {
	bool _escaped;
	uint32_t _error_bad_crc;
	ByteStream& _stream;
	uint16_t _src;
public:
	SlipStream(int size, ByteStream& stream);
	virtual ~SlipStream();
	enum Magic {
		ESC = 0xDB, END = 0xC0, ESC_ESC = 0xDD, ESC_END = 0xDC
	};
	void loop();

	//PUBLIC
	//_________________________________________________________________________

	void streamWriteCrc(Bytes& bytes);
	Crc Fletcher16(uint8_t *begin, int length);
	bool isGoodCrc();
	void removeCrc();
	void streamWriteEscaped(uint8_t b);
	void send(Bytes& bytes);
	void onRecv(uint8_t b);
	void src(uint16_t src) {_src=src;};
	void setup();
	void onEvent(Cbor& cbor);
};

#endif /* SLIPSTREAM_H_ */
