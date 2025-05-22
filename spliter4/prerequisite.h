#pragma once
#include "elements.h"

class Prerequisite {
private:
	std::string preqs;
	std::string expended;
public:
	Prerequisite(const std::string& p) : preqs(p) {};
	void print();
	std::string GetPreqs();
	bool IsVariable();
	bool IsFunction();
	std::string GetVariable();
	void SetExpended(const std::string& exp);

};