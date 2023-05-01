

#include "src/vm/EvalVM.h"

int main(int argc, char const *argv[])
{
    EvaVM vm;

    //(if (< 5 10) 1 2)
    auto result = vm.exec (R"(
        (var x 5)
        (set x (+ x 10))
        (+ x )
    )");

/*
    auto result = vm.exec (R"(
        (if (< 5 10) 1 2)
    )");
*/
    //log(AS_NUMBER(result));
    log(result);

    std::cout << "All done!\n";

    return 0;
}
