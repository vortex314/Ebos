#include "Uid.h"
#include <Log.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

Uid::Uid(uint32_t max) {
  _max = max;
  _uids = new uid_t[_max];
  _labels = new const char*[_max];
  for (uint32_t i = 0; i < _max; i++) {
    _uids[i] = 0;
  }
  _index = 0;
}

Uid::~Uid() {}

uid_t Uid::hash(const char* str) {
  return add(str);
}

uid_t Uid::hash(Str& str) {
  return add(str.c_str());
}

int Uid::uidIndex(uid_t uid) {
  for (uint32_t i = 0; i < _index; i++) {
    if (_uids[i] == uid) {
      return i;
    }
  }
  return -1;
}

char uid_str[20];

const char* Uid::label(uid_t uid) {
  if (uid == 0) return "0";
  int index = uidIndex(uid);
  if (index < 0) {
    sprintf(uid_str, "%d", uid);
    return uid_str;
  } else
    return _labels[index];
}

void Uid::add(const char** list, uint32_t max) /* sizeof(list)/sizeof(char*)*/
{
  INFO(" added %d labels to %d ", max, _index);
  for (uint32_t i = 0; i < max; i++) add(list[i]);
}

uid_t Uid::add(const char* s) {
  uid_t uid = H(s);
  if (uidIndex(uid) < 0) {
    if (_index < _max) {
      _labels[_index] = s;
      _uids[_index] = uid = H(s);
      _index++;
    } else {
      WARN(" too many labels, cannot add '%s'", s);
    }
  };
  return uid;
}

uid_t Uid::create(Str& str) {
  uid_t uid = H(str.c_str());
  if (uidIndex(uid) < 0) {
    const char* label = (const char*)malloc(str.length() + 1);
    strcpy((char*)label, str.c_str());
    uid = add(label);
    INFO(" new label '%s' : %d ", label, uid);
  }
  return uid;
}

uid_t Uid::create(const char* s) {
  uid_t uid = H(s);
  if (uidIndex(uid) < 0) {
    const char* label = (const char*)malloc(strlen(s) + 1);
    if (label) {
      strcpy((char*)label, s);
      uid = add(label);
    }
  }
  return uid;
}
