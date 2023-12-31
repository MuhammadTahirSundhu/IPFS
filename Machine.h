
#ifndef MACHINE_H
#define MACHINE_H
#include<iostream>
#include <queue>
#include"BigInt.h"
#include"FileHandling.h"
#include"BTrees.h"
#include<cmath>
using  std::string;
using  std::cout;
using  std::endl;
class Machine_Node;

class RoutingTable_Node {
public:

	int index;
	BigInt nextMachineID;
	Machine_Node* nextMachineAddress;
	RoutingTable_Node* next;
	RoutingTable_Node* previous;

	RoutingTable_Node()
	{
		index = 0;
		nextMachineAddress = nullptr;
		next = nullptr;
		previous = nullptr;
	}

	RoutingTable_Node(int i)
	{
		index = i;
		next = nullptr;
		nextMachineAddress = nullptr;
		previous = nullptr;
	}
};

class RoutingTable {
public:
	RoutingTable_Node* Head;
	RoutingTable_Node* tail;
	int count;

	RoutingTable()
	{
		Head = nullptr;
		tail = nullptr;
		count = 1;
	}

	void AddNode()
	{
		RoutingTable_Node* newNode = new RoutingTable_Node(count);
		newNode->index = count;

		if (Head == nullptr) {
			Head = newNode;
			tail = newNode;
		}
		else
		{
			RoutingTable_Node* temp = Head;
			while (temp->next) {
				temp = temp->next;
			}
			newNode->next = temp->next;
			temp->next = newNode;
			newNode->previous = temp;
			tail = newNode;
		}
		count++;
	}

	void Diaplay()
	{
		RoutingTable_Node* temp = Head;
		while (temp != nullptr)
		{
			cout << "index " << temp->index << "\t";
			cout << "nextmachineid\t" << temp->nextMachineID << "\n";
			temp = temp->next;
		}
		cout << endl;
	}
	~RoutingTable() {
		RoutingTable_Node* temp = Head;
		while (temp != nullptr) {
			Head = Head->next;
			delete temp;
			temp = Head;
		}
	}
};

struct Machine_Node {
	BigInt ID;
	Machine_Node* next;
	RoutingTable FT;
	BTree* btree;
	BigInt fileCount;
	Machine_Node(BigInt id, int sizeofTables, int sizeofBtree) {
		ID = id;
		next = nullptr;
		for (int i = 0; i < sizeofTables; i++) {
			FT.AddNode();
		}
		createDirectory(id.getData());
		btree = new BTree(sizeofBtree);
	}
	~Machine_Node() {

	}
};

class Machine_list {
	Machine_Node* Head;
	Machine_Node* last;
	BigInt count;
	BigInt maxid;
	int no_of_bits_used;
	int sizeofTables;
	int sizeofBtree;

	void manageSuccessorsHelper(Machine_Node* temp) {
		int i = 1;
		RoutingTable_Node* tableTemp = temp->FT.Head;
		last = Head;
		while (last->next != Head)
		{
			last = last->next;
		}
		while (tableTemp != nullptr) {

			BigInt p = temp->ID;
			BigInt calculatedId("1");
			Machine_Node* temp2 = temp->next;

			calculatedId = p + calculatedId.pow(i - 1);

			while (calculatedId > last->ID) {
				calculatedId = calculatedId - maxid;
				temp2 = Head;
			}

			while ((temp2->ID < calculatedId) && (temp2->next != temp)) {

				temp2 = temp2->next;
			}

			if (temp2 != temp) {

				tableTemp->nextMachineAddress = temp2;
				tableTemp->nextMachineID = temp2->ID;
			}
			else {
				tableTemp->nextMachineAddress = temp2->next;
				tableTemp->nextMachineID = temp2->next->ID;
			}
			tableTemp = tableTemp->next;
			i++;

		}
		temp = temp->next;
	}
	void mangesuccessors() {
		if (!Head)
			return;
		Machine_Node* temp = Head;

		while (temp->next != Head) {
			manageSuccessorsHelper(temp);
			temp = temp->next;
		}
		if (temp->next == Head) {
			manageSuccessorsHelper(temp);
		}
	}
public:

	Machine_list() {
		Head = nullptr;
		sizeofTables = 0;
		no_of_bits_used = 0;
	}

	void SetMaxId(BigInt max) {
		maxid = max;
	}

	Machine_list(int total, int used)
	{
		Head = nullptr;
		no_of_bits_used = used;
	}

	void SetsizeofBtree(int size) {
		sizeofBtree = size;
	}

	bool AddMachine(BigInt ID,bool isInitial) {
		Machine_Node* newNode = new Machine_Node(ID, sizeofTables, sizeofBtree);

		if (!Head) {
			Head = newNode;
			Head->next = Head;
		}
		else {
			if (newNode->ID < Head->ID) {
				Machine_Node* last = Head;
				while (last->next != Head) {
					last = last->next;
				}
				if (last->ID == newNode->ID)
					return false;
				last->next = newNode;
				newNode->next = Head;
				Head = newNode;
			}
			else if (newNode->ID > Head->ID) {
				Machine_Node* current = Head;
				while (current->next != Head && current->next->ID < newNode->ID)
				{
					current = current->next;
				}
				if (current->next->ID == newNode->ID)
					return false;
				newNode->next = current->next;
				current->next = newNode;
			}
			else {
				return false;
			}
		}
		++count;
		mangesuccessors();
		if (isInitial == false) {
			Machine_Node* prev = nullptr;
			BigInt Id;
			if (newNode == Head) {
				prev = last;
				Id = last->ID;
				++Id;
				Machine_Node* nextNode = newNode->next;
				while (Id < maxid) {
					std::string path = "";
					nextNode->btree->search(Id, path);
					if (path != "") {
						++(newNode->fileCount);
						--(nextNode->fileCount);
						bool status = false;
						string fileContent = getfileContent(path, status);
						string extension = "";
						int i = path.length();
						while (path[i] != '.') {
							extension = path[i] + extension;
							i--;
						}
						extension = "." + extension;
						std::string newPath = newNode->ID.getData() + "\\" + (newNode->fileCount.getData()) + extension; // Replace this with your desired file path
						newNode->btree->insert(newPath, Id);
						bool textEntesion1 = true;
						string textextenion = ".txt";
						for (int i = 0; i < 4; i++) {
							if (extension[i] != textextenion[i])
								textEntesion1 = false;
						}
						if (textEntesion1)
							writeTextFile(newPath, fileContent);
						else
							writeFile(newPath, fileContent);
						nextNode->btree->remove(Id,true);
						deleteFile1(path);
						/*Tahir delete the file form path -> path*/
					}
					else {
						++Id;
					}

				}
				BigInt zero("0");
				Id = zero;
				while (Id <= newNode->ID) {
					std::string path = "";
					nextNode->btree->search(Id, path);
					if (path != "") {
						++(newNode->fileCount);
						--(nextNode->fileCount);
						bool status = false;
						string fileContent = getfileContent(path, status);
						string extension = "";
						int i = path.length();
						while (path[i] != '.') {
							extension = path[i] + extension;
							i--;
						}
						extension = "." + extension;
						std::string newPath = newNode->ID.getData() + "\\" + (newNode->fileCount.getData()) + extension; // Replace this with your desired file path
						newNode->btree->insert(newPath, Id);
						bool textEntesion1 = true;
						string textextenion = ".txt";
						for (int i = 0; i < 4; i++) {
							if (extension[i] != textextenion[i])
								textEntesion1 = false;
						}
						if (textEntesion1)
							writeTextFile(newPath, fileContent);
						else
							writeFile(newPath, fileContent);
						nextNode->btree->remove(Id,true);
						deleteFile1(path);
					}
					else {
						++Id;
					}
				}
			}
			else {
				prev = last;
				while (prev->next != newNode) {
					prev = prev->next;
				}
				Id = prev->ID;
				Machine_Node* nextNode = newNode->next;
				while (Id <= newNode->ID) {
					std::string path = "";
					nextNode->btree->search(Id, path);
					if (path != "") {
						++(newNode->fileCount);
						--(nextNode->fileCount);
						bool status = false;
						string fileContent = getfileContent(path, status);
						string extension = "";
						int i = path.length();
						while (path[i] != '.') {
							extension = path[i] + extension;
							i--;
						}
						extension = "." + extension;
						std::string newPath = newNode->ID.getData() + "\\" + (newNode->fileCount.getData()) + extension; // Replace this with your desired file path
						newNode->btree->insert(newPath, Id);
						bool textEntesion1 = true;
						string textextenion = ".txt";
						for (int i = 0; i < 4; i++) {
							if (extension[i] != textextenion[i])
								textEntesion1 = false;
						}
						if (textEntesion1)
							writeTextFile(newPath, fileContent);
						else
							writeFile(newPath, fileContent);
						nextNode->btree->remove(Id,true);
						deleteFile1(path);
					}
					else {
						++Id;
					}

				}

			}

		}
		return true;
	}

	
	bool deleteMachine(BigInt ID) {

		if (Head == nullptr) {
			return false;
		}
		std::queue<File*> q;
		Machine_Node* temp = Head;
		Machine_Node* prev = nullptr;
		Machine_Node* nextNode = nullptr;
		bool status = false;
		if (Head->ID == ID) {
			// if head to be deleted and only one node
			if (Head->next == Head) {
				int choice;
				cout << "There is only one machine in The sytem.\n";
				cout << "If you have removed this then you will lost all the data\n";
				cout << "Pick a choice\n";
				cout << "1.Any way delete the machine\n";
				cout << "2.No Dont' delte machine\n";
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
					Head->btree->clear(q);
					delete Head;
					Head = nullptr;
					status = true;
				}
				else {
					status = false;
				}
			}
			else {
				prev = Head;
				while (prev->next != Head) {
					prev = prev->next;
				}
				prev->next = Head->next;
				Head = Head->next;
				nextNode = temp->next;
				temp->btree->clear(q);
				delete temp;
				status = true;
			}
		}
		else {
			while (temp->next != Head && temp->ID != ID) {
				prev = temp;
				temp = temp->next;
			}
			if (temp->ID == ID) {
				prev->next = temp->next;
				nextNode = temp->next;
				temp->btree->clear(q);
				delete temp;
				status = true;
			}
		}
		if (status) {
			mangesuccessors();
			if (nextNode != nullptr) {
				int j = 0;
				while (j < q.size()) {
					File* top = q.front();
					++(nextNode->fileCount);
					bool status = false;
					string fileContent = getfileContent(top->Path, status);
					string extension = "";
					int i = top->Path.length();
					while (top->Path[i] != '.') {
						extension = top->Path[i] + extension;
						i--;
					}
					extension = "." + extension;
					std::string newPath = nextNode->ID.getData() + "\\" + (nextNode->fileCount.getData()) + extension; // Replace this with your desired file path
					nextNode->btree->insert(newPath, top->Hash);
					bool textEntesion1 = true;
					string textextenion = ".txt";
					for (int i = 0; i < 4; i++) {
						if (extension[i] != textextenion[i])
							textEntesion1 = false;
					}
					if (textEntesion1)
						writeTextFile(newPath, fileContent);
					else
						writeFile(newPath, fileContent);
					q.pop();
					q.push(top);
					j++;
				}
				/* ---------- Tahir delete all the files which are in queue.like this	*/
				while (!q.empty()){
					deleteFile1(q.front()->Path);
					q.pop();
				}

			}
		//Here The folder related 
			deleteDirectory(ID.getData());
		}
		/*Sotring data to new place*/
		

		return status;
	}


	void setSizeofTables(int n) {
		sizeofTables = n;
	}

	void PrintAllRoutingTables()
	{
		Machine_Node* temp = Head;
		cout << temp->ID << "\n";
		temp->FT.Diaplay();
		temp = temp->next;
		while (temp != Head)
		{
			cout << temp->ID << "\n";
			temp->FT.Diaplay();
			temp = temp->next;
		}
	}

	void printMachineList() {
		if (Head == nullptr) {
			cout << "sorry but there is no machine in system\n";
		}
		else {
			Machine_Node* temp = Head;
			while (temp->next != Head) {
				cout << temp->ID << " ";
				temp = temp->next;
			}
			cout << temp->ID << "\n";
		}
	}

	void PrintRoutingTable(BigInt Id) {
		if (Head->ID == Id) {
			cout << "Machine Id: " << Head->ID << endl;
			cout << "Routing Table\n";
			Head->FT.Diaplay();
		}
		else {
			Machine_Node* temp = Head->next;
			while (temp->next != Head)
			{
				if (temp->ID == Id) {
					cout << "Machine Id: " << temp->ID << endl;
					cout << "Routing Table\n";
					Head->FT.Diaplay();
					return;
				}
				temp = temp->next;
			}
			cout << "We are Relly sorry but no such machine with this Id exist\n";
		}
	}

	Machine_Node* mappingIdToMachine(BigInt fileHash, string& pathTaken) {
		Machine_Node* temp = Head;
		pathTaken += "machine" + temp->ID.getData() + " ";
		while (temp->ID < fileHash && temp->next != Head) {
			RoutingTable_Node* Table_Temp = temp->FT.Head;
			for (int i = 0; i < sizeofTables - 1; i++) {
				if (Table_Temp->next->nextMachineID <= fileHash && Table_Temp->next->nextMachineID >= Table_Temp->nextMachineID)
					Table_Temp = Table_Temp->next;
				else
					break;
			}
			temp = Table_Temp->nextMachineAddress;
			pathTaken += "Table[" + std::to_string(Table_Temp->index) + "]  ";
			pathTaken += "machine" + temp->ID.getData() + " ";
		}
		return temp;
	}

	void StoringFile(string fileContent, BigInt fileHash, string extension) {
		string pathTaken = "";
		Machine_Node* temp = nullptr;
		if (fileHash > last->ID) {
			temp = Head;
			pathTaken = pathTaken += "machine" + temp->ID.getData() + " ";
		}
		else {
			temp = mappingIdToMachine(fileHash, pathTaken);
		}
		cout << "File storing Path : " << pathTaken << "\n";
		++(temp->fileCount);
		std::string newPath = temp->ID.getData() + "\\" + (temp->fileCount.getData()) + extension; // Replace this with your desired file path
		temp->btree->insert(newPath, fileHash);
		bool textEntesion1 = true;
		string textextenion = ".txt";
		for (int i = 0; i < 4; i++) {
			if (extension[i] != textextenion[i])
				textEntesion1 = false;
		}
		if (textEntesion1)
			writeTextFile(newPath, fileContent);
		else
			writeFile(newPath, fileContent);
	}

	void deletingAFile(BigInt id) {
		std::string pathTaken = "";
		Machine_Node* temp = nullptr;
		if (id > last->ID) {
			temp = Head;
			pathTaken = pathTaken += "machine" + temp->ID.getData() + " ";
		}
		else {
			temp = mappingIdToMachine(id, pathTaken);
		}
		std::cout << "File Searching Path : " << pathTaken << "\n";
		std::string path = "";
		temp->btree->search(id, path);
		if (path != "") {
			temp->btree->remove(id);
			deleteFile1(path);
			cout << "The file is removed successfully form machine " << temp->ID << "\n";
			--(temp->fileCount);
		}
		else {
			cout << "Sorry! NO such file found\n";
		}



	}

	void searchFile(BigInt id) {
		std::string path = "";
		std::string pathTaken = "";
		Machine_Node* temp = nullptr;
		if (id > last->ID) {
			temp = Head;
			pathTaken = pathTaken += "machine" + temp->ID.getData() + " ";
		}
		else {
			temp = mappingIdToMachine(id, pathTaken);
		}
		temp->btree->search(id, path);
		std::cout << "File searching  Path : " << pathTaken << " \n";
		if (path != "") {
			std::cout << "\t" << path << "\n";
		}
		else {
			cout << "No Such file found\n";
		}
	}

	void PrintBtree(BigInt id) {
		
		if (Head == nullptr)
			return;
		if (Head->ID == id) {
			Head->btree->display();
		}
		else {
			Machine_Node* temp = Head->next;
			while (temp->ID != id && temp->next != Head)			{
				temp = temp->next;
			}
			if (temp->ID == id) {
				temp->btree->display();
			}else{
					cout << "NO machine with this id\n";
			}
		}

	}

	~Machine_list() {
		Machine_Node* temp = Head;
		while (Head->next != Head) {
			last->next = Head->next;
			Head = Head->next;
			std::queue<File*> q;
			temp->btree->clear(q);
			while (!q.empty()){
				deleteFile1(q.front()->Path);
				q.pop();
			}
			deleteDirectory(temp->ID.getData());
			delete temp;
			temp = Head;
		}
		deleteDirectory(Head->ID.getData());
		Head = nullptr;
		last = nullptr;
	}
};

#endif 