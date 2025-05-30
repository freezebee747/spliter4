#pragma once
#include <set>
#include <unordered_set>

#include "error.h"
#include "file.h"
#include "rule.h"
#include "explicit_rule.h"
#include "static_pattern_rule.h"

class MakefileText {
private:
	std::vector<std::unique_ptr<Rule>> rules;
	static std::unordered_map<std::string, std::string> variables;
	std::vector<std::pair<std::string, std::string>> raw_variables;

	std::unordered_map<std::string, std::vector<std::string>> phony_targets;
	//std::unordered_map<std::string, std::string> special_targets;
	std::vector<std::string> include_files;
	FileManagement fm;
	ErrorCollector err;

public:
	const std::unordered_map< std::string, std::vector<std::string >>& GetPhony() const;
	static const std::unordered_map<std::string, std::string>& GetVariables();
	std::vector<std::pair<std::string, std::string>>& GetRawVariable();
	const std::vector<std::unique_ptr<Rule>>& GetRules() const;
	const FileManagement& GetFileManager() const;

	void ThrowError(const Error& error);
	void AddRule(std::unique_ptr<RuleArg> ra);
	void AddVariable(std::string key, std::string value);
	void AddPhonyTarget(std::string key, std::vector<std::string> value);
	void Expend();
	void print();
};
