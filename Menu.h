#pragma once
#include<iostream>
#include"DHT.h"
#include"FileHandling.h"
using std::cout;
using std::cin;
using std::string;
class Menu {
private:
	DHT dh;
	bool BigIntVarification(string num) {
		for (int i = 0; i < num.length(); i++) {
			if (num[i] < '0' || num[i] > '9')
				return false;
		}
		return true;
	}
	void AddMachine() {
		dh.addMachine();
	}

	void DeleteMachine() {
		string id;
		cout << "Enter machine Id: ";
		cin >> id;
		bool status = BigIntVarification(id);
		if (status == true) {
			dh.deleteMachine(id);
		}
		else
			cout << "This is not a valid id\n";
	}

	void printmachineWithTable() {
		dh.PrintAllMachinesWithRoutingTalbes();
	}

	void SpecificTable() {
		string id;
		cout << "Enter machine Id: ";
		cin >> id;
		bool status = BigIntVarification(id);
		if (status == true) {
			dh.printRoutingTable(id);
		}
		else {
			cout << "which is not a valid id\n";
		}
	}

	void PrintBTree() {
		string id;
		cout << "Enter machine Id: ";
		cin >> id;
		bool status = BigIntVarification(id);
		if (status == true) {
			dh.printingBTree(id);
		}
		else {
			cout << "which is not a valid id\n";
		}
	}

	void AddFile() {

		cin.ignore(); // Clear the input buffer before using getline
		string filePath = "";
		cout << "Please Enter the accurate File Path:\t";
		getline(cin, filePath);

		bool status = false;
		string fileContent = getfileContent(filePath, status);
		if (status == true) {
			SHA1 checksum;
			checksum.update(fileContent);
			const std::string hash = checksum.final();
			string extension = "";
			int i = filePath.length();
			while (filePath[i] != '.') {
				extension = filePath[i] + extension;
				i--;
			}
			extension = "." + extension;
			dh.storeFile(hash, fileContent, extension);
		}
		else {
			cout << "problem in file opening\n";
		}

	}

	void deleteFile() {
		string id;
		cout << "Enter the key of file to be delted\n";
		cin >> id;
		bool status = BigIntVarification(id);
		if (status == true) {
			dh.deleteAfile(id);
		}
		else
			cout << "This is not a valid id\n";
	}

	void openAFile() {
		string id;
		cout << "Enter the id :\n";
		cin >> id;
		if (BigIntVarification(id) == true) {
			dh.openAfile(id);
		}
		else {
			cout << "invalid format\n";
		}
			
	}
public:	
	void mainController() {
		int choice = 1;
		while (choice != 0)
		{			
			cout << "\n\n\n\t\t\t\t\"Menu\"\n";
			cout << "\t1.Add new machine\n";
			cout << "\t2.Add delete machine\n";
			cout << "\t3.Print All machines with routing Tables\n";
			cout << "\t4.Print routing Tables of a specific machine\n";
			cout << "\t5.Print B Tree of a specific machine\n";
			cout << "\t6.Add file to the system\n";
			cout << "\t7.Delete a file from system\n";
			cout << "\t8.Open a specific file\n";
			cout << "\t0.Exit the system\n";
			cin >> choice;
			while (std::cin.fail() || choice < 0 || choice > 8) {
				std::cin.clear(); // Clear error flags
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard input buffer
				std::cout << "Invalid input: ";
				std::cin >> choice;
			}
			if (choice == 1) {
				this->AddMachine();
			}
			else if (choice == 2) {
				this->DeleteMachine();
			}
			else if (choice == 3) {
				this->printmachineWithTable();
			}
			else if (choice == 4) {
				this->SpecificTable();
			}
			else if (choice == 5) {
				this->PrintBTree();
			}
			else if (choice == 6) {
				this->AddFile();
			}
			else if (choice == 7) {
				this->deleteFile();
			}
			else if (choice == 8) {
				this->openAFile();
			}
			char ch;
			if (choice != 0) {
				cout << "\n\t\tEnter any key to continue\n";
				cin >> ch;
			}
		}

	}

};