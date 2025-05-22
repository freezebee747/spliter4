#include "explicit_rule.h"

Explicit_rule::Explicit_rule(RuleArg& ra) {
	AddTargets(ra.targets);
	AddPreqs(ra.prereq);
	AddRecipe(ra.recipes);
}

void Explicit_rule::variable_expend(std::unordered_map<std::string, std::string>& variable) {
	TargetVariableExpend(variable);
	PrereqVariableExpend(variable);
	RecipeVariableExpended(variable);
}
//* 와일드카드의 확장은 사용자가 와일드카드에 어떤 파일이 존재하는지를 확인하기 위한 확장이지
//  구문 분석을 위한 확장은 아님에 주의하라.
void Explicit_rule::wildcard_expend(FileManagement& fm){
	for (const auto& i : targets) {
		std::vector<std::string> wf = fm.glob(i->GetTarget());
		i->SetWildcard(wf);
	}
}

void Explicit_rule::function_expend(){

}

void Explicit_rule::AddTargets(const std::vector<std::string>& target) {
	for (const auto& i : target) {
		targets.push_back(std::make_unique<Target>(i));
	}
}
void Explicit_rule::AddPreqs(const std::vector<std::string>& preq) {
	for (const auto& i : preq) {
		preqs.push_back(std::make_unique<Prerequisite>(i));
	}
}
void Explicit_rule::AddRecipe(const std::vector<std::string>& recipe) {
	for (const auto& i : recipe) {
		recipes.push_back(std::make_unique<Recipe>(i));
	}
}

void Explicit_rule::TargetVariableExpend(std::unordered_map<std::string, std::string>& variables){
	for (const auto& target : targets) {
		if (target->IsVariable()) {
			std::string temp = target->GetVariable();
			auto i = variables.find(temp);
			if (i != variables.end()) {
				target->SetExpended(i->second);
			}
		}
	}
}


void Explicit_rule::PrereqVariableExpend(std::unordered_map<std::string, std::string>& variables) {
	for (const auto& preq : preqs) {
		if (preq->IsVariable()) {
			std::string temp = preq->GetVariable();
			auto i = variables.find(temp);
			if (i != variables.end()) {
				preq->SetExpended(i->second);
			}
		}
	}
}


void Explicit_rule::RecipeVariableExpended(std::unordered_map<std::string, std::string>& variables) {

	for (auto& recipe : recipes) {
		std::vector<std::string> extend = recipe->SplitRecipe();
		for (auto& token : extend) {
			if (token.size() < 2 || token[0] != '$') continue;

			char symbol = token[1];

			switch (symbol) {
			case '^':
				HandleCaretToken(token, preqs);
				break;
			case '@':
				HandleAtToken(token, targets);
				break;
			case '<':
				HandleRBracketToken(token, preqs);
				break;
			case '(':
				if (token.back() == ')') {
					HandleFunctionToken(token, variables);
				}
				break;
			default:

				break;
			}
		}
		recipe->SetExpended(extend);
	}
}



void Explicit_rule::print() {
	std::cout << "Targets : \n";
	for (const auto& i : targets) {
		i->print();
	}
	std::cout << "\n";
	std::cout << "Prerequisite : \n";
	for (const auto& i : preqs) {
		i->print();
	}
	std::cout << "\n";
	std::cout << "Recipe : \n";
	for (const auto& i : recipes) {
		i->print();
	}

	std::cout << "\n";
}

