#include "MakefileText.h"

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
    }
}
void MakefileText::AddVariable(std::string key, std::string value) {
    variables.emplace(key, value);
}
void MakefileText::AddPhonyTarget(std::string key, std::string value) {
    phony_targets.emplace(key, value);
}
void MakefileText::print() {
    for (const auto& [key, value] : variables) {
        std::cout << key << " : " << value << std::endl;
    }
    for (const auto& [key, value] : phony_targets) {
        std::cout << key << " : " << value << std::endl;
    }
    for (const auto& i : rules) {
        i->print();
    }
}