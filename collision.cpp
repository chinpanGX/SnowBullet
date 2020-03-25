/*-----------------------------
	collision.cpp
------------------------------*/

#include "game.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "enemy.h"
#include "bullet.h"
#include "enemybullet.h"
#include "enemylaser.h"
#include "explosion.h"
#include "scene.h"
#include "tilemap.h"
#include "sound.h"
#include <d3dx9.h>

//�v���g�^�C�v�錾
static bool HitCircle(const CIRCLE* pCircle_a, const CIRCLE* pCircle_b);
void Collision_Player_vs_Enemy(void);
void Collision_Bullet_vs_Enemy(void);
void Collision_Player_vs_EnemyBullet(void);
void Collision_Player_vs_EnemyLaser(void);

//�~�̓����蔻��i�T�[�N���R���C�_�[�j
bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
	//��������x�N�g���^�̕ϐ������
	D3DXVECTOR2 dst1(p_circle_a->cx, p_circle_a->cy);
	D3DXVECTOR2 dst2(p_circle_b->cx, p_circle_b->cy);
	//��_�Ԃ̃x�N�g�������
	D3DXVECTOR2 distance = dst2 - dst1;
	//������x�N�g���̒��������߂�
	float length = D3DXVec2Length(&distance);
	//���݂��̔��a�𑫂����l�����߂�
	float size = p_circle_a->r + p_circle_b->r;

	//�x�N�g���̒����Ƃ��݂��̔��a�𑫂����l���r����
	//���x�N�g���̒����̕�����������΃q�b�g���Ă���I
	if (length < size){
		return true;
	}
	//�q�b�g���Ă��Ȃ�����
	return false;
}

#if 0
	bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
	{
	//��������x�N�g���^�̕ϐ������
	D3DXVECTOR2 dst1(p_circle_a->cx, p_circle_a->cy);
	D3DXVECTOR2 dst2(p_circle_b->cx, p_circle_b->cy);
	//��_�Ԃ̃x�N�g�������
	D3DXVECTOR2 distance = dst2 - dst1;
	//������x�N�g���̒��������߂�
	float length = D3DXVec2LengthSq(&distance);
	//���݂��̔��a�𑫂����l�����߂�
	float size = (p_circle_a->r + p_circle_b->r) * (p_circle_a->r + p_circle_b->r);

	//�x�N�g���̒����Ƃ��݂��̔��a�𑫂����l���r����
	//���x�N�g���̒����̕�����������΃q�b�g���Ă���I
	if (length < (p_circle_a->r + p_circle_b->r)){
	return true;
	}
	//�q�b�g���Ă��Ȃ�����
	return false;
}
#endif

//�J�v�Z���R���C�_�[
bool HitCupsule(const CIRCLE* p_circle, const CUPSULE* p_cupsule)
{
	//�~�ƃJ�v�Z���̈ʒu
	D3DXVECTOR2 cir_pos = D3DXVECTOR2(p_circle->cx, p_circle->cy);
	D3DXVECTOR2 cup_pos = D3DXVECTOR2(p_cupsule->x, p_cupsule->y);
	D3DXVECTOR2 cup_end = D3DXVECTOR2(p_cupsule->ex, p_cupsule->ey);

	D3DXVECTOR2 distance = cir_pos - cup_pos;

	float t = 0.0f;

	//��_�����߂�
	t = (cup_end.x * distance.x + cup_end.y * distance.y) /
		(cup_end.x * cup_end.x  + cup_end.y * cup_end.y);

	if (t < 0.0f)
		t = 0.0f;
	if (t > 1.0f)
		t = 1.0f;

	D3DXVECTOR2 pos_cross;
	pos_cross.x = (cup_end.x * t) + cup_pos.x;
	pos_cross.y = (cup_end.y * t) + cup_pos.y;

	//��_�܂ł̒����Ƃ��݂��̔��a�𑫂������̂̑傫�����ׂ�
	float cross_len = (pos_cross.x - cir_pos.x) * (pos_cross.x - cir_pos.x) +
                	  (pos_cross.y - cir_pos.y) * (pos_cross.y - cir_pos.y);

	float size = (p_circle->r + p_cupsule->r);

	//���x�N�g���̒����̕�����������΃q�b�g���Ă���I
	if (cross_len < size * size)
	{
		return true;
	}

	//�q�b�g���Ă��Ȃ�����
	return false;
}

//������
void Collision_Initialize(void)
{
}

//�X�V
void Collision_Update(void)
{
//	Collision_Player_vs_Enemy();
	Collision_Bullet_vs_Enemy();
	Collision_Player_vs_EnemyBullet();
	Collision_Player_vs_EnemyLaser();
}

//�v���C���[�ƃG�l�~�[�̔���
void Collision_Player_vs_Enemy(void)
{
	for (int i = 0; i < ENEMY_COUNT; i++) {

		// �G�l�~�[�͗L�����H
		if( !Enemy_IsEnable(i) ) {
			continue;
		}

		// �v���C���[�̃R���W�����ƃG�l�~�[�̃R���W����
		if (HitCircle(Player_GetCollision(), Enemy_GetCollision(i)))
		{
			// �������Ă�->�G�l�~�[�̏���
			Enemy_Destroy(i);
		}
	}
}

//�v���C���[�ƃG�l�~�[�̒e�̔���
void Collision_Player_vs_EnemyBullet(void)
{
	for (int i = 0; i < ENEMYBULLET_MAX; i++) {

		// �G�l�~�[�͗L�����H
		if (!EnemyBullet_IsEnable(i)) {
			continue;
		}

		// �v���C���[�̃R���W�����ƃG�l�~�[�̃R���W����
		if (HitCircle(Player_GetCollision(), EnemyBullet_GetCollision(i)))
		{
			// �v���C���[�ɓ������Ă� 
			Player_AddDamage(30);	//30�̃_���[�W

			// �����G�t�F�N�g�̍쐬
			Explosion_Create(EnemyBullet_GetCollision(i)->cx, EnemyBullet_GetCollision(i)->cy);
			PlaySound(SOUND_LABEL_SE_EXPLOSION);	//����SE�̍Đ�

			// �e�̏��ŏ���
			EnemyBullet_Destroy(i);
		}
	}
}

//�v���C���[�ƃG�l�~�[�̃��[�U�[�̔���
void Collision_Player_vs_EnemyLaser(void)
{
	for (int i = 0; i < ENEMYLASER_MAX; i++) {

		// �G�l�~�[�͗L�����H
		if (!EnemyLaser_IsEnable(i)) {
			continue;
		}

		// �v���C���[�̃R���W�����ƃ��[�U�[�̃R���W����
		if (HitCupsule(Player_GetCollision(), EnemyLaser_GetCollision(i)))
		{
			// �v���C���[�ɓ������Ă�
			Player_AddDamage(10);	//10�̃_���[�W

			// �����G�t�F�N�g�̍쐬
			Explosion_Create(Player_GetCollision()->cx, Player_GetCollision()->cy);
			PlaySound(SOUND_LABEL_SE_EXPLOSION);	//����SE�Đ�

		}
	}
}

//�v���C���[�̒e�ƃG�l�~�[�̔���
void Collision_Bullet_vs_Enemy(void) 
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		// �e�͗L�����H
		if (!Bullet_IsEnable(i)) {
			continue;
		}

		for (int j = 0; j < ENEMY_COUNT; j++)
		{
			// �G�l�~�[�͗L�����H
			if (!Enemy_IsEnable(j)) {
				continue;
			}
			

			// �e�̃R���W�����ƃG�l�~�[�̃R���W����
			if (HitCircle(Bullet_GetCollision(i), Enemy_GetCollision(j)))
			{
				/*-------------------
				|	�������Ă���	|
				--------------------*/

				// ���_�̒ǉ�
 				//Game_AddScore(100);
				
				//�G�l�~�[�̃L���J�E���^�[���{�P����
				Enemy_KillCounter(1);

				// �����G�t�F�N�g�̍쐬
				Explosion_Create(Enemy_GetCollision(j)->cx, Enemy_GetCollision(j)->cy);

				// �G�̏��ŏ���
				Enemy_Destroy(j);

				// �e�̏��ŏ���
				Bullet_Destroy(i);

				// ���̒e�̏����͏I��
				break;
			}
		}
	}
}

//�}�b�v�̃q�b�g�`�F�b�N
bool Collision_HitCheck_TileMap(D3DXVECTOR2 dst, D3DXVECTOR2* pOut)
{
	TILE_DATA tile;
	bool hit = false;

	//���̈ړ��`�F�b�N
	tile = GetTileInfo(dst.x + pOut->x, dst.y);
	if (tile.isWalk == 0)
	{
		pOut->x = 0.0f;
		hit = true;
	}

	//�c�̈ړ��`�F�b�N
	tile = GetTileInfo(dst.x, dst.y + pOut->y);
	//�����Ȃ��}�b�v��������q�b�g
	if (tile.isWalk == 0)
	{
		pOut->y = 0.0f;
		hit = true;
	}

	return hit;
}

