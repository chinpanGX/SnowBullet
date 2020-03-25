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

//�X�^�e�B�b�N�ϐ�
static bool g_bEnd = false;			//true+�ŉ�ʑJ��

//�^�C�g���̏�����
void Title_Initialize(void)
{
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
	g_bEnd = false;
	PlaySound(SOUND_LABEL_BGM000);	//�^�C�g��BGM�Đ�
}

//�^�C�g���̏I������
void Title_Finalize(void)
{
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
}

//�^�C�g���̍X�V
void Title_Update(void)
{
	//���X�e�B�b�N���E�ɓ|���Ƒ����ʂ�
	if (Keyboard_IsTrigger(DIK_RIGHT) || GamePad_IsTrigger(0, BUTTON_RIGHT))
	{
		Scene_Change(SCENE_INDEX_SOUSA);
	}
	//���X�e�B�b�N�����ɓ|���ƃ��[����ʂ�
	if (Keyboard_IsTrigger(DIK_LEFT) || GamePad_IsTrigger(0, BUTTON_LEFT))
	{
		Scene_Change(SCENE_INDEX_RULE);
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

//�^�C�g����ʂ̕`��
void Title_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_TITLE, 0.0f, 0.0f);
}
