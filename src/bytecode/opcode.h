#ifndef __OPCODE_H
#define __OPCODE_H

#include "../../logger.h"

#define OP_HALT 0x00

#define OP_CONST 0x01

#define OP_ADD 0x02
#define OP_SUB 0x03
#define OP_MUL 0x04
#define OP_DIV 0x05


#define OP_COMPARE 0x06

#define OP_JMP_IF_FALSE 0x07
#define OP_JMP 0x08

#define OP_GET_GLOBAL 0x09

#define OP_SET_GLOBAL 0x10

#define OP_POP 0x11

#define OP_STR(op) \
    case OP_##op: \
        return #op

std::string opcodeToString(uint8_t opcode) {
    switch (opcode)
    {
        OP_STR(HALT);
        OP_STR(CONST);
        OP_STR(ADD);
        OP_STR(SUB);
        OP_STR(MUL);
        OP_STR(DIV);
        OP_STR(COMPARE);
        OP_STR(JMP_IF_FALSE );
        OP_STR(JMP);
        OP_STR(GET_GLOBAL);
        OP_STR(SET_GLOBAL);
        OP_STR(POP);
        
        default:
            DIE << "opcodetosting: unknow opcode " << (int)opcode;
    }
    return "Unknow";
}

#endif