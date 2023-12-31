#pragma once
#ifndef DHT_H
#define DHT_H
#include "Machine.h"
#include "BigInt.h"
#include "sha1.hpp"
#include <string>
#include <iostream>

using  std::string;
using  std::cout;
using  std::cin;
using  std::endl;

class DHT {

    Machine_list machines;
    int sizeofSpace = 0;
    BigInt noofMachines;
    BigInt maximumId;
    int sizeofBtree;

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

    string multiplication(string num1, string num2) {
        if (num1.length() > num2.length()) {
            swap(num1, num2);
        }

        int carry = 0;
        string total = "";

        for (int i = num1.length() - 1; i >= 0; i--) {
            string result = "";

            for (int j = num2.length() - 1; j >= 0; j--) {
                int n1 = num1[i] - '0';
                int n2 = num2[j] - '0';
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

            for (int j = num1.length() - 1; j > i; j--)
                result = result + '0';

            total = add(total, result);
        }

        return total;
    }

    string powerOf16(int j) {
        string no = "1";

        for (int i = 0; i < j; i++) {
            no = multiplication(no, "16");
        }

        return no;
    }

    string hexToDecimal(string hex) {
        string decimal = "";
        int j = 0;
        bool sign = 0;

        if (hex[0] == '-') {
            sign = 1;
            hex = hex.substr(1, hex.length() - 1);
        }

        for (int i = hex.length() - 1; i >= 0; i--) {
            int digit;
            char c = hex[i];

            if (c >= '0' && c <= '9') {
                digit = c - '0';
            }
            else if (c >= 'A' && c <= 'F') {
                digit = c - 'A' + 10;
            }
            else if (c >= 'a' && c <= 'f') {
                digit = c - 'a' + 10;
            }
            else {
                return "Invalid input";
            }

            decimal = add(decimal, multiplication((std::to_string(digit)), powerOf16(j)));
            j++;
        }

        if (sign) {
            decimal = "-" + decimal;
        }

        return decimal;
    }

    void CalculateMaxId() {
        if (sizeofSpace < 0) {
            cout << "size of space must be positive\n";
            return;
        }

        BigInt temp( "1");
        BigInt temp2 ("2");

        for (int i = 0; i < sizeofSpace; i++) {
            temp = temp * temp2;
        }

        maximumId = temp;
        machines.SetMaxId(maximumId);
    }

    bool isValidID(string id) {
        for (int i = 0; i < id.length(); i++) {
            if (id[i] < '0' || id[i] > '9')
                return false;
        }

        if (maximumId > id)
            return true;
        return false;
    }

    bool isValidNoofMachines(BigInt id) {

        if (!id.isValid())
            return false;
        if (!id.isValid())
            return false;
        if (id.getData() == "0")
            return false;

        if (maximumId >= id)
            return true;

        return false;
    }

    void initialMesseage() {

        cout << "\t\t\tWELLCOME TO INTERPLANETARY FILE SYSTEM\n";
        cout << "Please enter the size of identifier space between 0 and 160: ";
        cin >> sizeofSpace;
        while (std::cin.fail() || sizeofSpace <= 0 || sizeofSpace > 160) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard input buffer
            std::cout << "Invalid input! Please enter a number between 0 and 160: ";
            std::cin >> sizeofSpace;
        }

        machines.setSizeofTables(sizeofSpace);
        CalculateMaxId();

        cout << "enter the no of machines you want to add: ";
        cin >> noofMachines;

        while (!isValidNoofMachines(noofMachines)) {
            cout << "please enter a valid No of machines\n";
            cout << " The no of machines must in range of 1 to " << maximumId << " (inclusive)\n";
            cin >> noofMachines;

        }
    }

    void takeInputByIds() {

        for (BigInt i( "0"); i < noofMachines; ++i) {
            cout << "Please enter the id of machine:\t";
            string id;
            cin >> id;

            while (!isValidID(id)) {

                cout << "Invalid Id\n";
                cout << "id must be in range of 0 to  " << maximumId << "\n";
                cout << "Please enter again\n";
                cin >> id;
            }

            BigInt ids(id);
            bool status = machines.AddMachine(ids,true);
            if (!status) {
                cout << "Machine with same key already Exits\n";
                cout << "Enter id againt\n";
                --i;
                continue;
            }
        }
    }

    void takeInputByNames() {
        for (BigInt i ( "0"); i < noofMachines; ++i) {

            cout << "Please enter the name of machine:\t ";
            string name;
            cin >> name;

            SHA1 checksum;
            checksum.update(name);

            string hashedId = checksum.final();
            hashedId = Mod(hashedId);
            cout << "Calculated id of machine i: " << hashedId << "\n";
            BigInt ids(hashedId);
            bool status = machines.AddMachine(ids,true);

            if (!status) {
                cout << "Machine with same key already Exits\n";
                cout << "Enter id again\n";
                --i;
                continue;
            }
        }
    }

    void ModHelper(string numberHex, unsigned int number[5]) {
        int digitValue = 0;
        int digitCount = 0;
        for (int i = 0; i < 5; i++) {
            number[i] = 0;
        }
        while (numberHex.length() < 40) {
            numberHex = '0' + numberHex;
        }
        for (char hexChar : numberHex) {
            if (isdigit(hexChar)) {
                digitValue = hexChar - '0';
            }
            else {
                digitValue = tolower(hexChar) - 'a' + 10;
            }
            if (digitCount < 8) {
                number[0] <<= 4;
                number[0] += digitValue;
            }
            else if (digitCount < 16) {
                number[1] <<= 4;
                number[1] += digitValue;
            }
            else if (digitCount < 24) {
                number[2] <<= 4;
                number[2] += digitValue;
            }
            else if (digitCount < 32) {
                number[3] <<= 4;
                number[3] += digitValue;
            }
            else {
                number[4] <<= 4;
                number[4] += digitValue;
            }

            digitCount++;
        }
    }

    string Mod(string hex) {

        unsigned int number[5];

        for (int i = 0; i < 5; i++) {
            number[i] = 0;
        }

        ModHelper(hex, number);

        std::stringstream stream;
        stream << "";

        unsigned   int current = 0;

        if (sizeofSpace <= 32 && sizeofSpace != 0) {
            current = number[4];
            current = current << 32 - sizeofSpace;
            current = current >> (32 - sizeofSpace);
            stream << std::hex << current;
        }

        else if (sizeofSpace <= 64 && sizeofSpace != 0) {
            current = number[3];
            sizeofSpace -= 32;
            current = current << 32 - sizeofSpace;
            current = current >> (32 - sizeofSpace);
            stream << std::hex << current;
            stream << std::hex << number[4];
        }

        else if (sizeofSpace <= 96 && sizeofSpace != 0) {
            current = number[2];
            sizeofSpace -= 64;
            current = current << 32 - sizeofSpace;
            current = current >> (32 - sizeofSpace);
            stream << std::hex << current;
            stream << std::hex << number[3];
            stream << std::hex << number[4];
        }

        else if (sizeofSpace <= 128 && sizeofSpace != 0) {
            current = number[1];
            sizeofSpace -= 96;
            current = current << 32 - sizeofSpace;
            current = current >> (32 - sizeofSpace);
            stream << std::hex << current;
            stream << std::hex << number[2];
            stream << std::hex << number[3];
            stream << std::hex << number[4];
        }

        else if (sizeofSpace != 0 && sizeofSpace < 160) {
            current = number[0];
            sizeofSpace -= 128;
            current = current << (32 - sizeofSpace);
            current = current >> (32 - sizeofSpace);
            stream << std::hex << current;
            for (int i = 1; i < 5; i++) {
                stream << std::hex << number[i];
            }
        }

        else if (sizeofSpace != 0) {
            for (int i = 0; i < 5; i++) {
                stream << std::hex << number[i];

            }
        }

        int length = stream.str().length();
        std::stringstream stream2;

        while (stream2.str().length() < 40 - length)
            stream2 << 0;

        stream2 << stream.str();
        string str = hexToDecimal(stream2.str());
        while (str[0] == '0') {
            str = str.substr(1, str.length() - 1);
        }
        return str;
    }

public:
    DHT() {
        string name = "";
        int choice = 0;
        initialMesseage();

        cout << "please enter the size of B-Tree:\n";
        cin >> sizeofBtree;
        while (std::cin.fail()) {
            if (std::cin.fail()) {
                std::cin.clear(); // Clear error flags
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard input buffer
            }
            cout << "Enter a valid size:\t";
            cin >> sizeofBtree;
        }
        machines.SetsizeofBtree(sizeofBtree);

        cout << "Do you want to give ids  or names  to each machine\n";
        cout << "1.Ids\n";
        cout << "2.Names\n";

        cin >> choice;
        while (std::cin.fail() || (choice != 1 && choice != 2)) {
            if (std::cin.fail()) {
                std::cin.clear(); // Clear error flags
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard input buffer
            }
            cout << "Enter a valid choice\n";
            cin >> choice;
        }

        if (choice == 1) {
            takeInputByIds();
        }
        else {
            takeInputByNames();
        }


    }

    void deleteMachine(string ID) {
        BigInt Id(ID);
        bool status = machines.deleteMachine(Id);
        if (status == true) {
            cout << "\nThe machine With Id " << ID << " is Succefully removed.\n";
        }
        else {
            cout << "\nThe machine with  Id " << ID << " does not exits\n";
        }
    }

    void addMachine() {
        int choice = 0;
        string id;
        cout << "Do you want to give ids  or names  to each machine\n";
        cout << "1.Ids\n";
        cout << "2.Names\n";
        cin >> choice;
        while (std::cin.fail() || (choice != 1 && choice != 2)) {
            if (std::cin.fail()) {
                std::cin.clear(); // Clear error flags
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard input buffer
            }
            cout << "Enter a valid choice\n";
            cin >> choice;
        }
        if (choice == 1) {
            cout << "Please enter the id\n";
            cin >> id;
            while (!isValidID(id)) {
                cout << "Invalid Id\n";
                cout << "id must be in range of 0 to  " << maximumId << "\n";
                cout << "Please enter again\n";
                cin >> id;
            }
        }
        else {
            cout << "Please enter the name of machine " << " \n";
            string name;
            cin >> name;

            SHA1 checksum;
            checksum.update(name);

            id = checksum.final();
            id = Mod(id);
        }

        bool status = machines.AddMachine(BigInt(id), false);
        if (status == true) {
            cout << "The machine is addes successfully\n";
        }
        else {
            cout << "Machine with with id already exists\n";
        }
    }

    void PrintAllMachinesWithRoutingTalbes() {
        machines.printMachineList();
        machines.PrintAllRoutingTables();
    }

    void printMachineList() {
        machines.printMachineList();
    }

    void printRoutingTable(string id) {
        BigInt temp(id);
        if (temp > this->maximumId) {
            cout << "File of this id can't exist is this system\n";
            return;
        }
        machines.PrintRoutingTable(temp);
    }

    void storeFile(string id, string content, string extension) {
        string modedId = Mod(id);
        BigInt fileId(modedId);
        cout << "Key of file is: " << modedId << "\n";
        machines.StoringFile(content, fileId, extension);
    }

    void openAfile(string id) {
        BigInt id1(id);
        if (id1 > this->maximumId) {
            cout << "File of this id can't exist is this system\n";
            return;
        }
        machines.searchFile(id1);
    }
   
    void deleteAfile(string id) {
        BigInt id1(id);
        if (id1 > this->maximumId) {
            cout << "File of this id can't exist is this system\n";
            return;
        }
         machines.deletingAFile(id1);
    }
    
    void printingBTree(string id) {
        BigInt id1(id);
        if (id1 > this->maximumId) {
            cout << "File of this id can't exist is this system\n";
            return;
        }
        machines.PrintBtree(id1);
    }
    
    ~DHT() {

    }
};
#endif 