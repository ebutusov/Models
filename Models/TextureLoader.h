#pragma once

class CTextureLoader
{
private:
	CTextureLoader(void);
public:
	static GLuint LoadTexture(LPTSTR file, BOOL fromRes);
	static BOOL LoadTextureToList(LPTSTR file, GLuint *list, BOOL fromRes);
	~CTextureLoader(void);
};
