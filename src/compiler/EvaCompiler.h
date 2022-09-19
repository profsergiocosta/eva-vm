#ifndef EVACOMPILER_H
#define EVACOMPILER_H

#include "../parser/EvaParser.h"
#include "../vm/EvaValue.h"
#include "../bytecode/opcode.h"

class EvaCompiler
{
private:
    /* data */
public:
    EvaCompiler(/* args */);
    ~EvaCompiler();

    CodeObject *compile(const Exp &exp);
    void gen (const Exp &exp);

    private:
        void emit (uint8_t code) {
            co->code.push_back(code);

        }

        


        size_t stringConstIdx(const std::string& value) {
            for (auto i=0; i < co->constants.size(); i++) {
                if (!IS_STRING(co->constants[i])) {
                    continue;
                }
                if (AS_CPPSTRING (co->constants[i])== value) {
                    return i;
                }
            }
            co->constants.push_back(ALLOC_STRING(value));
            return co->constants.size() -1;

        }

        size_t numericConstIdx(double value) {
            for (auto i=0; i < co->constants.size(); i++) {
                if (!IS_NUMBER(co->constants[i])) {
                    continue;
                }
                if (AS_NUMBER (co->constants[i])== value) {
                    return i;
                }
            }
            co->constants.push_back(NUMBER(value));
            return co->constants.size() -1;

        }
    CodeObject *co;
};

EvaCompiler::EvaCompiler(/* args */)
{
}

EvaCompiler::~EvaCompiler()
{
}

CodeObject *EvaCompiler::compile(const Exp &exp)
{
    co = AS_CODE(ALLOC_CODE("main"));

    gen(exp);

    emit(OP_HALT);

    return co;
}

void EvaCompiler::gen(const Exp &exp)
{
    switch (exp.type)
    {
    case ExpType::NUMBER:
        /* code */
        emit(OP_CONST);
        emit(numericConstIdx(exp.number));
        break;

    case ExpType::STRING:
        /* code */
        emit(OP_CONST);
        emit(stringConstIdx(exp.string));
        break;
    
    default:
        break;
    }
}

#endif