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
    if (!isCorrectString(numStr)) { return std::string("������ �������� ������������ �����!"); }

    long long integerPart = 0;
    if (!getIntegerPart(numStr, &integerPart)) {    // ��������� ����� ����� � �������� � �� ������������
        return std::string("������ �������� ������������ �����!");
    }

    std::string decimalPart = std::string();
    if (!getDecimalPart(numStr, decimalPart)) {     // ��������� ���������� ����� � �������� � �� ������������
        return std::string("������ �������� ������������ �����!");
    }
    bool isDecimalPartEmpty = decimalPart.empty();

    bool negativeNum = false;
    if (integerPart < 0) { 
        negativeNum = true;
        integerPart = -integerPart;
    }

    std::string numTxt = std::string();
    if (integerPart == 0) { numTxt.append("���� "); }
    if (integerPart % 1000 != 0) {      // ��������� ����� �������� �� 1 000, 10 000, 100 000 � �.�.
        if (isDecimalPartEmpty) {
            numTxt.append(llongToText(integerPart, true, "", "", ""));
        }
        else {
            numTxt.append(llongToText(integerPart, false, "�����", "�����", "�����"));
        }
    }
    else {      // ��������� 1 000, 10 000, 100 000 � �.�.
        numTxt.append(llongToText(integerPart, false, "", "", ""));
        if (!isDecimalPartEmpty) { numTxt.append("����� "); }
    }

    if (negativeNum) { numTxt.insert(0, "����� "); }
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
            if (commaCnt > 1) { break; }    // ������� ����� ���� ������ ����
        }
        if (*it == '-' && (it != numStr.begin()                 // ����� ����� ���� ������ � ������
                           || numStr.length() == 1              // ���� ����� ������� ������ 
                           || !std::isdigit(*(it + 1)))) {      // ����� ������ ����� ���� ������ ����� 
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
        std::cout << "������� ������� ����� �����!\n";
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
    if (commaInd != std::string::npos) {    // ���� ������� ���� � �����
        decimalPartStr = decimalPartStr.substr(commaInd + 1, decimalPartStr.length());
    }
    else {                                  // �����, ������� �� �������
        decimalPartStr.erase();
    }

    if (decimalPartStr.length() > maxDecimalPartNums) {
        std::cout << "������� ������� ������� �����!\n";
        return false;
    }

    dest = decimalPartStr;
    return true;
}

void NumTxtConverter::addThousands(long long num, std::string & numStr)
{
    num /= 1000;
    numStr.insert(0, lastThreeDigitsToTxt(num, false, "������", "������", "�����"));
    num /= 1000;
    numStr.insert(0, lastThreeDigitsToTxt(num, true, "�������", "��������", "���������"));
    num /= 1000;
    numStr.insert(0, lastThreeDigitsToTxt(num, true, "��������", "���������", "����������"));
    num /= 1000;
    numStr.insert(0, lastThreeDigitsToTxt(num, true, "��������", "���������", "����������"));
    num /= 1000;
    numStr.insert(0, lastThreeDigitsToTxt(num, true, "���������", "����������", "�����������"));
}

void NumTxtConverter::addDecimalPartTxt(const std::string& decimalPartStr, std::string& numStr)
{
    long long decimalPart = std::stoll(decimalPartStr);

    switch (decimalPartStr.length()) {
        case 1:
            numStr.append(llongToText(decimalPart, false, "�������", "�������", "�������"));
            break;
        case 2:
            numStr.append(llongToText(decimalPart, false, "�����", "�����", "�����"));
            break;
        case 3:
            numStr.append(llongToText(decimalPart, false, "��������", "��������", "��������"));
            break;
        case 4:
            numStr.append(llongToText(decimalPart, false, "��������������", "��������������", "��������������"));
            break;
        case 5:
            numStr.append(llongToText(decimalPart, false, "�����������", "�����������", "�����������"));
            break;
        case 6:
            numStr.append(llongToText(decimalPart, false, "����������", "����������", "����������"));
            break;
        case 7:
            numStr.append(llongToText(decimalPart, false, "����������������", "����������������", "����������������"));
            break;
        case 8:
            numStr.append(llongToText(decimalPart, false, "�������������", "�������������", "�������������"));
            break;
        case 9:
            numStr.append(llongToText(decimalPart, false, "�����������", "�����������", "�����������"));
            break;
        case 10:
            numStr.append(llongToText(decimalPart, false, "�����������������", "�����������������", "�����������������"));
            break;
        case 11:
            numStr.append(llongToText(decimalPart, false, "��������������", "��������������", "��������������"));
            break;
        case 12:
            numStr.append(llongToText(decimalPart, false, "����������", "�����������", "�����������"));
            break;
        case 13:
            numStr.append(llongToText(decimalPart, false, "����������������", "�����������������", "�����������������"));
            break;
        case 14:
            numStr.append(llongToText(decimalPart, false, "�������������", "��������������", "��������������"));
            break;
        default:
            numStr.append("");
    }
}

std::string NumTxtConverter::lastThreeDigitsToTxt(long long num, bool masculine, const std::string& one,
                                                  const std::string& twoToFour, const std::string& fiveToNine)
{
    std::array<std::string, 20> zeroTo19 = {
        "", "���� ", "��� ", "��� ", "������ ",
        "���� ", "����� ", "���� ", "������ ", "������ ",
        "������ ", "����������� ", "���������� ", "���������� ", "������������ ",
        "���������� ", "����������� ", "���������� ", "������������ ", "������������ "
    };

    long long lastThreeDigitsNum = num % 1000;      // ����� �� ��������� ���� ����
    if (lastThreeDigitsNum == 0) { return ""; }
    if (!masculine) {                               // ������ �� ������� ��� 
        zeroTo19[1] = "���� ";                      // (�������� ��� ������ ����� 1002,5 - ���� ������ ��� ����� ���� �������)
        zeroTo19[2] = "��� ";
    }

    std::string numToText = std::string(hundreds[lastThreeDigitsNum / 100]);

    if (lastThreeDigitsNum % 100 < 20) {
        numToText.append(zeroTo19[lastThreeDigitsNum % 100]);
    }
    else {      // �� 20 �� 100
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
    long long temp = (num % 100 > 20) ? num % 10 : num % 20;    // ����� ����� �����, ��������:
    switch (temp) {
        case 1: return one;                                     // ���� ������
        case 2: case 3: case 4: return twoToFour;               // ��� ������
        default: return fiveToNine;                             // ����� �����
    }
}
