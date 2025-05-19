#include "target.h"

void Target::print() {
	std::cout << target << std::endl;
}

std::string Target::GetTarget() { 
	return target; 
}

bool Target::IsVariable() {
	if (target[0] == '$' && target[1] == '(' && target.back() == ')') {
		return true;
	}
	return false;
}

void Target::SetWildcard(std::vector<std::string>& wildcard){
	wildcard_files = wildcard;
}

std::string Target::GetVariable() {
	if (IsVariable()) {
		return target.substr(2, target.size() - 3);
	}
	else return "";

}

void Target::SetExpended(const std::string& exp) {
	expended = exp;
}
