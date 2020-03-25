/*-----------------------------
	title.cpp
------------------------------*/

#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include <d3d9.h>
#include "sound.h"

//スタティック変数
static bool g_bEnd = false;			//true+で画面遷移

//タイトルの初期化
void Title_Initialize(void)
{
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
	g_bEnd = false;
	PlaySound(SOUND_LABEL_BGM000);	//タイトルBGM再生
}

//タイトルの終了処理
void Title_Finalize(void)
{
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
}

//タイトルの更新
void Title_Update(void)
{
	//左スティックを右に倒すと操作画面へ
	if (Keyboard_IsTrigger(DIK_RIGHT) || GamePad_IsTrigger(0, BUTTON_RIGHT))
	{
		Scene_Change(SCENE_INDEX_SOUSA);
	}
	//左スティックを左に倒すとルール画面へ
	if (Keyboard_IsTrigger(DIK_LEFT) || GamePad_IsTrigger(0, BUTTON_LEFT))
	{
		Scene_Change(SCENE_INDEX_RULE);
	}

	//ゲームスタート
	if (!g_bEnd)
	{
		//タイトルからゲーム画面へ遷移
		if (Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsTrigger(0, BUTTON_Y))
		{
			Fade_Start(true, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_bEnd = true;
			StopSound(SOUND_LABEL_BGM000);	//タイトルBGM停止
		}
	}
	else 
	{
		if (!Fade_IsFade()) 
		{
			Scene_Change(SCENE_INDEX_GAME);
		}
	}
}

//タイトル画面の描画
void Title_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_TITLE, 0.0f, 0.0f);
}
