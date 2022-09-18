#ifndef EVACOMPILER_H
#define EVACOMPILER_H


//#include "../parser/EvaParser.h"

class EvaCompiler
{
private:
    /* data */
public:
    EvaCompiler(/* args */);
    ~EvaCompiler();

    CodeObject* compile (const Exp& exp) ;

};

EvaCompiler::EvaCompiler(/* args */)
{
}

EvaCompiler::~EvaCompiler()
{
}

CodeObject* EvaCompiler::compile(const Exp& exp)
{
    
}


#endif