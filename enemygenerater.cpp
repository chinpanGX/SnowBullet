/*------------------------------------
	enemygenerater.cpp
--------------------------------------*/

#include "enemygenerater.h"
#include "enemy.h"
#include "sprite.h"
#include "texture.h"

//�X�^�e�B�b�N�ϐ�
static int frameCounter = 0;	//�������ł�0����Y�ꂸ��

//������
void EnemyGenerater_Init(void)
{

}

//�I������
void EnemyGenerater_Uninit(void)
{

}

//�X�V
void EnemyGenerater_Update(void)
{
	if(frameCounter % 40 == 0)
	{
		Enemy_Create();
	}
	frameCounter++;
}