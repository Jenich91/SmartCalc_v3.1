#include "model.h"
#include <iomanip>

int trunc(double val, int digits) {
    double pow10 = pow(10,digits);
    return trunc(val * pow10) / pow10;
}

int main(int argc, char *argv[]) {
    if(argc < 3) return 1;

    s21::Model* model = new s21::Model();
    std::string mode = argv[1];
    // if mode == valid -> is valid mode, maybe contain x, like -> valid "2+2", or valid "2+x" "2", return result of validation, without compute
    // if mode == compute -> is compute mode, maybe contain x, like -> compute "2+2", or compute "2+x" "2", return result of compute, without validation

    std::string input = argv[2];
    double x;
    // x may contain expression, but without X
    if(argc == 4 and strlen(argv[3]) ) {
        std::string tempX = argv[3];
        if(mode=="valid" and !model->IsNormalInputString(tempX) or tempX.find('x')<tempX.length() ) return 1;
        model->SetInputString(tempX);
        x = model->GetResult();
    }

    if(mode=="compute") {
        model->SetInputString(input);
        if(x) model->SetX(x);
        double result = model->GetResult();

        std::cout << std::fixed << std::setprecision(7);
        std::isnormal(result) || fabs(result - 0) < 10e7 ? std::cout <<  result << std::endl : std::cout << "NaN" << std::endl;
    } else if(mode=="valid" and model->IsNormalInputString(input) ) {
        std::cout << "OK";
    } else std::cout << "NOPE";

    return 0;
}