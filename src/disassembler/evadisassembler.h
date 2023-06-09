#ifndef EVADISASSEMBLER_H
#define  EVADISASSEMBLER_H

#include "../compiler/EvaCompiler.h"
#include "../../logger.h"

#include "../vm/Global.h"

#include <iomanip>

class EvaDisassembler {

    public:
    EvaDisassembler (std::shared_ptr<Global> global) : global(global) {}
    void printByteCode (CodeObject* co) {
        std::cout << "\n -------------- ByteCode: " << co->name
                    << " ----------------\n\n";

        size_t offset = 0;
        while (offset < co->code.size()) {
            std::cout << std::uppercase << std::hex  <<  std::setfill('0') << std::setw(4) 
                << (int)co->code[offset] << " ";
            //std::cout << "\n" ;
            offset++;
        }
    }

    void disassemble (CodeObject* co) {
        std::cout << "\n -------------- Disassembly: " << co->name
                    << " ----------------\n\n";

        size_t offset = 0;
        while (offset < co->code.size()) {
            offset = disassembleInstruction (co, offset);
            std::cout << "\n" ;
        }
    }

    size_t disassembleInstruction(CodeObject* co, size_t offset) {
        std::ios_base::fmtflags f(std::cout.flags());

        std::cout << std::uppercase << std::hex  <<  std::setfill('0') << std::setw(4) 
                << offset << "   ";

        auto opcode = co->code[offset];

        switch ( opcode ) 
        {
        case OP_HALT:
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
        case OP_POP:
             return disassembleSimple(co, opcode, offset);
        case OP_CONST:
            return disassembleConst(co, offset);
        case OP_COMPARE:
            return disassembleCompare(co, offset);
        
        case OP_JMP_IF_FALSE:
        case OP_JMP:
            return disassembleJump(co, opcode, offset);

        case OP_SET_GLOBAL:
        case OP_GET_GLOBAL:
            return disassembleGlobal(co, opcode, offset);
        
        default:
            DIE << "dissasembleInstruction: no disassembly for " << opcodeToString(opcode);
            
        }

        std::cout.flags(f);

        return 0;
    }


    size_t disassembleGlobal(CodeObject* co, uint8_t opcode, size_t offset) {
        std::ios_base::fmtflags f(std::cout.flags());
       
        dumpBytes(co,  offset,2);
        printOpCode(opcode);
      
        auto globalIndex = co->code[offset+1];
 
        std::cout << (int) globalIndex << " (" << global->get(globalIndex).name << ")";

        return offset + 2;
    }

    size_t disassembleJump(CodeObject* co, uint8_t opcode, size_t offset) {
        std::ios_base::fmtflags f(std::cout.flags());
       
        dumpBytes(co,  offset,3);
        printOpCode(opcode);
        uint16_t address = readWordAtOffset(co, offset+1);
 
        std::cout << std::uppercase << std::hex  <<  std::setfill('0') << std::setw(4) 
                << (int) address << " ";

    
        std::cout.flags(f);

        return offset + 3;
    }

    uint16_t readWordAtOffset(CodeObject* co, size_t offset) {
        return (uint16_t) ((co->code[offset] << 8 ) | co->code[offset+1]);
    }

    size_t disassembleSimple(CodeObject* co, uint8_t opcode, size_t offset) {
        dumpBytes(co,  offset,1);
        printOpCode(opcode);
        return offset + 1;
    }

    size_t disassembleCompare (CodeObject* co, size_t offset) {
        dumpBytes(co,  offset,2);
        printOpCode(OP_COMPARE);
        auto compareOp = co->code[offset+1];
        std::cout << (int) compareOp << " (" 
            << inverseCompareOps_[compareOp] << ")";
        return offset + 2;
        
    }

    size_t disassembleConst(CodeObject* co, size_t offset) {
        dumpBytes(co,  offset,2);
        printOpCode(OP_CONST);
        auto constIndex = co->code[offset+1];
    

         std::cout << (int) constIndex << " (" 
            << evaValueToConstantString (co->constants[constIndex]) << ")";
        return offset + 2;
        
    }



    void dumpBytes (CodeObject*co , size_t offset, size_t count) {
        std::ios_base::fmtflags f(std::cout.flags());
        std::stringstream ss;
        for (auto i=0; i < count; i++) {
            ss << std::uppercase << std::hex  <<  std::setfill('0') << std::setw(2) 
                << ((int)co->code[offset+i] & 0xFF) << " ";
        }
        std::cout << std::left << std::setfill (' ') << std::setw(12) << ss.str();

        std::cout.flags(f);

    }

    void printOpCode(uint8_t opcode) {

        std::ios_base::fmtflags f(std::cout.flags());

        std::cout << std::left << std::setfill (' ') << std::setw(20) 
                << opcodeToString(opcode) << " ";

        std::cout.flags(f);
    }

    static std::array<std::string, 6> inverseCompareOps_ ;
    std::shared_ptr<Global> global;

};

std::array<std::string,6> EvaDisassembler::inverseCompareOps_ = {
        "<", ">","==","<=",">=","!="
    };

#endif