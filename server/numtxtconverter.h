#pragma once

#include <array>
#include <string>

class NumTxtConverter {
public:
    std::string decimalToText(std::string numStr);

private:
    bool isCorrectString(const std::string& numStr);
    bool isNumberPart(const char ch);
    bool getIntegerPart(const std::string& numStr, long long* dest);
    std::string llongToText(long long num, bool masculine, const std::string& one,
                            const std::string& twoToFour, const std::string& fiveToNine);
    bool getDecimalPart(const std::string& numStr, std::string& dest);
    void addThousands(long long num, std::string & numStr);
    std::string lastThreeDigitsToTxt(long long num, bool masculine, const std::string& one,
                                     const std::string& twoToFour, const std::string& fiveToNine);
    std::string txtDependingOnNum(long long num, const std::string& one,
                                  const std::string& twoToFour, const std::string& fiveToNine);
    void addDecimalPartTxt(const std::string & decimalPartStr, std::string & numStr);

    std::array<std::string, 10> tens = {
        "", "десять ", "двадцать ", "тридцать ", "сорок ",
        "пятьдесят ", "шестьдесят ", "семьдесят ", "восемьдесят ", "девяносто "
    };

    std::array<std::string, 10> hundreds = {
        "", "сто ", "двести ", "триста ", "четыреста ",
        "пятьсот ", "шестьсот ", "семьсот ", "восемьсот ", "девятьсот "
    };
};
