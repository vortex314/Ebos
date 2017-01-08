#include "Uid.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

extern char* static_labels[];
extern uint32_t static_labels_count;

Uid::Uid(uint32_t max)
{
	_max=max;
	_uids = new uid_t[_max];
	_labels = new const char*[_max];
	for(int i=0; i< _max; i++) {
		_uids[i]=0;
	}
	ASSERT(max > static_labels_count);
	for(int i=0; i<static_labels_count ; i++) {
		_uids[i]=H(static_labels[i]);
		_labels[i] = static_labels[i];
	}
	_dynamic_start = static_labels_count;
	_dynamic_current = static_labels_count;
}

Uid::~Uid()
{
}

uid_t Uid::hash(Str& str)
{
	uint32_t hh = FNV_OFFSET;
	str.offset(0);
	while (str.hasData()) {
		hh = hh * FNV_PRIME;
		hh = hh ^ str.read();
	}
	uid_t uid= hh & FNV_MASK;
	if ( uidIndex(uid) < 0 ) newLabel(str,uid);
	return uid;
}



uid_t Uid::newLabel(Str& str,uid_t uid)
{
	LOGF(" >>>>>>>>>>>>>>>>>>>>>>>>>>< new label %s:%d at %d ",str.c_str(),uid,_dynamic_current);
	if ( _uids[_dynamic_current]) {
		LOGF(" free ");
		free((void*)_labels[_dynamic_current]); // free old label
	}
	_uids[_dynamic_current]=uid;
	const char* label=(const char*) malloc(str.length()+1);
	_labels[_dynamic_current]=label;
	strncpy((char*)label,(char*)str.data(),str.length()+1);
	LOGF(" label : ----%s = %d---- at %d ",label,_uids[_dynamic_current],_dynamic_current);
	if ( _dynamic_current++ == _max) { // overwrite older labels
		_dynamic_current=_dynamic_start;
	}
}

int Uid::uidIndex(uid_t uid)
{
//	LOGF("Uid max : %d static_labels : %d  dynam : %d",_max,static_labels_count,_dynamic_current);
	for(int i=0; i<_max; i++) {
//		LOGF("%d:%s",_uids[i],_labels[i]);
		if(_uids[i]==uid) {
			return i;
		} else if (_uids[i]==0) {
			LOGF(" %d not found after %d",uid,i);
			break;
		}
	}
	return -1;
}

char uid_str[20];

const char* Uid::label(uid_t uid)
{
	int index = uidIndex(uid);
	if ( index < 0 ) {
		sprintf(uid_str,"%d",uid);
		return uid_str;
	} else return _labels[index];
}
