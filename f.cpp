#include <iostream>
#include <string>
#include <algorithm>
#include <utility>

class LongNumber {
private:
    std::string number; // Хранит число в виде строки

public:
    // Конструкторы
    LongNumber() : number("0") {}
    LongNumber(const std::string& num) : number(num) {}

    // Метод для получения строкового представления числа
    std::string toString() const {
        return number;
    }

    // Оператор сложения с заданной точностью
    LongNumber operator+(const LongNumber& other) const {
        // По умолчанию точность 32 бита
        return addWithPrecision(other, 32);
    }
    LongNumber operator-(const LongNumber& other) const {
        // По умолчанию точность 32 бита
        return subWithPrecision(other, 32);
    }
    LongNumber operator/(const LongNumber& other) const {
            return divWithPrecision(other, 32); // По умолчанию точность 32 бита
    }
    // Оператор умножения
    LongNumber operator*(const LongNumber& other) const {
        return multiplyWithPrecision(other, 32);
    }
    bool operator==(const LongNumber& other) const {
        return compare(other) == 0;
    }

    bool operator!=(const LongNumber& other) const {
        return compare(other) != 0;
    }

    bool operator<(const LongNumber& other) const {
        return compare(other) < 0;
    }

    bool operator>(const LongNumber& other) const {
        return compare(other) > 0;
    }
    LongNumber subWithPrecision(const LongNumber& other, int frac_bits) const {
        return LongNumber(subBinaryWithFraction(number, other.number, frac_bits));
    }
    // Метод для сложения с заданной точностью
    LongNumber addWithPrecision(const LongNumber& other, int frac_bits) const {
        return LongNumber(addBinaryWithFraction(number,other.number, frac_bits));
    }
    LongNumber divWithPrecision(const LongNumber& other, int frac_bits) const {
        return LongNumber(divBinaryWithFraction(number, other.number, frac_bits));
    }
    LongNumber multiplyWithPrecision(const LongNumber& other, int frac_bits) const {
        return LongNumber(multiplyBinaryWithFraction(number, other.number, frac_bits));
    }


    // Вспомогательные функции
private:
    int compare(const LongNumber& other) const {
        const std::string& num1 = number;
        const std::string& num2 = other.number;

        // Проверка на отрицательные числа
        bool isNegative1 = (num1[0] == '-');
        bool isNegative2 = (num2[0] == '-');

        // Если одно число отрицательное, а другое положительное
        if (isNegative1 && !isNegative2) return -1; // num1 < num2
        if (!isNegative1 && isNegative2) return 1;  // num1 > num2

        // Если оба числа отрицательные, меняем их местами для сравнения
        if (isNegative1 && isNegative2) {
            return LongNumber(num2.substr(1)).compare(LongNumber(num1.substr(1)));
        }
        // Разделяем числа на целую и дробную части
        auto [intPart1, fracPart1] = getTwoStrings(num1);
        auto [intPart2, fracPart2] = getTwoStrings(num2);

        // Сравниваем целые части
        if (intPart1.size() < intPart2.size()) return -1;
        if (intPart1.size() > intPart2.size()) return 1;

        // Если целые части равны по длине, сравниваем посимвольно
        if (intPart1 != intPart2) {
            return intPart1 < intPart2 ? -1 : 1;
        }
        // Сравниваем дробные части
        if (fracPart1.size() < fracPart2.size()) {
            fracPart1.append(fracPart2.size() - fracPart1.size(), '0');
        } else if (fracPart1.size() > fracPart2.size()) {
            fracPart2.append(fracPart1.size() - fracPart2.size(), '0');
        }

        if (fracPart1 < fracPart2) return -1;
        if (fracPart1 > fracPart2) return 1;

        // Если все части равны
        return 0;
    }
    static std::string divideByTwo(const std::string& number) {
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

    static std::string decimalToBinary(const std::string& number) {
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

    static std::pair<std::string, std::string> getTwoStrings(const std::string& number) {
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

    static std::string multiplyByTwo(const std::string& numStr) {
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

    static std::string Binary_Faction(const std::string& number, int frac_bits = 32) {
        std::string binary;
        std::string fractionalPart = number;
        int frac_part_size = fractionalPart.size();

        for (int i = 0; i < frac_bits && !fractionalPart.empty(); ++i) {
            std::string multiplied = multiplyByTwo(fractionalPart);
            if (multiplied.size() == frac_part_size) {
                binary.push_back('0');
                fractionalPart = multiplied;
            } else {
                binary.push_back('1');
                fractionalPart = multiplied.substr(1);
            }
            if (multiplied.size() == 0) {
                fractionalPart = "0";
            }
        }
        return binary;
    }

    static std::string addBinaryWithFraction(const std::string& num1, const std::string& num2, int frac_bits = 32) {
        auto [intPart1, fracPart1] = getTwoStrings(num1);
        auto [intPart2, fracPart2] = getTwoStrings(num2);
        intPart1 = decimalToBinary(intPart1);
        intPart2 = decimalToBinary(intPart2);
        fracPart1 = Binary_Faction(fracPart1);
        fracPart2 = Binary_Faction(fracPart2);
        if (num2[0] == '-' && num1[0] == '-') {
            std::string num3 = num1.substr(1);
            std::string num4 = num2.substr(1);
            return '-' + addBinaryWithFraction(num3, num4, frac_bits);
        } else if (num1[0] == '-' && num2[0]!='-') {
            std::string num3 = num1.substr(1);
            return subBinaryWithFraction(num2, num3, frac_bits);
        } else if (num1[0] != '-' && num2[0] == '-') {
            std::string num4 = num2.substr(1);
            return subBinaryWithFraction(num1, num4, frac_bits);           
        }
        size_t maxFracLength = std::max(fracPart1.size(), fracPart2.size());
        fracPart1.append(maxFracLength - fracPart1.size(), '0');
        fracPart2.append(maxFracLength - fracPart2.size(), '0');

        std::reverse(fracPart1.begin(), fracPart1.end());
        std::reverse(fracPart2.begin(), fracPart2.end());

        std::string frac_res;
        int carry = 0;
        for (size_t i = 0; i < maxFracLength; ++i) {
            int bit1, bit2;
            if (i < fracPart1.size()){
                bit1 = (fracPart1[i] - '0');
            } else {
                bit1 = 0;
            }
            if (i < fracPart2.size()) {
                bit2 = (fracPart2[i] - '0') ;
            } else {
                bit2 = 0;
            }
            int sum = bit1 + bit2 + carry;
            carry = sum / 2;
            frac_res.push_back((sum % 2) + '0');
        }

        // Обрезаем дробную часть до frac_bits битов
        if (frac_res.size() > frac_bits) {
            frac_res.resize(frac_bits);
        }

        std::reverse(frac_res.begin(), frac_res.end());
        std::reverse(intPart1.begin(), intPart1.end());
        std::reverse(intPart2.begin(), intPart2.end());

        std::string int_res;
        size_t maxIntLength = std::max(intPart1.size(), intPart2.size());
        for (size_t i = 0; i < maxIntLength || carry; ++i) {
            int bit1, bit2;
            if (i < intPart1.size()){
                bit1 = (intPart1[i] - '0');
            } else {
                bit1 = 0;
            }
            if (i < intPart2.size()){
                bit2 = (intPart2[i] - '0');
            } else {
                bit2 = 0;
            }
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

    static std::string subBinaryWithFraction(const std::string& num1, const std::string& num2, int frac_bits = 32) {
        auto [intPart1, fracPart1] = getTwoStrings(num1);
        auto [intPart2, fracPart2] = getTwoStrings(num2);
    
        // Преобразуем целые и дробные части в двоичный вид
        intPart1 = decimalToBinary(intPart1);
        intPart2 = decimalToBinary(intPart2);
        fracPart1 = Binary_Faction(fracPart1);
        fracPart2 = Binary_Faction(fracPart2);
    
        // Дополняем дробные части до одинаковой длины
        size_t maxFracLength = std::max(fracPart1.size(), fracPart2.size());
        fracPart1.append(maxFracLength - fracPart1.size(), '0');
        fracPart2.append(maxFracLength - fracPart2.size(), '0');
    
        // Реверсируем дробные части для удобства вычитания
        std::reverse(fracPart1.begin(), fracPart1.end());
        std::reverse(fracPart2.begin(), fracPart2.end());
    
        // Определяем, какое число больше
        bool isNegative = false;
        if (intPart1.size() < intPart2.size() || (intPart1.size() == intPart2.size() && intPart1 < intPart2)) {
            isNegative = true;
            std::swap(intPart1, intPart2);
            std::swap(fracPart1, fracPart2);
        } else if (intPart1.size() == intPart2.size() && intPart1 == intPart2) {
            // Если целые части равны, сравниваем дробные части
            if (fracPart1 < fracPart2) {
                isNegative = true;
                std::swap(fracPart1, fracPart2);
            }
        }

        std::string frac_res;
        int borrow = 0;
    
        // Вычитаем дробные части
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

        // Обрезаем дробную часть до frac_bits битов
        if (frac_res.size() > frac_bits) {
            frac_res.resize(frac_bits);
        }

        // Реверсируем дробную часть обратно
        std::reverse(frac_res.begin(), frac_res.end());

        // Реверсируем целые части для удобства вычитания
        std::reverse(intPart1.begin(), intPart1.end());
        std::reverse(intPart2.begin(), intPart2.end());

        std::string int_res;
        size_t maxIntLength = std::max(intPart1.size(), intPart2.size());

        // Вычитаем целые части
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

        // Реверсируем целую часть обратно
        std::reverse(int_res.begin(), int_res.end());
    
        // Убираем ведущие нули
        int_res.erase(0, int_res.find_first_not_of('0'));
        if (int_res.empty()) {
            int_res = "0";
        }
    
        // Собираем результат
        std::string ans = int_res + "." + frac_res;
    
        // Добавляем знак минус, если результат отрицательный
        if (isNegative) {
            ans = "-" + ans;
        }
    
        return ans;
    }

    static int compareBinary(const std::string& num1, const std::string& num2) {
        if (num1.size() < num2.size()) {
            return -1;
        } else if (num1.size() > num2.size()) {
            return 1;
        } else {
            return num1.compare(num2);
        }
    }

    static std::string subtractBinary(const std::string& num1, const std::string& num2) {
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
    //деление в столбик
    static std::string divBinaryWithFraction(const std::string& num1, const std::string& num2, int frac_bits = 32) {
        auto [intPart1, fracPart1] = getTwoStrings(num1);
        auto [intPart2, fracPart2] = getTwoStrings(num2);
        if (intPart1[0] == '-' && intPart2[0] == '-'){
            std::string num3 = num1.substr(1);
            std::string num4 = num2.substr(1);
            return divBinaryWithFraction(num3, num4, frac_bits);
        } else if ((intPart1[0] == '-' && intPart2[0] != '-')) {
            std::string num3 = num1.substr(1);
            return '-' + divBinaryWithFraction(num3, num2, frac_bits);        
        } else if ((intPart1[0] != '-' && intPart2[0] == '-')) {
            std::string num4 = num2.substr(1);
            return '-' + divBinaryWithFraction(num1, num4, frac_bits);  
        }
    
        // Преобразуем целые и дробные части в двоичный вид
        intPart1 = decimalToBinary(intPart1);
        intPart2 = decimalToBinary(intPart2);
        fracPart1 = Binary_Faction(fracPart1);
        fracPart2 = Binary_Faction(fracPart2);
    
        // Объединяем целую и дробную части в одно двоичное число
        std::string binaryNum1 = intPart1 + fracPart1;
        std::string binaryNum2 = intPart2 + fracPart2;
    
        // Удаляем ведущие нули
        binaryNum1.erase(0, binaryNum1.find_first_not_of('0'));
        binaryNum2.erase(0, binaryNum2.find_first_not_of('0'));
    
        if (binaryNum2.empty()) {
            throw std::invalid_argument("Division by zero");
        }
    
        // Инициализируем результат и остаток
        std::string result;
        std::string remainder = "0";
    
        // Деление в столбик
        for (size_t i = 0; i < binaryNum1.size() + frac_bits; ++i) {
            remainder.push_back(i < binaryNum1.size() ? binaryNum1[i] : '0');
    
            // Убираем ведущие нули в остатке
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
    
        // Вставляем точку в результат
        result.insert(result.begin() + binaryNum1.size(), '.');
    
        // Убираем ведущие нули в целой части
        size_t dotPos = result.find('.');
        std::string intPartResult = result.substr(0, dotPos);
        intPartResult.erase(0, intPartResult.find_first_not_of('0'));
        if (intPartResult.empty()) {
            intPartResult = "0";
        }
    
        // Убираем лишние нули в дробной части
        std::string fracPartResult = result.substr(dotPos + 1);
        fracPartResult.erase(fracPartResult.find_last_not_of('0') + 1);
    
        // Собираем результат
        std::string finalResult = intPartResult + "." + fracPartResult;
    
        return finalResult;
    }    
    static std::string multiplyBinaryWithFraction(const std::string& num1, const std::string& num2, int frac_bits=32) {
        // Разделяем числа на целую и дробную части
        auto [intPart1, fracPart1] = getTwoStrings(num1);
        auto [intPart2, fracPart2] = getTwoStrings(num2);
        if (intPart1[0] == '-' && intPart2[0] == '-'){
            std::string num3 = num1.substr(1);
            std::string num4 = num2.substr(1);
            return multiplyBinaryWithFraction(num3, num4, frac_bits);
        } else if ((intPart1[0] == '-' && intPart2[0] != '-')) {
            std::string num3 = num1.substr(1);
            return '-' + multiplyBinaryWithFraction(num3, num2, frac_bits);        
        } else if ((intPart1[0] != '-' && intPart2[0] == '-')) {
            std::string num4 = num2.substr(1);
            return '-' + multiplyBinaryWithFraction(num1, num4, frac_bits);  
        }


        // Преобразуем целые и дробные части в двоичный вид
        intPart1 = decimalToBinary(intPart1);
        intPart2 = decimalToBinary(intPart2);
        fracPart1 = Binary_Faction(fracPart1, frac_bits);
        fracPart2 = Binary_Faction(fracPart2, frac_bits);

        // Объединяем целую и дробную части в одно двоичное число
        std::string binaryNum1 = intPart1 + fracPart1;
        std::string binaryNum2 = intPart2 + fracPart2;

        // Выполняем умножение двоичных чисел
        std::string result = multiplyBinary(binaryNum1, binaryNum2);

        // Вставляем точку в правильное место
        size_t totalFracBits = fracPart1.size() + fracPart2.size();
        if (totalFracBits > 0) {
            size_t dotPosition = result.size() - totalFracBits;
            result.insert(dotPosition, ".");
        }

        // Убираем ведущие нули
        result.erase(0, result.find_first_not_of('0'));
        if (result.empty()) {
            result = "0";
        }

        return result;
    }

    // Вспомогательная функция для умножения двоичных чисел
    static std::string multiplyBinary(const std::string& num1, const std::string& num2) {
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

        return result;
    }
    

};
int main() {
    LongNumber num1("-2.5");
    LongNumber num2("7.3");
    LongNumber res = num1 / num2;
    std::cout<<res.toString()<<std::endl;
    // Программист задает точность вручную
    LongNumber result = num1.addWithPrecision(num2, 16); // Точность 16 битов
    std::cout << "Result (16 bits): " << result.toString() << std::endl;

    LongNumber result2 = num1.addWithPrecision(num2, 32); // Точность 32 бита
    std::cout << "Result (32 bits): " << result2.toString() << std::endl;

    return 0;
}