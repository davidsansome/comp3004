#ifndef TEXTURE_H
#define TEXTURE_H

#include <list>

#include "utilities.h"

class Texture
{
public:
	enum Settings
	{
		Defaults = 0x00,
		Mipmaps = 0x01,
		Mirrored = 0x02
	};
	Texture(Settings settings = Defaults);
	~Texture();
	
	void bind(int texUnit = 0, int shaderLoc = -1);
	
	Settings settings() const { return m_settings; }
	const string& filename() const { return m_filename; }
	
	static Texture* load(const string& filename, Settings settings = Mipmaps);
	static void reloadAll();

private:
	Texture(const string& filename, Settings settings);
	
	void load();
	void free();
	void reload();
	
	void setTexParams(GLenum target);
	
	string m_filename;
	Settings m_settings;
	uint m_id;
	
	static list<Texture*> s_textures;
};

#endif
