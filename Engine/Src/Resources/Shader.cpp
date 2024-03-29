﻿#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

namespace OGLE
{
	void Shader::Initialize(const std::string& vertexPath, const std::string& fragmentPath,
		const std::string& geometryPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;


		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			// if geometry shader path is present, also load a geometry shader
			if (!geometryPath.empty())
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch ([[maybe_unused]] std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);

		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);

		// if geometry shader is given, compile geometry shader
		unsigned int geometry;
		if (!geometryPath.empty())
		{
			const char* gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, nullptr);
			glCompileShader(geometry);
		}
		// shader Program
		m_shaderID = glCreateProgram();
		glAttachShader(m_shaderID, vertex);
		glAttachShader(m_shaderID, fragment);

		if (!geometryPath.empty())
			glAttachShader(m_shaderID, geometry);

		glLinkProgram(m_shaderID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (!geometryPath.empty())
			glDeleteShader(geometry);
	}

	Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) : m_shaderID(0)
	{
		Initialize(vertexPath, fragmentPath, geometryPath);
	}

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath,
		const std::string& geometryPath) : m_shaderID(0)
	{
		Initialize(vertexPath, fragmentPath, geometryPath);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_shaderID);
	}

	void Shader::SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), static_cast<int>(value));
	}

	void Shader::SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), value);
	}

	void Shader::SetVec2(const std::string& name, const Vector2& value) const
	{
		glUniform2fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, value_ptr(value));
	}

	void Shader::SetVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_shaderID, name.c_str()), x, y);
	}

	void Shader::SetVec3(const std::string& name, const Vector3& value) const
	{
		glUniform3fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, value_ptr(value));
	}

	void Shader::SetVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_shaderID, name.c_str()), x, y, z);
	}

	void Shader::SetVec4(const std::string& name, const Vector4& value) const
	{
		glUniform4fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, value_ptr(value));
	}

	void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(m_shaderID, name.c_str()), x, y, z, w);
	}

	void Shader::SetMat2(const std::string& name, const Matrix2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, value_ptr(mat));
	}

	void Shader::SetMat3(const std::string& name, const Matrix3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, value_ptr(mat));
	}

	void Shader::SetMat4(const std::string& name, const Matrix4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, value_ptr(mat));
	}
}
