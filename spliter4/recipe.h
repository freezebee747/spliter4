#pragma once

#include "elements.h"
#include "strcontrol.h"

class Recipe {
private:
	std::string recipe;
	std::string expended;
public:
	Recipe(const std::string& r) : recipe(r) {}
	void print();
	void SetExpended(const std::string& exp);
	void SetExpended(std::vector<std::string>& exp);

	std::vector<std::string> SplitRecipe();

};