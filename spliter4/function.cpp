#include <algorithm>
#include "function.h"

using FunctionHandler = std::function<std::string(const std::vector<std::string>&)>;

std::unordered_map<std::string, FunctionHandler> function_map = {
    {"subst", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() >= 3) return function_subst(args[0], args[1], args[2]);
        return ""; // or throw error
    }},
    {"patsubst", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() >= 3) return function_patsubst(args[0], args[1], args[2]);
        return "";
    }},
    {"strip", [](const std::vector<std::string>& args) -> std::string {
        if (!args.empty()) return function_strip(args[0]);
        return "";
    }},
    {"findstring", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() >= 2) return function_findstring(args[0], args[1]);
        return "";
    }},
    {"filter", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() >= 2) return function_filter(args[0], args[1]);
        return "";
    }},
    {"sort", [](const std::vector<std::string>& args)-> std::string {
         if (args.size() >= 1) return function_sort(args[0]);
         return "";
    }},
    {"word",[](const std::vector<std::string>& args)->std::string {
        if (args.size() >= 2) return function_word(args[0], args[1]);
    }}

    // 추가
};

std::string Active_function(const std::string& function_name, const std::vector<std::string>& args) {
    auto it = function_map.find(function_name);
    if (it != function_map.end()) {
        return it->second(args);
    }
    else {
        return "";
    }
}

std::string patsubs(const std::string& wildcard, const std::string& changer, const std::string& target_str) {
    if (hasWhitespace(wildcard)) return std::string();

    size_t pos = wildcard.find('%');
    if (pos == std::string::npos) return std::string(); 

    std::string begin = safe_substr(wildcard, 0, pos);
    std::string end = safe_substr(wildcard, pos + 1, wildcard.size() - pos - 1);

    size_t cpos = changer.find('%');
    if (cpos == std::string::npos) return std::string(); 

    std::string cbegin = safe_substr(changer, 0, cpos);
    std::string cend = safe_substr(changer, cpos + 1, changer.size() - cpos - 1);

    if (target_str.size() < begin.size() + end.size()) return target_str;

    if (target_str.substr(0, begin.size()) == begin &&
        target_str.substr(target_str.size() - end.size()) == end) {

        std::string stem = target_str.substr(
            begin.size(), target_str.size() - begin.size() - end.size()
        );

        return cbegin + stem + cend;
    }

    return target_str; 
}

std::string function_subst(const std::string& from, const std::string& to, const std::string& text){
	int begin = 0;
	std::string return_string = text;
	while (text.find(from, begin) != std::string::npos) {
		begin = text.find(from, begin);
		return_string.replace(begin, from.size(), to);
		begin += to.size();
	}
	return return_string;
}

std::string function_patsubst(const std::string& pattern, const std::string& replacement, const std::string& text){
	std::string result;
	std::vector<std::string> temp = SplitSpace(text);
	for (const auto& i : temp) {
		result = result + ' ' + patsubs(pattern, replacement, i);
	}

	return trim(result);

}

std::string function_strip(const std::string& string){
    std::vector<std::string> temp = SplitSpace(string);
    std::string result;
    for (const auto& i : temp) {
        result = result + ' ' + i;
    }
    return trim(result);
}

std::string function_findstring(const std::string& find, const std::string& in){
    if (in.find(find)) {
        return find;
    }
    return "";
}

std::string function_filter(const std::string& patterns, const std::string& text) {
    std::vector<std::string> pattern = SplitSpace(patterns);
    std::vector<std::string> texts = SplitSpace(text);
    std::string result;
    for (const auto& i : pattern) {
        for (const auto& j : texts) {

            size_t pos = i.find('%');
            if (pos == std::string::npos) return std::string();

            std::string begin = safe_substr(i, 0, pos);
            std::string end = safe_substr(i, pos + 1, i.size() - pos - 1);

            if (j.substr(0, begin.size()) == begin &&
                j.substr(j.size() - end.size()) == end) {
                result = result + ' ' + j;
            }

        }
    }
    return trim(result);
}

std::string function_sort(const std::string& list) {
    std::string result;
    std::vector<std::string> elems = SplitSpace(list);
    std::sort(elems.begin(), elems.end());
    for (const auto& elem : elems) {
        result = result + ' ' + elem;
    }
    return trim(result);
}

std::string function_word(const std::string& n, const std::string& text) {
    int num;
    try {
        num = std::stoi(n);
    }
    catch (const std::invalid_argument& e) {
        // 숫자가 아닌 입력 처리
        return "Invalid number format: " + n;
    }
    catch (const std::out_of_range& e) {
        // 숫자가 int 범위를 넘을 경우 처리
        return "Number out of range: " + n;
    }

    std::vector<std::string> texts = SplitSpace(text);
    if (num < 0 || num > static_cast<int>(texts.size())) {
        return "Index out of bounds: " + std::to_string(num);
    }

    return texts[num - 1];
}
