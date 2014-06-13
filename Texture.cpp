/*

Copyright 2011 Etay Meiri

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include "OpenGLincludes.h"
#include "Texture.h"
#include "Utils.h"

#include "stb_image.h"
#include "Shader.h"



Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
	m_textureTarget = TextureTarget;
	m_fileName = FileName;
	image_data = NULL;
}

bool Texture::Load()
{

	image_data = stbi_load(m_fileName.c_str(), &image_width, &image_height, &image_pixel_components, 0);

	if (image_data == NULL){
		std::cout << "Error loading texture '" << m_fileName << std::endl;
		return false;
	}


	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);

	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);




	glTexImage2D(m_textureTarget, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	//stbi_image_free(image_data);
	return true;
}



void Texture::Bind(GLenum TextureUnit, Shader *shader)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
	//GLint tex = shader->GetUniformLocation("TexCoord");
	//glUniform1i(tex, m_textureObj);
}
