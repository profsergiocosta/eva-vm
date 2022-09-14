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

#define NUMBER(value) ((EvaValue){EvaValueType::NUMBER, .number = value})

#endif
