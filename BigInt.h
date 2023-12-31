#ifndef BIGINT_H
#define BIGINT_H
#include<iostream>
#include<string>
using std::cout;
using std::cin;
using std::string;

class BigInt {
    bool sign;
    string data;
    string add(string num1, string num2) {
        if (num1.length() < num2.length()) {
            swap(num1, num2);
        }

        while (num2.length() != num1.length()) {
            num2 = "0" + num2;
        }

        string result = "";
        int carry = 0;
        int sumIndex = 0;

        for (int i = num1.length() - 1; i >= 0; i--) {
            sumIndex = (num1[i] - '0') + (num2[i] - '0') + carry;
            carry = sumIndex / 10;
            sumIndex = sumIndex % 10;
            result = std::to_string(sumIndex) + result;
        }

        if (carry != 0)
            result = std::to_string(carry) + result;

        return result;
    }
    string subtract(string num1, string num2) {
        string data = num1;
        string data2 = num2;
        while (data2.length() < data.length()) {
            data2 = "0" + data2;
        }

        for (int i = 0; i < data2.length(); i++) {
            data2[i] = (9 - (data2[i] - '0') + '0');
        }
        data2 = add(data2, "1");

        string result = add(data2, data);
        while (result[0] == '0') {
            result = result.substr(1, result.length() - 1);
        }

        if (result.length() > num2.length() && result[0] == '1') {
            result = result.substr(1, result.length() - 1);
        }
        else {
            for (int i = 0; i < result.length(); i++) {
                result[i] = (9 - (result[i] - '0') + '0');
            }
            result = add(result, "1");
        }
        return result;
    }
    BigInt absoulute() {
        BigInt b2(this->data, 0);
        return b2;
    }
public:
    BigInt(string data) {

        this->data = data;
        sign = 0;
    }

    BigInt(string data, bool sign) {
        this->data = data;
        this->sign = sign;
    }

    BigInt() {
        data = "";
        sign = 0;
    }

    BigInt operator+(BigInt num2) {

        string data = this->data;
        string data2 = num2.data;

        bool resultSign = 0;
        string result = "";
        if (this->sign == num2.sign) {
            resultSign = this->sign;
            result = add(data, data2);

        }
        else {
            if (this->absoulute() > num2.absoulute()) {
                resultSign = this->sign;
                result = subtract(data, data2);
            }
            else {
                resultSign = num2.sign;
                result = subtract(data2, data);
            }
        }

        BigInt b2(result, resultSign);
        return b2;
    }

    BigInt operator-(BigInt num2) {

        string data = this->data;
        string data2 = num2.data;
        string result = "";
        bool sign = 0;

        if (this->sign == num2.sign) {
            result = subtract(data, data2);
            sign = this->absoulute() > num2.absoulute() ? this->sign : !this->sign;
        }
        else {
            result = add(data, data2);
            sign = !(num2.sign);
        }
        while (result[0] == '0')
        {
            result = result.substr(1, result.length() - 1);
        }
        BigInt b2(result, sign);
        return b2;
    }

    BigInt& operator++() {
        BigInt b("1");
        *this = *this + b;
        return *this;
    }

    BigInt& operator--() {
        BigInt b("1");
        *this = *this - b;
        return *this;
    }

    BigInt operator *(BigInt num2) {

        string data = this->data;
        string data2 = num2.data;

        if (data.length() > data2.length()) {
            swap(data, data2);
        }

        int carry = 0;
        string total = "";


        for (int i = data.length() - 1; i >= 0; i--) {
            string result = "";

            for (int j = data2.length() - 1; j >= 0; j--) {
                int n1 = data[i] - '0';
                int n2 = data2[j] - '0';
                int current = n1 * n2 + carry;
                carry = (current / 10);

                if (current % 10 == 0) {
                    result = '0' + result;
                }
                else {
                    result = std::to_string(current % 10) + result;
                }
            }

            if (carry != 0)
                result = std::to_string(carry) + result;

            carry = 0;

            for (int j = data.length() - 1; j > i; j--)
                result = result + '0';

            total = add(total, result);
        }
        while (total[0] == '0' && total.length() != 1)
            total = total.substr(1, total.length() - 1);
        BigInt b2(total);
        return b2;
    }

    bool operator == (BigInt num2) {
        for (int i = 0; i < num2.data.length(); i++) {
            if (num2.data[i] < '0' || num2.data[i] > '9')
                return false;
        }

        if (this->sign != num2.sign)
            return false;

        string data = this->data;
        string data2 = num2.data;
        if (data.length() < data2.length()) {
            swap(data, data2);
        }

        while (data2.length() != data.length()) {
            data2 = "0" + data2;
        }

        for (int i = data2.length() - 1; i >= 0; i--) {
            if (data2[i] != data[i])
                return false;
        }

        return true;
    }

    bool operator != (BigInt num2) {
        return (!(*this == num2));
    }

    bool operator > (BigInt num2) {

        string data = this->data;
        string data2 = num2.data;
        if (this->sign == false && num2.sign == true)
            return true;
        else if (this->sign == true && num2.sign == false)
            return false;

        while (data[0] == '0')
        {
            data = data.substr(1, data.length() - 1);
        }

        while (data2[0] == '0')
        {
            data2 = data2.substr(1, data2.length() - 1);
        }

        if (data.length() > data2.length())
            return true;
        else if (data.length() < data2.length())
            return false;
        if (this->sign == false && num2.sign == false) {
            for (int i = 0; i < data.length(); i++) {

                if (data[i] > data2[i])
                    return true;

                else if (data[i] < data2[i])
                    return false;

            }
        }
        else {
            for (int i = 0; i < data.length(); i++) {

                if (data[i] < data2[i])
                    return true;

                else if (data[i] > data2[i])
                    return false;

            }
        }
        return false;
    }

    bool operator < (BigInt num2) {
        if (*this > num2 || *this == num2)
            return false;
        return true;
    }

    bool operator >= (BigInt num2) {
        if (*this == num2 || *this > num2)
            return true;
        return false;
    }

    bool operator <= (BigInt num2) {
        if (*this == num2 || *this < num2)
            return true;
        return false;
    }

    bool operator == (string num2) {
        for (int i = 0; i < num2.length(); i++) {
            if (num2[i] < '0' || num2[i] > '9')
                return false;
        }

        string data = this->data;
        string data2 = num2;
        if (data.length() < data2.length()) {
            swap(data, data2);
        }

        while (data2.length() != data.length()) {
            data2 = "0" + data2;
        }

        for (int i = data2.length() - 1; i >= 0; i--) {
            if (data2[i] != data[i])
                return false;
        }

        return true;
    }

    bool operator != (string num2) {
        return (!(*this == num2));
    }

    bool operator > (string num2) {

        string data = this->data;
        string data2 = num2;
        while (data[0] == '0')
        {
            data = data.substr(1, data.length() - 1);
        }

        while (data2[0] == '0')
        {
            data2 = data2.substr(1, data2.length() - 1);
        }

        if (data.length() > data2.length())
            return true;
        else if (data.length() < data2.length())
            return false;

        for (int i = 0; i < data.length(); i++) {

            if (data[i] > data2[i])
                return true;

            else if (data[i] < data2[i])
                return false;

        }
        return false;
    }

    bool operator < (string num2) {
        if (*this > num2 || *this == num2)
            return false;
        return true;
    }

    bool operator >= (string num2) {
        if (*this == num2 || *this > num2)
            return true;
        return false;
    }

    bool operator <= (string num2) {
        if (*this == num2 || *this < num2)
            return true;
        return false;
    }

    BigInt operator+ (string num2) {

        BigInt temp(num2);
        return *this + temp;
    }

    BigInt operator *(string num2) {

        string data = this->data;
        string data2 = num2;

        if (data.length() > data2.length()) {
            swap(data, data2);
        }

        int carry = 0;
        string total = "";


        for (int i = data.length() - 1; i >= 0; i--) {
            string result = "";

            for (int j = data2.length() - 1; j >= 0; j--) {
                int n1 = data[i] - '0';
                int n2 = data2[j] - '0';
                int current = n1 * n2 + carry;
                carry = (current / 10);

                if (current % 10 == 0) {
                    result = '0' + result;
                }
                else {
                    result = std::to_string(current % 10) + result;
                }
            }

            if (carry != 0)
                result = std::to_string(carry) + result;

            carry = 0;

            for (int j = data.length() - 1; j > i; j--)
                result = result + '0';

            total = add(total, result);
        }
        while (total[0] == '0' && total.length() != 1)
            total = total.substr(1, total.length() - 1);
        BigInt b2(total);
        return b2;
    }

    string getData() {
        return this->data;
    }

    BigInt pow(int i) {
        BigInt temp = *this;
        for (int j = 0; j < i; j++) {
            temp = (temp * "2");
        }
        return temp;
    }

    bool isValid() {
        for (int i = 0; i < this->data.length(); i++) {
            if (data[i] < '0' || data[i] > '9')
                return false;
        }
        return true;
    }



    friend  std::istream& operator >> (std::istream& in, BigInt& num);

    friend  std::ostream& operator << (std::ostream& out, BigInt& num);
};

std::istream& operator >> (std::istream& in, BigInt& num)
{
    in >> num.data;
    return in;
}

std::ostream& operator << (std::ostream& out, BigInt& num) {
    if (num.sign == true)
        out << "-";
    out << num.data;
    return out;
}
#endif 