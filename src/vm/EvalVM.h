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

#define READ_BYTE() *ip++

#define GET_CONST() constants[READ_BYTE()]

#define STACK_LIMIT 512

class EvaVM {
    public:
        EvaVM () {

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

            constants.push_back(NUMBER(2));
            constants.push_back(NUMBER(3));

            code = {OP_CONST,0,OP_CONST,1, OP_ADD, OP_HALT};

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
                    auto op1 = AS_NUMBER (pop());
                    auto op2 = AS_NUMBER (pop());
                    auto result = op1 + op2;
                    push(NUMBER(result));
                    break;

                }

                default:
                    DIE << "unknow code : " << std::hex << +opcode;
                }
            }
        }

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