/*-----------------------------
	texture.h
------------------------------*/

#pragma once

#include <d3d9.h>

typedef enum
{
	TEXTURE_INDEX_YUKIDARUMA,
	TEXTURE_INDEX_BULLET,
	TEXTURE_INDEX_LASER,
	TEXTURE_INDEX_EXPLOSION,
	TEXTURE_INDEX_NUMBER,
	TEXTURE_INDEX_TITLE,
	TEXTURE_INDEX_RESULT,
	TEXTURE_INDEX_TILEMAP,
	TEXTURE_INDEX_SOUSA,
	TEXTURE_INDEX_RULE,

    TEXTURE_INDEX_MAX
}TextureIndex;

int Texture_Load(void); //èâä˙âª
void Texture_Release(void); //èIóπèàóù
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index);
int Texture_GetWidth(TextureIndex index);
int Texture_GetHeight(TextureIndex index);
