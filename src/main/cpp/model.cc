#include "model.h"
void s21::Model::ResetValues() {
  inputString.clear();
  outputString.clear();
  charStack = {};
  doubleStack = {};
}

bool s21::Model::IsNormalInputString(const std::string& inputStr) {
  if (inputStr.empty() || inputStr.length() > 255 || !CheckCharacters(inputStr)) {
    return false;
  }
  std::string tmpStr = {inputStr};

  ReplaceFuncName(&tmpStr);
  ReplaceUnarySign(&tmpStr);

  return (CheckDot(tmpStr)
        && CheckBracket(tmpStr)
        && CheckFuncName(tmpStr)
        && CheckOperator(tmpStr)
        && CheckScientificNotation(tmpStr));
}

bool s21::Model::CheckCharacters(const std::string& inputStr) {
  for (auto i = inputStr.begin(); i < inputStr.end(); ++i) {
    if (IsFuncName(i) || IsMod(i)) {
      while (isalpha(*i)) {
        ++i;
      }
    } else if (!IsLegalChar(*i)) {
      return false;
    }
  }
  return true;
}

bool s21::Model::IsFuncName(std::string::const_iterator i) {
  std::string tempStr{};
  while (isalpha(*i)) {
    tempStr.push_back(*i);
    ++i;
  }
  return (tempStr == "cos") || (tempStr == "sin") || (tempStr == "tan") ||
         (tempStr == "acos") || (tempStr == "asin") || (tempStr == "atan") ||
         (tempStr == "sqrt") || (tempStr == "ln") || (tempStr == "log");
}

bool s21::Model::IsLegalChar(const char ch) {
  const char* operators = "%-+/*^xe().1234567890";
  return (strchr(operators, (int)ch) != NULL);
}

bool s21::Model::IsOperator(const char ch) {
  const char* operators = "%-+/*^u";
  return (strchr(operators, (int)ch) != NULL);
}

bool s21::Model::IsFuncShortname(const char ch) {
  const char* operators = "SsCcTtqLl";
  return (strchr(operators, (int)ch) != NULL);
}

bool s21::Model::CheckDot(const std::string& inputStr) {
  for (auto i = inputStr.begin()++; i < inputStr.end()--; ++i) {
    auto prev = std::prev(i, 1);
    auto next = std::next(i, 1);
    if ((*prev == '.' && IsOperator(*i) && *next == '.') ||
        (*prev == '.' && *i == '.')) {
      return false;
    }
  }
  return true;
}

void s21::Model::ReplaceUnarySign(std::string* inputStr) {
  auto end = (*inputStr).end()--;
  for (auto i = (*inputStr).begin()++; i < end; ++i) {
    auto prev = std::prev(i, 1);
    auto next = std::next(i, 1);
    size_t index = std::distance((*inputStr).begin(), i);
    if ((index == 0 && IsOperator(*i) &&
         (isdigit(*next) || *next == '.' || *next == '(' ||
          (IsFuncShortname(*next) || *next == 'u') || *next == 'x')) ||
        (index > 0 && (IsOperator(*prev) || *prev == '(' || *next == '%') &&
         IsOperator(*i) &&
         (isdigit(*next) || IsFuncShortname(*next) || *next == 'x' ||
          *next == '('))) {
      if (*i == '-') {
        *i = 'u';
      } else if (*i == '+') {
        *i = ' ';
      }
    }
  }
  RemoveSpace(inputStr);
}

void s21::Model::RemoveSpace(std::string* inputStr) {
  (*inputStr).erase(std::remove((*inputStr).begin(), (*inputStr).end(), ' '),
                    (*inputStr).end());
}

bool s21::Model::CheckBracket(const std::string& inputStr) {
  if (inputStr[0] == ')') {
    return false;
  }

  int open_bracket_count = 0;
  int close_bracket_count = 0;

  for (auto i = inputStr.begin(); i < inputStr.end()--; ++i) {
    auto next = std::next(i, 1);
    auto moreNext = std::next(i, 2);
    if (*i == '(') {
      open_bracket_count++;
    } else if (*i == ')') {
      close_bracket_count++;
    }
    if (close_bracket_count > open_bracket_count) {
      return false;
    }
    if (*i == '(' && *next == ')') {
      return false;
    } else if (*i == '(' && IsOperator(*next) &&
               (*moreNext == ')' || IsOperator(*moreNext))) {
      return false;
    } else if (*i == '(' && (IsOperator(*next) && *next != 'u')) {
      return false;
    }
  }

  return open_bracket_count == close_bracket_count;
}

bool s21::Model::CheckOperator(const std::string& inputStr) {
  if (IsOperator(*inputStr.begin()) && *inputStr.begin() != 'u') {
    return false;
  }

  for (auto i = inputStr.begin(); i < inputStr.end()--; ++i) {
    auto next = std::next(i, 1);
    if ((IsOperator(*i) && *i != 'u') && (IsOperator(*next) && *next != 'u')) {
      return false;
    }
  }
  return true;
}

bool s21::Model::CheckScientificNotation(const std::string& inputStr) {
  std::regex reg1("e[\\d\\+\\-]\\d*\\.[\\d]|e[\\d\\+\\-]\\d*\\.$");
  std::regex reg2("e[\\+\\-]{2}\\d*");
  std::regex reg3("^e|[^\\d]e|e[^\\d\\+\\-]|e$");
  return !std::regex_search(inputStr, reg1) &&
         !std::regex_search(inputStr, reg2) &&
         !std::regex_search(inputStr, reg3);
}

bool s21::Model::CheckFuncName(const std::string& inputStr) {
  for (std::string::const_iterator i = inputStr.begin(); i < inputStr.end()--;
       ++i) {
    auto next = std::next(i, 1);
    size_t index = std::distance(inputStr.begin(), i);
    if (IsFuncShortname(*i) && *next != '(') {
      return false;
    } else if (*i == '%') {
      if (index == 0) {
        return false;
      }
    }
  }
  return true;
}

bool s21::Model::IsMod(std::string::const_iterator i) {
  std::string tempStr{};
  int j = 0;
  while (isalpha(*i) && j < 3) {
    tempStr.push_back(*i);
    ++i;
    ++j;
  }
  return tempStr == "mod";
}

void s21::Model::PrepareToCalc() {
  ReplaceFuncName(&inputString);
  ReplaceUnarySign(&inputString);
  ParseToPolishNotation();
}

void s21::Model::ParseToPolishNotation() {
  for (auto i = inputString.begin(); i < inputString.end(); i++) {
    // Число - добавляем в строку вывода
    if (*i == 'x') {
      outputString.push_back('x');
      outputString.push_back(' ');
      continue;
    }
    if (isdigit(*i) || *i == '.') {
      while (isdigit(*i) || *i == '.') {
        size_t shift_value = 0;
        long double num = std::stold(i.base(), &shift_value);
        std::advance(i, shift_value);

        std::string tmp = std::to_string(num);
        outputString += tmp;
      }
      outputString.push_back(' ');
    }
    // Функция или открывающая скобка - помещаем в стек.
    if (*i == '(' || (isalpha(*i) && *i != 'u')) {
      charStack.push(*i);
    } else if (IsOperator(*i)) {
      if (*i != '^') {
        while (!charStack.empty() &&
               CheckPriority(charStack.top()) >= CheckPriority(*i)) {
          outputString.push_back(charStack.top());
          charStack.pop();
        }
      }
      charStack.push(*i);
    } else if (*i == ')') {
      // Пока лексема на вершине стека не станет открывающей скобкой
      // перекладываем лексемы-операторы из стека в выходную очередь.
      while (charStack.size() > 0 && charStack.top() != '(') {
        outputString.push_back(charStack.top());
        charStack.pop();
      }
      // Удаляем из стека открывающую скобку.
      if (!charStack.empty()) {
        charStack.pop();
      }
      // Если лексема на вершине стека — функция, перекладываем её в выходную
      // очередь.
      if (!charStack.empty() && isalpha(charStack.top())) {
        outputString.push_back(charStack.top());
        charStack.pop();
      }
    }
  }

  while (!charStack.empty()) {
    outputString.push_back(charStack.top());
    charStack.pop();
  }
}

int s21::Model::CheckPriority(char ch1) {
  int chPrior = 0;
  switch (ch1) {
    case '(':
      chPrior = LOW;
      break;
    case '-':
      chPrior = MID;
      break;
    case '+':
      chPrior = MID;
      break;
    case '*':
      chPrior = HIGH;
      break;
    case '/':
      chPrior = HIGH;
      break;
    case '%':
      chPrior = HIGH;
      break;
    case '^':
      chPrior = ULTRA;
      break;
    case 'u':
      chPrior = ULTRA;
    default:
      break;
  }
  return chPrior;
}

double s21::Model::GetResult() {
  return Calculate(x);
  }

auto s21::Model::PopToDoublestack() {
  auto v = std::move(doubleStack.top());
  doubleStack.pop();
  return v;
}

void s21::Model::CalcOperator(std::string::const_iterator i) {
  double operand1 = PopToDoublestack();
  double operand2 = PopToDoublestack();
  switch (*i) {
    case '-':
      doubleStack.push(operand2 - operand1);
      break;
    case '+':
      doubleStack.push(operand2 + operand1);
      break;
    case '/':
      doubleStack.push(operand2 / operand1);
      break;
    case '*':
      doubleStack.push(operand2 * operand1);
      break;
    case '%':
      doubleStack.push(fmod(operand2, operand1));
      break;
    case '^':
      doubleStack.push(pow(operand2, operand1));
    default:
      break;
  }
}

void s21::Model::CalcFunction(std::string::const_iterator i) {
  switch (*i) {
    case 'c':
      doubleStack.push(cos(PopToDoublestack()));
      break;
    case 's':
      doubleStack.push(sin(PopToDoublestack()));
      break;
    case 't':
      doubleStack.push(tan(PopToDoublestack()));
      break;
    case 'C':
      doubleStack.push(acos(PopToDoublestack()));
      break;
    case 'S':
      doubleStack.push(asin(PopToDoublestack()));
      break;
    case 'T':
      doubleStack.push(atan(PopToDoublestack()));
      break;
    case 'q':
      doubleStack.push(sqrt(PopToDoublestack()));
      break;
    case 'l':
      doubleStack.push(log(PopToDoublestack()));
      break;
    case 'L':
      doubleStack.push(log10(PopToDoublestack()));
      break;
  }
}

double s21::Model::Calculate(const double x_value) {
  for (auto i = outputString.begin(); i < outputString.end(); i++) {
    if (*i == 'x') {
      doubleStack.push(x_value);
    } else if (isdigit(*i)) {
      long double num = 0.0;
      size_t shift_value = 0;
      num = std::stold(i.base(), &shift_value);
      std::advance(i, shift_value);
      doubleStack.push(num);
    } else if (IsOperator(*i)) {
      if (*i == 'u' && doubleStack.size() >= 1) {
        doubleStack.top() *= -1;
      } else if (doubleStack.size() >= 2) {
        CalcOperator(i);
      }
    } else if (isalpha(*i) && doubleStack.size() >= 1) {
      CalcFunction(i);
    }
  }

  return PopToDoublestack();
}

void s21::Model::Replacer(const std::string& from, const std::string& to,
                          std::string* src) {
  while ((*src).find(from) != std::string::npos) {
    (*src).replace((*src).find(from), from.length(), to);
  }
}

void s21::Model::ReplaceFuncName(std::string* inputStr) {
  Replacer("asin", "S", inputStr);
  Replacer("sin", "s", inputStr);
  Replacer("acos", "C", inputStr);
  Replacer("cos", "c", inputStr);
  Replacer("atan", "T", inputStr);
  Replacer("tan", "t", inputStr);
  Replacer("sqrt", "q", inputStr);
  Replacer("log", "L", inputStr);
  Replacer("ln", "l", inputStr);
  Replacer("mod", "%", inputStr);
}

// Вход: общая сумма кредита, срок, процентная ставка, тип (аннуитетный,
// дифференцированный)
double s21::Model::CreditCalc(double totalAmount, double period,
                              double percentageRate, int mode) {
  double res = 0;
  double percent = (percentageRate / 100) / 12;
  if (mode == 1) {
    res =
        totalAmount * (percent + (percent / (pow((1 + percent), period) - 1)));
  } else if (mode == 2) {
    double debtPart = totalAmount / period;
    percent = totalAmount * percent;
    res = debtPart + percent;
  }
  return res;
}

void s21::Model::SetInputString(const std::string& src) {
  ResetValues();
  inputString = src;
  PrepareToCalc();
}
