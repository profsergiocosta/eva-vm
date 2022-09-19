

#include "src/vm/EvalVM.h"

int main(int argc, char const *argv[])
{
    EvaVM vm;

    auto result = vm.exec (R"(
        (+ "ola " "mundo")
    )");

    //log(AS_NUMBER(result));
    log(AS_CPPSTRING(result));
    std::cout << "All done!\n";

    return 0;
}
