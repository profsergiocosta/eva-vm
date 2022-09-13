/*
Eva virtual Machine

*/

#ifndef __EVAVM_H
#define __EVAVM_H


#include <string>
#include <vector>

#include "../bytecode/opcode.h"


#define READ_BYTE() *ip++

class EvaVM {
    public:
        EvaVM () {

        }

        void exec (const std::string & program) {

            code = {OP_HALT};

            ip = &code[0];

            return eval();
        }

        void eval () {
            for (;;) {
                switch (READ_BYTE())
                {
                case OP_HALT:
                    return;
                
                default:
                    break;
                }
            }
        }

        // instruction pointer
        uint8_t* ip;

        std::vector<uint8_t> code;
};

#endif