#include "rule.h"

// $^ 贸府
void Rule::HandleCaretToken(std::string& token, std::vector<std::unique_ptr<Prerequisite>>& preq) {
	std::string preq_str;
	for (const auto& i : preq) {
		preq_str = preq_str + ' ' + i->GetPreqs();
	} 
	token.swap(preq_str);
}
//$@ 贸府
void Rule::HandleAtToken(std::string& token, std::vector<std::unique_ptr<Target>>& target) {
	std::string target_str;
	for (const auto& i : target) {
		target_str = target_str + ' ' + i->GetTarget();
	}
	token.swap(target_str);
}

//$< 贸府
void Rule::HandleRBracketToken(std::string& token, std::vector<std::unique_ptr<Prerequisite>>& preq) {
	std::string preq_str = preq.front()->GetPreqs();
	token.swap(preq_str);
}


// $(VAR) 肚绰 窃荐 摹券 贸府
void Rule::HandleFunctionToken(std::string& token, std::unordered_map<std::string, std::string>& variables) {
	std::string variable = token.substr(2, token.size() - 3);
	auto test = variables.find(variable);
	if (test != variables.end()) {
		std::string temp = test->second;
		token.swap(temp);
	}

}
