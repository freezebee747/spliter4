#pragma once

#include <sstream>
#include <vector>

int SeparatorCounter(const std::string& target, char sep);
// ���� ���� ����
std::string ltrim(const std::string& s);
// ������ ���� ����
std::string rtrim(const std::string& s);
// ���� ���� ����
std::string trim(const std::string& s);
//��ū ������ ������ ���ڿ��� ���� ��ū ������ ������ ����
int calc_space(const std::string& str, char token, int pos);
std::string safe_substr(const std::string& str, size_t pos, size_t count);
std::vector<std::string> SplitSpace(const std::string& target);
std::vector<std::string> SplitComma(const std::string& target);
bool hasWhitespace(const std::string& str);
std::string join(const std::vector<std::string>& vec, const std::string& delimiter);