#include <iostream>
#include "numtxtconverter.h"

std::string NumTxtConverter::llongToText(long long num, bool masculine, const std::string& one,
                                         const std::string& twoToFour, const std::string& fiveToNine)
{
    std::string numToText = lastThreeDigitsToTxt(num, masculine, one, twoToFour, fiveToNine);
    addThousands(num, numToText);
    return numToText;
}

std::string NumTxtConverter::decimalToText(std::string numStr)
{
    if (!isCorrectString(numStr)) { return std::string("Строка содержит некорректное число!"); }

    long long integerPart = 0;
    if (!getIntegerPart(numStr, &integerPart)) {    // Получение целой части и проверка её на корректность
        return std::string("Строка содержит некорректное число!");
    }

    std::string decimalPart = std::string();
    if (!getDecimalPart(numStr, decimalPart)) {     // Получение десятичной части и проверка её на корректность
        return std::string("Строка содержит некорректное число!");
    }
    bool isDecimalPartEmpty = decimalPart.empty();

    bool negativeNum = false;
    if (integerPart < 0) { 
        negativeNum = true;
        integerPart = -integerPart;
    }

    std::string numTxt = std::string();
    if (integerPart == 0) { numTxt.append("ноль "); }
    if (integerPart % 1000 != 0) {      // Обработка чисел отличных от 1 000, 10 000, 100 000 и т.п.
        if (isDecimalPartEmpty) {
            numTxt.append(llongToText(integerPart, true, "", "", ""));
        }
        else {
            numTxt.append(llongToText(integerPart, false, "целая", "целых", "целых"));
        }
    }
    else {      // Обработка 1 000, 10 000, 100 000 и т.п.
        numTxt.append(llongToText(integerPart, false, "", "", ""));
        if (!isDecimalPartEmpty) { numTxt.append("целых "); }
    }

    if (negativeNum) { numTxt.insert(0, "минус "); }
    if (!isDecimalPartEmpty) { addDecimalPartTxt(decimalPart, numTxt); }

    return numTxt;
}

bool NumTxtConverter::isCorrectString(const std::string& numStr)
{
    int commaCnt = 0;
    std::string::const_iterator it = numStr.begin();
    while (it != numStr.end() && isNumberPart(*it)) {
        if (*it == ',') {
            ++commaCnt;
            if (commaCnt > 1) { break; }    // Запятая может быть только одна
        }
        if (*it == '-' && (it != numStr.begin()                 // Минус может быть только в начале
                           || numStr.length() == 1              // Один минус вводить нельзя 
                           || !std::isdigit(*(it + 1)))) {      // После минуса может идти только цифра 
            break; 
        }
        ++it;
    }

    return !numStr.empty() && it == numStr.end();
}

bool NumTxtConverter::isNumberPart(const char ch)
{
    return (ch >= '0' && ch <= '9') || ch == '-' || ch == ',';
}

bool NumTxtConverter::getIntegerPart(const std::string& numStr, long long* dest)
{
    const int maxIntegerPartNums = 18;
    std::string integerPartStr = numStr;
    
    size_t commaInd = integerPartStr.find(',');
    if (commaInd != std::string::npos) {
        integerPartStr = integerPartStr.substr(0, commaInd);
    }
    if (integerPartStr.length() > maxIntegerPartNums) {
        std::cout << "Слишком большая целая часть!\n";
        return false;
    }

    if (integerPartStr.length() > 0) {
        *dest = stoll(integerPartStr);
    }
    return true;
}

bool NumTxtConverter::getDecimalPart(const std::string& numStr, std::string& dest)
{
    const int maxDecimalPartNums = 14;

    std::string decimalPartStr = numStr;

    size_t commaInd = decimalPartStr.find(',');
    if (commaInd != std::string::npos) {    // Если запятая есть в числе
        decimalPartStr = decimalPartStr.substr(commaInd + 1, decimalPartStr.length());
    }
    else {                                  // Иначе, запятая не найдена
        decimalPartStr.erase();
    }

    if (decimalPartStr.length() > maxDecimalPartNums) {
        std::cout << "Слишком большая дробная часть!\n";
        return false;
    }

    dest = decimalPartStr;
    return true;
}

void NumTxtConverter::addThousands(long long num, std::string & numStr)
{
    num /= 1000;
    numStr.insert(0, lastThreeDigitsToTxt(num, false, "тысяча", "тысячи", "тысяч"));
    num /= 1000;
    numStr.insert(0, lastThreeDigitsToTxt(num, true, "миллион", "миллиона", "миллионов"));
    num /= 1000;
    numStr.insert(0, lastThreeDigitsToTxt(num, true, "миллиард", "миллиарда", "миллиардов"));
    num /= 1000;
    numStr.insert(0, lastThreeDigitsToTxt(num, true, "триллион", "триллиона", "триллионов"));
    num /= 1000;
    numStr.insert(0, lastThreeDigitsToTxt(num, true, "триллиард", "триллиарда", "триллиардов"));
}

void NumTxtConverter::addDecimalPartTxt(const std::string& decimalPartStr, std::string& numStr)
{
    long long decimalPart = std::stoll(decimalPartStr);

    switch (decimalPartStr.length()) {
        case 1:
            numStr.append(llongToText(decimalPart, false, "десятая", "десятых", "десятых"));
            break;
        case 2:
            numStr.append(llongToText(decimalPart, false, "сотая", "сотых", "сотых"));
            break;
        case 3:
            numStr.append(llongToText(decimalPart, false, "тысячная", "тысячных", "тысячных"));
            break;
        case 4:
            numStr.append(llongToText(decimalPart, false, "десятитысячная", "десятитысячных", "десятитысячных"));
            break;
        case 5:
            numStr.append(llongToText(decimalPart, false, "стотысячная", "стотысячных", "стотысячных"));
            break;
        case 6:
            numStr.append(llongToText(decimalPart, false, "миллионная", "миллионных", "миллионных"));
            break;
        case 7:
            numStr.append(llongToText(decimalPart, false, "десятимиллионная", "десятимиллионных", "десятимиллионных"));
            break;
        case 8:
            numStr.append(llongToText(decimalPart, false, "стомиллионная", "стомиллионных", "стомиллионных"));
            break;
        case 9:
            numStr.append(llongToText(decimalPart, false, "миллиардная", "миллиардных", "миллиардных"));
            break;
        case 10:
            numStr.append(llongToText(decimalPart, false, "десятимиллиардная", "десятимиллиардных", "десятимиллиардных"));
            break;
        case 11:
            numStr.append(llongToText(decimalPart, false, "стомиллиардная", "стомиллиардных", "стомиллиардных"));
            break;
        case 12:
            numStr.append(llongToText(decimalPart, false, "триллионая", "триллионных", "триллионных"));
            break;
        case 13:
            numStr.append(llongToText(decimalPart, false, "десятитриллионая", "десятитриллионных", "десятитриллионных"));
            break;
        case 14:
            numStr.append(llongToText(decimalPart, false, "стотриллионая", "стотриллионных", "стотриллионных"));
            break;
        default:
            numStr.append("");
    }
}

std::string NumTxtConverter::lastThreeDigitsToTxt(long long num, bool masculine, const std::string& one,
                                                  const std::string& twoToFour, const std::string& fiveToNine)
{
    std::array<std::string, 20> zeroTo19 = {
        "", "один ", "два ", "три ", "четыре ",
        "пять ", "шесть ", "семь ", "восемь ", "девять ",
        "десять ", "одиннадцать ", "двенадцать ", "тринадцать ", "четырнадцать ",
        "пятнадцать ", "шестнадцать ", "семнадцать ", "восемнадцать ", "девятнадцать "
    };

    long long lastThreeDigitsNum = num % 1000;      // Число из последних трех цифр
    if (lastThreeDigitsNum == 0) { return ""; }
    if (!masculine) {                               // Замена на женский род 
        zeroTo19[1] = "одна ";                      // (например для вывода числа 1002,5 - одна тысяча две целых пять десятых)
        zeroTo19[2] = "две ";
    }

    std::string numToText = std::string(hundreds[lastThreeDigitsNum / 100]);

    if (lastThreeDigitsNum % 100 < 20) {
        numToText.append(zeroTo19[lastThreeDigitsNum % 100]);
    }
    else {      // От 20 до 100
        numToText.append(tens[lastThreeDigitsNum % 100 / 10]);
        numToText.append(zeroTo19[lastThreeDigitsNum % 10]);
    }

    numToText.append(txtDependingOnNum(lastThreeDigitsNum, one, twoToFour, fiveToNine));

    if (numToText.length() != 0) { numToText.append(" "); }

    return numToText;
}

std::string NumTxtConverter::txtDependingOnNum(long long num, const std::string& one,
                                               const std::string& twoToFour, const std::string& fiveToNine)
{
    long long temp = (num % 100 > 20) ? num % 10 : num % 20;    // Выбор формы числа, например:
    switch (temp) {
        case 1: return one;                                     // одна тысячА
        case 2: case 3: case 4: return twoToFour;               // три тысячИ
        default: return fiveToNine;                             // шесть тысяЧ
    }
}
