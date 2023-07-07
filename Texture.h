#pragma once

#include "Render.h"
#include <glad/glad.h>
#include <string>
#include <iostream>

class Texture
{
private:
	int width, height, number_of_channel;
	const char* image_file;
	unsigned char* data;
	unsigned int m_texture_buffer;
	unsigned int m_slot_id;

public:
	Texture(const char* image_file, unsigned int format);
	~Texture();

	void Bind();
	void Unbind();

	void Usetexture(unsigned int texture_slot);

	unsigned char* ReadImage();

	unsigned int GetSlot();
};