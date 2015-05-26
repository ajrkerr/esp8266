#include "json.h"

JSONNode::JSONNode() {
  next = NULL;
  parent = NULL;
}

JSONNode::~JSONNode() {
  if(next != NULL) {
    delete next;
  }
}

JSONNode* JSONNode::integer(String key, int value) {
  next = new JSONInteger(key, value);;
  next->setParent(parent);
  return next;
}

JSONNode* JSONNode::string(String key, String value) {
  next = new JSONString(key, value);;
  next->setParent(parent);
  return next;
}

JSONNode* JSONNode::boolean(String key, bool value) {
  next = new JSONBoolean(key, value);;
  next->setParent(parent);
  return next;
}

JSONNode* JSONNode::object(String key) {
  JSONObject *object = new JSONObject(key);
  next = object;
  next->setParent(parent);
  return object->getChild();
}

void JSONNode::setParent(JSONNode *aParent) {
  parent = aParent;
}

JSONNode* JSONNode::done() {
  return parent;
}

String JSONNode::toString() {
  return "";
}

// JSON Root
String JSONRoot::toString() {
  if(next == NULL) {
    return "{}";
  } else {
    return "{" + next->toString() + "}";
  }
}


// JSON Integer
JSONInteger::JSONInteger(String aKey, int aValue) {
  key = aKey;
  value = aValue;
}

String JSONInteger::toString() {
  String nodeString = "\"" + String(key) + "\": " + String(value);
  if(next == NULL) {
    return nodeString;  
  } else {
    return nodeString + ", " + next->toString();
  }
}

// JSON Object
JSONObject::JSONObject(String aKey) {
  key = aKey;
  child = new JSONRoot();
  child->setParent(this);
}

JSONObject::~JSONObject() {
  if(next != NULL) {
    delete next;
  }

  if(child != NULL) {
    delete child;
  }
}

JSONRoot* JSONObject::getChild() {
  return child;
}

String JSONObject::toString() {
  String nodeString = "\"" + key + "\": " + child->toString();
  if(next == NULL) {
    return nodeString;
  } else {
    return nodeString + ", " + next->toString();
  }
}

// JSON String
JSONString::JSONString(String aKey, String aValue) {
  key = aKey;
  value = aValue;
}

String JSONString::toString() {
  value.replace("\"", "\\\"");
  String nodeString = "\"" + String(key) + "\": \"" + value + "\"";
  if(next == NULL) {
    return nodeString;  
  } else {
    return nodeString + ", " + next->toString();
  }
}


// JSON Boolean
JSONBoolean::JSONBoolean(String aKey, bool aValue) {
  key = aKey;
  value = aValue;
}

String JSONBoolean::toString() {
  String nodeString = "\"" + String(key) + "\": " + ((value)? "true" : "false");
  if(next == NULL) {
    return nodeString;  
  } else {
    return nodeString + ", " + next->toString();
  }
}
