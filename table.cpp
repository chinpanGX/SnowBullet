/*-----------------------------
	table.cpp
------------------------------*/

#include "table.h"

//アニメーションテーブル
D3DXVECTOR2 animeTable[3][4][3] =
{
	{
		{//水色　下向き
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(0.125f, 0.0f),
			D3DXVECTOR2(0.25f, 0.0f),
		},
		{//水色　左向き
			D3DXVECTOR2(0.0f, 0.125f),
			D3DXVECTOR2(0.125f, 0.125f),
			D3DXVECTOR2(0.25f, 0.125f),
		},
		{//水色　右向き
			D3DXVECTOR2(0.0f, 0.25f),
			D3DXVECTOR2(0.125f, 0.25f),
			D3DXVECTOR2(0.25f, 0.25f),
		},
		{//水色　上向き
			D3DXVECTOR2(0.0f, 0.375f),
			D3DXVECTOR2(0.125f, 0.375f),
			D3DXVECTOR2(0.25f, 0.375f),
		},
	},
	{
		{//赤色　下向き
			D3DXVECTOR2(0.375f, 0.0f),
			D3DXVECTOR2(0.5f,   0.0f),
			D3DXVECTOR2(0.625f, 0.0f),
		},
		{//赤色　左向き
			D3DXVECTOR2(0.375f, 0.125f),
			D3DXVECTOR2(0.5f,   0.125f),
			D3DXVECTOR2(0.625f, 0.125f),
		},
		{//赤色　右向き
			D3DXVECTOR2(0.375f, 0.25f),
			D3DXVECTOR2(0.5f,   0.25f),
			D3DXVECTOR2(0.625f, 0.25f),
		},
		{//赤色　上向き
			D3DXVECTOR2(0.375f, 0.375f),
			D3DXVECTOR2(0.5f,   0.375f),
			D3DXVECTOR2(0.625f, 0.375f),
		},
	},
	{
		{//青色　下向き
			D3DXVECTOR2(0.0f,   0.5f),
			D3DXVECTOR2(0.125f, 0.5f),
			D3DXVECTOR2(0.25f,  0.5f),
		},
		{//青色　左向き
			D3DXVECTOR2(0.0f,   0.625f),
			D3DXVECTOR2(0.125f, 0.625f),
			D3DXVECTOR2(0.25f,  0.625f),
		},
		{//青色　右向き
			D3DXVECTOR2(0.0f,   0.75f),
			D3DXVECTOR2(0.125f, 0.75f),
			D3DXVECTOR2(0.25f,  0.75f),
		},
		{//青色　上向き
			D3DXVECTOR2(0.0f,   0.875f),
			D3DXVECTOR2(0.125f, 0.875f),
			D3DXVECTOR2(0.25f,  0.875f),
		},
	},
};

D3DXVECTOR2 GetAnimTbl(int set, int muki, int anim)
{
	return animeTable[set][muki][anim];
}
