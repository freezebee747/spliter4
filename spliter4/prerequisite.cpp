#include "prerequisite.h"
#include "parser.h"

std::string Prerequisite::GetPreqs() {
	return preqs;
}

void Prerequisite::print() {
	std::cout << preqs << std::endl;
}

//지언 확장
void Prerequisite::pattern_expend(std::unordered_set<std::string>& filenames) {
	std::vector<std::string> temp = ExpendPatternRule(preqs, filenames);
	for (const auto& i : temp) {
		expended = expended + " " + i;
	}
	expended = trim(expended);
}