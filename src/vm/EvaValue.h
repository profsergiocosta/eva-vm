#ifndef EVAVALUE_H
#define EVAVALUE_H


enum class EvaValueType {
    NUMBER,
};

struct EvaValue {
    EvaValueType type;
    union {
        double number;
    };
};

// constructor
#define NUMBER(value) ((EvaValue){EvaValueType::NUMBER, .number = value})

// acessor
#define AS_NUMBER(evaValue) ((double)(evaValue).number)

#endif
