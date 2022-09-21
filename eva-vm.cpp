

#include "src/vm/EvalVM.h"

int main(int argc, char const *argv[])
{
    EvaVM vm;

    auto result = vm.exec (R"(
        (> 10 50)
    )");

    //log(AS_NUMBER(result));
    log(result);
    std::cout << "All done!\n";

    return 0;
}
