#include "parser.h"


std::vector<std::pair<unsigned, std::string>> ReadFileWithLineNumbers(const std::string& filename) {
	std::ifstream in(filename);
	std::vector<std::pair<unsigned, std::string>> lines;
	std::string line;
	unsigned line_num = 1;

	if (in.is_open()) {
		while (std::getline(in, line)) {
			if (line.find('#')) {
				line = safe_substr(line, 0, line.find('#'));
			}
			lines.emplace_back(line_num, line);
			line_num++;
		}
	}
	return lines;
}
//$(var)���� ���� ������
//���� "$(var1) $(var2)" ���ڿ��� �Է����� ���Դٸ�, $(var1)�� $(var2)�� ������ �װ��� ������ �����Ѵ�.
//�Լ��� ��� ������ ���� �Է°��̴�.
std::vector<std::string> SplitValues(std::string& target) {
	int count = 0;
	int depth = 0;
	std::vector<std::string> result;
	std::vector<int> loc;

	while (count < target.size()) {
		if (target[count] == '$') {
			count = count + 2;
			depth++;
			loc.push_back(count);
		}
		else if (target[count] == ')') {
			//$( ) ����
			std::string str = target.substr(loc.back() - 2, count - loc.back() + 3);
			result.push_back(str);
			if (!loc.empty()) {
				loc.pop_back();
			}
			count++;
		}
		else count++;
	}
	return result;
}


// ������ ��ϴ����� ������.
std::vector<Block> SplitByBlock(std::vector<std::pair<unsigned, std::string>>& parsing) {
	std::vector<Block> blocks;
	bool recipe_flag = false;

	// �齽���÷� �̾����� ���ε��� ��ġ�� ���� �ڵ�
	std::vector<std::pair<unsigned, std::string>> combinedLines;
	for (size_t i = 0; i < parsing.size(); ++i) {
		const auto& [lineNum, lineText] = parsing[i];
		std::string combined = lineText;
		unsigned originalLine = lineNum;

		while (!combined.empty() && combined.back() == '\\') {
			combined.pop_back();
			if (++i < parsing.size()) {
				combined += trim(parsing[i].second);
			}
			else {
				break;
			}
		}

		combinedLines.emplace_back(originalLine, combined);
	}


	std::unique_ptr<Block> block;
	block.reset();

	for (auto it = combinedLines.begin(); it != combinedLines.end(); ++it) {
		const auto& [line, str] = *it;

		//���� �� �����̸� �ϴ� �ѱ��.
		if (str.empty())continue;
		
		//���� ó��
		//���� tab���� �������� �����鼭 = �� ���ԵǾ��ִٸ� �����̹Ƿ� �� ������� ó���Ѵ�..
		//�ϴ� ��� Ȯ�� �����̴� ���� �����̴� ������ �������
		if (str.find("=") != std::string::npos && str[0] != '\t') {
            block = std::make_unique<Block>();
			block->type = BlockType::variable;
			block->var_line = { line, str };
			blocks.push_back(*block);
			block.reset();
			continue;
		}

		//��Ģ ó��
		//���� tab���� �������� �����鼭 : �� ���ԵǾ� �ִٸ� ��Ģ�̹Ƿ� �� ����� �����Ѵ�.
		//���� ������ tab���� �������� ���� �� ���� recipe �� ����Ѵ�.
		//tab�� ���Ե� �� ���� ��Ͽ� �����ϰ�, tab�� ���Ե��� ���� �� ���� ������.
		if (str.find(":") != std::string::npos && str[0] != '\t') {
			block = std::make_unique<Block>();
			block->type = BlockType::rule;
			block->_lines.push_back({ line, str });
		}
		if (block && str[0] == '\t') {
			block->_lines.push_back({line, str });
			//���� ���� ������(�� ������ ù��° ���ǹ����� ��������.) ������ �������� �ʴ´ٸ� block�� ������ �� Ŭ�����Ѵ�.
			auto next_it = std::next(it);
			if (next_it != combinedLines.end() && next_it->second[0] != '\t') {
				blocks.push_back(*block);
				block.reset();
			}
			continue;
		}

	}

	if (block) {
		blocks.push_back(*block);
	}

	return blocks;
}

void Immediate_Evaluation(std::vector<std::string>& targets) {
	std::unordered_map<std::string, std::string> variables = MakefileText::GetVariables();
	for (auto& i : targets) {
		std::vector<std::string> vect = SplitValues(i);
		if (IsFunction_func(i)) {
			if (!vect.empty()) {
				vect.pop_back();
			}
			Immediate_Evaluation(vect);
			std::string temp = i.substr(2, i.find_first_of(" ") - 2);
			i = Active_function(temp, vect);
		}
		else {
			for (auto& j : vect) {
				if (j.size() >= 4 && j[0] == '$' && j[1] == '(' && j.back() == ')') {
					std::string temp = j.substr(2, j.size() - 3);
					auto it = variables.find(temp);
					if (it != variables.end()) {
						i.replace(i.find(j), j.size(), it->second);
						IE_variable(j, variables);
					}
				}
			}
		}
	}
}
//�߰� ���� Ȯ���� ���� �Լ�
//����
//CC = gcc
//TEST := $(CC)
//TEMP := $(TEST)
// �� ��
//TEMP�� $(TEST)->$(CC)->gcc �� ��� Ȯ���ϵ��� �ϱ� ���� �Լ��̴�.
//
void IE_variable(std::string& value, std::unordered_map<std::string, std::string>& variables) {
	if (value.size() >= 4 && value[0] == '$' && value[1] == '(' && value.back() == ')') {
		std::string temp = value.substr(2, value.size() - 3);
		auto it = variables.find(temp);
		if (it != variables.end()) {
			value = temp;
			IE_variable(value, variables);
		}
	}
}

//���ο� �ļ�!!
//�׳� �ļ��� Ŭ������ �ٲٴ� �Ϳ� ���� ������ ��� �ʿ�
MakefileText ParseMakefileTextFromLines(std::vector<Block>& blocks) {
	MakefileText makefileText;
	std::unique_ptr<RuleArg> RA;

	for (const auto& block : blocks) {
		if (block.type == BlockType::variable) {
			if (block.var_line.second.find(":=") != std::string::npos) {
				int Sep = block.var_line.second.find('=');
				std::string key = trim(safe_substr(block.var_line.second, 0, Sep - 1));
				//���� �̸� �˻�
				std::string value = trim(safe_substr(block.var_line.second, Sep + 1, block.var_line.second.size()));
				std::vector<std::string> temp;
				temp.push_back(value);
				Immediate_Evaluation(temp);
				value = temp[0];
				//���� �ߺ� �˻�
				//���� ��ȯ �˻�
				makefileText.AddVariable(key, value);
			}
			else {
				int Sep = block.var_line.second.find('=');
				std::string key = trim(safe_substr(block.var_line.second, 0, Sep - 1));
				//���� �̸� �˻�
				std::string value = trim(safe_substr(block.var_line.second, Sep + 1, block.var_line.second.size()));
				std::pair raw(key, value);
				makefileText.GetRawVariable().push_back(raw);
				//���� �ߺ� �˻�
				
				//���� ��ȯ �˻�

				makefileText.AddVariable(key, value);
			}
		}

		else if (block.type == BlockType::rule) {
			RA = std::make_unique<RuleArg>();
			std::string first_line = block._lines[0].second;
			if (SeparatorCounter(first_line, ':') == 1) {
				size_t colon_pos = first_line.find(':');


				if (trim(safe_substr(first_line, colon_pos + 1, first_line.size() - colon_pos - 1)) == "") {
					std::string phony_name = trim(safe_substr(first_line, 0, colon_pos));
					std::vector<std::string> phony_recipe;
					for (size_t i = 1; i < block._lines.size(); i++) {
						phony_recipe.push_back(trim(block._lines[i].second));
					}
					makefileText.AddPhonyTarget(phony_name, phony_recipe);
					RA.reset();
					continue;
				}
				else {
					RA->sr = SelectRule::explicit_rule;
					RA->targets = SplitSpace(trim(safe_substr(first_line, 0, colon_pos)));
					Immediate_Evaluation(RA->targets);

					size_t intTemp = (first_line.find(';') != std::string::npos) ? first_line.find(';') : first_line.size();
					RA->prereq = SplitSpace(trim(safe_substr(first_line, colon_pos + 1, intTemp - colon_pos - 1)));
					Immediate_Evaluation(RA->prereq);

					if (first_line.find(';') != std::string::npos) {
						std::string rec = safe_substr(first_line, intTemp, first_line.size() - intTemp);
						if (rec != "") {
							RA->recipes.push_back(rec);
						}
					}
				}
			}
			else if (SeparatorCounter(first_line, ':') == 2) {
				RA->sr = SelectRule::static_pattern_rule;
				int colon_pos = first_line.find(':');
				RA->targets = SplitSpace(trim(safe_substr(first_line, 0, colon_pos)));
				Immediate_Evaluation(RA->targets);

				int next_colon = first_line.find(':', colon_pos + 1);
				RA->target_pattern = trim(safe_substr(first_line, colon_pos + 1, next_colon - colon_pos - 1));
				RA->prereq_pattern = trim(safe_substr(first_line, next_colon + 1, first_line.size() - next_colon - 1));
			}

			if (first_line.find(";") != std::string::npos) {
				int semi_pos = first_line.find(';');
				std::string temp = trim(safe_substr(first_line, semi_pos + 1, first_line.size() - semi_pos - 1));
				RA->recipes.push_back(temp);
			}

			for (size_t i = 1; i < block._lines.size(); i++) {
				RA->recipes.push_back(trim(block._lines[i].second));
			}
			makefileText.AddRule(std::move(RA));
			RA.reset();
		}
	}

	return makefileText;
}

std::vector<std::string>ExpendPatternRule(const std::string& pattern, const std::unordered_set<std::string>& filenames) {
	if (SeparatorCounter(pattern, '%') != 1) {
		return std::vector<std::string>();
	}
	size_t sep = pattern.find("%");
	std::string prefix = safe_substr(pattern, 0, sep);
	std::string suffix = pattern.substr(sep + 1, pattern.size() - sep);
	std::vector<std::string> result;

	for (const auto& i : filenames) {
		if (i.substr(0, prefix.size()) == prefix && i.substr(i.size() - suffix.size(), suffix.size()) == suffix) {
			result.push_back(i);
		}
	}
	return result;
}