#pragma once
#include "elements.h"
#include <unordered_set>

class Prerequisite {
private:
	std::string preqs;
	std::string expended;
public:
	Prerequisite(const std::string& p) : preqs(p) {};
	void print();
	void pattern_expend(std::unordered_set<std::string>& filenames);
	std::string GetPreqs();
};