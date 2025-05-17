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


MakefileText ParseMakefileTextFromLines(std::vector<std::pair<unsigned, std::string>>& parsing) {
	MakefileText makefileText;
	std::unique_ptr<RuleArg> RA;
	std::string phony_str;
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

	for (const auto& [line, str] : combinedLines) {
		if (str.empty()) continue;

		// ���ο� rule ���� ���� ���� rule ó��
		if (str[0] != '\t' && RA) {
			makefileText.AddRule(std::move(RA));
			RA.reset(); // �����ϰ� ����
		}

		if (str.find('=') != std::string::npos) {
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

				int intTemp = (str.find(';') != std::string::npos) ? str.find(';') : str.size();
				RA->prereq = SplitSpace(trim(safe_substr(str, colon_pos + 1, intTemp - colon_pos - 1)));

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
				RA->recipes.push_back(trim(safe_substr(str, semi_pos + 1, str.size() - semi_pos - 1)));
				makefileText.AddRule(std::move(RA));
				RA.reset();
			}
		}
		else if (str[0] == '\t') {
			if (RA) {
				RA->recipes.push_back(trim(str));
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

	return makefileText;
}
