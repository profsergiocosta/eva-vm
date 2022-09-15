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

using syntax::EvaParser;

#define READ_BYTE() *ip++

#define GET_CONST() constants[READ_BYTE()]

#define STACK_LIMIT 512

#define BINARY_OP(op) \
do {\
    auto op2 = AS_NUMBER (pop());\
    auto op1 = AS_NUMBER (pop());\
    push(NUMBER(op1 op op2));\
} while (false)

class EvaVM {
    public:
        EvaVM  () : parser(std::make_unique<EvaParser>()) {

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

            //auto ast = parser->parse("\"ola\"");
            auto ast = parser->parse(program);
            log(ast.number );
            std::cout << ast.list[0].string;
            

            std::cout << "valor " << ast.string << " aqui \n";
            std::cout << "valor " << ast.number << " aqui \n";

            constants.push_back(NUMBER(10));
            constants.push_back(NUMBER(3));

            code = {OP_CONST,0,OP_CONST,1, OP_SUB, OP_HALT};

            ip = &code[0];
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
                    BINARY_OP(+);
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

        // instruction pointer
        uint8_t* ip;

        /*
        constant pool
        */

       std::vector<EvaValue> constants;

       /*
       stack pointer
       */
      EvaValue* sp;

      std::array<EvaValue,STACK_LIMIT> stack;

       std::vector<uint8_t> code;
};

#endif