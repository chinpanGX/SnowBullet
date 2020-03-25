/*-----------------------------
	bullet.cpp
------------------------------*/

#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "bullet.h"
#include "tileMap.h"

#define _USE_MATH_DEFINES
#include <math.h>

//マクロ定義
#define BULLET_WIDTH  (32)
#define BULLET_HEIGHT (32)
#define BULLET_SPEED_X (20.0f)
#define BULLET_SPEED_Y (20.0f)
#define BULLET_VISIBLE_FRAME 15

//弾の構造体
typedef struct Bullet_tag
{
	D3DXVECTOR2 pos;
	float x, y;				//表示位置
	float move_x, move_y;	//
	float rotation;
	CIRCLE collision;
	float frame;
	bool bEnable;
} Bullet;

static Bullet g_Bullets[BULLET_MAX];

void Bullet_Initialize(void)
{
	//配列内の全てを無効化する
	for( int i = 0; i < BULLET_MAX; i++ ) {
		g_Bullets[i].bEnable = false;
	}
}

void Bullet_Update(void)
{
	TILE_DATA tile;
	
	for( int i = 0; i < BULLET_MAX; i++ ) 
	{

		if( g_Bullets[i].bEnable ) 
		{
			g_Bullets[i].x += g_Bullets[i].move_x * BULLET_SPEED_X;
			g_Bullets[i].y += g_Bullets[i].move_y * BULLET_SPEED_Y;
			g_Bullets[i].collision.cx = g_Bullets[i].x;
			g_Bullets[i].collision.cy = g_Bullets[i].y;

			// 画面外に出たら
			int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			if( g_Bullets[i].x - tw * 0.5f > SCREEN_WIDTH ) 
			{
				g_Bullets[i].bEnable = false;
			}

			// 有効フレームを超えたら
			++g_Bullets[i].frame;
			if(g_Bullets[i].frame > BULLET_VISIBLE_FRAME)
			{
				g_Bullets[i].bEnable = false;
			}

			//弾が壁に当たったら
			D3DXVECTOR2 dir = D3DXVECTOR2(g_Bullets[i].move_x, g_Bullets[i].move_y);
			if (Collision_HitCheck_TileMap(D3DXVECTOR2(g_Bullets[i].x, g_Bullets[i].y),&dir) == true)
			{
				g_Bullets[i].bEnable = false;
			}
		}
		
	}
}

void Bullet_Draw(void)
{
	for( int i = 0; i < BULLET_MAX; i++ ) {

		if( g_Bullets[i].bEnable ) {
			int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			int th = Texture_GetHeight(TEXTURE_INDEX_BULLET);
			Sprite_Draw(TEXTURE_INDEX_BULLET, 
				g_Bullets[i].x,
				g_Bullets[i].y,
				0,
				0,
				tw,
				th,
				tw * 0.5f,
				th * 0.5f,
				1.0f,
				1.0f,
				g_Bullets[i].rotation);
		}
	}
}

void Bullet_Create(float x, float y, D3DXVECTOR2 dir)
{
	for( int i = 0; i < BULLET_MAX; i++ ) 
	{

		// もし弾がすでに有効だったら？
		if( g_Bullets[i].bEnable ) 
		{
			continue;
		}

		// 弾の座標に引数の値を代入する
		g_Bullets[i].x = x;
		g_Bullets[i].y = y;

		// 弾の移動方向を指定する
		D3DXVec2Normalize(&dir, &dir);
		g_Bullets[i].move_x = dir.x;
		g_Bullets[i].move_y = dir.y;

		// 弾の移動方向から弾の回転角度を求める
		g_Bullets[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		g_Bullets[i].collision.cx = x;
		g_Bullets[i].collision.cy = y;
		g_Bullets[i].collision.r = BULLET_WIDTH * 0.8f;

		// 有効フレームを初期化
		g_Bullets[i].frame = 0;

		// 弾を有効にする
		g_Bullets[i].bEnable = true;

		break;
	}
}

void Bullet_Destroy(int index)
{
	g_Bullets[index].bEnable = false;
}

bool Bullet_IsEnable(int index )
{
	return g_Bullets[index].bEnable;
}

const CIRCLE* Bullet_GetCollision(int index)
{
	return &g_Bullets[index].collision;
}

