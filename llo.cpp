#include "llo.hpp"

// Конструктор
LongNumber::LongNumber(const std::string &num, int frac_bits) : number(num), frac_bits(frac_bits) {}

// Метод для преобразования числа в строку
std::string LongNumber::toString() const {
    return number;
}

// Операторы арифметических операций
LongNumber LongNumber::operator+(const LongNumber &other) const {
    return addWithPrecision(other, std::max(frac_bits, other.frac_bits));
}

LongNumber LongNumber::operator-(const LongNumber &other) const {
    return subWithPrecision(other, std::max(frac_bits, other.frac_bits));
}

LongNumber LongNumber::operator/(const LongNumber &other) const {
    return divWithPrecision(other, std::max(frac_bits, other.frac_bits));
}

LongNumber LongNumber::operator*(const LongNumber &other) const {
    return multiplyWithPrecision(other, std::max(frac_bits, other.frac_bits));
}

// Операторы сравнения
bool LongNumber::operator==(const LongNumber &other) const {
    return compare(other) == 0;
}

bool LongNumber::operator!=(const LongNumber &other) const {
    return compare(other) != 0;
}

bool LongNumber::operator<(const LongNumber &other) const {
    return compare(other) < 0;
}

bool LongNumber::operator>(const LongNumber &other) const {
    return compare(other) > 0;
}

bool LongNumber::operator<=(const LongNumber &other) const {
    return compare(other) <= 0;
}

bool LongNumber::operator>=(const LongNumber &other) const {
    return compare(other) >= 0;
}

// Методы для операций с заданной точностью
LongNumber LongNumber::addWithPrecision(const LongNumber &other, int frac_bits) const {
    return LongNumber(addBinaryWithFraction(number, other.number, frac_bits), frac_bits);
}

LongNumber LongNumber::subWithPrecision(const LongNumber &other, int frac_bits) const {
    return LongNumber(subBinaryWithFraction(number, other.number, frac_bits), frac_bits);
}

LongNumber LongNumber::divWithPrecision(const LongNumber &other, int frac_bits) const {
    return LongNumber(divBinaryWithFraction(number, other.number, frac_bits), frac_bits);
}

LongNumber LongNumber::multiplyWithPrecision(const LongNumber &other, int frac_bits) const {
    return LongNumber(multiplyBinaryWithFraction(number, other.number, frac_bits), frac_bits);
}

// Метод для преобразования числа в десятичную строку
std::string LongNumber::toDecimalString() const {
    bool isNegative = (number[0] == '-');
    std::string num = isNegative ? number.substr(1) : number;

    auto [integerPart, fractionalPart] = getTwoStrings(num);

    std::string decimalInteger = binaryToDecimal(integerPart);
    std::string decimalFraction = binaryFractionToDecimal(fractionalPart);

    std::string result = decimalInteger;
    if (!decimalFraction.empty() && decimalFraction != "0") {
        result += "." + decimalFraction;
    }

    if (isNegative) {
        result = "-" + result;
    }

    return result;
}

// Вспомогательные методы
std::string LongNumber::binaryFractionToDecimal(const std::string &binaryFraction) {
    std::string decimal = "0";
    std::string power = "5"; // 2^(-1) = 0.5

    for (char bit : binaryFraction) {
        if (bit == '1') {
            decimal = addDecimalStrings(decimal, power);
        }
        power = divideByTwo(power);
    }

    while (!decimal.empty() && decimal.back() == '0') {
        decimal.pop_back();
    }

    return decimal.empty() ? "0" : decimal;
}

std::string LongNumber::binaryToDecimal(const std::string &binary) {
    std::string decimal = "0";
    std::string power = "1";

    for (int i = binary.size() - 1; i >= 0; --i) {
        if (binary[i] == '1') {
            decimal = addDecimalStrings(decimal, power);
        }
        power = multiplyByTwo(power);
    }

    return decimal;
}

std::string LongNumber::addDecimalStrings(const std::string &num1, const std::string &num2) {
    std::string result;
    int carry = 0;

    size_t maxLength = std::max(num1.size(), num2.size());

    for (size_t i = 0; i < maxLength || carry; ++i) {
        int digit1 = (i < num1.size()) ? (num1[num1.size() - 1 - i] - '0') : 0;
        int digit2 = (i < num2.size()) ? (num2[num2.size() - 1 - i] - '0') : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        result.push_back((sum % 10) + '0');
    }

    std::reverse(result.begin(), result.end());
    return result;
}

std::string LongNumber::divideByTwo(const std::string &number) {
    std::string result;
    int carry = 0;

    for (char ch : number) {
        int digit = (ch - '0') + carry * 10;
        carry = digit % 2;
        result.push_back((digit / 2) + '0');
    }

    result.erase(0, result.find_first_not_of('0'));
    return result.empty() ? "0" : result;
}

std::string LongNumber::multiplyByTwo(const std::string &numStr) {
    std::string result = "";
    int carry = 0;

    for (int i = numStr.size() - 1; i >= 0; i--) {
        int value = (numStr[i] - '0') * 2 + carry;
        carry = value / 10;
        result = std::to_string(value % 10) + result;
    }

    if (carry > 0) {
        result = std::to_string(carry) + result;
    }

    return result;
}

std::pair<std::string, std::string> LongNumber::getTwoStrings(const std::string &number) {
    std::string integerPart;
    std::string fractionalPart;
    size_t dotPosition = number.find('.');

    if (dotPosition != std::string::npos) {
        integerPart = number.substr(0, dotPosition);
        fractionalPart = number.substr(dotPosition + 1);
    } else {
        integerPart = number;
        fractionalPart = "";
    }

    return {integerPart, fractionalPart};
}

std::string LongNumber::decimalToBinary(const std::string &number) {
    std::string num = number;
    std::string binary;
    bool isNegative = false;

    if (num[0] == '-') {
        isNegative = true;
        num.erase(0, 1);
    }

    while (num != "0") {
        int lastDigit = num.back() - '0';
        binary.push_back((lastDigit % 2) + '0');
        num = divideByTwo(num);
    }

    if (binary.empty()) {
        binary.push_back('0');
    }

    std::reverse(binary.begin(), binary.end());
    if (isNegative) {
        binary.insert(binary.begin(), '-');
    }

    return binary;
}

std::string LongNumber::Binary_Faction(const std::string &number, int frac_bits) {
    std::string binary;
    std::string fractionalPart = number;

    for (int i = 0; i < frac_bits && !fractionalPart.empty(); ++i) {
        std::string multiplied = multiplyByTwo(fractionalPart);

        if (multiplied.size() == fractionalPart.size()) {
            binary.push_back('0');
            fractionalPart = multiplied;
        } else {
            binary.push_back('1');
            fractionalPart = multiplied.substr(1);
        }
    }

    return binary;
}

std::string LongNumber::addBinaryWithFraction(const std::string &num1, const std::string &num2, int frac_bits) {
    auto [intPart1, fracPart1] = getTwoStrings(num1);
    auto [intPart2, fracPart2] = getTwoStrings(num2);

    intPart1 = decimalToBinary(intPart1);
    intPart2 = decimalToBinary(intPart2);
    fracPart1 = Binary_Faction(fracPart1, frac_bits);
    fracPart2 = Binary_Faction(fracPart2, frac_bits);

    size_t maxFracLength = std::max(fracPart1.size(), fracPart2.size());
    fracPart1.append(maxFracLength - fracPart1.size(), '0');
    fracPart2.append(maxFracLength - fracPart2.size(), '0');

    std::reverse(fracPart1.begin(), fracPart1.end());
    std::reverse(fracPart2.begin(), fracPart2.end());

    std::string frac_res;
    int carry = 0;

    for (size_t i = 0; i < maxFracLength; ++i) {
        int bit1 = (i < fracPart1.size()) ? (fracPart1[i] - '0') : 0;
        int bit2 = (i < fracPart2.size()) ? (fracPart2[i] - '0') : 0;
        int sum = bit1 + bit2 + carry;
        carry = sum / 2;
        frac_res.push_back((sum % 2) + '0');
    }

    std::reverse(frac_res.begin(), frac_res.end());

    std::reverse(intPart1.begin(), intPart1.end());
    std::reverse(intPart2.begin(), intPart2.end());

    std::string int_res;
    size_t maxIntLength = std::max(intPart1.size(), intPart2.size());

    for (size_t i = 0; i < maxIntLength || carry; ++i) {
        int bit1 = (i < intPart1.size()) ? (intPart1[i] - '0') : 0;
        int bit2 = (i < intPart2.size()) ? (intPart2[i] - '0') : 0;
        int sum = bit1 + bit2 + carry;
        carry = sum / 2;
        int_res.push_back((sum % 2) + '0');
    }

    std::reverse(int_res.begin(), int_res.end());

    int_res.erase(0, int_res.find_first_not_of('0'));
    if (int_res.empty()) {
        int_res = "0";
    }

    std::string ans = int_res + "." + frac_res;
    return ans;
}

std::string LongNumber::subBinaryWithFraction(const std::string &num1, const std::string &num2, int frac_bits) {
    auto [intPart1, fracPart1] = getTwoStrings(num1);
    auto [intPart2, fracPart2] = getTwoStrings(num2);

    intPart1 = decimalToBinary(intPart1);
    intPart2 = decimalToBinary(intPart2);
    fracPart1 = Binary_Faction(fracPart1, frac_bits);
    fracPart2 = Binary_Faction(fracPart2, frac_bits);

    size_t maxFracLength = std::max(fracPart1.size(), fracPart2.size());
    fracPart1.append(maxFracLength - fracPart1.size(), '0');
    fracPart2.append(maxFracLength - fracPart2.size(), '0');

    bool isNegative = false;

    if (compareBinary(intPart1 + fracPart1, intPart2 + fracPart2) < 0) {
        isNegative = true;
        std::swap(intPart1, intPart2);
        std::swap(fracPart1, fracPart2);
    }

    std::reverse(fracPart1.begin(), fracPart1.end());
    std::reverse(fracPart2.begin(), fracPart2.end());

    std::string frac_res;
    int borrow = 0;

    for (size_t i = 0; i < maxFracLength; ++i) {
        int bit1 = (i < fracPart1.size()) ? (fracPart1[i] - '0') : 0;
        int bit2 = (i < fracPart2.size()) ? (fracPart2[i] - '0') : 0;
        int diff = bit1 - bit2 - borrow;

        if (diff < 0) {
            diff += 2;
            borrow = 1;
        } else {
            borrow = 0;
        }

        frac_res.push_back((diff % 2) + '0');
    }

    std::reverse(frac_res.begin(), frac_res.end());

    std::reverse(intPart1.begin(), intPart1.end());
    std::reverse(intPart2.begin(), intPart2.end());

    std::string int_res;
    size_t maxIntLength = std::max(intPart1.size(), intPart2.size());

    for (size_t i = 0; i < maxIntLength || borrow; ++i) {
        int bit1 = (i < intPart1.size()) ? (intPart1[i] - '0') : 0;
        int bit2 = (i < intPart2.size()) ? (intPart2[i] - '0') : 0;
        int diff = bit1 - bit2 - borrow;

        if (diff < 0) {
            diff += 2;
            borrow = 1;
        } else {
            borrow = 0;
        }

        int_res.push_back((diff % 2) + '0');
    }

    std::reverse(int_res.begin(), int_res.end());

    int_res.erase(0, int_res.find_first_not_of('0'));
    if (int_res.empty()) {
        int_res = "0";
    }

    std::string ans = int_res + "." + frac_res;

    if (isNegative) {
        ans = "-" + ans;
    }

    return ans;
}

std::string LongNumber::divBinaryWithFraction(const std::string &num1, const std::string &num2, int frac_bits) {
    auto [intPart1, fracPart1] = getTwoStrings(num1);
    auto [intPart2, fracPart2] = getTwoStrings(num2);

    intPart1 = decimalToBinary(intPart1);
    intPart2 = decimalToBinary(intPart2);
    fracPart1 = Binary_Faction(fracPart1, frac_bits);
    fracPart2 = Binary_Faction(fracPart2, frac_bits);

    std::string binaryNum1 = intPart1 + fracPart1;
    std::string binaryNum2 = intPart2 + fracPart2;

    binaryNum1.erase(0, binaryNum1.find_first_not_of('0'));
    binaryNum2.erase(0, binaryNum2.find_first_not_of('0'));

    if (binaryNum2.empty()) {
        throw std::invalid_argument("Division by zero");
    }

    std::string result;
    std::string remainder = "0";

    for (size_t i = 0; i < binaryNum1.size() + frac_bits; ++i) {
        remainder.push_back(i < binaryNum1.size() ? binaryNum1[i] : '0');
        remainder.erase(0, remainder.find_first_not_of('0'));

        if (remainder.empty()) {
            remainder = "0";
        }

        if (compareBinary(remainder, binaryNum2) >= 0) {
            result.push_back('1');
            remainder = subtractBinary(remainder, binaryNum2);
        } else {
            result.push_back('0');
        }
    }

    result.insert(result.begin() + binaryNum1.size(), '.');
    size_t dotPos = result.find('.');
    std::string intPartResult = result.substr(0, dotPos);
    intPartResult.erase(0, intPartResult.find_first_not_of('0'));

    if (intPartResult.empty()) {
        intPartResult = "0";
    }

    std::string fracPartResult = result.substr(dotPos + 1);
    fracPartResult.erase(fracPartResult.find_last_not_of('0') + 1);

    std::string finalResult = intPartResult + "." + fracPartResult;

    return finalResult;
}

std::string LongNumber::multiplyBinaryWithFraction(const std::string &num1, const std::string &num2, int frac_bits) {
    auto [intPart1, fracPart1] = getTwoStrings(num1);
    auto [intPart2, fracPart2] = getTwoStrings(num2);

    intPart1 = decimalToBinary(intPart1);
    intPart2 = decimalToBinary(intPart2);
    fracPart1 = Binary_Faction(fracPart1, frac_bits);
    fracPart2 = Binary_Faction(fracPart2, frac_bits);

    std::string binaryNum1 = intPart1 + fracPart1;
    std::string binaryNum2 = intPart2 + fracPart2;

    std::string result = multiplyBinary(binaryNum1, binaryNum2);

    size_t totalFracBits = fracPart1.size() + fracPart2.size();

    if (totalFracBits > 0) {
        size_t dotPosition = result.size() - totalFracBits;
        result.insert(dotPosition, ".");
    }

    result.erase(0, result.find_first_not_of('0'));

    if (result.empty()) {
        result = "0";
    }

    return result;
}

std::string LongNumber::multiplyBinary(const std::string &num1, const std::string &num2) {
    std::string result(num1.size() + num2.size(), '0');

    for (int i = num1.size() - 1; i >= 0; --i) {
        int carry = 0;

        for (int j = num2.size() - 1; j >= 0; --j) {
            int temp = (result[i + j + 1] - '0') + (num1[i] - '0') * (num2[j] - '0') + carry;
            result[i + j + 1] = (temp % 2) + '0';
            carry = temp / 2;
        }

        result[i] += carry;
    }

    result.erase(0, result.find_first_not_of('0'));

    if (result.empty()) {
        result = "0";
    }

    return result;
}

std::string LongNumber::subtractBinary(const std::string &num1, const std::string &num2) {
    std::string result;
    int borrow = 0;

    size_t maxLength = std::max(num1.size(), num2.size());

    for (size_t i = 0; i < maxLength; ++i) {
        int bit1 = (i < num1.size()) ? (num1[num1.size() - 1 - i] - '0') : 0;
        int bit2 = (i < num2.size()) ? (num2[num2.size() - 1 - i] - '0') : 0;

        int diff = bit1 - bit2 - borrow;

        if (diff < 0) {
            diff += 2;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result.push_back((diff % 2) + '0');
    }

    std::reverse(result.begin(), result.end());
    result.erase(0, result.find_first_not_of('0'));

    if (result.empty()) {
        result = "0";
    }

    return result;
}

int LongNumber::compareBinary(const std::string &num1, const std::string &num2) {
    if (num1.size() < num2.size()) {
        return -1;
    } else if (num1.size() > num2.size()) {
        return 1;
    } else {
        return num1.compare(num2);
    }
}

int LongNumber::compare(const LongNumber &other) const {
    const std::string &num1 = number;
    const std::string &num2 = other.number;

    bool isNegative1 = (num1[0] == '-');
    bool isNegative2 = (num2[0] == '-');

    if (isNegative1 && !isNegative2) return -1;
    if (!isNegative1 && isNegative2) return 1;

    if (isNegative1 && isNegative2) {
        return LongNumber(num2.substr(1)).compare(LongNumber(num1.substr(1)));
    }

    auto [intPart1, fracPart1] = getTwoStrings(num1);
    auto [intPart2, fracPart2] = getTwoStrings(num2);

    if (intPart1.size() < intPart2.size()) return -1;
    if (intPart1.size() > intPart2.size()) return 1;

    if (intPart1 != intPart2) {
        return intPart1 < intPart2 ? -1 : 1;
    }

    if (fracPart1.size() < fracPart2.size()) {
        fracPart1.append(fracPart2.size() - fracPart1.size(), '0');
    } else if (fracPart1.size() > fracPart2.size()) {
        fracPart2.append(fracPart1.size() - fracPart2.size(), '0');
    }

    if (fracPart1 < fracPart2) return -1;
    if (fracPart1 > fracPart2) return 1;

    return 0;
}

// Оператор для создания LongNumber из литерала с плавающей точкой
LongNumber operator""_longnum(long double number) {
    std::string numStr = std::to_string(number);
    return LongNumber(numStr, 32);
}
// int main() {
//         LongNumber num1("10.5", 2);
//     LongNumber num2("20.25", 2);
//     LongNumber res = num1 + num2;
//     std::cout << res.toString() << std::endl;
//     std::cout << "Decimal: " << res.toDecimalString() << std::endl;
//     return 0;
    
// }