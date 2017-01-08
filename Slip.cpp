#include "Slip.h"

Slip::Slip(uint32_t size) :
		Bytes(size) {
	_escaped = false;
}

Slip::~Slip() {
	//dtor
}

//PUBLIC
//_________________________________________________________________________

Slip& Slip::addCrc() //PUBLIC
//_________________________________________________________________________
{
	AddCrc(*this);
	return *this;
}

void Slip::AddCrc(Bytes& bytes) {
	bytes.offset(-1); // position at end
	uint16_t crc = Fletcher16(bytes.data(), bytes.used());
	bytes.write(crc >> 8);
	bytes.write(crc & 0xFF);
}

uint16_t Slip::Fletcher16(uint8_t *begin, int length) {
	uint16_t sum1 = 0;
	uint16_t sum2 = 0;
	uint8_t *end = begin + length;
	uint8_t *index;

	for (index = begin; index < end; index++) {
		sum1 = (sum1 + *index) % 255;
		sum2 = (sum2 + sum1) % 255;
//		LOGF(" %X , %X ", sum1, sum2);
	}

	return (sum2 << 8) | sum1;
}
//_________________________________________________________________________

bool Slip::isGoodCrc() //PUBLIC
//_________________________________________________________________________
{
	if (_limit < 3)
		return false; // need at least 3 bytes
	uint16_t crc = Fletcher16(_start, _limit - 2);
	if ((*(_start + _limit - 2) == (crc >> 8))
			&& ((*(_start + _limit - 1) == (crc & 0xFF))))
		return true;
	return false;
}

//_________________________________________________________________________

Slip& Slip::removeCrc() //PUBLIC
//_________________________________________________________________________
{
	_limit -= 2;
	return *this;
}
//PUBLIC

//PUBLIC
//_________________________________________________________________________
Slip& Slip::decode() //PUBLIC
//_________________________________________________________________________
{
	uint8_t *p, *q;
	uint8_t *_capacity = _start + _limit;
	for (p = _start; p < _capacity; p++) {
		if (*p == ESC) {
			if (*(p + 1) == ESC_ESC)
				*p = ESC;
			else if (*(p + 1) == ESC_END)
				*p = END;
//			*p = (uint8_t) (*(p + 1) ^ 0x20);
			for (q = p + 1; q < _capacity; q++)
				*q = *(q + 1);
			_capacity--;
			//		p++; // skip next uint8_t could also be an escape
		}
	}
	_limit = _capacity - _start;
	return *this;
}
/*
 #define END 0xC0
 #define ESC 0xDB
 */
//_________________________________________________________________________
Slip& Slip::encode() //PUBLIC
//_________________________________________________________________________
{
	Encode(*this);

	return *this;
}

void Slip::Encode(Bytes& bytes){
	uint8_t *p, *q;
	uint8_t *end = bytes._start +bytes._limit;
	for (p = bytes._start; p < end; p++) {
		if ((*p == END) || (*p == ESC)) {
			for (q = end; q > p; q--)
				*(q + 1) = *q;
			end++;
			if (*p == END)
				*(p + 1) = ESC_END;
			else
				*(p + 1) = ESC_ESC;
			*p = ESC;
		}
	}
	bytes._limit = end - bytes._start;
}

//_________________________________________________________________________

Slip& Slip::frame() //PUBLIC
//_________________________________________________________________________
{
	uint8_t *q;
	uint8_t *end = _start + _limit;
	for (q = end; q >= _start; q--)
		*(q + 1) = *q;
	*_start = END;
	*(end + 1) = END;
	end += 2;
	_limit = end - _start;
	return *this;
}

void Slip::Frame(Bytes& bytes){
	uint8_t *q;
		uint8_t *end = bytes._start + bytes._limit;
		for (q = end; q >= bytes._start; q--)
			*(q + 1) = *q;
		*bytes._start = END;
		*(end + 1) = END;
		end += 2;
		bytes._limit = end - bytes._start;
}

void Slip::reset() {
	clear();
	_escaped = false;
}

bool Slip::fill(uint8_t b) {
	if (b == END) {
		if (offset() > 0)
			return true;
		else {
			return false; // don't add SOF
		}
	} else if (b == ESC) {
		_escaped = true;
	} else if (b == ESC_ESC && _escaped) {
		write(ESC);
		_escaped = false;
	} else if (b == ESC_END && _escaped) {
		write(END);
		_escaped = false;
	} else {
		write(b);
	}
	return false;
}

