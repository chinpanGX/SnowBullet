/*-----------------------------
	tileMap.h
------------------------------*/

#pragma once

// マクロ定義
#define TILEMAP_TEXTURE		_T("data/TEXTURE/kokosozai.png")	// サンプル用画像
#define TILEMAP_SIZE_W		(512)	// テクスチャサイズ
#define TILEMAP_SIZE_H		(512)	// 同上
#define TILEMAP_POS_X		(0)		// ポリゴンの初期位置X(左上)
#define TILEMAP_POS_Y		(0)		// 同上

#define TILEMAP_DIVIDE_X	(16)	// 縦の分割数
#define TILEMAP_DIVIDE_Y	(16)	// 横の分割数


typedef struct
{
	float u;
	float v;
	int isWalk;	//歩行可能フラグ

}TILE_DATA;

void TileMap_Initialize(void);
void TileMap_Finalize(void);
void TileMap_Update(void);
void TileMap_Draw(void);

TILE_DATA GetTileInfo(float x, float y);

void SetTileType(int x, int y, int type);
