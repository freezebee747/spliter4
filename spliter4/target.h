#pragma once
#include "elements.h"
#include <unordered_set>
class Target {
private:
	std::string target;
	std::string expended;
	std::vector<std::string> wildcard_files;
public:
	Target(const std::string& t) :target(t) {};
	std::string GetTarget();
	void pattern_expend(std::unordered_set<std::string>& filenames);
	void print();
};