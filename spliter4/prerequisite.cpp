#include "prerequisite.h"

std::string Prerequisite::GetPreqs() {
	return preqs;
}

bool Prerequisite::IsVariable() {
	if (preqs[0] == '$' && preqs[1] == '(' && preqs.back() == ')') {
		return true;
	}
	return false;
}

std::string Prerequisite::GetVariable() {
	if (IsVariable()) {
		return preqs.substr(2, preqs.size() - 3);
	}
	else return "";

}

void Prerequisite::SetExpended(const std::string& exp) {
	expended = exp;
}

void Prerequisite::print() {
	std::cout << preqs << std::endl;
}
