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
//$(var)들을 각각 나눈다
//예로 "$(var1) $(var2)" 문자열이 입력으로 들어왔다면, $(var1)과 $(var2)로 나누고 그것의 집합을 리턴한다.
//함수의 경우 마지막 값이 입력값이다.
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
			//$( ) 보존
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
//즉시 확장을 위한 함수이다.
//즉시 확장 변수가 있는지 확인하고, 있으면 바로 확장한다.
//함수일 때랑 변수일 때랑의 차이가 존재한다.
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

//**파싱함수**
//이 함수 대대적인 수술이 팔요할 것으로 보이는데...
MakefileText ParseMakefileTextFromLines(std::vector<std::pair<unsigned, std::string>>& parsing) {
	MakefileText makefileText;
	std::unique_ptr<RuleArg> RA;
	std::string phony_str;
	std::unordered_map<std::string, std::string> IE;//즉시 평가를 위한 변수
	bool IsLong = false;
	bool IsPhony = false;

	//백슬래시 처리를 위한 전처리기
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

		// 새로운 rule 시작 전에 이전 rule 처리
		if (str[0] != '\t' && RA) {
			makefileText.AddRule(std::move(RA));
			RA.reset(); // 안전하게 해제
		}

		if (str.find(":=") != std::string::npos) {
			int Sep = str.find('=');
			std::string key = trim(safe_substr(str, 0, Sep - 1));
			std::string value = trim(safe_substr(str, Sep + 1, str.size()));
			std::vector<std::string> temp;
			temp.push_back(value);
			Immediate_Evaluation(IE, temp);
			IE.emplace(key, temp.front());
			//phony_str에 문자가 남아있는가? 이 경우 target만 있는 explicit rule로 취급하자
			FlushPhonyAsRule();
			continue;
		}

		if (str.find('=') != std::string::npos && !RA) {
			int Sep = str.find('=');
			std::string key = trim(safe_substr(str, 0, Sep - 1));
			std::string value = trim(safe_substr(str, Sep + 1, str.size()));
			makefileText.AddVariable(key, value);
			//phony_str에 문자가 남아있는가? 이 경우 target만 있는 explicit rule로 취급하자
			FlushPhonyAsRule();
			continue;
		}

		if (str.find(':') != std::string::npos) {

			//phony_str에 문자가 남아있는가? 이 경우 target만 있는 explicit rule로 취급하자
			FlushPhonyAsRule();
			//콜론 이후에 문자열이 없는가?
			if (trim(safe_substr(str, str.find(':') + 1, str.size())) == "") {
				//phony_target 처리
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

			// recipe가 한 줄에 같이 있을 경우
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

	// 마지막 rule 처리
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
// 새로운 파서!!
// 문서를 블록단위로 나눈다.
std::vector<Block> SplitByBlock(std::vector<std::pair<unsigned, std::string>>& parsing) {
	std::vector<Block> blocks;
	bool recipe_flag = false;

	// 백슬래시로 이어지는 라인들을 합치기 위한 코드
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

		//만일 빈 라인이면 일단 넘긴다.
		if (str.empty())continue;
		
		//변수 처리
		//만일 tab으로 시작하지 않으면서 = 가 포함되어있다면 변수이므로 한 블록으로 처리한다..
		//일단 즉시 확장 변수이던 누적 변수이던 변수로 취급하자
		if (str.find("=") != std::string::npos && str[0] != '\t') {
            block = std::make_unique<Block>();
			block->type = BlockType::variable;
			block->variable_line = str;
			blocks.push_back(*block);
			block.reset();
			continue;
		}

		//규칙 처리
		//만일 tab으로 시작하지 않으면서 : 가 포함되어 있다면 규칙이므로 그 블록을 저장한다.
		//다음 문장이 tab으로 시작하지 않을 때 까지 recipe 로 취급한다.
		//tab이 포함된 빈 줄은 블록에 포함하고, tab이 포함되지 않은 빈 줄은 버린다.
		if (str.find(":") != std::string::npos && str[0] != '\t') {
			block = std::make_unique<Block>();
			block->type = BlockType::rule;
			block->lines.push_back(str);
		}
		if (block && str[0] == '\t') {
			block->lines.push_back(str);
			//만일 다음 라인이(빈 라인은 첫번째 조건문에서 버려진다.) 탭으로 시작하지 않는다면 block를 저장한 후 클리어한다.
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
