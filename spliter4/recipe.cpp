#include <sstream>

#include "elements.h"
#include "strcontrol.h"

std::vector<std::string> Recipe::SplitRecipe() {
	return SplitSpace(recipe);

}
void Recipe::SetExpended(std::vector<std::string>& exp) {
	expended.clear();
	for (const auto& i : exp) {
		expended = expended + ' ' + i;
	}
}

void Recipe::SetExpended(const std::string& exp) {
	expended = exp;
}