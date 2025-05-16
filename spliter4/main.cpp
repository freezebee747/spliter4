#include "parser.h"

int main(){
	std::vector<std::pair<unsigned, std::string>> test = ReadFileWithLineNumbers("test.txt");
	MakefileText mk = ParseMakefileTextFromLines(test);
	mk.print();

}