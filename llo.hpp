#ifndef LONGNUMBER_HPP
#define LONGNUMBER_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <utility>

class LongNumber {
private:
    std::string number; // Хранит число в виде строки
    int frac_bits;      // Количество цифр после запятой

public:
    // Конструктор с указанием точности
    LongNumber(const std::string &num, int frac_bits = 200);

    // Метод для преобразования числа в строку
    std::string toString() const;

    // Операторы арифметических операций
    LongNumber operator+(const LongNumber &other) const;
    LongNumber operator-(const LongNumber &other) const;
    LongNumber operator/(const LongNumber &other) const;
    LongNumber operator*(const LongNumber &other) const;

    // Операторы сравнения
    bool operator==(const LongNumber &other) const;
    bool operator!=(const LongNumber &other) const;
    bool operator<(const LongNumber &other) const;
    bool operator>(const LongNumber &other) const;
    bool operator<=(const LongNumber &other) const;
    bool operator>=(const LongNumber &other) const;

    // Методы для операций с заданной точностью
    LongNumber addWithPrecision(const LongNumber &other, int frac_bits) const;
    LongNumber subWithPrecision(const LongNumber &other, int frac_bits) const;
    LongNumber divWithPrecision(const LongNumber &other, int frac_bits) const;
    LongNumber multiplyWithPrecision(const LongNumber &other, int frac_bits) const;

    // Метод для преобразования числа в десятичную строку
    std::string toDecimalString() const;

private:
    // Вспомогательные методы
    static std::string binaryFractionToDecimal(const std::string &binaryFraction);
    static std::string binaryToDecimal(const std::string &binary);
    static std::string addDecimalStrings(const std::string &num1, const std::string &num2);
    static std::string divideByTwo(const std::string &number);
    static std::string multiplyByTwo(const std::string &numStr);
    static std::pair<std::string, std::string> getTwoStrings(const std::string &number);
    static std::string decimalToBinary(const std::string &number);
    static std::string Binary_Faction(const std::string &number, int frac_bits = 200);
    static std::string addBinaryWithFraction(const std::string &num1, const std::string &num2, int frac_bits);
    static std::string subBinaryWithFraction(const std::string &num1, const std::string &num2, int frac_bits);
    static std::string divBinaryWithFraction(const std::string &num1, const std::string &num2, int frac_bits);
    static std::string multiplyBinaryWithFraction(const std::string &num1, const std::string &num2, int frac_bits);
    static std::string multiplyBinary(const std::string &num1, const std::string &num2);
    static std::string subtractBinary(const std::string &num1, const std::string &num2);
    static int compareBinary(const std::string &num1, const std::string &num2);
    int compare(const LongNumber &other) const;
};

LongNumber operator""_longnum(long double number);

#endif 