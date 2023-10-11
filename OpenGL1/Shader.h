#pragma once

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexShaderCode = getShaderCode(vertexPath);
		std::string fragmentShaderCode = getShaderCode(fragmentPath);

		unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderCode.c_str(), "Vertex Shader");
		unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode.c_str(), "Fragment Shader");

		int succes;
		char infoLog[1024];
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_COMPILE_STATUS, &succes);
		if (!succes)
		{
			glGetProgramInfoLog(ID, 1024, NULL, infoLog);
			std::cout << "ERROR Linking Program:\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
	{
		std::string vertexShaderCode = getShaderCode(vertexPath);
		std::string geometryShaderCode = getShaderCode(geometryPath);
		std::string fragmentShaderCode = getShaderCode(fragmentPath);

		unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderCode.c_str(), "Vertex Shader");
		unsigned int geometryShader = compileShader(GL_GEOMETRY_SHADER, geometryShaderCode.c_str(), "Geometry Shader");
		unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode.c_str(), "Fragment Shader");

		int succes;
		char infoLog[1024];
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, geometryShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_COMPILE_STATUS, &succes);
		if (!succes)
		{
			glGetProgramInfoLog(ID, 1024, NULL, infoLog);
			std::cout << "ERROR Linking Program:\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
	}

	void use()
	{
		glUseProgram(ID);
	}

	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, int value)
	{
		glUniform1i(glGetUniformLocation(ID, name), value);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, unsigned int value)
	{
		glUniform1ui(glGetUniformLocation(ID, name), value);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, float value)
	{
		glUniform1f(glGetUniformLocation(ID, name), value);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, double value)
	{
		glUniform1d(glGetUniformLocation(ID, name), value);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, int value1, int value2)
	{
		glUniform2i(glGetUniformLocation(ID, name), value1, value2);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, unsigned int value1, unsigned int value2)
	{
		glUniform2ui(glGetUniformLocation(ID, name), value1, value2);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, float value1, float value2)
	{
		glUniform2f(glGetUniformLocation(ID, name), value1, value2);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, double value1, double value2)
	{
		glUniform2d(glGetUniformLocation(ID, name), value1, value2);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, int value1, int value2, int value3)
	{
		glUniform3i(glGetUniformLocation(ID, name), value1, value2, value3);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, unsigned int value1, unsigned int value2, unsigned int value3)
	{
		glUniform3ui(glGetUniformLocation(ID, name), value1, value2, value3);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, float value1, float value2, float value3)
	{
		glUniform3f(glGetUniformLocation(ID, name), value1, value2, value3);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, double value1, double value2, double value3)
	{
		glUniform3d(glGetUniformLocation(ID, name), value1, value2, value3);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, int value1, int value2, int value3, int value4)
	{
		glUniform4i(glGetUniformLocation(ID, name), value1, value2, value3, value4);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
	{
		glUniform4ui(glGetUniformLocation(ID, name), value1, value2, value3, value4);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, float value1, float value2, float value3, float value4)
	{
		glUniform4f(glGetUniformLocation(ID, name), value1, value2, value3, value4);
	}
	/*
	* @brief	suitable for single use
	*/
	void set(const char* name, double value1, double value2, double value3, double value4)
	{
		glUniform4d(glGetUniformLocation(ID, name), value1, value2, value3, value4);
	}


	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, int value)
	{
		glUniform1i(location, value);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, unsigned int value)
	{
		glUniform1ui(location, value);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, float value)
	{
		glUniform1f(location, value);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, double value)
	{
		glUniform1d(location, value);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, int value1, int value2)
	{
		glUniform2i(location, value1, value2);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, unsigned int value1, unsigned int value2)
	{
		glUniform2ui(location, value1, value2);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, float value1, float value2)
	{
		glUniform2f(location, value1, value2);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, double value1, double value2)
	{
		glUniform2d(location, value1, value2);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, int value1, int value2, int value3)
	{
		glUniform3i(location, value1, value2, value3);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, unsigned int value1, unsigned int value2, unsigned int value3)
	{
		glUniform3ui(location, value1, value2, value3);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, float value1, float value2, float value3)
	{
		glUniform3f(location, value1, value2, value3);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, double value1, double value2, double value3)
	{
		glUniform3d(location, value1, value2, value3);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, int value1, int value2, int value3, int value4)
	{
		glUniform4i(location, value1, value2, value3, value4);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
	{
		glUniform4ui(location, value1, value2, value3, value4);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, float value1, float value2, float value3, float value4)
	{
		glUniform4f(location, value1, value2, value3, value4);
	}
	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void set(unsigned int location, double value1, double value2, double value3, double value4)
	{
		glUniform4d(location, value1, value2, value3, value4);
	}

	/*
	* @brief	suitable for multiple use, use getSetLocation to get the location of the uniform
	*/
	void setMat4(unsigned int location, glm::mat4& value)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	/*
	* @brief	save location in variable for fast multiple calls of use
	*/
	unsigned int getSetLocation(const char* name)
	{
		int id = glGetUniformLocation(ID, name);
		if (id > -1)
		{
			return id;
		}
		std::cout << "ERROR Uniform <" << name << "> doesn't exists" << std::endl;
		return -1;
	}

	void remove()
	{
		glDeleteProgram(ID);
	}


private:
	/*
	* @brief	get the Shader source code from a file
	*
	* @param	path	Path to the Shader file (E.g.: ../Shaders/MyShader.vert)
	*
	* @return	returns the source code
	*/
	std::string getShaderCode(const char* path)
	{
		std::ifstream shaderFile;

		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			shaderFile.open(path);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			return shaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "Error couldn't read File (Path: " << path << " )\n" << "Errorcode: " << e.code() << "\n" << e.what() << std::endl;
		}
		return "";
	}

	/*
	* @brief	create and compile a OpenGl-shader
	*
	* @param	type	Shader type (E.g.: GL_VERTEX_SHADER)
	* @param	code	sourcecode of the shader
	* @param	shaderName	name to show in errormessage
	*
	* @return	returns a Shader ID
	*/
	unsigned int compileShader(GLenum type, const char* code, std::string shaderName)
	{

		int succes;
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &code, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
		if (!succes)
		{
			char infoLog[1024];
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR Shader <" << shaderName << "> Compilation Failed:\n" << infoLog << std::endl;
		}
		return shader;
	}
};
