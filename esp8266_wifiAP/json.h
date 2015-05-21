#ifndef JSON_H
#define JSON_H

#include <Arduino.h>

class JSONNode {
protected:
  String key;
  JSONNode *next;
  JSONNode *parent;

public:
  JSONNode();
  ~JSONNode();

  JSONNode* integer(String key, int value);
  JSONNode* string(String key, String value);
  JSONNode* boolean(String key, bool value);
  JSONNode* object(String key);

  void setParent(JSONNode *aParent);
  JSONNode* done();

  virtual String toString();
};


class JSONRoot : public JSONNode {
public:
  String toString();
};


class JSONInteger : public JSONNode {
  int value;

public:
  JSONInteger(String aKey, int aValue);
  String toString();
};


class JSONString : public JSONNode {
  String value;

public:
  JSONString(String aKey, String aValue);
  String toString();
};


class JSONBoolean : public JSONNode {
  bool value;

public:
  JSONBoolean(String aKey, bool aValue);
  String toString();
};


class JSONObject : public JSONNode {
  JSONRoot *child;

public:
  JSONObject(String aKey);
  ~JSONObject();
  JSONRoot* getChild();
  String toString();
};

#endif