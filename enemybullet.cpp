/*-----------------------------
	enemybullet.cpp
------------------------------*/
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "enemybullet.h"

#define _USE_MATH_DEFINES
#include <math.h>

//マクロ定義
#define ENEMYBULLET_WIDTH  (32)
#define ENEMYBULLET_HEIGHT (32)
#define ENEMYBULLET_SPEED_X (3.0f)
#define ENEMYBULLET_SPEED_Y (3.0f)
#define ENEMYBULLET_VISIBLE_FRAME 300

//構造体
typedef struct
{
	float x, y;				//表示位置
	float move_x, move_y;	//
	float rotation;
	CIRCLE collision;
	float frame;
	bool bEnable;

}EnemyBullet;

static EnemyBullet g_EnemyBullets[ENEMYBULLET_MAX];

//初期化
void EnemyBullet_Initialize(void)
{
	//配列内の全てを無効化する
	for( int i = 0; i < ENEMYBULLET_MAX; i++ ) {
		g_EnemyBullets[i].bEnable = false;
	}
}

//更新
void EnemyBullet_Update(void)
{
	for (int i = 0; i < ENEMYBULLET_MAX; i++) {

		if (g_EnemyBullets[i].bEnable)
		{
			g_EnemyBullets[i].x += g_EnemyBullets[i].move_x * ENEMYBULLET_SPEED_X;
			g_EnemyBullets[i].y += g_EnemyBullets[i].move_y * ENEMYBULLET_SPEED_Y;
			g_EnemyBullets[i].collision.cx = g_EnemyBullets[i].x;
			g_EnemyBullets[i].collision.cy = g_EnemyBullets[i].y;

			// 画面外に出たら
			int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			if (g_EnemyBullets[i].x - tw * 0.5f > SCREEN_WIDTH) {
				g_EnemyBullets[i].bEnable = false;
			}

			// 有効フレームを超えたら
			++g_EnemyBullets[i].frame;
			if (g_EnemyBullets[i].frame > ENEMYBULLET_VISIBLE_FRAME){
				g_EnemyBullets[i].bEnable = false;
			}
		}
	}
}

//描画
void EnemyBullet_Draw(void)
{
	for (int i = 0; i < ENEMYBULLET_MAX; i++) {

		if (g_EnemyBullets[i].bEnable) {
			int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			int th = Texture_GetHeight(TEXTURE_INDEX_BULLET);
			Sprite_Draw(TEXTURE_INDEX_BULLET, 
				g_EnemyBullets[i].x,
				g_EnemyBullets[i].y,
				0,
				0,
				tw,
				th,
				tw * 0.5f,
				th * 0.5f,
				1.0f,
				1.0f,
				g_EnemyBullets[i].rotation);
		}
	}
}

//弾の初期配置
void EnemyBullet_Create(float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < ENEMYBULLET_MAX; i++) {

		// もし弾がすでに有効だったら？
		if (g_EnemyBullets[i].bEnable) {
			continue;
		}

		// 弾の座標に引数の値を代入する
		g_EnemyBullets[i].x = x;
		g_EnemyBullets[i].y = y;

		// 弾の移動方向を指定する
		D3DXVec2Normalize(&dir, &dir);
		g_EnemyBullets[i].move_x = dir.x;
		g_EnemyBullets[i].move_y = dir.y;

		// 弾の移動方向から弾の回転角度を求める
		g_EnemyBullets[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		g_EnemyBullets[i].collision.cx = x;
		g_EnemyBullets[i].collision.cy = y;
		g_EnemyBullets[i].collision.r = ENEMYBULLET_WIDTH * 0.8f;

		// 有効フレームを初期化
		g_EnemyBullets[i].frame = 0;

		// 弾を有効にする
		g_EnemyBullets[i].bEnable = true;

		break;
	}
}

//弾を消去する関数
void EnemyBullet_Destroy(int index)
{
	g_EnemyBullets[index].bEnable = false;
}

//弾の有効フラグ
bool EnemyBullet_IsEnable(int index)
{
	return g_EnemyBullets[index].bEnable;
}

//弾のコリジョン
const CIRCLE* EnemyBullet_GetCollision(int index)
{
	return &g_EnemyBullets[index].collision;
}

