/*-----------------------------
	enemylaser.cpp
------------------------------*/
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "enemylaser.h"

#define _USE_MATH_DEFINES
#include <math.h>

//マクロ定義
#define ENAMYLASER_IMAGEMAX (100)
#define ENEMYLASER_WIDTH	(32)
#define ENEMYLASER_HEIGHT	(32)
#define ENEMYLASER_SPEED	(32)
#define ENEMYLASER_VISIBLE_FRAME 100


typedef struct
{
	float		x, y;				//表示位置
	float		move_x, move_y;	//
	float		rotation;
	float		frame;
	bool		bEnable;

}LaserIMG;

typedef struct
{
	float		x, y;			//表示位置
	float		move_x, move_y;	//
	float		rotation;
	CUPSULE		collision;
	LaserIMG	laserIMG[ENAMYLASER_IMAGEMAX];
	float		frame;
	bool		bEnable;

}EnemyLaser;

EnemyLaser g_EnemyLaser[ENEMYLASER_MAX];

//初期化
void EnemyLaser_Initialize(void)
{
	//配列内の全てを無効化する
	for( int i = 0; i < ENEMYLASER_MAX; i++ ) {
		g_EnemyLaser[i].bEnable = false;
	}
}

//更新
void EnemyLaser_Update(void)
{
	for (int i = 0; i < ENEMYLASER_MAX; i++) {

		if (g_EnemyLaser[i].bEnable) {

			//座標更新
			for (int n = 0; n < ENAMYLASER_IMAGEMAX; n++){
				if (g_EnemyLaser[i].laserIMG[n].bEnable)
				{
					g_EnemyLaser[i].laserIMG[n].x += g_EnemyLaser[i].laserIMG[n].move_x * ENEMYLASER_SPEED;
					g_EnemyLaser[i].laserIMG[n].y += g_EnemyLaser[i].laserIMG[n].move_y * ENEMYLASER_SPEED;

					++g_EnemyLaser[i].laserIMG[n].frame;
					if (g_EnemyLaser[i].laserIMG[n].frame > 40)
						g_EnemyLaser[i].laserIMG[n].bEnable = false;

				}
			}

			//描画追加
			for (int n = 0; n < ENAMYLASER_IMAGEMAX; n++)
			{
				if (!g_EnemyLaser[i].laserIMG[n].bEnable)
				{
					g_EnemyLaser[i].laserIMG[n].x = g_EnemyLaser[i].x;
					g_EnemyLaser[i].laserIMG[n].y = g_EnemyLaser[i].y;
					g_EnemyLaser[i].laserIMG[n].move_x = g_EnemyLaser[i].move_x;
					g_EnemyLaser[i].laserIMG[n].move_y = g_EnemyLaser[i].move_y;
					g_EnemyLaser[i].laserIMG[n].rotation = g_EnemyLaser[i].rotation;
					g_EnemyLaser[i].laserIMG[n].frame = 0;
					g_EnemyLaser[i].laserIMG[n].bEnable = true;
					break;
				}
			}

			// 有効フレームを超えたら
			++g_EnemyLaser[i].frame;
			if (g_EnemyLaser[i].frame > ENEMYLASER_VISIBLE_FRAME)
				g_EnemyLaser[i].bEnable = false;
		}
	}
}

//描画
void EnemyLaser_Draw(void)
{
	for (int i = 0; i < ENEMYLASER_MAX; i++) {

		if (g_EnemyLaser[i].bEnable) {
			for (int n = 0; n < ENAMYLASER_IMAGEMAX; n++){
				if (g_EnemyLaser[i].laserIMG[n].bEnable){
					int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
					int th = Texture_GetHeight(TEXTURE_INDEX_BULLET);
					Sprite_Draw(TEXTURE_INDEX_LASER,
						g_EnemyLaser[i].laserIMG[n].x,
						g_EnemyLaser[i].laserIMG[n].y,
						0,
						0,
						tw,
						th,
						tw * 0.5f,
						th * 0.5f,
						1.0f,
						1.0f,
						g_EnemyLaser[i].laserIMG[n].rotation);
				}
			}
		}
	}
}

//レーザーの初期配置
void EnemyLaser_Create(float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < ENEMYLASER_MAX; i++) {

		// もし弾がすでに有効だったら？
		if (g_EnemyLaser[i].bEnable) {
			continue;
		}

		// 弾の座標に引数の値を代入する
		g_EnemyLaser[i].x = x;
		g_EnemyLaser[i].y = y;

		// 弾の移動方向を指定する
		D3DXVec2Normalize(&dir, &dir);
		g_EnemyLaser[i].move_x = dir.x;
		g_EnemyLaser[i].move_y = dir.y;

		// 弾の移動方向から弾の回転角度を求める
		g_EnemyLaser[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		// カプセルの設定
		g_EnemyLaser[i].collision.x = x;
		g_EnemyLaser[i].collision.y = y;
		g_EnemyLaser[i].collision.ex = dir.x * 1000;
		g_EnemyLaser[i].collision.ey = dir.y * 1000;
		g_EnemyLaser[i].collision.r = ENEMYLASER_WIDTH * 0.8f;

		// 有効フレームを初期化
		g_EnemyLaser[i].frame = 0;

		for (int n = 0; n < ENAMYLASER_IMAGEMAX; n++){
			g_EnemyLaser[i].laserIMG[n].bEnable = false;
		}

		// 弾を有効にする
		g_EnemyLaser[i].bEnable = true;

		break;
	}
}

//レーザーを消去する関数
void EnemyLaser_Destroy(int index)
{
	g_EnemyLaser[index].bEnable = false;
}

//レーザーの有効判定
bool EnemyLaser_IsEnable(int index)
{
	return g_EnemyLaser[index].bEnable;
}

//レーザーのコリジョン
const CUPSULE* EnemyLaser_GetCollision(int index)
{
	return &g_EnemyLaser[index].collision;
}

