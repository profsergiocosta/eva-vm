/*
Eva virtual Machine

*/

#ifndef __EVAVM_H
#define __EVAVM_H


#include <string>
#include <vector>
#include <iostream>

#include "../bytecode/opcode.h"
#include "../../logger.h"


#define READ_BYTE() *ip++

class EvaVM {
    public:
        EvaVM () {

        }

        void exec (const std::string & program) {

            code = {5};

            ip = &code[0];

            return eval();
        }

        void eval () {
            for (;;) {
                uint8_t opcode = READ_BYTE();
                log(opcode);   

                switch (opcode)
                {
                case OP_HALT:
                    return;
                
                default:
                    DIE << "unknow code : " << std::hex << +opcode;
                }
            }
        }

        // instruction pointer
        uint8_t* ip;

        std::vector<uint8_t> code;
};

#endif