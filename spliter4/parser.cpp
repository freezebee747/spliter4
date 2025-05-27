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
//��� Ȯ���� ���� �Լ��̴�.
//��� Ȯ�� ������ �ִ��� Ȯ���ϰ�, ������ �ٷ� Ȯ���Ѵ�.
//�Լ��� ���� ������ ������ ���̰� �����Ѵ�.
void Immediate_Evaluation(std::unordered_map<std::string, std::string>& ie, std::vector<std::string>& targets) {
	for (auto& i : targets) {
		std::vector<std::string> vect = SplitValues(i);
		if (IsFunction_func(i)) {
			if (!vect.empty()) {
				vect.pop_back();
			}
			Immediate_Evaluation(ie, vect);
			std::string temp = i.substr(2, i.find_first_of(" ") - 2);
			i = Active_function(temp, vect);
		}
		else {
			for (auto& j : vect) {
				if (j.size() >= 4 && j[0] == '$' && j[1] == '(' && j.back() == ')') {
					std::string temp = j.substr(2, j.size() - 3);
					auto it = ie.find(temp);
					if (it != ie.end()) {
						i.replace(i.find(j), j.size(), it->second);
					}
				}
			}
		}
	}
}

//**�Ľ��Լ�**
//�� �Լ� ������� ������ �ȿ��� ������ ���̴µ�...
MakefileText ParseMakefileTextFromLines(std::vector<std::pair<unsigned, std::string>>& parsing) {
	MakefileText makefileText;
	std::unique_ptr<RuleArg> RA;
	std::string phony_str;
	std::unordered_map<std::string, std::string> IE;//��� �򰡸� ���� ����
	bool IsLong = false;
	bool IsPhony = false;

	//�齽���� ó���� ���� ��ó����
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

	auto FlushPhonyAsRule = [&]() {
		if (!phony_str.empty()) {
			RA = std::make_unique<RuleArg>();
			RA->sr = SelectRule::explicit_rule;
			RA->targets.push_back(phony_str);
			makefileText.AddRule(std::move(RA));
			phony_str.clear();
			IsPhony = false;
			RA.reset();
		}
	};

	auto Immediate_Evaluation_And_Join = [](std::unordered_map<std::string, std::string>& ie, std::string& s) {
		std::vector<std::string> tokens = SplitSpace(s);
		for (auto& token : tokens) {
			if (token.size() >= 4 && token[0] == '$' && token[1] == '(' && token.back() == ')') {
				std::vector<std::string> temp;
				temp.push_back(token);
				Immediate_Evaluation(ie, temp);
				token = temp.front();
			}
		}
		s = join(tokens, " ");
	};


	for (const auto& [line, str] : combinedLines) {
		if (str.empty()) continue;

		// ���ο� rule ���� ���� ���� rule ó��
		if (str[0] != '\t' && RA) {
			makefileText.AddRule(std::move(RA));
			RA.reset(); // �����ϰ� ����
		}

		if (str.find(":=") != std::string::npos) {
			int Sep = str.find('=');
			std::string key = trim(safe_substr(str, 0, Sep - 1));
			std::string value = trim(safe_substr(str, Sep + 1, str.size()));
			std::vector<std::string> temp;
			temp.push_back(value);
			Immediate_Evaluation(IE, temp);
			IE.emplace(key, temp.front());
			//phony_str�� ���ڰ� �����ִ°�? �� ��� target�� �ִ� explicit rule�� �������
			FlushPhonyAsRule();
			continue;
		}

		if (str.find('=') != std::string::npos && !RA) {
			int Sep = str.find('=');
			std::string key = trim(safe_substr(str, 0, Sep - 1));
			std::string value = trim(safe_substr(str, Sep + 1, str.size()));
			makefileText.AddVariable(key, value);
			//phony_str�� ���ڰ� �����ִ°�? �� ��� target�� �ִ� explicit rule�� �������
			FlushPhonyAsRule();
			continue;
		}

		if (str.find(':') != std::string::npos) {

			//phony_str�� ���ڰ� �����ִ°�? �� ��� target�� �ִ� explicit rule�� �������
			FlushPhonyAsRule();
			//�ݷ� ���Ŀ� ���ڿ��� ���°�?
			if (trim(safe_substr(str, str.find(':') + 1, str.size())) == "") {
				//phony_target ó��
				IsPhony = true;
				phony_str = trim(safe_substr(str, 0, str.find(':')));
				continue;
			}

			RA = std::make_unique<RuleArg>();

			if (SeparatorCounter(str, ':') == 1) {
				int colon_pos = str.find(':');

				RA->sr = SelectRule::explicit_rule;
				RA->targets = SplitSpace(trim(safe_substr(str, 0, colon_pos)));
				Immediate_Evaluation(IE, RA->targets);

				int intTemp = (str.find(';') != std::string::npos) ? str.find(';') : str.size();
				RA->prereq = SplitSpace(trim(safe_substr(str, colon_pos + 1, intTemp - colon_pos - 1)));
				Immediate_Evaluation(IE, RA->prereq);
			}
			else if (SeparatorCounter(str, ':') == 2) {
				RA->sr = SelectRule::static_pattern_rule;
				int colon_pos = str.find(':');
				RA->targets = SplitSpace(trim(safe_substr(str, 0, colon_pos)));
				int next_colon = str.find(':', colon_pos + 1);
				RA->target_pattern = trim(safe_substr(str, colon_pos + 1, next_colon - colon_pos - 1));
				RA->prereq_pattern = trim(safe_substr(str, next_colon + 1, str.size() - next_colon - 1));
			}
			else {
				std::cout << "error line: " << line << std::endl;
				exit(1);
			}

			// recipe�� �� �ٿ� ���� ���� ���
			if (str.find(';') != std::string::npos) {
				int semi_pos = str.find(';');
				std::string temp = trim(safe_substr(str, semi_pos + 1, str.size() - semi_pos - 1));
				Immediate_Evaluation_And_Join(IE, temp);
				RA->recipes.push_back(temp);
				makefileText.AddRule(std::move(RA));
				RA.reset();
			}
		}
		else if (str[0] == '\t') {
			if (RA) {
				std::string temp = trim(safe_substr(str, 1, str.size() - 1));
				Immediate_Evaluation_And_Join(IE, temp);
				RA->recipes.push_back(temp);
			}
			else if (IsPhony) {
				makefileText.AddPhonyTarget(phony_str, trim(str));
				phony_str.clear();
				IsPhony = false;
			}
			else {
				throw std::runtime_error("Recipe without rule at line: " + std::to_string(line));
			}
		}
	}

	// ������ rule ó��
	if (RA) {
		makefileText.AddRule(std::move(RA));
		RA.reset();
	}
	FlushPhonyAsRule();

	for (const auto& i : IE) {
		std::cout << i.first << ": " << i.second << '\n';
	}


	return makefileText;
}
//////////////////////////////////////////////////////////////////////////////
// ���ο� �ļ�!!
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
			block->variable_line = str;
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
			block->lines.push_back(str);
		}
		if (block && str[0] == '\t') {
			block->lines.push_back(str);
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

void Immediate_Evaluation_two(std::vector<std::string>& targets) {
	std::unordered_map<std::string, std::string> variables = MakefileText::GetVariables();
	for (auto& i : targets) {
		std::vector<std::string> vect = SplitValues(i);
		if (IsFunction_func(i)) {
			if (!vect.empty()) {
				vect.pop_back();
			}
			Immediate_Evaluation_two(vect);
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
						//Immediate_Evaluation_two(vect);
					}
				}
			}
		}
	}
}

MakefileText ParseMakefileTextFromLines_redefine(std::vector<Block>& blocks) {
	MakefileText makefileText;
	std::unique_ptr<RuleArg> RA;

	for (const auto& block : blocks) {
		if (block.type == BlockType::variable) {
			if (block.variable_line.find(":=") != std::string::npos) {
				int Sep = block.variable_line.find('=');
				std::string key = trim(safe_substr(block.variable_line, 0, Sep - 1));
				std::string value = trim(safe_substr(block.variable_line, Sep + 1, block.variable_line.size()));
				std::vector<std::string> temp;
				temp.push_back(value);
				Immediate_Evaluation_two(temp);
				value = temp[0];
				makefileText.AddVariable(key, value);
			}
			else {
				int Sep = block.variable_line.find('=');
				std::string key = trim(safe_substr(block.variable_line, 0, Sep - 1));
				std::string value = trim(safe_substr(block.variable_line, Sep + 1, block.variable_line.size()));
				makefileText.AddVariable(key, value);
			}
		}

		else if (block.type == BlockType::rule) {
			RA = std::make_unique<RuleArg>();
			std::string first_line = block.lines[0];
			if (SeparatorCounter(first_line, ':') == 1) {
				int colon_pos = first_line.find(':');

				RA->sr = SelectRule::explicit_rule;
				RA->targets = SplitSpace(trim(safe_substr(first_line, 0, colon_pos)));
				Immediate_Evaluation_two(RA->targets);

				int intTemp = (first_line.find(';') != std::string::npos) ? first_line.find(';') : first_line.size();
				RA->prereq = SplitSpace(trim(safe_substr(first_line, colon_pos + 1, intTemp - colon_pos - 1)));
				Immediate_Evaluation_two(RA->prereq);
			}
			else if (SeparatorCounter(first_line, ':') == 2) {
				RA->sr = SelectRule::static_pattern_rule;
				int colon_pos = first_line.find(':');
				RA->targets = SplitSpace(trim(safe_substr(first_line, 0, colon_pos)));
				Immediate_Evaluation_two(RA->targets);

				int next_colon = first_line.find(':', colon_pos + 1);
				RA->target_pattern = trim(safe_substr(first_line, colon_pos + 1, next_colon - colon_pos - 1));
				RA->prereq_pattern = trim(safe_substr(first_line, next_colon + 1, first_line.size() - next_colon - 1));
			}

			if (first_line.find(";") != std::string::npos) {
				int semi_pos = first_line.find(';');
				std::string temp = trim(safe_substr(first_line, semi_pos + 1, first_line.size() - semi_pos - 1));
				RA->recipes.push_back(temp);
			}

			for (size_t i = 1; i < block.lines.size(); i++) {
				RA->recipes.push_back(trim(block.lines[i]));
			}
			makefileText.AddRule(std::move(RA));
			RA.reset();
		}
	}

	return makefileText;
}
