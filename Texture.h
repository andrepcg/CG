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

#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include "OpenGLincludes.h"
#include "Shader.h"

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName);

	bool Load();

	void Bind(GLenum TextureUnit, Shader *shader);
	GLuint m_textureObj;
private:
	std::string m_fileName;
	GLenum m_textureTarget;
	

	unsigned char *image_data;
	int image_width;
	int image_height;
	int image_pixel_components;
};


#endif	/* TEXTURE_H */

