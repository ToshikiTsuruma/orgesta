//=============================================================================
//
// �G�X�|�i�[���� [enemySpawner.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "enemySpawner.h"
#include "manager.h"
#include "sound.h"
#include "gameScene.h"
#include "timer.h"
#include "enemy_normal.h"
#include "enemy_human.h"
#include "enemy_sorcerer.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemySpawner::CEnemySpawner()
{
	m_nLevel = 0;
	m_nCntSpawn = 0;
	m_nSpanSpawn = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemySpawner::~CEnemySpawner()
{

}

//=============================================================================
// �G�X�|�i�[�̐�������
//=============================================================================
CEnemySpawner* CEnemySpawner::Create(int nSpan, float fRadius, int nDistMin, int nDistMax) {
	CEnemySpawner* pEnemySpawner;
	pEnemySpawner = new CEnemySpawner();
	if (pEnemySpawner == nullptr) return nullptr;

	pEnemySpawner->m_nSpanSpawn = nSpan;
	pEnemySpawner->m_fRadiusSpawn = fRadius;
	pEnemySpawner->m_nDistEnemyMin = nDistMin;
	pEnemySpawner->m_nDistEnemyMax = nDistMax;
	pEnemySpawner->Init();

	return pEnemySpawner;
}

//=============================================================================
// �G�X�|�i�[�̏���������
//=============================================================================
HRESULT CEnemySpawner::Init(void) {
	m_nCntSpawn = 0;

	return S_OK;
}

//=============================================================================
// �G�X�|�i�[�̏I������
//=============================================================================
void CEnemySpawner::Uninit(void) {
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �G�X�|�i�[�̍X�V����
//=============================================================================
void CEnemySpawner::Update(void) {
	//�����^�C�~���O
	if (m_nCntSpawn > m_nSpanSpawn) {
		m_nCntSpawn = 0;

		//�G���X�|�[��������
		SetEnemyCircle(D3DXVECTOR3(0.0f, 1500.0f, 0.0f), 1, m_fRadiusSpawn, m_nDistEnemyMin, m_nDistEnemyMax);

		//�}�l�[�W���[�̎擾
		CManager* pManager = CManager::GetManager();
		//�T�E���h�̎擾
		CSound* pSound = nullptr;
		if (pManager != nullptr) pSound = pManager->GetSound();
		//�X�|�[�����̍Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::SPAWN_ENEMY);
	}
	else {
		m_nCntSpawn++;
	}

	//�^�C�}�[�ɂ�郌�x���̐ݒ�
	SetTimeLevel();
}

//=============================================================================
// ���Ԍo�߂ɂ�郌�x���̐ݒ�
//=============================================================================
void CEnemySpawner::SetTimeLevel(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	if (pManager == nullptr) return;
	CGameScene* pGameScene = pManager->GetGameScene();;	//�Q�[���V�[���̃|�C���^
	if (pGameScene == nullptr) return;
	CTimer* pTimer = pGameScene->GetTimer();;	//�^�C�}�[�̃|�C���^
	if (pTimer == nullptr) return;

	//���Ԃ��؂�ւ�����u�Ԃ̂�
	if (pTimer->GetCountTimer() != 0) return;

	switch (pTimer->GetScore())
	{
	case 260:
		//�X�|�[���Ԋu�̌���
		AddSpan(-60);
		//���x���̐ݒ�
		SetLevel(1);
		break;

	case 220:
		//�X�|�[���Ԋu�̌���
		AddSpan(-120);
		break;
		//���x���̐ݒ�
		SetLevel(2);

	case 180:
		//�X�|�[���Ԋu�̌���
		AddSpan(-140);
		//���x���̐ݒ�
		SetLevel(3);
		//�S�[���h���b�V���L��
		CEnemy::SetGoldRush(true);
		break;

	case 100:
		//�X�|�[���Ԋu�̌���
		AddSpan(-160);
		//�S�[���h���b�V������
		CEnemy::SetGoldRush(false);
		break;

	case 60:
		//�X�|�[���Ԋu�̌���
		AddSpan(-180);
		//���x���̐ݒ�
		SetLevel(0);	//��Ԉړ����x�������G�݂̂ɂ���
		break;

	case 40:
		//���x���̐ݒ�
		SetLevel(4);	//��ԋ����G�͑����o��
		//�X�|�[���̔��a���k�߂�
		SetSpawnRadius(1000.0f);
		break;

	case 10:
		//�X�|�[���Ԋu�̌���
		m_nSpanSpawn = 60;
		//���x���̐ݒ�
		SetLevel(5);	//������
		break;
	}
}

//=============================================================================
// ���S�~��Ƀ����_���Ɋp�x���炵�ēG��z�u����
//=============================================================================
//�����F���S�̈ʒu�@�~�̐��@�~���Ƃ̋����i���a�j�@�i�����~�̈�O�̓G�Ƃ́j�ŒZ�����A�Œ�����
void CEnemySpawner::SetEnemyCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fDistCircle, int nMinDist, int nMaxDist) {
	int nCntCircle = 0;	//�쐬�����؂̉~�̐�
	float fRotCreate = 0.0f;	//�z�u����p�x
	float fRandStartRot = (rand() % 314) * 0.01f;	//�z�u�̊J�n�p�x�̈ʒu�������_�� �O�����ߑa�C���ɂȂ邽��

	while (nCntCircle < nNumCircle)
	{
		int nRandDist = rand() % (nMaxDist + 1 - nMinDist) + nMinDist;	//��O�̖؂Ƃ̋���
		fRotCreate += nRandDist / ((nCntCircle + 1) * fDistCircle * D3DX_PI) * D3DX_PI;	//�����_���ȋ��� / �~���̒����̔��� * pi �Ń��W�A�������߂�

		//����z�u�����ꍇ
		if (fRotCreate >= D3DX_PI * 2.0f) {
			nCntCircle++;	//�J�E���g�̉��Z
			fRotCreate = 0.0f;	//�z�u����p�x�̃��Z�b�g
			fRandStartRot = (rand() % 314) * 0.01f;	//�z�u����p�x�̃����_���l���Đݒ�
		}
		else {
			//�G�𐶐�����ʒu�̐ݒ�
			D3DXVECTOR3 posCreate = posCenter;	//�z�u����ʒu
			posCreate.x += sinf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			posCreate.z += cosf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);

			int aRateType[(int)ENEMYTYPE::ENUM_MAX] = {};	//��ނ��Ƃ̐����m��
			//�����m�����擾
			GetSpawnRate(m_nLevel, aRateType);
			//���ꂼ��̃^�C�v�ȑO�̃^�C�v�̊����̍��v�𑫂�
			for (int nCnt = 1; nCnt < (int)ENEMYTYPE::ENUM_MAX; nCnt++)
			{
				aRateType[nCnt] += aRateType[nCnt - 1];
			}

			//�G�̎�ނ������_���Ō��߂�
			int nRandType = rand() % 100;
			//�G�̐���
			if (nRandType < aRateType[(int)ENEMYTYPE::NORMAL]) {
				//�ʏ�G
				CEnemyNormal::Create(posCreate);
			}
			else if (nRandType < aRateType[(int)ENEMYTYPE::HUMAN]) {
				//�l�`�G
				CEnemyHuman::Create(posCreate);
			}
			else if (nRandType < aRateType[(int)ENEMYTYPE::SORCERER]) {
				//���p�t�G
				CEnemySorcerer::Create(posCreate);
			}
		}
	}
}

//=============================================================================
// ���x�����Ƃ̐����m���̎擾
//=============================================================================
void CEnemySpawner::GetSpawnRate(int nLevel, int* pRateArray) {
	if (pRateArray == nullptr) return;
		 
	switch (nLevel)
	{
	case 0:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 100;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 0;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 0;
		break;

	case 1:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 60;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 40;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 0;
		break;

	case 2:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 30;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 65;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 5;
		break;

	case 3:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 10;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 80;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 10;
		break;

	case 4:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 30;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 10;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 60;
		break;

	case 5:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 30;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 30;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 40;
		break;

	default:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 100;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 0;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 0;
		break;
	}
}