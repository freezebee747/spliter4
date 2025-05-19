#pragma once

//static pattern rule을 위한 클래스
//target-pattern, prereq-pattern을 구현하기 위해 사용한다.
class Pattern {
private:
	std::string patterns;
public:
	Pattern(const std::string& p) : patterns(p) {}
	void print() {
		std::cout << patterns << std::endl;
	}
};