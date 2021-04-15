#include<fstream>
#include<sstream>

class program {
private:
	unsigned int programID;
	std::string pathToString(const char*);
	void printShaderLog(unsigned int);
	void printProgramLog(unsigned int);
public:
	program(const char*, const char*);
	void use();
	~program();
};
	
	