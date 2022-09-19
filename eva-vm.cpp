

#include "src/vm/EvalVM.h"

int main(int argc, char const *argv[])
{
    EvaVM vm;

    auto result = vm.exec (R"(
        (+ 1 (+ 40 1))
    )");

    log(AS_NUMBER(result));
    //log(AS_CPPSTRING(result));
    std::cout << "All done!\n";

    return 0;
}
