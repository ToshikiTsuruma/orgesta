//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "sound.h"
#include "gameScene.h"
#include "player.h"
#include "terrain.h"
#include "effect.h"
#include "billboard.h"
#include "particleEffect.h"
#include "gauge3D.h"
#include "appleTree.h"
#include "apple_black.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define FALLDOWN_TIME (35)	//���S���̓|��鎞��
#define FINISH_TIME_CHANGE_COLOR_GAMEOVER (60)	//���S��̐F�ύX�ɂ����鎞��
#define START_CHANGE_COLOR_DELETE (180)	//���S��̔j���J�n����
#define FINISH_TIME_DELETE (60)		//���S��̔j���ɂ����鎞��

#define GOLD_RATE (7)	//���̓G�ɂȂ�m��
#define DEFAULT_GROW_VALUE (1)	//�f�t�H���g�̓G���S���̗ь�̖ؐ�����
#define GOLD_GROW_VALUE (10)		//���̓G���S���̗ь�̖ؐ�����

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
bool CEnemy::m_bGoldRush = false;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy() : m_fMoveSpeed(0.0f), m_fRotateSpeed(0.0f), m_nMaxLife(0), m_fDistAttack(0.0f), m_fHeightLifeGauge(0.0f)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, float fMoveSpeed, float fRotSpeed, int nLife, float fDistAttack, float fHeightLifeGauge)
	: CObjectMotion(pPartsInfoArray, nNumParts, pMotionInfoArray, nNumTypeMotion, false), m_fMoveSpeed(fMoveSpeed), m_fRotateSpeed(fRotSpeed), m_nMaxLife(nLife), m_fDistAttack(fDistAttack), m_fHeightLifeGauge(fHeightLifeGauge)
{
	SetUpdatePriority(UPDATE_PRIORITY::ENEMY);	//�X�V���̐ݒ�
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//�`�揇�̐ݒ�
	SetObjType(OBJTYPE_ENEMY);	//�I�u�W�F�N�g�^�C�v�̐ݒ�

	SetColorOutlineAll(D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f));	//�֊s�̐F�̐ݒ�
	SetColorGlowAll(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	//�֊s�̔����F�̐ݒ�

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDestY = 0.0f;
	m_nLife = nLife;
	m_bDead = false;
	m_nCntDead = 0;
	m_nKillScore = 0;

	m_bLand = false;
	m_pAttackTarget = nullptr;
	m_bAttackDist = false;
	m_bGoldEnemy = false;

	//�̗̓Q�[�W�̐���
	m_pGaugeLife = CGauge3D::Create(m_nMaxLife, false, m_nLife, FPS * 10, false);
	//�Q�[�W�̔w�i�̐ݒ�
	m_pGaugeLife->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55.0f, 7.0f);
	m_pGaugeLife->SetGaugeBGColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	//�Q�[�W�̐ݒ�
	m_pGaugeLife->CreateGauge(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, 4.0f);
	m_pGaugeLife->SetGaugeColor(D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
	//�Q�[�W���\��
	m_pGaugeLife->SetGaugeDraw(false);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// �G�l�~�[�̏���������
//=============================================================================
HRESULT CEnemy::Init(void) {
	//�����蔻�肠��
	SetEnableCollision(true);

	//�����_���ŋ��F�̓G
	int nRandGold = rand() % GOLD_RATE;
	if (nRandGold == 0 || m_bGoldRush) {
		m_bGoldEnemy = true;
		SetDiffuseModelAll(D3DXCOLOR(0.7f, 0.5f, 0.0f, 1.0f), 0);
		SetSpecularModelAll(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f), 0);
	}

	//���[�V�����I�u�W�F�N�g�̏���������
	CObjectMotion::Init();
	return S_OK;
}

//=============================================================================
// �G�l�~�[�̏I������
//=============================================================================
void CEnemy::Uninit(void) {
	//�̗̓Q�[�W�̔j��
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}
	//�I������
	CObjectMotion::Uninit();
}

//=============================================================================
// �G�l�~�[�̍X�V����
//=============================================================================
void CEnemy::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//----------------------------
	//���S��
	//----------------------------
	if (m_bDead) {
		m_nCntDead++;

		//�d��
		SetPos(GetPos() + D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f));

		//�n�`�Ƃ̓����蔻��
		bool bLand = false;	//�ڒn���Ă��邩�ǂ���
		D3DXVECTOR3 posColTerrain = GetPos();

		bLand = CTerrain::Collision(posColTerrain);
		//�ڒn��
		if (bLand) {
			SetPos(posColTerrain);	//�ʒu�̈ړ�
		}

		//�|��鎞��
		if (m_nCntDead == FALLDOWN_TIME) {
			//�|��鉹
			if (pSound != nullptr)pSound->PlaySound(CSound::SOUND_LABEL::STUN);

			//�p�[�e�B�N���G�t�F�N�g�̐���
			D3DXVECTOR3 posParticle;	//�p�[�e�B�N���̈ʒu
			GetPosCollision(&posParticle, D3DXVECTOR3(0.0f, 20.0f, 0.0f), 0);	//���̂̈ʒu�̎擾

			//�p�[�e�B�N���G�t�F�N�g�̍ŏ��̐F
			D3DXCOLOR colEffectStart = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			if (m_bGoldEnemy) colEffectStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

			//�p�[�e�B�N���̏��
			CParticleEffect::PARTICLE_INFO particleInfo = { 60, 25.0f, -0.25f, 2.0f, D3DXVECTOR3(0.0f, -0.01f, 0.0f), colEffectStart, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) };
			//�G�t�F�N�g�̐���
			CParticleEffect::Create(particleInfo, posParticle, START_CHANGE_COLOR_DELETE - FALLDOWN_TIME, 10, 0.05f * D3DX_PI, true);

			//�F�𔒂��ύX
			StartChangeDiffuseAll(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FINISH_TIME_CHANGE_COLOR_GAMEOVER);
		}

		//�G�l�~�[�̐F�̕ύX�J�n
		if (m_nCntDead == START_CHANGE_COLOR_DELETE) {
			//�����ɂ��Ă���
			StartChangeDiffuseAll(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), FINISH_TIME_DELETE);
		}

		//�G�l�~�[�̔j��
		if (m_nCntDead >= START_CHANGE_COLOR_DELETE + FINISH_TIME_DELETE) {
			//�I������
			Uninit();
			return;
		}

		//���[�V�����̍X�V
		CObjectMotion::Update();
		return;
	}


	//----------------------------
	//��]
	//----------------------------
	D3DXVECTOR3 rot = GetRot();	//�p�x�̎擾

	//�ڕW�p�x�Ɗp�x���s��v�̏ꍇ
	if (m_rotDestY != GetRot().y) {	//GetMotionCategory() != MOTION_CATEGORY::ATTACK && 

		//�ڕW�p�x�։�]������

		//�ڕW�p�x���}�C�i�X�A�p�x���v���X�̂Ƃ��ɁA���Z�������ʂ�-�΂𒴂����ꍇ
		if (m_rotDestY - rot.y < -D3DX_PI) {
			//���Z
			rot.y += m_fRotateSpeed;
		}
		//�ڕW�p�x���v���X�A�p�x���}�C�i�X�̂Ƃ��ɁA���Z�������ʂ��΂𒴂����ꍇ
		else if (m_rotDestY - rot.y > D3DX_PI) {
			//���Z
			rot.y -= m_fRotateSpeed;
		}
		//�X�̐����͊֌W�Ȃ��ꍇ
		else if (m_rotDestY - rot.y > 0) {
			//���Z
			rot.y += m_fRotateSpeed;
			//�ڕW�p�x���ߎ��̒���
			if (rot.y > m_rotDestY) {
				rot.y = m_rotDestY;
			}
		}
		else if (m_rotDestY - rot.y < 0) {
			//���Z
			rot.y -= m_fRotateSpeed;
			//�ڕW�p�x���ߎ��̒���
			if (rot.y < m_rotDestY) {
				rot.y = m_rotDestY;
			}
		}


		//�Β��ߎ��̒���
		if (rot.y > D3DX_PI) {
			rot.y -= D3DX_PI * 2;
		}
		else if (rot.y < -D3DX_PI) {
			rot.y += D3DX_PI * 2;
		}

		//�p�x�̐ݒ�
		SetRot(rot);
	}

	//----------------------------
	//�ړ�
	//----------------------------
	//�ړ�
	if (m_bMove) {
		m_move.x = sinf(rot.y + D3DX_PI) * m_fMoveSpeed;
		m_move.z = cosf(rot.y + D3DX_PI) * m_fMoveSpeed;
		//�U���\�����̏ꍇ�͓����Ȃ�
		if (m_bAttackDist) {
			m_move.x = 0.0f;
			m_move.z = 0.0f;
		}
	}

	//�ʒu�̐ݒ�
	SetPos(GetPos() + m_move);

	//----------------------------
	//�����蔻��
	//----------------------------
	//�n�ʂƂ̓����蔻��
	bool bLand = false;	//�ڒn���Ă��邩�ǂ���
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//�ڐG�ʒu�A�J�n�x�N�g���A�I���x�N�g��
	vecStart = GetPos();	//�G�̈ʒu���擾
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//������̃x�N�g��

	bLand = CTerrain::Collision(posColTerrain, vecStart, vecEnd);
	//�ڒn��
	if (bLand) {
		m_bLand = true;
		m_move.y = -POWER_GRAVITY_GROUND;
		SetPos(posColTerrain);	//�ʒu�̈ړ�

		//�U���Ώۂ�T��	�U�����Ŗ����A�U���\�����łȂ��ꍇ�T��
		if (GetMotionCategory() != MOTION_CATEGORY::ATTACK && !m_bAttackDist) SearchAttackTarget();
		//�U���ڕW�̕����ւ̊p�x��ڕW�p�x�Ƃ��Đݒ�
		SetRotToTarget();

		//���p�t�G���ړ����[�V�����Ȃ��ɂ��Ă���̂ňړ��s���ɐڒn���Ă���Έړ����[�V����
		if (!m_bMove && GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
			//�ړ����[�V�����̐ݒ�
			SetMoveMotion();
			m_bMove = true;
		}
	}
	else {
		m_bLand = false;
		//�d�͂��|����
		m_move.y -= POWER_GRAVITY;
	}

	//������
	if (GetPos().y < DEAD_HEIGHT) {
		Uninit();
		return;
	}

	//�U���\�ȋ������ǂ����v�Z
	AttackDistance();

	//�U���\�Ȋp�x���ǂ���
	bool bAngleAttack = fabsf(m_rotDestY - GetRot().y) <= D3DX_PI * 0.2f || fabsf(m_rotDestY - GetRot().y) >= D3DX_PI * 2.0f - D3DX_PI * 0.2f;

	//�U���J�n �i�U�����[�V�������@�U�������@�n��@�U���ł���p�x�j
	if (GetMotionCategory() != MOTION_CATEGORY::ATTACK && m_bAttackDist && m_bLand && bAngleAttack) {
		Attack();
	}

	//���[�V�����̍X�V
	CObjectMotion::Update();

	//���[�V�����I����
	if (GetEndMotion()) {
		MotionEnd();
	}

	//�I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X���X�V
	UpdateMtxWorldAll();
	//���[�V�������̏���
	MotionAct();

	if (m_pGaugeLife != nullptr) {
		//�̗̓Q�[�W�̈ʒu���X�V
		m_pGaugeLife->SetAllGaugePos(D3DXVECTOR3(GetPos().x, GetPos().y + m_fHeightLifeGauge, GetPos().z));
	}
}

//=============================================================================
// �G�l�~�[�̕`�揈��
//=============================================================================
void CEnemy::Draw(void) {
	CObjectMotion::Draw();
}

//=============================================================================
// �ړ��ʂ̐ݒ�
//=============================================================================
void CEnemy::SetMove(D3DXVECTOR3 move) {
	m_move = move;
	//�e����΂��ꂽ�肵�ăW�����v����悤�Ȏ�������΃j���[�g�������[�V������ݒ�i�ړ����[�V�������͈ړ����Œ肳��邽�߁j
	if (m_move.y > 0.0f) {
		SetMotion(0);
		AttackStop();
		m_bMove = false;
	}
}

//=============================================================================
// �G�l�~�[�̃_���[�W
//=============================================================================
void CEnemy::Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead) {
	if (m_bDead) return;

	//�_���[�W
	m_nLife -= nDamage;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//�U���̃^�C�v���Ƃ̏���
	switch (typeDamage)
	{
	case DAMAGE_TYPE::PLAYER_PUNCH:
	case DAMAGE_TYPE::SHOCKWAVE:
		//�_���[�W�G�t�F�N�g�̐���
		CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f), CEffect::EFFECT_TYPE::DAMAGE_PLAYER, 40.0f, 40.0f, false);
		//�_���[�W���̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		break;
	case DAMAGE_TYPE::THUNDERBOLT:
		//�_���[�W�G�t�F�N�g�̐���
		CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f), CEffect::EFFECT_TYPE::THUNDER, 50.0f, 50.0f, false);
		//�_���[�W���̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_THUNDER);
		break;
	}

	//���S����
	if (m_nLife <= 0) {
		//���S
		if (pDead != nullptr) *pDead = true;
		Dead();
	}
	else {
		if (pDead != nullptr) *pDead = false;

		//�̗̓Q�[�W�̐ݒ�
		if (m_pGaugeLife != nullptr) {
			m_pGaugeLife->SetGaugeValue(m_nLife);
		}
	}
}

//=============================================================================
// �G�l�~�[�̎��S
//=============================================================================
void CEnemy::Dead(void) {
	//���S�̐ݒ�
	m_bDead = true;
	//���S�J�E���g�̏�����
	m_nCntDead = 0;
	//�����蔻��𖳌�
	SetEnableCollision(false);
	//���S���[�V�����̐ݒ�
	SetDeadMotion();
	//�֊s���\��
	SetDrawOutlineAll(false);

	//�̗̓Q�[�W�̔j��
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	//�Q�[���V�[���̎擾
	CGameScene* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//�ь�̖؂̎擾
	CAppleTree* pAppleTree = nullptr;
	if (pGame != nullptr) {
		//�|�����X�R�A�̉��Z
		pGame->AddGameScore(m_nKillScore);
		//�|�����G�̐��̉��Z
		pGame->AddNumKillEnemy(1);
		//�����S�̖؂��擾
		pAppleTree = pGame->GetAppleTree();
	}

	//�ь�̖؂𐬒�������
	if (pAppleTree != nullptr) {
		int nGrowValue = DEFAULT_GROW_VALUE; //������
		//���̓G�̏ꍇ��������
		if (m_bGoldEnemy) nGrowValue = GOLD_GROW_VALUE;
		//�؂𐬒�
		pAppleTree->AddGrow(nGrowValue);
	}

	/*
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//���̍Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::EXPLOSION);
	*/
}

//=============================================================================
// �U���ڕW��T��
//=============================================================================
void CEnemy::SearchAttackTarget(void) {
	CObject* pLastTarget = m_pAttackTarget;	//�Ō�̃^�[�Q�b�g

	//�ł��߂��؂̃I�u�W�F�N�g���擾
	m_pAttackTarget = GetNearObject(GetPos(), ENEMY_ATTACK_TARGET , OBJFLAG_ENABLECOLLISION, nullptr, nullptr, nullptr);

	//�U���ΏۊO�Ȃ��Ȃ����ꍇ��~
	if (m_pAttackTarget == nullptr && pLastTarget != nullptr) {
		//�������~������
		SetMotion(0);
		m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
		m_bMove = false;
	}
}

//=============================================================================
// �U���ڕW�̕����ւ̊p�x�ɐݒ�
//=============================================================================
void CEnemy::SetRotToTarget(void) {
	if (m_pAttackTarget == nullptr) return;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�Q�[���V�[���̎擾
	CGameScene* pGame = pManager->GetGameScene();
	if (pGame == nullptr) return;

	D3DXVECTOR3 posTarget = m_pAttackTarget->GetPos();	//�U���Ώۂ̈ʒu���擾

	//�G����U���Ώۂւ̊p�x
	m_rotDestY = (float)atan2(posTarget.x - GetPos().x, posTarget.z - GetPos().z);

	//���f�������΂Ȃ̂Ŕ��Ό����Ɍ�����
	if (m_rotDestY >= 0.0f) {
		m_rotDestY -= D3DX_PI;
		if (m_rotDestY < D3DX_PI * -2.0f) m_rotDestY += D3DX_PI * 2.0f;
	}
	else if (m_rotDestY < 0.0f) {
		m_rotDestY += D3DX_PI;
		if (m_rotDestY > D3DX_PI * 2.0f) m_rotDestY -= D3DX_PI * 2.0f;
	}
}

//=============================================================================
// �U���\�ȋ������ǂ������v�Z
//=============================================================================
void CEnemy::AttackDistance(void) {
	if (m_pAttackTarget == nullptr) {
		m_bAttackDist = false;
		return;
	}

	D3DXVECTOR3 posTarget = m_pAttackTarget->GetPos();	//�U���Ώۂ̈ʒu���擾
	float fRadiusTarget = m_pAttackTarget->GetRadius();	//�U���Ώۂ̔��a
	float fDistTarget = D3DXVec2Length(&D3DXVECTOR2(posTarget.x - GetPos().x, posTarget.z - GetPos().z));	//�U���ΏۂƓG�Ƃ̋���

	//�U���\���ǂ����̔���
	m_bAttackDist = fDistTarget - fRadiusTarget <= m_fDistAttack;
}

//=============================================================================
//�@�U���̏���
//=============================================================================
void CEnemy::Attack(void) {
	//�U���ς݃��X�g�̏�����
	InitObjAttacked();
	//�ړ��ʂ��d�݂͂̂ɂ���
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bMove = false;
	//�U���J�n
	AttackStart();
}