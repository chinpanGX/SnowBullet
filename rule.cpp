/*--------------------
	rule.cpp
----------------------*/

#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include <d3d9.h>
#include "sound.h"
#include "sousa.h"

//スタティック変数
static bool g_bEnd = false;			//trueで画面遷移

void Rule_Init()
{
	g_bEnd = false;
	//PlaySound(SOUND_LABEL_BGM000);	//タイトルBGM再生
}

void Rule_Uninit()
{

}

void Rule_Update()
{
	//左スティックを右へ倒すとタイトル画面へ
	if (Keyboard_IsPress(DIK_RIGHT) || GamePad_IsPress(0, BUTTON_RIGHT))
	{
		Scene_Change(SCENE_INDEX_TITLE);
	}
	
	//左スティックを左へ倒すと操作画面へ
	if (Keyboard_IsPress(DIK_LEFT) || GamePad_IsPress(0, BUTTON_LEFT))
	{
		Scene_Change(SCENE_INDEX_SOUSA);
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

void Rule_Draw()
{
	Sprite_Draw(TEXTURE_INDEX_RULE, 0.0f, 0.0f);
}