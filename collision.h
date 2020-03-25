/*-----------------------------
	collision.h
------------------------------*/

#pragma once

#include <d3dx9.h>


typedef struct
{
	float cx, cy;	//中心座標
	float r;		//半径

}CIRCLE;

typedef struct
{
	float x, y;		//始点座標
	float ex, ey;	//ベクトル方向（終点）
	float r;		//半径

}CUPSULE;

void Collision_Initialize(void);
void Collision_Update(void);

void Collision_Player_vs_Enemy(void);

bool Collision_HitCheck_TileMap(D3DXVECTOR2 dst, D3DXVECTOR2* pOut);

