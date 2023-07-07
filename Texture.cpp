#include "Texture.h"
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Texture::Texture(const char* image, unsigned int format) : image_file(image)
{
	GLCall(glGenTextures(1, &m_texture_buffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_texture_buffer));
	data = ReadImage();
	if (data)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << image_file << " not exist!" << std::endl;
	}
	stbi_image_free(data);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_texture_buffer));
}

void Texture::Bind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_texture_buffer));
}
void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Usetexture(unsigned int texture_slot)
{
	m_slot_id = texture_slot - GL_TEXTURE0;
	GLCall(glActiveTexture(texture_slot));
}

unsigned char* Texture::ReadImage()
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(image_file, &width, &height, &number_of_channel, 0);
	return data;
}

unsigned int Texture::GetSlot()
{
	return m_slot_id;
}