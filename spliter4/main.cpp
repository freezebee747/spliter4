#include "parser.h"
#include "function.h"
int main(){
	std::vector<std::pair<unsigned, std::string>> test = ReadFileWithLineNumbers("test.txt");
	std::vector<Block> blocks= SplitByBlock(test);
	MakefileText mk = ParseMakefileTextFromLines_redefine(blocks);
	mk.print();

	
}