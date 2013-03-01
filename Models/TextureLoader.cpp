#include "StdAfx.h"
#include "TextureLoader.h"

CTextureLoader::CTextureLoader(void)
{
}

CTextureLoader::~CTextureLoader(void)
{
}

HBITMAP Load(LPTSTR file, BOOL fromRes)
{
	UINT flags = LR_CREATEDIBSECTION;
	if (!fromRes)
		flags |= LR_LOADFROMFILE;
	return (HBITMAP)LoadImage(::GetModuleHandle(NULL), file, IMAGE_BITMAP, 0, 0, flags);
}

GLuint CTextureLoader::LoadTexture(LPTSTR file, BOOL fromRes)
{
	BITMAP BMP;
	HBITMAP hBMP;
	GLuint texture = 0;

	glGenTextures(1, &texture);
	
	hBMP = Load(file, fromRes);
	if (hBMP == NULL)
		return 0;
	GetObject(hBMP, sizeof(BMP), &BMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Pixel Storage Mode (Word Alignment / 4 Bytes)
	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture); // Bind To The Texture ID
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);        // Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        // Linear Mag Filter
	DeleteObject(hBMP);
	return texture;
}


BOOL CTextureLoader::LoadTextureToList(LPTSTR file, GLuint *list, BOOL fromRes)
{
	HBITMAP hBMP;
	BITMAP BMP;
	GLuint texture;

	glGenTextures(1, &texture);
	hBMP = Load(file, fromRes);
	if(!hBMP)
		return FALSE;

	GetObject(hBMP, sizeof(BMP), &BMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Pixel Storage Mode (Word Alignment / 4 Bytes)
	// Typical Texture Generation Using Data From The Bitmap
	
	glNewList(*list = glGenLists(1), GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture);  // Bind To The Texture ID
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glEndList();
	DeleteObject(hBMP);
	return TRUE;
}