#ifndef EVAVALUE_H
#define EVAVALUE_H

#include <string>

enum class EvaValueType {
    NUMBER,
    OBJECT,
 
};

enum class ObjectType {
    STRING,
    CODE,
};

struct Object {
    Object(ObjectType type) : type (type) {}
    ObjectType type;
};

struct StringObject: public Object {
    StringObject(const std::string &str) :  Object(ObjectType::STRING), string(str) {}
    std::string string;
};

struct EvaValue {
    EvaValueType type;
    union {
        double number;
        Object* object;
    };
};

struct CodeObject: public Object {

    CodeObject(const std::string& name) : Object(ObjectType::CODE), name (name) {}

    std::string name;

    /*
     constant pool
     */

    std::vector<EvaValue> constants;

    
    std::vector<uint8_t> code;
};

// constructor
#define NUMBER(value) ((EvaValue){EvaValueType::NUMBER, .number = value})

#define ALLOC_STRING(value) ((EvaValue){EvaValueType::OBJECT, .object= (Object*)new StringObject(value)})

#define ALLOC_CODE(name) ((EvaValue){EvaValueType::OBJECT, .object= (Object*)new CodeObject(name)})


// acessor
#define AS_NUMBER(evaValue) ((double)(evaValue).number)
#define AS_STRING(evaValue) ((StringObject*)(evaValue).object)
#define AS_CPPSTRING(evaValue) (AS_STRING(evaValue)->string)
#define AS_OBJECT(evaValue) ((Object*)(evaValue).object)

#define AS_CODE(evaValue) ((CodeObject*)(evaValue).object)

// tester
#define IS_NUMBER(evaValue) ((evaValue).type == EvaValueType::NUMBER)
#define IS_OBJECT(evaValue) ((evaValue).type == EvaValueType::OBJECT)

#define IS_OBJECT_TYPE(evaValue, objectType) (IS_OBJECT(evaValue) && AS_OBJECT(evaValue)->type == objectType )

#define IS_STRING(evaValue) (IS_OBJECT_TYPE(evaValue, ObjectType::STRING))
#define IS_CODE(evaValue) (IS_OBJECT_TYPE(evaValue, ObjectType::CODE))



#endif
