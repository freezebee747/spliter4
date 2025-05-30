#include "MakefileText.h"

std::unordered_map<std::string, std::string> MakefileText::variables;

const std::unordered_map<std::string, std::string>& MakefileText::GetVariables() {
	return variables;
}

std::vector<std::pair<std::string, std::string>>& MakefileText::GetRawVariable(){
	return raw_variables;
}

const std::vector<std::unique_ptr<Rule>>& MakefileText::GetRules() const{
	return this->rules;
}

const std::unordered_map<std::string, std::vector<std::string>>& MakefileText::GetPhony() const{
	return this->phony_targets;
}

const FileManagement& MakefileText::GetFileManager() const{
	return this->fm;
}

void MakefileText::ThrowError(const Error& error){
	err.Add(error);

}

void MakefileText::AddRule(std::unique_ptr<RuleArg> ra) {
	switch (ra->sr) {
	case SelectRule::explicit_rule:
		rules.push_back(std::make_unique<Explicit_rule>(*ra));
		break;
	case SelectRule::static_pattern_rule:
		rules.push_back(std::make_unique<Static_pattern_Rule>(*ra));
		break;
	}
}
void MakefileText::Expend() {
	for (const auto& i : rules) {
		i->variable_expend(variables);
		i->function_expend(variables);
	}
}
void MakefileText::AddVariable(std::string key, std::string value) {
	variables.emplace(key, value);
}

void MakefileText::AddPhonyTarget(std::string key, std::vector<std::string> value)
{
	phony_targets.emplace(key, value);
}

void MakefileText::print() {
	for (const auto& [key, value] : variables) {
		std::cout << key << ": " << value << std::endl;
	}
	for (const auto& [key, value] : phony_targets) {
		std::cout << key << ": \n";
		for (const auto& i : value) {
			std::cout << i << "\n";
		}
	}
	for (const auto& i : rules) {
		i->print();
	}
}