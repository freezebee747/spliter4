#pragma once
#include "elements.h"

class Target {
private:
	std::string target;
	std::string expended;
	std::vector<std::string> wildcard_files;
public:
	Target(const std::string& t) :target(t) {};
	std::string GetTarget();
	void print();
	bool IsVariable();
	bool IsFunction();
	void SetWildcard(std::vector<std::string>& wildcard);
	std::string GetVariable();
	void SetExpended(const std::string& exp);
};