#pragma once
#include <set>

#include "file.h"
#include "rule.h"
#include "explicit_rule.h"
#include "static_pattern_rule.h"

class MakefileText {
private:
    std::vector<std::unique_ptr<Rule>> rules;
    std::unordered_map<std::string, std::string> variables;
    std::unordered_map<std::string, std::string> phony_targets;
//    std::unordered_map<std::string, std::string> special_targets;
    std::vector<std::string> include_files;
    FileManagement fm;

public:
    void AddRule(std::unique_ptr<RuleArg> ra);
    void AddVariable(std::string key, std::string value);
    void AddPhonyTarget(std::string key, std::string value);
    void Expend();
    void print();
};