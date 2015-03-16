#include "shader.h"

#include <fstream>
#include <iostream>

list<Shader*> Shader::s_shaders;
Shader* Shader::s_current;

Shader::Shader(const string& vert, const string& frag)
	: m_vertFilename(vert),
	  m_fragFilename(frag)
{
	load();
	s_shaders.push_back(this);
}

Shader::~Shader()
{
	free();
	
	for (list<Shader*>::iterator it = s_shaders.begin() ; it != s_shaders.end() ; it++)
	{
		if (*it != this)
			continue;
		
		s_shaders.erase(it);
		break;
	}
}

void Shader::load()
{
	m_program = glCreateProgram();
	m_vert = loadShader(m_program, GL_VERTEX_SHADER, m_vertFilename);
	m_frag = loadShader(m_program, GL_FRAGMENT_SHADER, m_fragFilename);
	
	glLinkProgram(m_program);
	
	cerr << "Loaded shader pair: " << m_vertFilename << ".glsl, " << m_fragFilename << ".glsl" << endl;
}

void Shader::free()
{
	if (glIsShader(m_vert))
		glDeleteShader(m_vert);
	if (glIsShader(m_frag))
		glDeleteShader(m_frag);
	if (glIsProgram(m_program))
		glDeleteProgram(m_program);
}

void Shader::reload()
{
	free();
	load();
}

void Shader::reloadAll()
{
	cerr << "Reloading all shaders..." << endl;
	for (list<Shader*>::iterator it = s_shaders.begin() ; it != s_shaders.end() ; it++)
		(*it)->reload();
}

uint Shader::loadShader(uint program, int type, const string& filename)
{
	// Load the source file
	string source, line;
	ifstream in((filename + ".glsl").c_str());
	if (!in.is_open())
		Utilities::fatal("Could not open shader " + filename);
	while (getline(in, line))
		source += line + "\n";
	
	// Create the shader
	uint ret = glCreateShader(type);
	
	const char* data[1];
	data[0] = source.c_str();
	glShaderSource(ret, 1, data, NULL);
	
	// Compile the shader
	glCompileShader(ret);
	
	// Get the log
	int logLength;
	glGetShaderiv(ret, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		char* log = new char[logLength];
		glGetShaderInfoLog(ret, logLength, NULL, log);
		cout << "Compilation log for " << filename << ": " << log << endl;
	}
	
	// Check if it was ok
	int success;
	glGetShaderiv(ret, GL_COMPILE_STATUS, &success);
	if (!success)
		Utilities::fatal("Error compiling GLSL " + filename);
	
	// Attach it to the program
	glAttachShader(program, ret);
	
	return ret;
}

void Shader::bind()
{
	glUseProgram(m_program);
	s_current = this;
}

void Shader::unbind()
{
	glUseProgram(0);
}

int Shader::uniformLocation(const string& name)
{
	return glGetUniformLocation(m_program, name.c_str());
}

bool Shader::isCurrent() const
{
	return s_current == this;
}
