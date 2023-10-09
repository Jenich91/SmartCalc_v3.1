#ifndef SRC_MODEL_H_
#define SRC_MODEL_H_

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <regex>
#include <stack>
#include <string>
#include <limits>

namespace s21 {
class Model {
    enum operatorPrior { LOW, MID, HIGH, ULTRA };

 private:
    std::string inputString{};
    std::string outputString{};
    std::stack<char> charStack{};
    std::stack<double> doubleStack{};
    double x{};

    void PrepareToCalc();
    void ParseToPolishNotation();
    double Calculate(const double x_value);

    void ReplaceUnarySign(std::string* inputStr);
    void RemoveSpace(std::string* inputStr);
    void ReplaceFuncName(std::string* inputStr);
    void Replacer(const std::string& from, const std::string& to, std::string* str);

    bool CheckDot(const std::string& inputStr);
    bool CheckBracket(const std::string& inputStr);
    bool CheckFuncName(const std::string& inputStr);
    bool CheckOperator(const std::string& inputStr);
    bool CheckCharacters(const std::string& inputStr);
    int CheckPriority(const char ch);
    bool CheckScientificNotation(const std::string& inputStr);

    void CalcFunction(std::string::const_iterator i);
    void CalcOperator(std::string::const_iterator i);

    bool IsOperator(const char ch);
    bool IsFuncName(std::string::const_iterator i);
    bool IsFuncShortname(const char ch);
    bool IsLegalChar(const char ch);
    bool IsMod(std::string::const_iterator i);

    auto PopToDoublestack();

 public:
    Model() : inputString(), outputString{}, charStack{}, doubleStack{}, x{0} {};

    bool IsNormalInputString(const std::string& inputStr);
    double GetResult();
    double CreditCalc(double totalAmount, double period, double percentageRate, int mode);

    void SetInputString(const std::string& src);
    void SetX(double inputX) { x = inputX; }
    void ResetValues();
};
}  // namespace s21
#endif  // SRC_MODEL_H_
