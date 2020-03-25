/*-----------------------------
	player.cpp
------------------------------*/

#include "player.h"
#include "main.h"
#include "input.h"
#include "sprite.h"
#include "table.h"
#include "bullet.h"
#include "collision.h"
#include "sound.h"

//マクロ定義
#define PLAYER_WIDTH	(32)
#define PLAYER_HEIGHT	(32)

#define ANIME_PATTERN_MAX		3
#define ANIME_PATTERN_SKIPFRAME 8

//構造体
typedef struct
{
	D3DXVECTOR2 pos;			//位置
	float		speed;			//移動速度
	int			muki;			//向き
	int			color;			//色

	int			animePattern;	//アニメパターン
	int			animeFrame;		//パターン切り替えウエイト

	CIRCLE		collision;		//円の当たり判定
	int			hitpoint;		//ヒットポイント
//	int			limit;			//瞬間移動の回数
}PLAYER;

PLAYER player;

//プレイヤーの初期化
void Player_Initialize(void)
{
	//プレイヤーの初期化
	player.pos.x = SCREEN_WIDTH / 2;
	player.pos.y = SCREEN_HEIGHT / 2;

	player.speed = 0.5f;	//移動速度

	player.color = 0;		//色
	player.muki = 0;		//向き

	player.collision.cx = player.pos.x;			//コリジョンx
	player.collision.cy = player.pos.y;			//コリジョンy
	player.collision.r = PLAYER_WIDTH * 0.8f;	//コリジョン半径

	player.hitpoint = 5000;	//ヒットポイント
	//player.limit = 5;			//瞬間移動の回数	
}

//プレイヤーの終了処理
void Player_Finalize(void)
{

}

//プレイヤーの更新
void Player_Update(void)
{
	D3DXVECTOR2 dir = D3DXVECTOR2(0.0f, 0.0f);

//----------------------------------------------------------------------------------------------------	
	/*--------------------------------------
		プレイヤーのアクション
	----------------------------------------*/
	/*-----------------------
	|	移動のインプット	|
	------------------------*/
	//上
	if (Keyboard_IsPress(DIK_UP) ||Keyboard_IsPress(DIK_W) || GamePad_IsPress(0, BUTTON_UP))
	{
		dir.y += -1;
		player.muki = 3;
		if (player.pos.y < 0 + 50)
		{
			player.pos.y = 0 + 50;
		}
	}
	//下
	if (Keyboard_IsPress(DIK_DOWN) || Keyboard_IsPress(DIK_S) || GamePad_IsPress(0, BUTTON_DOWN)){
		dir.y -= -1;
		player.muki = 0;
		if (player.pos.y > SCREEN_HEIGHT - 50)
		{
			player.pos.y = SCREEN_HEIGHT - 50;
		}
	}
	//左
	if (Keyboard_IsPress(DIK_LEFT) || Keyboard_IsPress(DIK_A) || GamePad_IsPress(0, BUTTON_LEFT)){
		dir.x += -1;
		player.muki = 1;
		if (player.pos.x > SCREEN_WIDTH - 50)
		{
			player.pos.x = SCREEN_WIDTH - 50;
		}
	}
	//右
	if (Keyboard_IsPress(DIK_RIGHT) || Keyboard_IsPress(DIK_D) || GamePad_IsPress(0, BUTTON_RIGHT)){
		dir.x -= -1;
		player.muki = 2;
		if (player.pos.x < 0 + 50)
		{
			player.pos.x = 0 + 50;

		}
	}

	/*---------------------------
	|	アクションインプット	|
	----------------------------*/
	
	//スペース＆ゲームパッドのBボタンが押されたら弾を発射
	if (Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsTrigger(0, BUTTON_B))
	{
		PlaySound(SOUND_LABEL_SE_SHOT);	//ショットSEの再生
		
		//カーソルキー入力がされていない場合、向きから発射方向を作成する
		if (D3DXVec2Length(&dir) < 0.01f) 
		{
			switch (player.muki)
			{
			case 0://下向き
				dir.y = 1.0f;
				break;
			case 1://左向き
				dir.x = -1.0f;
				break;
			case 2://右向き
				dir.x = 1.0f;
				break;
			case 3://上向き
				dir.y = -1.0f;
				break;
			}
		}
		Bullet_Create(player.pos.x, player.pos.y, dir);
		dir = D3DXVECTOR2(0.0f, 0.0f);
	}
#if 0
	//プレイヤーのリミットが０以上
	if (player.limit > 0)
	{
		//Qキー＆ゲームパッドのLボタン
		if (Keyboard_IsPress(DIK_Q) || GamePad_IsPress(0, BUTTON_L))
		{
			dir.x += -1;
			player.muki = 1;
			if (player.pos.x > SCREEN_WIDTH - 50)
			{
				player.pos.x = SCREEN_WIDTH - 50;
			}
			player.limit--;
		}
		//Eキー＆ゲームパッドのRボタン
		if (Keyboard_IsPress(DIK_E) || GamePad_IsPress(0, BUTTON_R))
		{
			dir.x -= -1;
			player.muki = 2;
			if (player.pos.x < 0 + 50)
			{
				player.pos.x = 0 + 50;

			}
			player.limit--;
		}
	}
#endif
//--------------------------------------------------------------------------------------------

	//マップとのヒットチェック
	Collision_HitCheck_TileMap(player.pos, &dir);

	//斜めの入力
	D3DXVec2Normalize(&dir, &dir);
	D3DXVECTOR2 vecSpeed = dir * player.speed;
	player.pos += vecSpeed;
	

	// 座標の更新処理
	player.pos += dir;

	//当たり判定用座標の更新
	player.collision.cx = player.pos.x;
	player.collision.cy = player.pos.y;

	//スキップするフレーム値を超えたら
	if (++player.animeFrame > ANIME_PATTERN_SKIPFRAME)
	{
		//アニメパターンを進める(最大値を超えたらリセット)
		if (++player.animePattern >= ANIME_PATTERN_MAX)
			player.animePattern = 0;
		//フレームは元に戻す
		player.animeFrame = 0;
	}
}

//プレイヤーの描画
void Player_Draw(void)
{
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
	Sprite_Draw(TEXTURE_INDEX_YUKIDARUMA,
		player.pos.x,
		player.pos.y,
		GetAnimTbl(player.color, player.muki, player.animePattern).x * 256,
		GetAnimTbl(player.color, player.muki, player.animePattern).y * 256,
		32,
		32,
		16,
		16,
		1.0f,
		1.0f,
		0.0f);
}

//プレイヤーのコリジョン
const CIRCLE* Player_GetCollision()
{
	return &player.collision;
}

//プレイヤーのダメージ
void Player_AddDamage(int damage)
{
	PlaySound(SOUND_LABEL_SE_HIT);	//ヒットSEの再生
	player.hitpoint -= damage;
	if (player.hitpoint < 0)
		player.hitpoint = 0;
}

//ヒットポイントのゲッター
int Player_GetHitPoint()
{
	return player.hitpoint;
}
