#include "target.h"
#include "parser.h"

void Target::print() {
	std::cout << target << std::endl;
}

std::string Target::GetTarget() { 
	return target; 
}
//���� Ȯ��
void Target::pattern_expend(std::unordered_set<std::string>& filenames){
	std::vector<std::string> temp = ExpendPatternRule(target, filenames);
	for (const auto& i : temp) {
		expended = expended + " " + i;
	}
	expended = trim(expended);
}
