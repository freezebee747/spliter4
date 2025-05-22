#include "recipe.h"

std::vector<std::string> Recipe::SplitRecipe() {
	return SplitSpace(recipe);

}
void Recipe::SetExpended(std::vector<std::string>& exp) {
	expended.clear();
	for (const auto& i : exp) {
		expended = expended + ' ' + i;
	}
}

bool Recipe::IsFunction(){
	return IsFunction_func(recipe);
}

void Recipe::SetExpended(const std::string& exp) {
	expended = exp;
}

void Recipe::print() {
	std::cout << recipe << std::endl;
	std::cout << "expend: " << expended << '\n';
}
