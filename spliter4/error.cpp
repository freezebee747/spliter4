#include "error.h"
#include "MakefileText.h"

const ErrorMessage& ErrorMessage::GetInstance() {
    static ErrorMessage instance;
    return instance;
}

ErrorMessage::ErrorMessage() {
    messages["E001"] = "변수 이름이 잘못되었습니다.";
    messages["E002"] = "";
    messages["E003"] = "";
    // ...
}

std::string ErrorMessage::GetMessage(const std::string& code) const {
    auto it = messages.find(code);
    return (it != messages.end()) ? it->second : "Unknown error";
}

//변수 이름 검사
void ErrorCollector::VariableNameCheck(unsigned line, const std::string& var_name) {
    for (int i = 0; i < var_name.size(); i++) {
        if (!((var_name[i] >= 'a' && var_name[i] <= 'z') ||
            (var_name[i] >= 'A' && var_name[i] <= 'Z') ||
            var_name[i] == '_')) {
            Error er;
            er.severity = Severity::Error;
            er.line_number = line;
        }
    }
}

void ErrorCollector::VariableDuplicationCheck(unsigned line, const std::string& var_name) {
    const std::unordered_map<std::string, std::string>& variable_map = MakefileText::GetVariables();
    
    auto i = variable_map.find(var_name);
    if (i != variable_map.end()) {

    }

}
void ErrorCollector::VariableCircleCheck(const std::string& key, const std::string& value) {
   const std::unordered_map<std::string, std::string>& variable_map = MakefileText::GetVariables();

}