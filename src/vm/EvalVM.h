/*
Eva virtual Machine

*/

#ifndef EVAVM_H
#define EVAVM_H


#include <string>
#include <vector>
#include <iostream>
#include <array>



#include "../bytecode/opcode.h"
#include "../../logger.h"
#include "EvaValue.h"
#include "../parser/EvaParser.h"
#include "../compiler/EvaCompiler.h"

using syntax::EvaParser;

#define READ_BYTE() *ip++

#define GET_CONST() co->constants[READ_BYTE()]

#define STACK_LIMIT 512

#define BINARY_OP(op) \
do {\
    auto op2 = AS_NUMBER (pop());\
    auto op1 = AS_NUMBER (pop());\
    push(NUMBER(op1 op op2));\
} while (false)

class EvaVM {
    public:
        EvaVM  () : parser(std::make_unique<EvaParser>()), compiler(std::make_unique<EvaCompiler>()) {

        }

        void push (const EvaValue& value) {
            if((size_t)(sp-stack.begin()  == STACK_LIMIT)) {
                 DIE << "push(): Stack overflow.\n"  ; 
            }

            *sp = value;
            sp++;
        }

        EvaValue pop () {
           /// if (stack.size() == 0) {
            if (sp == stack.begin()) {
                DIE << "pop(): empty stack.\n";
            }
            --sp;
            return *sp;
        }

        EvaValue exec (const std::string & program) {

            auto ast = parser->parse(program);

            co = compiler->compile(ast);

            ip = &co->code[0];
            sp = &stack[0];

            return eval();
        }

        EvaValue eval () {
            for (;;) {
                uint8_t opcode = READ_BYTE();

                switch (opcode)
                {
                case OP_HALT:
                    return pop();
                
                case OP_CONST: {
                    push (GET_CONST());
                    break;

                }

                case OP_ADD: {
                    //BINARY_OP(+);
                    auto op2 = pop();
                    auto op1 = pop();
                    if (IS_NUMBER(op1) && IS_NUMBER(op2)) {
                        auto v1 = AS_NUMBER(op1);
                        auto v2 = AS_NUMBER(op2);
                        push(NUMBER(v1+v2));

                    } else if (IS_STRING(op1) && IS_STRING(op2)) {
                        auto v1 = AS_CPPSTRING(op1);
                        auto v2 = AS_CPPSTRING(op2);
                        push(ALLOC_STRING(v1+v2));

                    }

                    break;

                }

                case OP_SUB: {
                    BINARY_OP(-);
                    break;

                }

                case OP_MUL: {
                    BINARY_OP(*);
                    break;

                }

                case OP_DIV: {
                    BINARY_OP(/);
                    break;

                }

                default:
                    DIE << "unknow code : " << std::hex << +opcode;
                }
            }
        }

        std::unique_ptr<EvaParser> parser;

        std::unique_ptr<EvaCompiler> compiler;

        // instruction pointer
        uint8_t* ip;

        /*
        constant pool
        */

    

        CodeObject* co;

       /*
       stack pointer
       */
      EvaValue* sp;

      std::array<EvaValue,STACK_LIMIT> stack;

    
};

#endif