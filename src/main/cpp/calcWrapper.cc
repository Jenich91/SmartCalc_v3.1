#include "calcWrapper.h"

int trunc(double val, int digits) {
    double pow10 = pow(10,digits);
    return trunc(val * pow10) / pow10;
}

double stodpre(std::string const &str, std::size_t const p) {
  std::stringstream sstrm;
  sstrm << std::setprecision(p) << std::fixed << str << std::endl;
  double d;
  sstrm >> d;
  return d;
}

std::string getNativeResult(std::string mode, std::string exp, std::string xValue) {
    if(mode.empty() || exp.empty() ) return "NOPE";

    s21::Model* model = new s21::Model();
//    std::string mode = argv[1];
    // if mode == valid -> is valid mode, maybe contain x, like -> valid "2+2", or valid "2+x" "2", return result of validation, without compute
    // if mode == compute -> is compute mode, maybe contain x, like -> compute "2+2", or compute "2+x" "2", return result of compute, without validation

//    std::string exp = argv[2];
    double x;
    // x may contain expression, but without X
    if(!xValue.empty() ) {
        std::string tempX = xValue;
        if(mode=="valid" and !model->IsNormalInputString(tempX) or tempX.find('x')<tempX.length() ) return "NOPE";
        model->SetInputString(tempX);
        x = model->GetResult();
    }

    if(mode=="compute") {
        model->SetInputString(exp);
        if(x) model->SetX(x);
        double result = model->GetResult();

//        std::cout << std::fixed << std::setprecision(7);
//        std::isnormal(result) || fabs(result - 0) < 10e7 ? std::cout <<  result << std::endl : std::cout << "NaN" << std::endl;
//      return std::isnormal(result) || fabs(result - 0) < 10e7 ?  result : "NaN";

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << result;
        std::string mystring = ss.str();

        return mystring;
    } else if(mode=="valid" and model->IsNormalInputString(exp) ) {
        return "OK";
    } return "NOPE";
}
