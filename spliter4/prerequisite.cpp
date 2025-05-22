#include "prerequisite.h"

std::string Prerequisite::GetPreqs() {
	return preqs;
}

bool Prerequisite::IsVariable() {
	return IsVariable_func(preqs);
}

bool Prerequisite::IsFunction(){
	return IsFunction_func(preqs);
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
