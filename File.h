#pragma once
#include <string>
#include <iostream>
#include "BigInt.h"
struct File {
	BigInt Hash;
	std::string Path;
	File* next;
	File(std::string path, BigInt& hash) {
		Path = path;
		Hash = hash;
		next = nullptr;
	}

	bool operator>(File& file) {
		return Hash > file.Hash;
	}
	bool operator>(BigInt& hash) {
		return Hash > hash;
	}
	bool operator<(File& file) {
		return Hash < file.Hash;
	}bool operator<(BigInt& hash) {
		return Hash < hash;
	}
	bool operator == (File& file) {
		return Hash == file.Hash;
	}
	bool operator ==(BigInt& hash) {
		return Hash == hash;
	}
	bool operator <= (File& file) {
		return Hash <= file.Hash;
	}
	bool operator <= (BigInt& hash) {
		return Hash <= hash;
	}
	bool operator >= (File& file) {
		return Hash >= file.Hash;
	}
	bool operator >= (BigInt& hash) {
		return Hash >= hash;
	}
	friend std::ostream& operator<<(std::ostream& os, File& obj) {
			os << obj.Hash << " " << obj.Path;
			return os;
	}

};



