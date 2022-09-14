

#include "src/vm/EvalVM.h"

int main(int argc, char const *argv[])
{
    EvaVM vm;

    auto result = vm.exec (R"(
        42
    )");

    log(result.number);
    std::cout << "All done!\n";

    return 0;
}
