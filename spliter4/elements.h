#pragma once
#include <iostream>
#include <string>
#include <vector>
#if defined(_WIN32)
const std::string DEFAULT_SEARCH_DIR = "C:\\Program Files";
#elif defined(__APPLE__) && defined(__MACH__)
const std::string DEFAULT_SEARCH_DIR = "/Applications";
#elif defined(__linux__)
const std::string DEFAULT_SEARCH_DIR = "/usr/local/share";
#else
const std::string DEFAULT_SEARCH_DIR = "./";
#endif

//디렉토리 처리를 위한 singleton 클래스
class DirSingleton {
private:
	static DirSingleton* instance;
	std::string dir = DEFAULT_SEARCH_DIR;
protected:
	DirSingleton() = default;
public:
	static DirSingleton& GetInstance() {
		static DirSingleton instance;
		return instance;
	}
	std::string& Getdir() {
		return dir;
	}
	void SetDir(const std::string& usr_dir) {
		dir = usr_dir;
	}
	DirSingleton(const DirSingleton&) = delete;
	DirSingleton& operator=(const DirSingleton&) = delete;
};

class Target {
private:
	std::string target;

public:
	Target(const std::string& t) :target(t) {};
	std::string GetTarget() { return target; }
	void print() {
		std::cout << target << std::endl;
	}
};

class Prerequisite {
private:
	std::string preqs;
	std::string expended;
public:
	Prerequisite(const std::string& p) : preqs(p) {};
	void print() {
		std::cout << preqs << std::endl;
	}
	std::string GetPreqs();
	bool IsVariable();
	std::string GetVariable();
	void SetExpended(const std::string& exp);


};

class Recipe {
private:
	std::string recipe;
	std::string expended;
public:
	Recipe(const std::string& r) : recipe(r) {}
	void print() {
		std::cout << recipe << std::endl;
	}
	void SetExpended(const std::string& exp);
	void SetExpended(std::vector<std::string>& exp);

	std::vector<std::string> SplitRecipe();

};

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