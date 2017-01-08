#ifndef UID_H
#define UID_H

#include <Str.h>

typedef uint16_t uid_t;
#define UID_LENGTH 16
#define UID_MAX 100

#if UID_LENGTH==16
#define FNV_PRIME  16777619
#define FNV_OFFSET 2166136261
#define FNV_MASK	0xFFFF
#endif

#if UID_LENGTH==32
#define FNV_PRIME  16777619
#define FNV_OFFSET 2166136261
#define FNV_MASK	0xFFFFFFFFu
#endif

#if UID_LENGTH==64
#define FNV_PRIME 1099511628211ull
#define FNV_OFFSET 14695981039346656037ull
#endif

constexpr uint32_t fnv1(uint32_t h, const char* s)
{
	return (*s == 0) ?
	       h : fnv1((h * FNV_PRIME) ^ static_cast<uint32_t>(*s), s + 1);
}

constexpr uint16_t H(const char* s)
{
//    uint32_t  h = fnv1(FNV_OFFSET, s) ;
	return (fnv1(FNV_OFFSET, s) & FNV_MASK);
}

#define UID(xxx) H(xxx)


class Uid
{
	uint32_t _max;
	uid_t* _uids;
	const char** _labels;
	uint32_t _dynamic_start;
	uint32_t _dynamic_current;
public:
	Uid(uint32_t max);
	~Uid();
//	 uid_t hash(const char* s);
	uid_t hash(Str& str);
	const char* label(uid_t id);
//	 const char* label(uint32_t id);
	uid_t newLabel(Str& str ,uid_t uid);
	int uidIndex(uid_t id);

};

extern Uid uid;

/*
 * **
 * ----- not good method -- makes compiler slow or run out of memory
 */
/*
#define H1(s,i,x)   (x*65599u+(uint8_t)s[(i)<sizeof(s)?sizeof(s)-1-(i):sizeof(s)])
#define H4(s,i,x)   H1(s,i,H1(s,i+1,H1(s,i+2,H1(s,i+3,x))))
#define H16(s,i,x)  H4(s,i,H4(s,i+4,H4(s,i+8,H4(s,i+12,x))))
#define H64(s,i,x)  H16(s,i,H16(s,i+16,H16(s,i+32,H16(s,i+48,x))))
#define H256(s,i,x) H64(s,i,H64(s,i+64,H64(s,i+128,H64(s,i+192,x))))
#define HASH(s)    (uint16_t)((uint32_t)(H256(s,0,0)^(H256(s,0,0)>>16)))
#define H_OLD(s)    (uint16_t)((uint32_t)(H256(s,0,0)^(H256(s,0,0)>>16)))

//#define H(__s__) HASH(__s__)
*/

/** doesn't always precompile **/
/*
uint16_t constexpr HH(char const *input) {
	return *input ? static_cast<uint16_t>(*input) + 33 * HH(input + 1) : 5381;
}
*/

#endif // UID_H
