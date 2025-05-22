#pragma once

#include <sstream>
#include <vector>

int SeparatorCounter(const std::string& target, char sep);
// 왼쪽 공백 제거
std::string ltrim(const std::string& s);
// 오른쪽 공백 제거
std::string rtrim(const std::string& s);
// 양쪽 공백 제거
std::string trim(const std::string& s);
//토큰 이전의 마지막 문자에서 부터 토큰 까지의 공백의 개수
int calc_space(const std::string& str, char token, int pos);
std::string safe_substr(const std::string& str, size_t pos, size_t count);
std::vector<std::string> SplitSpace(const std::string& target);
std::vector<std::string> SplitComma(const std::string& target);
bool hasWhitespace(const std::string& str);
std::string join(const std::vector<std::string>& vec, const std::string& delimiter);