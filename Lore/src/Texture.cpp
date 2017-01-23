#include "Texture.h"

using namespace std;
using namespace LORE;

Texture::Texture(std::string name): m__id(), m__nom(name), m__emplacementFichier("Textures/" + name)
{

	cout << this << " [Texture] constructor" << endl;

}


Texture::~Texture()
{

	SOIL_free_image_data( (unsigned char*)m__emplacementFichier.c_str() );

}


void Texture::load()
{

	cout << this << " [Texture] load" << endl;
	cout << this << " [Texture] load(): " << m__emplacementFichier << endl;

	if(!m__nom.empty())
	{

		m__id = SOIL_load_OGL_texture
		(
			(char*)m__emplacementFichier.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	}else
	{

		glGenTextures(1, &m__id);

	}



	if( 0 == m__id )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}


}
