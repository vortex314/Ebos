#include "Uid.h"
#include <Log.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>


Uid::Uid(uint32_t max)
{
	_max=max;
	_maxConst=0;
	_uids = new uid_t[_max];
	_labels = new const char*[_max];
	for(uint32_t i=0; i< _max; i++) {
		_uids[i]=0;
	}
	_dynamic_current=0;
	_dynamic_start=0;
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
//	DEBUG(" >>>>>>>>>>>>>>>>>>>>>>>>>>< new label %s:%d at %d ",str.c_str(),uid,_dynamic_current);
	if ( _uids[_dynamic_current]) {
		DEBUG(" free ");
		free((void*)_labels[_dynamic_current]); // free old label
	}
	_uids[_dynamic_current]=uid;
	const char* label=(const char*) malloc(str.length()+1);
	_labels[_dynamic_current]=label;
	strncpy((char*)label,(char*)str.data(),str.length()+1);
//	DEBUG(" label : ----%s = %d---- at %d ",label,_uids[_dynamic_current],_dynamic_current);
	if ( _dynamic_current++ == _max) { // overwrite older labels
		_dynamic_current=_dynamic_start;
	}
	return uid;
}

int Uid::uidIndex(uid_t uid)
{
//	DEBUG("Uid max : %d static_labels : %d  dynam : %d",_max,static_labels_count,_dynamic_current);
	for(uint32_t i=0; i<_max; i++) {
//		DEBUG("%d:%s",_uids[i],_labels[i]);
		if(_uids[i]==uid) {
			return i;
		} else if (_uids[i]==0) {
//			DEBUG(" %d not found after %d",uid,i);
			break;
		}
	}
	return -1;
}

char uid_str[20];

const char* Uid::label(uid_t uid)
{
    if ( uid==0 ) return "0";
	int index = uidIndex(uid);
	if ( index < 0 ) {
		sprintf(uid_str,"%d",uid);
		return uid_str;
	} else return _labels[index];
}



void Uid::add(const char** list,uint32_t max)   /* sizeof(list)/sizeof(char*)*/
{
	for(uint32_t i=0; i<max;i++) 
		add(list[i]);
}

uid_t Uid::add(const char* s)
{
	uid_t uid;
	// check if exist
	for(uint32_t i=0; i<_maxConst; i++ ) {
		if ( strcmp(s,_labels[i])==0) return _uids[i];
	}
//	DEBUG(" adding %s ",s);
	// else add const char* , hash(s)
	if ( _maxConst < _max ) {
		_labels[_maxConst]=s;
		_uids[_maxConst]=uid=H(s);
		_maxConst++;
		_dynamic_current=_maxConst;
		_dynamic_start=_maxConst;
	}
	return uid;
}


