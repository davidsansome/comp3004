#ifndef SHADER_H
#define SHADER_H

#include "utilities.h"

#include <list>

class Shader
{
public:
	Shader(const string& vert, const string& frag);
	~Shader();

	void bind();
	void unbind();
	
	int uniformLocation(const string& name);
	bool isCurrent() const;
	
	static void reloadAll();

private:
	uint loadShader(uint program, int type, const string& filename);
	
	void load();
	void free();
	void reload();
	
	string m_vertFilename;
	string m_fragFilename;
	
	uint m_program;
	uint m_vert;
	uint m_frag;
	
	static list<Shader*> s_shaders;
	static Shader* s_current;
};

#endif
