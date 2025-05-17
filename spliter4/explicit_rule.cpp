#include "explicit_rule.h"

Explicit_rule::Explicit_rule(RuleArg& ra) {
	AddTargets(ra.targets);
	AddPreqs(ra.prereq);
	AddRecipe(ra.recipes);
}

void Explicit_rule::variable_expend(std::unordered_map<std::string, std::string>& variable) {
	PrereqExpend(variable);
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
void Explicit_rule::PrereqExpend(std::unordered_map<std::string, std::string>& variables) {
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


void Explicit_rule::RecipeExpended(std::unordered_map<std::string, std::string>& variables) {

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

