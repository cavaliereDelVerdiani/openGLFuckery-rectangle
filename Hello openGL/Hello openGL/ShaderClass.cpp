#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"ShaderClass.h"

std::string program::pathToString(const char* path) {
	//pipeline di sta roba
	//1->leggi il file con una ifstream (input file stream)
	std::ifstream fileStream(path);
	std::stringstream stringStream;
	//2->ne leggi il contenuto con una stringstream
	stringStream << fileStream.rdbuf();
	//3->finalmente, leggi il contenuto della stringStream, lo salvi in una string
	std::string res = stringStream.str();
	//siamo in c++, quindi un pochino di memory management ci vuole
	fileStream.close();
	stringStream.clear();
	return res;
}

void program::printShaderLog(unsigned int shaderID) {
	int success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (success) {
		std::cout << "shader compiled successfully\n";
	}
	else {
		char infolog[512];
		glGetShaderInfoLog(shaderID, 512, NULL, infolog);
		std::cout << "\nshader compilation failed:\n"<<infolog<<std::endl;
	}
}

void program::printProgramLog(unsigned int programID) {
	int success;
	glGetProgramiv(programID, GL_COMPILE_STATUS, &success);
	if (success) {
		std::cout << "program compiled successfully\n";
	}
	else {
		char infolog[512];
		glGetProgramInfoLog(programID, 512, NULL, infolog);
		std::cout << "program compilation failed\n" << infolog << std::endl;
	}
}

program::program(const char* vertexShaderPath, const char* fragmentShaderPath) {
	//questo prende i file txt e ne mette i contenuti in stringhe
	std::string vertexShaderString = pathToString(vertexShaderPath);
	std::string fragmentShaderString = pathToString(fragmentShaderPath);
	//ma le funzioni che vogliamo usare usano c_str(stringhe stile c), quindi trasformiamo queste in c_str e ce ne sbarazziamo dopo
	const char* vertexShaderCStr = vertexShaderString.c_str();
	const char* fragmentShaderCStr = fragmentShaderString.c_str();

	//ora che abbiamo le c_str, possiamo creare gli oggetti shader
	//vertex shader
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderCStr, NULL);
	glCompileShader(vertexShaderID);
	std::cout << "vertex shader log:\n";
	printShaderLog(vertexShaderID);

	//fragment shader
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCStr, NULL);
	glCompileShader(fragmentShaderID);
	std::cout << "fragment shader log:\n";
	printShaderLog(fragmentShaderID);

	//ora che ne abbiamo usato le c_str, questi due cosi non ci servono
	vertexShaderString.clear();
	fragmentShaderString.clear();

	//fatto cio' possiamo rendere this->programID un programID funzionante
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	std::cout << "shader program log:\n";
	printProgramLog(programID);

	//ora che programID ha tutto il necessario non ci servono piu' gli oggetti vertex shader e fragment shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void program::use() {
	glUseProgram(programID);
}

program::~program() {
	glDeleteProgram(programID);
}