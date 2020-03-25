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

//�X�^�e�B�b�N�ϐ�
static bool g_bEnd = false;			//true�ŉ�ʑJ��

void Rule_Init()
{
	g_bEnd = false;
	//PlaySound(SOUND_LABEL_BGM000);	//�^�C�g��BGM�Đ�
}

void Rule_Uninit()
{

}

void Rule_Update()
{
	//���X�e�B�b�N���E�֓|���ƃ^�C�g����ʂ�
	if (Keyboard_IsPress(DIK_RIGHT) || GamePad_IsPress(0, BUTTON_RIGHT))
	{
		Scene_Change(SCENE_INDEX_TITLE);
	}
	
	//���X�e�B�b�N�����֓|���Ƒ����ʂ�
	if (Keyboard_IsPress(DIK_LEFT) || GamePad_IsPress(0, BUTTON_LEFT))
	{
		Scene_Change(SCENE_INDEX_SOUSA);
	}

	//�Q�[���X�^�[�g
	if (!g_bEnd)
	{
		//�^�C�g������Q�[����ʂ֑J��
		if (Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsTrigger(0, BUTTON_Y))
		{
			Fade_Start(true, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_bEnd = true;
			StopSound(SOUND_LABEL_BGM000);	//�^�C�g��BGM��~
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