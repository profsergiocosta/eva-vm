#ifndef EVACOMPILER_H
#define EVACOMPILER_H

#include "../parser/EvaParser.h"
#include "../vm/EvaValue.h"
#include "../bytecode/opcode.h"
#include "../disassembler/evadisassembler.h"
#include <map>
#include <string>


#define GEN_BYNARY_OP(op) do {gen(exp.list[1]); gen(exp.list[2]); emit(op);} while (false)

class EvaCompiler
{
private:
    /* data */
public:
    EvaCompiler(std::shared_ptr<Global> global): 
    global(global),
    disassembler(std::make_unique<EvaDisassembler>(global)) {};
    ~EvaCompiler() {};

    CodeObject *compile(const Exp &exp);
    void gen (const Exp &exp);

    void disassembleByteCode () {
        disassembler->disassemble(co);
    }

    void printByteCode() {
        disassembler->printByteCode(co);
    }

    private:
        std::shared_ptr<Global> global;
        
        std::unique_ptr<EvaDisassembler> disassembler;
        

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

        //booleanConstIdx

        size_t booleanConstIdx(bool value) {
            
            for (auto i=0; i < co->constants.size(); i++) {
                if (!IS_BOOLEAN(co->constants[i])) {
                    continue;
                }
                if (AS_BOOLEAN (co->constants[i])== value) {
                    return i;
                }
            }
            co->constants.push_back(BOOLEAN(value));
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

        size_t getOffSet() { return co->code.size(); }

        void writeByteAtOffset(size_t offset, uint8_t value) {
            co->code[offset] = value;
        }

        void patchJumpAddress (size_t offset, u_int16_t value ) {
            writeByteAtOffset(offset, (value >> 8) & 0xff);
            writeByteAtOffset(offset+1, value & 0xff);


        }
    CodeObject *co;

    static std::map<std::string, uint8_t> compareOps_;
};

std::map<std::string, uint8_t> EvaCompiler::compareOps_ = {
    {"<", 0},{">", 1},{"==", 2},{">=", 3},{"<=", 4},{"!=", 5},
};



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
    
    case ExpType::SYMBOL:
        if (exp.string == "true" || exp.string == "false") {
            emit(OP_CONST);
            emit(booleanConstIdx( exp.string == "true" ? true: false));

        } else {
            //variable

            if (!global->exists(exp.string)) {
                DIE << "[EvaCompiler]: Reference error " << exp.string;
            }

            emit(OP_GET_GLOBAL);
            auto num = global->getGlobalIndex(exp.string);
            std::cout << "emit " << num << std::endl;
            emit(num);
        }
        break;

    case ExpType::LIST: {
        auto tag = exp.list[0];

        if (tag.type == ExpType::SYMBOL) {
            auto op = tag.string;

            if (op == "+") {
                GEN_BYNARY_OP(OP_ADD);

            } else if (op == "-") {
                GEN_BYNARY_OP(OP_SUB);
            } else if (op == "*") {
                GEN_BYNARY_OP(OP_MUL);
            }else if (op == "/") {
                GEN_BYNARY_OP(OP_DIV);
            } else if (compareOps_.count(op) != 0) {
                gen(exp.list[1]);
                gen(exp.list[2]);
                emit(OP_COMPARE);
                emit(compareOps_[op]);
            }

            else if (op == "if") {
                // emit <test>
                gen(exp.list[1]);

                // else branch, init with 0 addres, will be patched
                emit(OP_JMP_IF_FALSE);

                
                // two bytes, considering large programs
                emit(0);
                emit(0);

                auto elseJmpAdr = getOffSet() - 2;

                // emit consequent
                gen (exp.list[2]);

                emit(OP_JMP);

                // two bytes, considering large programs
                emit(0);
                emit(0);

                auto endAddr = getOffSet() - 2;
                auto elseBranchAddr = getOffSet();

                patchJumpAddress(elseJmpAdr,elseBranchAddr);

                // emit alternative
                if (exp.list.size() == 4) {
                    gen(exp.list[3]);
                }

                // patch end
                auto endBranchAddr = getOffSet();
                patchJumpAddress(endAddr,endBranchAddr);

            }

            else if (op == "var") {
                global->define(exp.list[1].string);

                gen(exp.list[2]);

                emit(OP_SET_GLOBAL);
                emit(global->getGlobalIndex(exp.list[1].string));
            }


            else if (op == "set") {
                    auto varname = exp.list[1].string;

                    gen(exp.list[2]);
                    
                    int globalIndex = global->getGlobalIndex(varname);
                    if (globalIndex == -1) {
                        DIE << "Reference error: " << varname << " is not defined.";
                    }
                    emit(OP_SET_GLOBAL);
                    emit(globalIndex);
             }


             else if (op == "begin") {

                // compile each expression within the block

                for (int i = 1; i < exp.list.size(); i++) {
                    gen(exp.list[i]);
                }
             }

        }
        break;

    }


    default:
        break;
    }
}

#endif