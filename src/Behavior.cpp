#include "Behavior.h"
#include "CApp.h"

Value::Value(const char* str) : type(STRING) {
    val.str = new char[strlen(str)+1];
    strcpy(val.str, str);
}

Value::~Value() { 
    clear(); 
}


Value::Value(const std::string& str)
: type(STRING) {
    val.str = new char[str.length()+1];
    strcpy(val.str, str.c_str());
}

Value::Value (const Value& other) : type(NIL) {
    this->operator=(other);
}

Value& Value::operator= (const Value& other) {
    if (this != &other) {
        if (other.type == STRING) {
            assign(other.val.str);
        } else {
	    clear();
            type = other.type;
            val = other.val;
        }
    }
    return *this;
}


void Value::assign(const char* s) {
    clear();
    type = STRING;
    val.str = new char[strlen(s)+1];
    strcpy(val.str, s);
}

void Value::assign(double d) { 
    clear(); type=DOUBLE; val.dval=d; 
}

void Value::assign(int i) { 
    clear(); type=INT; val.ival=i; 
}

void Value::clear() {
    if (type == STRING)
	delete[] val.str;
    type = NIL;
}

double Value::get_double() const {
    return val.dval;
}

int Value::get_int() const {
    return val.ival;
}

const char* Value::get_string() const {
    return val.str;
}

int to_int(const Value &v) {
    switch (v.get_type()) {
    case Value::DOUBLE: return round_nearest<int>(v.get_double());
    case Value::STRING: return atoi(v.get_string());
    default: return 0;
    }
}

bool to_bool(const Value &v) {
    return (v.get_type() != Value::NIL);
}

double to_double(const Value &v) {
    switch (v.get_type()) {
    case Value::DOUBLE: return v.get_double();
    case Value::STRING: return atof(v.get_string());
    default: return 0;
    }
}

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

const char * to_string(const Value &v) {
    static char buf[30];
    switch (v.get_type()) {
    case Value::NIL: return "";
    case Value::DOUBLE:
        snprintf(buf, sizeof(buf), "%f", v.get_double());
        return buf;
    case Value::STRING: return v.get_string();
    default: return 0;
    }
}

//Direction to_direction (const Value &v) {
    //int val = Clamp(to_int(v), 0, 3);
    //return static_cast<Direction>(val);
//}

std::ostream& operator<<(std::ostream& os, const Value& val) {
    switch (val.get_type()) {
    case Value::NIL:   os << "nil"; break;
    case Value::DOUBLE: os << val.get_double(); break;
    case Value::INT: os << val.get_int(); break;
    case Value::STRING: os << val.get_string(); break;
    }
    return os;
}

Behavior::Behavior() {}

// destroy 
Behavior::~Behavior() {}

// decide whether this entity will be part of the next loop or not
void Behavior::OnLoop(){} 

// animate the sprite of the entity
void Behavior::OnAnimate(){}

// render the entity on the display surface
void Behavior::OnRender(){}

// operations to be done at clean up time : free memory
void Behavior::OnCleanup(){}

void Behavior::OnTriggeredAction(int id){}

void Behavior::kill(){}

void Behavior::OnInit(){}

void Behavior::OnStop(){
  if(getAttribute(STOP_ACTION).get_int() == CALLBACK) { // a callback needs to be called when the entity stops moving
    Value entityId(e->getEntityId());
    CallFunc(luaState, getAttribute(STOP_TARGET).get_string(), entityId) ;
  }
}


