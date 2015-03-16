#include "texture.h"
#include "utilities.h"

#ifdef __APPLE__
#include <SDL_image.h>
#else
#include <SDL/SDL_image.h>
#endif

#include <iostream>

list<Texture*> Texture::s_textures;

Texture* Texture::load(const string& filename, Settings settings)
{
	for (list<Texture*>::iterator it = s_textures.begin() ; it != s_textures.end() ; it++)
		if ((*it)->settings() == settings && (*it)->filename() == filename)
			return (*it);
	
	return new Texture(filename, settings);
}

Texture::Texture(const string& filename, Settings settings)
	: m_filename(filename),
	  m_settings(settings)
{
	load();
	s_textures.push_back(this);
}

Texture::Texture(Settings settings)
	: m_settings(settings)
{
	load();
	s_textures.push_back(this);
}

Texture::~Texture()
{
	free();
}

void Texture::load()
{
	glGenTextures(1, (GLuint*)&m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	setTexParams(GL_TEXTURE_2D);
	
	if (!m_filename.empty())
	{
		SDL_Surface* surface = IMG_Load(m_filename.c_str());
		if (surface == NULL)
			Utilities::fatal("Couldn't load texture " + m_filename);
		
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, surface->w, surface->h, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		
		SDL_FreeSurface(surface);
		
		cerr << "Loaded texture: " << m_filename << endl;
	}
}

void Texture::free()
{
	if (glIsTexture(m_id))
		glDeleteTextures(1, &m_id);
}

void Texture::reload()
{
	free();
	load();
}

void Texture::reloadAll()
{
	cerr << "Reloading all textures..." << endl;
	for (list<Texture*>::iterator it = s_textures.begin() ; it != s_textures.end() ; it++)
		(*it)->reload();
}

void Texture::setTexParams(GLenum target)
{
	if (m_settings & Mipmaps)
	{
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	if (m_settings & Mirrored)
	{
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
	else
	{
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}

void Texture::bind(int texUnit, int shaderLoc)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, m_id);
	
	if (shaderLoc != -1)
		glUniform1i(shaderLoc, texUnit);
}

