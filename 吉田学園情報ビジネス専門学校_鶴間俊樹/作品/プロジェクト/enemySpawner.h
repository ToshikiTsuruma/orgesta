//=============================================================================
//
// �G�X�|�i�[���� [enemySpawner.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _ENEMY_SPAWNER_H_
#define _ENEMY_SPAWNER_H_

#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************

//*****************************************************************************
// �G�X�|�i�[�N���X
//*****************************************************************************
class CEnemySpawner : public CObject
{
public:
	//�G�̎��
	enum class ENEMYTYPE {
		NORMAL = 0,	//�ʏ�
		HUMAN,		//�l�`
		SORCERER,		//���p�t
		ENUM_MAX,	
	};

	CEnemySpawner();	//�f�t�H���g�R���X�g���N�^
	~CEnemySpawner();	//�f�X�g���N�^
	static CEnemySpawner* Create(int nSpan, float fRadius, int nDistMin, int nDistMax);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void) {}	//�`�揈��

	void AddLevel(int nLevel) { m_nLevel += nLevel; }	//���x���̑���
	void SetLevel(int nLevel) { m_nLevel = nLevel; }	//���x���̐ݒ�
	void SetSpan(int nSpan) { m_nSpanSpawn = nSpan; }	//�X�|�[���̃X�p����ݒ�
	void AddSpan(int nSpan) { m_nSpanSpawn += nSpan; }	//�X�|�[���̃X�p���𑝉�
	void SetSpawnRadius(float fRadius) { m_fRadiusSpawn = fRadius; }	//�X�|�[���̔��a��ݒ�

private:
	static void GetSpawnRate(int nLevel, int* pRateArray);	//���x�����Ƃ̐����m���̎擾
	void SetTimeLevel(void);	//���Ԍo�߂ɂ�郌�x���̐ݒ�
	void SetEnemyCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fRadiusCircle, int nMinDist, int nMaxDist);	//���S�~��Ƀ����_���Ɋp�x���炵�ēG��z�u����

	int m_nLevel;		//�X�|�i�[�̃��x��
	int m_nSpanSpawn;	//�X�|�[���̃X�p��
	int m_nCntSpawn;	//�X�|�[���܂ł̃J�E���g
	float m_fRadiusSpawn;	//�X�|�[���̔��a
	int m_nDistEnemyMin;	//�X�|�[������G���m�̋����̍ŏ�
	int m_nDistEnemyMax;	//�X�|�[������G���m�̋����̍ő�
};

#endif // !_ENEMY_SPAWNER_H_