#pragma once

#include <sstream>
#include <vector>

int SeparatorCounter(const std::string& target, char sep) {

	int count = 0;
	std::string temp = target;
	while (true) {
		int sep_pos = temp.find_first_of(sep);
		if (sep_pos == std::string::npos) break;
		count++;
		temp = temp.substr(sep_pos + 1);
	}

	return count;
}
// ���� ���� ����
std::string ltrim(const std::string& s) {
	size_t start = s.find_first_not_of(" \t\n\r");
	return (start == std::string::npos) ? "" : s.substr(start);
}

// ������ ���� ����
std::string rtrim(const std::string& s) {
	size_t end = s.find_last_not_of(" \t\n\r");
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

// ���� ���� ����
std::string trim(const std::string& s) {
	return rtrim(ltrim(s));
}

//��ū ������ ������ ���ڿ��� ���� ��ū ������ ������ ����
int calc_space(const std::string& str, char token, int pos) {
	int token_length = str.find(token, pos);
	if (token_length == std::string::npos) {
		return 0;
	}
	if (str[token_length - 1] != ' ') return 0;
	int temp = token_length - 1;
	while (temp >= 0) {
		if (str[temp] == ' ') {
			temp--;
		}
		else break;
	}

	return token_length - temp;
}

std::string safe_substr(const std::string& str, size_t pos, size_t count) {
	if (pos == count) {
		return "";  // �� �Ű������� ������ �� ���ڿ� ��ȯ
	}
	if (pos >= str.size()) {
		return "";  // pos�� ������ ����� �� ���ڿ�
	}
	return str.substr(pos, count);
}

std::vector<std::string> SplitSpace(const std::string& target) {
	std::istringstream iss(target);
	std::vector<std::string> tokens;
	std::string token;
	while (iss >> token) {
		tokens.push_back(token);
	}
	return tokens;
}