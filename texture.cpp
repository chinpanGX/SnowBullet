/*-----------------------------
	texture.cpp
------------------------------*/

#include <d3dx9.h>
//#include "debugPrintf.h"
#include "mydirect3d.h"
#include "texture.h"

#define TEXTURE_FILENAME_MAX (64)	//ファイルの文字数


typedef struct TextureFile_tag
{
	char filename[TEXTURE_FILENAME_MAX];
	int width;
	int height;
} TextureFile;

//テクスチャファイルの一覧
static const TextureFile g_TextureFiles[] = {
    { "asset/texture/yukidaruma.tga", 256, 256 },
	{ "asset/texture/bullet00.png", 32, 32 },
	{ "asset/texture/bullet01.png", 32, 32 },
	{ "asset/texture/exp.png", 128, 128 },
	{ "asset/texture/number.tga", 320, 32 },
	{ "asset/texture/title.png", 1024, 576 },
	{ "asset/texture/result.png", 1024, 576 },
	{ "asset/texture/kokosozai.png", 512, 512 },
	{ "asset/texture/sousa.png", 1024, 576},
	{ "asset/texture/rule.png", 1024, 576},
};

static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);

static_assert(TEXTURE_INDEX_MAX == TEXTURE_FILE_COUNT, "TEXTURE_INDEX_MAX != TEXTURE_FILE_COUNT");

static LPDIRECT3DTEXTURE9 g_pTextures[TEXTURE_FILE_COUNT] = {};

//テクスチャファイルの読み込み
int Texture_Load(void)
{   
    LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if( !pDevice ) {
		return TEXTURE_FILE_COUNT;
	}

	int failed_count = 0;

	for( int i = 0; i < TEXTURE_FILE_COUNT; i++ ) {
		
		if( FAILED(D3DXCreateTextureFromFile(pDevice, g_TextureFiles[i].filename, &g_pTextures[i])) ) {
            // DebugPrintf("テクスチャの読み込みに失敗 ... %s\n", g_TextureFiles[i].filename);
			failed_count++;
		}
	}

	return failed_count;
}

//テクスチャファイルの解放
void Texture_Release(void)
{
	for( int i = 0; i < TEXTURE_FILE_COUNT; i++ ) {
		
		if( g_pTextures[i] ) {
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
}

LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_pTextures[index];
}

int Texture_GetWidth(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_TextureFiles[index].width;
}

int Texture_GetHeight(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_TextureFiles[index].height;
}
