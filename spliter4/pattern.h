#pragma once

//static pattern rule�� ���� Ŭ����
//target-pattern, prereq-pattern�� �����ϱ� ���� ����Ѵ�.
class Pattern {
private:
	std::string patterns;
public:
	Pattern(const std::string& p) : patterns(p) {}
	void print() {
		std::cout << patterns << std::endl;
	}
};