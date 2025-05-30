#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

enum class Severity { Warning, Error, Fatal };

struct Error {
    std::string code;
    unsigned line_number;
    Severity severity;
};

class ErrorMessage {
private:
    std::unordered_map<std::string, std::string> messages;
    ErrorMessage();

public:
    static const ErrorMessage& GetInstance();
    std::string GetMessage(const std::string& code) const;
};

class ErrorCollector {
private:
    std::vector<Error> errors;
public:
    void Add(const Error& e) { errors.push_back(e); }
    const std::vector<Error>& GetAll() const { return errors; }

    void VariableNameCheck(unsigned line, const std::string& var_name);
    void VariableDuplicationCheck(unsigned line, const std::string& var_name);
    void VariableCircleCheck(const std::string& key, const std::string& value);
};

