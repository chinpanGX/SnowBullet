//=============================================================================
//
// 背景処理 [tileMap.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "tileMap.h"
#include "input.h"
#include "player.h"
#include "texture.h"
#include "sprite.h"


#define TILETABLE_TABLE_NUM			5		//マップテーブルサイズ

#define TILETABLE_TABLE_SIZE_X		32		//マップサイズ横
#define TILETABLE_TABLE_SIZE_Y		18		//マップサイズ縦


TILE_DATA TileTable[TILETABLE_TABLE_NUM] =
{
	{ 0.375f,  0.3125f, 1 },	//0:地面
	{ 0.0f,    0.3125f, 0 },	//1:壁ブロック
	{ 0.0625f, 0.3125f, 1 },	//2:はしご
	{ 0.125f,  0.3125f, 0 },	//3:宝箱
	{ 0.3125f, 0.3125f, 0 },	//4:宝箱（開）
};

//マップデータ
int MAP_DATA[TILETABLE_TABLE_SIZE_Y][TILETABLE_TABLE_SIZE_X] =
{
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

//マップの初期化
void TileMap_Initialize(void)
{
}

//マップの終了処理
void TileMap_Finalize(void)
{

}

//マップの更新
void TileMap_Update(void)
{
}

//マップの描画
void TileMap_Draw(void)
{
	float w = TILEMAP_SIZE_W / TILEMAP_DIVIDE_X;
	float h = TILEMAP_SIZE_H / TILEMAP_DIVIDE_Y;

	for (int y = 0; y < TILETABLE_TABLE_SIZE_Y; y++)
	{
		for (int x = 0; x < TILETABLE_TABLE_SIZE_X; x++)
		{
			Sprite_Draw(TEXTURE_INDEX_TILEMAP,
				x * w,
				y * h,
				TileTable[MAP_DATA[y][x]].u * TILEMAP_SIZE_W,
				TileTable[MAP_DATA[y][x]].v * TILEMAP_SIZE_H,
				w,
				h);
		}
	}
}

//マップ情報の取得
TILE_DATA GetTileInfo(float x, float y)
{
	int tile_x = x / (TILEMAP_SIZE_W / TILEMAP_DIVIDE_X);
	int tile_y = y / (TILEMAP_SIZE_H / TILEMAP_DIVIDE_Y);

	return TileTable[MAP_DATA[tile_y][tile_x]];
}

TILE_DATA GetTileInfo_Enemy(float x, float y)
{
	int tile_x = x / (TILEMAP_SIZE_W / TILEMAP_DIVIDE_X);
	int tile_y = y / (TILEMAP_SIZE_H / TILEMAP_DIVIDE_Y);

	return TileTable[MAP_DATA[tile_y][tile_x]];
}


void SetTileType(int x, int y, int type)
{
	//範囲チェック
	if (type >= TILETABLE_TABLE_NUM)
		return;

	MAP_DATA[y][x] = type;
}
