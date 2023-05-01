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
#include "Global.h"

using syntax::EvaParser;

#define READ_BYTE() *ip++

#define GET_CONST() (co->constants[READ_BYTE()])

#define STACK_LIMIT 512

#define READ_SHORT() (ip += 2, (uint16_t)((ip[-2] <<8) | ip[-1]))

#define TO_ADDRESS(index) (&co->code[index])


#define BINARY_OP(op) \
do {\
    auto op2 = AS_NUMBER (pop());\
    auto op1 = AS_NUMBER (pop());\
    push(NUMBER(op1 op op2));\
} while (false)

template<typename T>
bool compareNumValue (int op, T v1, T v2) {
    switch (op) {
        case 0: return v1 < v2;
        case 1: return v1 > v2;
        case 2: return v1 == v2;
        case 3: return v1 >= v2;
        case 4: return v1 <= v2;
        case 5: return v1 != v2;
    }
    return false;
}

class EvaVM {
    public:
        EvaVM  () : 
         global(std::make_shared<Global>()),
         parser(std::make_unique<EvaParser>()),
         compiler(std::make_unique<EvaCompiler>(global)) {
            setGlobalVariables();
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

        EvaValue peek (size_t offset=0) {
            if (sp == stack.begin()) {
                DIE << "peek(): empty stack.\n";
            }
            return *(sp-1-offset);
        }

        EvaValue exec (const std::string & program) {

            auto ast = parser->parse("(begin " + program + ")");

            co = compiler->compile(ast);

            ip = &co->code[0];
            sp = &stack[0];

            compiler->printByteCode();
            compiler->disassembleByteCode();

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

                case OP_COMPARE :{
                    auto op = READ_BYTE();

                    auto op2 = pop();
                    auto op1 = pop();

                    if (IS_NUMBER(op1) && (IS_NUMBER(op2))) {
                        auto v1 = AS_NUMBER(op1);
                        auto v2 = AS_NUMBER(op2);
                        auto r = compareNumValue(op, v1, v2);
                        push(BOOLEAN(r));
                    } else if (IS_STRING(op1) && (IS_STRING(op2))) {
                        auto v1 = AS_CPPSTRING(op1);
                        auto v2 = AS_CPPSTRING(op2);
                        auto r = compareNumValue(op, v1, v2);
                        push(BOOLEAN(r));
                    }
                    break;
                }

                case OP_JMP_IF_FALSE : {
                    auto cond = AS_BOOLEAN(pop());

                    auto address = READ_SHORT();

                    if (!cond) {
                        ip = TO_ADDRESS(address);
                    }
                    break;
                }

                case OP_JMP : {
                     ip = TO_ADDRESS(READ_SHORT());
                    break;
                }

                case OP_GET_GLOBAL : {
                     int globalIndex = READ_BYTE(); // o auto nao estao funcionando bem em alguns casos
                     push(global->get(globalIndex).value);
                      
                    break;
                }

                case OP_SET_GLOBAL : {
                    int  globalIndex = READ_BYTE(); 
                    auto value = peek(0);
                    global->set(globalIndex, value);
                    break;
                }

                default:
                    DIE << "unknow code : " << std::hex << +opcode;
                }
            }
        }

        void setGlobalVariables() {
            
            global->addConst("x", 10);
            global->addConst("y", 45);
        }

        std::shared_ptr<Global> global;

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