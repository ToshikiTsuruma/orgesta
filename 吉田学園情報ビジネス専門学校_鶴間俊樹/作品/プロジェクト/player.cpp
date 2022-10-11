//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sound.h"
#include "input.h"
#include "gameScene.h"

#include "wallCylinder.h"
#include "terrain.h"
#include "effect.h"
#include "particle.h"
#include "particleEffect.h"
#include "object2D.h"
#include "billboard.h"
#include "appleTree.h"

#include "shockWaveEmitter.h"
#include "thunderEmitter.h"

//=============================================================================
// �}�N����`
//=============================================================================
//--------------------------------
//�t�@�C����
//--------------------------------
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_player.txt"

//--------------------------------
//�ړ�
//--------------------------------
#define ADD_MOVE_SPEED (1.2f)			//����
#define DEC_MOVE_SPEED_GROUND (0.8f)	//�n��ł̌���
#define DEC_MOVE_SPEED_AIR (0.05f)		//�󒆂ł̌���
#define MAX_MOVE_SPEED_WALK (10.0f)		//���s���x
//#define MAX_MOVE_SPEED_DASH (12.0f)		//���s���x
#define MAX_MOVE_SPEED_AIR (2.0f)		//�󒆑��x
#define ROTATE_SPEED (0.1f)				//��]���x

//--------------------------------
//�����蔻��
//--------------------------------
#define COLLISION_RADIUS (70.0f)		//�����蔻��̔��a	�ǂƂ��Ɏg��
#define NUM_COLLISION (10)				//�����蔻��̐�

//--------------------------------
//�U��
//--------------------------------
#define ATTACK_DAMAGE_PUNCH (60)	//�p���`�̍U����
#define ATTACK_DAMAGE_STANP (180)	//���݂��̍U����

//--------------------------------
//�Q�[���I�[�o�[��
//--------------------------------
#define FINISH_TIME_CHANGE_COLOR_GAMEOVER (120)	//�Q�[���I�[�o�[��̐F�ύX�ɂ����鎞��
#define START_CHANGE_COLOR_CLEAR (180)	//�Q�[���I�[�o�[��̓����F�֕ύX����J�n����
#define FINISH_TIME_CLEAR (60)			//�Q�[���I�[�o�[��̓����F�ւ̕ύX�ɂ����鎞��

//--------------------------------
//���̑�
//--------------------------------
#define COLOR_OUTLINE (D3DXCOLOR(0.2f, 0.5f, 1.0f, 1.0f))	//���f���̗֊s�̐F

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CPlayer::m_nNumParts = 0;
CObjectMotion::PARTS_INFO* CPlayer::m_pPartsInfoArray = nullptr;
CObjectMotion::MOTION_INFO CPlayer::m_aMotionInfo[(int)CPlayer::MOTION_TYPE::ENUM_MAX] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() : CObjectMotion(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, false)
{
	SetObjType(OBJTYPE_PLAYER);	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	SetUpdatePriority(UPDATE_PRIORITY::PLAYER);	//�X�V���̐ݒ�
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//�`�揇�̐ݒ�

	SetColorOutlineAll(COLOR_OUTLINE);	//�֊s�̐F�̐ݒ�
	SetDiffuseModelAll(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0);	//���f���̐F�̐ݒ�
	SetColorGlowAll(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));	//�֊s�̔����F�̐ݒ�

	m_lastPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_destRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAddMoveSpeed = 0.0f;
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bLand = false;
	m_nAddDamage = 0;
	m_bValidAttack = false;
	m_nNumKillEnemy = 0;
	m_nCntLockAct = 0;

	m_nCntGameover = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// �v���C���[�̐�������
//=============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos) {
	CPlayer* pPlayer;
	pPlayer = new CPlayer();
	if (pPlayer == nullptr) return nullptr;

	pPlayer->CObjectMotion::SetPos(pos);
	pPlayer->Init();

	return pPlayer;
}

//=============================================================================
// ���[�V�����������[�h
//=============================================================================
void CPlayer::LoadMotionInfo(void) {
	//���[�h����O�ɓ��I�m�ۂ�����������delete����
	UnloadMotionInfo();

	LoadMotionText(TEXT_FILE_NAME_LOAD_MOTION, m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX);
}

//=============================================================================
// ���[�V���������A�����[�h
//=============================================================================
void CPlayer::UnloadMotionInfo(void) {
	m_nNumParts = 0;

	//�p�[�c���̉��
	if (m_pPartsInfoArray != nullptr) {
		delete[] m_pPartsInfoArray;
		m_pPartsInfoArray = nullptr;
	}
	//���[�V�������̉��
	for (int nCntMotionInfo = 0; nCntMotionInfo < (int)MOTION_TYPE::ENUM_MAX; nCntMotionInfo++)
	{
		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
		{
			if (m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray != nullptr) {
				delete[] m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray;
				m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray = nullptr;
			}
		}
	}
}

//=============================================================================
// �v���C���[�̏���������
//=============================================================================
HRESULT CPlayer::Init(void) {
	//�v���C���[�̏����ݒ�
	CObjectMotion::SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	m_destRot.y =  D3DX_PI;	//������

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	CObjectMotion::Init();
	SetMotion((int)MOTION_TYPE::NEUTRAL);	//���[�V�����̏����ݒ�

	return S_OK;
}

//=============================================================================
// �v���C���[�̏I������
//=============================================================================
void CPlayer::Uninit(void) {
	//�I������
	CObjectMotion::Uninit();
}

//=============================================================================
// �v���C���[�̍X�V����
//=============================================================================
void CPlayer::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	CInput* pInput = nullptr;
	CSound *pSound = nullptr;
	CCamera* pCamera = nullptr;
	CGameScene* pGame = nullptr;

	if (pManager != nullptr) {
		//���݂̓��̓f�o�C�X�̎擾
		pInput = pManager->GetInputCur();
		//�T�E���h�̎擾
		pSound = pManager->GetSound();
		//�J�����̎擾
		pCamera = pManager->GetCamera();
		//�Q�[���V�[���̎擾
		pGame = pManager->GetGameScene();
	}

	//�ʒu���̃|�C���^�̎擾
	D3DXVECTOR3* pPosPlayer = GetPtrPos();

	
	//----------------------------
	//�Q�[���I�[�o�[��
	//----------------------------
	if (pGame != nullptr) {
		//�Q�[���N���A��
		if (pGame->GetGameClear()) {
			//�U�������[�V�����I����
			if (GetEndMotion()) {
				EndMotion();
			}
			//���[�V�����̃A�b�v�f�[�g
			CObjectMotion::Update();
			return;
		}
		//�Q�[���I�[�o�[��
		if (pGame->GetGameOver()) {
			SetMotion((int)MOTION_TYPE::NEUTRAL);	//�j���[�g�������瓮���Ȃ��悤��
			//���[�V�����̃A�b�v�f�[�g
			CObjectMotion::Update();	//���f���̍X�V���K�v�Ȃ���

			//���ׂďI�������X�V�Ȃ�
			if (m_nCntGameover >= START_CHANGE_COLOR_CLEAR + FINISH_TIME_CLEAR) return;

			//�J�E���^�[�̉��Z
			m_nCntGameover++;

			//�����̕ύX���J�n
			if (m_nCntGameover == START_CHANGE_COLOR_CLEAR) {
				//�v���C���[�̐F�𓧖��ɂ���
				StartChangeDiffuseAll(0, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), FINISH_TIME_CLEAR);
			}
			return;
		}
	}
	//�Q�[���ȊO
	else {
		//���[�V�����̃A�b�v�f�[�g
		CObjectMotion::Update();
		return;
	}

	//----------------------------
	//�J�����̐ݒ�
	//----------------------------
	float fRotCameraY = 0.0f;	//�J�����̊p�x
	if (pCamera != nullptr) {
		fRotCameraY = pCamera->GetRot().y;	//�J�����̊p�x���擾
	}

	//----------------------------
	//�d����
	//----------------------------
	if (m_nCntLockAct > 0) {
		m_nCntLockAct--;
	}

	//----------------------------
	//�U��
	//----------------------------
	if (pInput != nullptr) {
		if (GetMotionCategory() != MOTION_CATEGORY::ATTACK && m_nCntLockAct <= 0) {
			StartAttack();
		}
	}

	//----------------------------
	//�ړ�
	//----------------------------
	//����
	if (!m_bLand) {
		m_move.y -= POWER_GRAVITY;	//�d�͂����Z
	}

	//�ړ��Ɖ�]
	if (pInput != nullptr && GetMotionCategory() != MOTION_CATEGORY::ATTACK && m_nCntLockAct <= 0) {
		Move(pInput, fRotCameraY);
	}

	//����
	DecMove();

	//----------------------------
	//�ړ��̔��f
	//----------------------------
	//�Ō�̈ʒu���W�̕ۑ�
	m_lastPos = *pPosPlayer;
	//�ړ�
	*pPosPlayer += m_move;

	//----------------------------
	//�����蔻��
	//----------------------------
	Collision(*pPosPlayer);

	//----------------------------
	//���[�V�����̍X�V
	//----------------------------
	CObjectMotion::Update();

	//----------------------------
	//���[�V�����I����
	//----------------------------
	if (GetEndMotion()) {
		EndMotion();
	}

	//----------------------------
	//���[�V�������̏���
	//----------------------------
	MotionAction();

	//----------------------------
	//�U���̃^�C�~���O
	//----------------------------
	if (m_bValidAttack) {
		//�v���C���[�̑S���f���̃��[���h�}�g���b�N�X���X�V
		UpdateMtxWorldAll();	//�����蔻��擾���Ɉʒu�̃Y���������邽��

		switch ((MOTION_TYPE)GetMotionType())
		{
		case MOTION_TYPE::PUNCH:
			AttackCollision(2, 4, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-50.0f, 0.0f, 0.0f), ATTACK_DAMAGE_PUNCH + m_nAddDamage);
			AttackCollision(4, 4, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 0.0f), ATTACK_DAMAGE_PUNCH + m_nAddDamage);
			break;

		case MOTION_TYPE::STAMP:
			AttackCollision(6, 3, 50.0f, D3DXVECTOR3(0.0f, 0.0f, -30.0f), D3DXVECTOR3(0.0f, -30.0f, 0.0f), ATTACK_DAMAGE_STANP + m_nAddDamage);
			AttackCollision(8, 3, 50.0f, D3DXVECTOR3(0.0f, 0.0f, -30.0f), D3DXVECTOR3(0.0f, -30.0f, 0.0f), ATTACK_DAMAGE_STANP + m_nAddDamage);
			break;
		}
	}
}

//=============================================================================
// �v���C���[�̕`�揈��
//=============================================================================
void CPlayer::Draw(void) {
	if (m_nCntGameover >= START_CHANGE_COLOR_CLEAR + FINISH_TIME_CLEAR) return;	//�t�F�[�h�������͕`�悵�Ȃ� �e�ⓧ�߂̉e�����Ȃ�������

	CObjectMotion::Draw();
}


//=============================================================================
// �v���C���[�̃Q�[���N���A���̏���
//=============================================================================
void CPlayer::GameClear(void) {
	//�j���[�g�������[�V�����̐ݒ�
	if(GetMotionCategory() != MOTION_CATEGORY::ATTACK) SetMotion((int)MOTION_TYPE::NEUTRAL);

	//�ړ��ʂ��d�݂͂̂ɂ���
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_nCntLockAct = 0;
}

//=============================================================================
// �v���C���[�̃Q�[���I�[�o�[���̏���
//=============================================================================
void CPlayer::GameOver(void) {
	//�ړ��ʂ��d�݂͂̂ɂ���
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_nCntLockAct = 0;

	//�v���C���[�̐F����������
	StartChangeDiffuseAll(0, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), FINISH_TIME_CHANGE_COLOR_GAMEOVER);

	//�p�[�e�B�N���G�t�F�N�g�̐���
	CParticleEffect::PARTICLE_INFO particleInfo;	//�p�[�e�B�N�����
	particleInfo.addMove = D3DXVECTOR3(0.0f, 0.08f, 0.0f); particleInfo.colEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); particleInfo.colStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	particleInfo.fAddSize = -1.0f; particleInfo.fSizeStart = 60.0f; particleInfo.fSpeedMove = 3.5f; particleInfo.nLife = 60;
	//�G�t�F�N�g�𐶐�
	CParticleEffect::Create(particleInfo, GetPos(), FINISH_TIME_CHANGE_COLOR_GAMEOVER, 1, 0.45f * D3DX_PI, true);
}

//=============================================================================
// �v���C���[�̓����蔻��̏��̎擾
//=============================================================================
void CPlayer::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//�����蔻��̐���ݒ�
	if (pNumCol != nullptr) *pNumCol = NUM_COLLISION;
	//�����蔻��̔��a��ݒ�
	if (pRadiusCol != nullptr) *pRadiusCol = COLLISION_RADIUS;
	//�����蔻��̈ʒu�̔z��̃|�C���^��null�̏ꍇ�I��
	if (ppPosColArray == nullptr) {
		return;
	}
	//pos�̃|�C���^����ł͂Ȃ��ꍇ�I��
	if (*ppPosColArray != nullptr) {
		return;
	}

	//pos�̕ϐ��𓖂��蔻��̐����z��Ŋm��
	*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];

	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���f����pos����̃I�t�Z�b�g
	int nIdxModel = 0;	//����

	//�����蔻��̈ʒu�̎擾
	for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
	{
		GetPosCollision(*ppPosColArray + nCntCol, posOffset, nIdxModel);
		posOffset += D3DXVECTOR3(0.0f, 5.0f, 0.0f);	//�I�t�Z�b�g�����Z
	}
}

//=============================================================================
// �v���C���[�̍Ō�̈ʒu���W�̎擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetLastPos(void) {
	return m_lastPos;
}

//=============================================================================
// �v���C���[�̈ړ��ʂ̐ݒ�
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move) { m_move = move; }

//=============================================================================
// �v���C���[�̈ړ��ʂ̎擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void) { return m_move; }

//=============================================================================
// �v���C���[�̓����蔻��̔��a�̎擾
//=============================================================================
float CPlayer::GetRadius(void) {
	return COLLISION_RADIUS;
}

//=============================================================================
// �U���J�n
//=============================================================================
void CPlayer::StartAttack(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//���̓f�o�C�X�̎擾
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;

	//�p���`
	if (pInput->GetPress(CInput::CODE::ATTACK_1)) {
		//���[�V�����̐ݒ�
		SetMotion((int)MOTION_TYPE::PUNCH);
	}
	//���݂�
	else if (pInput->GetPress(CInput::CODE::ATTACK_2)) {
		//���[�V�����̐ݒ�
		SetMotion((int)MOTION_TYPE::STAMP);
	}
}

//=============================================================================
// �ړ�
//=============================================================================
void CPlayer::Move(CInput* pInput, float fRotCameraY) {
	if (pInput == nullptr) return;

	//�㉺���E�L�[���͏�Ԃ̎擾
	const bool bPressUp = pInput->GetPress(CInput::CODE::MOVE_UP);
	const bool bPressDown = pInput->GetPress(CInput::CODE::MOVE_DOWN);
	const bool bPressLeft = pInput->GetPress(CInput::CODE::MOVE_LEFT);
	const bool bPressRight = pInput->GetPress(CInput::CODE::MOVE_RIGHT);

	bool bDiagonalMove = (bPressUp != bPressDown) && (bPressLeft != bPressRight);	//�΂߈ړ�
	bool bRotateUp, bRotateDown, bRotateLeft, bRotateRight;	//��]�������
	bRotateUp = bRotateDown = bRotateLeft = bRotateRight = false;

	D3DXVECTOR3 moveAddSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ǉ�����ړ���
	D3DXVECTOR3 moveMaxSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ��ʂ̍ő�

	//------------------------
	//�ړ����x�̍ő���v�Z
	//------------------------
	float fMaxSpeed;	//�ړ����x�̍ő�
	//�n��
	//if (pInput->GetPress(CInput::CODE::DASH)) {
	//	fMaxSpeed = MAX_MOVE_SPEED_DASH;
	//}
	//else {
		fMaxSpeed = MAX_MOVE_SPEED_WALK;
	//}
	//��
	if (!m_bLand) {
		fMaxSpeed = MAX_MOVE_SPEED_AIR;
	}
	//�ړ����x�̉��Z
	fMaxSpeed += m_fAddMoveSpeed;

	//------------------------
	//�ړ��ʂ̌���
	//------------------------
	//�O��ړ��̂ǂ��炩�̂݉�����Ă���ꍇ
	if (bPressUp != bPressDown) {
		if (bPressUp) {
			moveAddSpeed.z = ADD_MOVE_SPEED;
			moveMaxSpeed.z = fMaxSpeed;
			bRotateUp = true;
		}
		else if (bPressDown) {
			moveAddSpeed.z = -ADD_MOVE_SPEED;
			moveMaxSpeed.z = -fMaxSpeed;
			bRotateDown = true;
		}
		//�΂߈ړ�
		if (bDiagonalMove) {
			moveAddSpeed.z *= 0.7f;
			moveMaxSpeed.z *= 0.7f;
		}
	}
	//���E�ړ��̂ǂ��炩�̂݉�����Ă���ꍇ
	if (bPressLeft != bPressRight) {
		if (bPressLeft) {
			moveAddSpeed.x = -ADD_MOVE_SPEED;
			moveMaxSpeed.x = -fMaxSpeed;
			bRotateLeft = true;
		}
		else if (bPressRight) {
			moveAddSpeed.x = ADD_MOVE_SPEED;
			moveMaxSpeed.x = fMaxSpeed;
			bRotateRight = true;
		}
		//�΂߈ړ�
		if (bDiagonalMove) {
			moveAddSpeed.x *= 0.7f;
			moveMaxSpeed.x *= 0.7f;
		}
	}

	//------------------------
	//�J�����̊p�x�ɍ��킹�Ĉړ��ʂ̍ő�ʂ�ݒ�
	//------------------------
	D3DXVECTOR3 moveMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ��̍ő��
	moveMax.x += moveMaxSpeed.x * sinf(fRotCameraY + 0.5f * D3DX_PI);
	moveMax.x += moveMaxSpeed.z * sinf(fRotCameraY);

	moveMax.z += moveMaxSpeed.x * -sinf(fRotCameraY);
	moveMax.z += moveMaxSpeed.z * cosf(fRotCameraY);

	//------------------------
	//����
	//------------------------
	//X�����̈ړ��ʂ��ő�ʖ����̏ꍇ
	if ((moveMax.x > 0 && m_move.x < moveMax.x) || (moveMax.x < 0 && m_move.x > moveMax.x)) {
		//�J�����̊p�x�ɍ��킹�Ĉړ��ʂ�ǉ�
		m_move.x += moveAddSpeed.x * sinf(fRotCameraY + 0.5f * D3DX_PI);
		m_move.x += moveAddSpeed.z * sinf(fRotCameraY);
		//�������Ɉړ��ʂ��ő�𒴂����ꍇ
		if (moveMax.x > 0) {
			if (m_move.x > moveMax.x) {
				m_move.x = moveMax.x;
			}
		}
		else if (moveMax.x < 0) {
			if (m_move.x < moveMax.x) {
				m_move.x = moveMax.x;
			}
		}
	}
	//Z�����̈ړ��ʂ��ő�ʖ����̏ꍇ
	if ((moveMax.z > 0 && m_move.z < moveMax.z) || (moveMax.z < 0 && m_move.z > moveMax.z)) {
		//�J�����̊p�x�ɍ��킹�Ĉړ��ʂ�ǉ�
		m_move.z += moveAddSpeed.x * -sinf(fRotCameraY);
		m_move.z += moveAddSpeed.z * cosf(fRotCameraY);
		//�������Ɉړ��ʂ��ő�𒴂����ꍇ
		if (moveMax.z > 0) {
			if (m_move.z > moveMax.z) {
				m_move.z = moveMax.z;
			}
		}
		else if (moveMax.z < 0) {
			if (m_move.z < moveMax.z) {
				m_move.z = moveMax.z;
			}
		}
	}

	//------------------------
	//��]�����̌���
	//------------------------		
	bool bInputMove = false;	//�ړ��L�[�������Ă��邩�ǂ���
	bInputMove = bRotateUp || bRotateDown || bRotateLeft || bRotateRight;	//�ǂꂩ��������Ă���ꍇ
	//�ړ��L�[������
	if (bInputMove) {
		//�L�[�������Ă���������ڕW�̊p�x�ɂ���
		//��
		if (bRotateUp) {
			if (bRotateRight) {
				m_destRot.y = D3DX_PI * -0.75f + fRotCameraY;
			}
			else if (bRotateLeft) {
				m_destRot.y = D3DX_PI * 0.75f + fRotCameraY;
			}
			else {
				m_destRot.y = D3DX_PI + fRotCameraY;
			}
		}
		//��
		else if (bRotateDown) {
			if (bRotateRight) {
				m_destRot.y = D3DX_PI * -0.25f + fRotCameraY;
			}
			else if (bRotateLeft) {
				m_destRot.y = D3DX_PI * 0.25f + fRotCameraY;
			}
			else {
				m_destRot.y = 0.0f + fRotCameraY;
			}
		}
		//���E
		else {
			if (bRotateRight) {
				m_destRot.y = D3DX_PI * -0.5f + fRotCameraY;
			}
			else if (bRotateLeft) {
				m_destRot.y = D3DX_PI * 0.5f + fRotCameraY;
			}
		}

		//�p�C���ߎ�
		if (m_destRot.y > D3DX_PI) {
			m_destRot.y = -D3DX_PI * 2 + m_destRot.y;
		}
		else if (m_destRot.y < -D3DX_PI) {
			m_destRot.y = D3DX_PI * 2 + m_destRot.y;
		}

		D3DXVECTOR3 rotPlayer = CObjectMotion::GetRot();//�p�x�̎擾
		float fdeltaRot;	//�p�x�̍���
		//���݂̊p�x�ƖڕW�̊p�x�̍����̌v�Z
		if (m_destRot.y >= 0.0f) {
			if (rotPlayer.y >= 0.0f) {
				fdeltaRot = m_destRot.y - rotPlayer.y;
			}
			else if (rotPlayer.y < 0.0f) {
				if (m_destRot.y - rotPlayer.y >= D3DX_PI) {
					fdeltaRot = -D3DX_PI - rotPlayer.y - D3DX_PI + m_destRot.y;
				}
				else if (m_destRot.y - rotPlayer.y < D3DX_PI) {
					fdeltaRot = m_destRot.y - rotPlayer.y;
				}
			}
		}
		else if (m_destRot.y < 0.0f) {
			if (rotPlayer.y >= 0.0f) {
				if (rotPlayer.y - m_destRot.y >= D3DX_PI) {
					fdeltaRot = D3DX_PI - rotPlayer.y + D3DX_PI + m_destRot.y;
				}
				else if (rotPlayer.y - m_destRot.y < D3DX_PI) {
					fdeltaRot = m_destRot.y - rotPlayer.y;
				}
			}
			else if (rotPlayer.y < 0.0f) {
				fdeltaRot = m_destRot.y - rotPlayer.y;
			}
		}

		//��]�̔��f
		rotPlayer.y += fdeltaRot * ROTATE_SPEED;

		//�p�C���ߎ�
		if (rotPlayer.y > D3DX_PI) {
			rotPlayer.y = -D3DX_PI;
		}
		else if (rotPlayer.y < -D3DX_PI) {
			rotPlayer.y = D3DX_PI;
		}

		//�p�x�̐ݒ�
		CObjectMotion::SetRot(rotPlayer);
	}

	//------------------------
	//�n��ɂ��鎞�̃��[�V�����؂�ւ�
	//------------------------
	if (m_bLand && GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
		//�ړ��L�[�������Ă��鎞�̃��[�V�����؂�ւ�
		if (bInputMove) {
			if (GetMotionCategory() != MOTION_CATEGORY::MOVE) {
				SetMotion((int)MOTION_TYPE::WALK);
			}
		}
		//�ړ��L�[�������Ă��Ȃ��ꍇ�̃��[�V�����؂�ւ�
		if (!bInputMove) {
			//���[�V�����̐ݒ�
			if (GetMotionCategory() == MOTION_CATEGORY::MOVE) {
				SetMotion((int)MOTION_TYPE::NEUTRAL);
			}
		}
	}
}

//=============================================================================
// �ړ��ʂ̌���
//=============================================================================
void CPlayer::DecMove(void) {
	float fDecSpeed = DEC_MOVE_SPEED_GROUND;	//������
	if (!m_bLand) fDecSpeed = DEC_MOVE_SPEED_AIR;	//��
	D3DXVECTOR2 vecMoveDec = D3DXVECTOR2(-m_move.x, -m_move.z);	//�ړ��ʃx�N�g���̋t
	D3DXVec2Normalize(&vecMoveDec, &vecMoveDec);	//���K��
	vecMoveDec *= fDecSpeed;	//�ړ��ʂ̉��Z

	//����
	m_move.x += vecMoveDec.x;
	m_move.z += vecMoveDec.y;

	//X�����̈ړ��ʂ̒�~
	if (vecMoveDec.x > 0.0f) {
		if (m_move.x > 0.0f) {
			m_move.x = 0.0f;
		}
	}
	else if (vecMoveDec.x < 0.0f) {
		if (m_move.x < 0.0f) {
			m_move.x = 0.0f;
		}
	}
	//Z�����̈ړ��ʂ̒�~
	if (vecMoveDec.y > 0.0f) {
		if (m_move.z > 0.0f) {
			m_move.z = 0.0f;
		}
	}
	else if (vecMoveDec.x < 0.0f) {
		if (m_move.z < 0.0f) {
			m_move.z = 0.0f;
		}
	}
}

//=============================================================================
// �����蔻��
//=============================================================================
void CPlayer::Collision(D3DXVECTOR3& pos) {
	//�ǂƂ̓����蔻��
	CWallCylinder::Collision(&pos, m_lastPos, COLLISION_RADIUS);

	//�n�ʂƂ̓����蔻��
	bool bLand = false;	//�ڒn���Ă��邩�ǂ���
	D3DXVECTOR3 posColTerrain = pos;	//�ڐG�ʒu�A�J�n�x�N�g���A�I���x�N�g��

	bLand = CTerrain::Collision(posColTerrain);

	//�ڒn��
	if (bLand) {
		m_move.y = -POWER_GRAVITY_GROUND;

		pos = posColTerrain;	//�ʒu�̈ړ�
		//���n
		if (!m_bLand) {
			m_bLand = true;
		}
	}
	//����
	else if (!bLand) {
		//�n�ォ���
		if (m_bLand) {
			m_bLand = false;
		}
	}
}

//=============================================================================
// ���[�V�����I����
//=============================================================================
void CPlayer::EndMotion(void) {

	//�d������̃��[�V����
	if (GetMotionType() == (int)MOTION_TYPE::PUNCH) {
		m_nCntLockAct = 25;
	}
	else if (GetMotionType() == (int)MOTION_TYPE::STAMP) {
		m_nCntLockAct = 35;
	}

	//�j���[�g�������[�V������ݒ�
	SetMotion((int)MOTION_TYPE::NEUTRAL);
	//�ړ��ʂ��d�݂͂̂ɂ���
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);

	m_bValidAttack = false;
}

//=============================================================================
// ���[�V�������̏���
//=============================================================================
void CPlayer::MotionAction(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	bool bBeginAttack = false;	//�U���J�n�����ǂ���

	CGameScene* pGameScene = nullptr;
	if(pManager != nullptr) pGameScene = pManager->GetGameScene();

	CAppleTree* pAppleTree = nullptr;
	if (pGameScene != nullptr) pAppleTree = pGameScene->GetAppleTree();

	int nNumShockwave = 0;
	int nNumThunder = 0;

	if (pAppleTree != nullptr) {
		nNumShockwave = pAppleTree->CAppleTree::GetNumApple(CGlowApple::APPLE_TYPE::WHITE);
		nNumThunder = pAppleTree->CAppleTree::GetNumApple(CGlowApple::APPLE_TYPE::SILVER);
		nNumThunder *= 2;
	}

	switch ((MOTION_TYPE)GetMotionType())
	{

		//-------------------------
		//�ړ�
		//-------------------------
	case MOTION_TYPE::WALK:
		//�����n�ʂɐG�ꂽ�^�C�~���O�������ꍇ
		if ((GetCurKey() == 0 && GetCurMotionCnt() == 14 && !GetTransMotion()) || (GetCurKey() == 2 && GetCurMotionCnt() == 14)) {
			//���s���̉�
			if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::FOOTSTEP);

			//�������ɃG�t�F�N�g�𐶐�
			CParticleEffect::PARTICLE_INFO particleInfo;	//�p�[�e�B�N�����
			particleInfo.addMove = D3DXVECTOR3(0.0f, -0.5f, 0.0f); particleInfo.colEnd = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f); particleInfo.colStart = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
			particleInfo.fAddSize = -1.0f; particleInfo.fSizeStart = 30.0f; particleInfo.fSpeedMove = 6.5f; particleInfo.nLife = 30;

			D3DXVECTOR3 posFoot;//�������Ă�����̑��̃G�t�F�N�g�̈ʒu
			int nIdxModel = 0;	//���̃C���f�b�N�X

			if (GetCurKey() == 0) {
				nIdxModel = 6;	//�E��
			}
			else if (GetCurKey() == 2) {
				nIdxModel = 8;	//����
			}
			//���̈ʒu�̎擾
			GetPosCollision(&posFoot, D3DXVECTOR3(0.0f, -85.0f, 5.0f), nIdxModel);
			//�P�񕪂̃G�t�F�N�g�𓯎��ɏd�˂ĕ\��
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				CParticleEffect::Create(particleInfo, posFoot, 2, 1, 0.08f * D3DX_PI, true);
			}

			//�Ռ��g����
			//CShockWaveEmitter::Create(m_nNumShockWave, 4, posFoot, 30.0f, 2.0f, 4.0f, 40.0f, -1.0f, 15, D3DX_PI * 0.02f);
		
		}
		break;

		//-------------------------
		//�p���`�̍U��
		//-------------------------
		//�p���`
	case MOTION_TYPE::PUNCH:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurKey() == 1 || (GetCurKey() == 2 && GetCurMotionCnt() == 0)) && !GetTransMotion()) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
		}
		else {
			m_bValidAttack = false;
		}
		//�U�����n�ʂɓ��������^�C�~���O�������ꍇ
		if (GetCurKey() == 1 && GetCurMotionCnt() == 5) {
			//�Ռ���
			if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SHOCK_PUNCH);

			//�Ռ��g����
			D3DXVECTOR3 posWave = GetPos() + D3DXVECTOR3(sinf(GetRot().y + D3DX_PI) * 200.0f, -5.0f, cosf(GetRot().y + D3DX_PI) * 200.0f);	//�v���C���[�̑O��
			CShockWaveEmitter* pSWEmitter = CShockWaveEmitter::Create(nNumShockwave + 1, 4, posWave, 40.0f, 0.0f, 22.0f, 90.0f, -1.0f, 13, D3DX_PI * 0.02f);
			//���������Ƃɑ����F�̐ݒ�
			if (pSWEmitter != nullptr) pSWEmitter->SetColAddCreate(D3DXCOLOR(-0.15f, -0.15f, 0.0f, 0.0f));

			//���̐���
			if (nNumThunder > 0) CThunderEmitter::CreateStraight(nNumThunder + 1, 5, posWave, posWave - GetPos(), 100.0f);
		}
		break;

		//���݂�
	case MOTION_TYPE::STAMP:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurKey() == 1 || (GetCurKey() == 2 && GetCurMotionCnt() == 0)) && !GetTransMotion()) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
		}
		else {
			m_bValidAttack = false;
		}
		//�U�����n�ʂɓ��������^�C�~���O�������ꍇ
		if (GetCurKey() == 1 && GetCurMotionCnt() == 3) {
			//�Ռ���
			if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SHOCK_STAMP);

			//�Ռ��g����
			D3DXVECTOR3 posWave = GetPos() + D3DXVECTOR3(0.0f, -5.0f, 0.0f);
			CShockWaveEmitter* pSWEmitter = CShockWaveEmitter::Create(nNumShockwave + 2, 4, posWave, 50.0f, 0.0f, 30.0f, 110.0f, -1.0f, 15, D3DX_PI * 0.02f);
			//���������Ƃɑ����F�̐ݒ�
			if (pSWEmitter != nullptr) pSWEmitter->SetColAddCreate(D3DXCOLOR(0.0f, -0.05f, -0.15f, 0.0f));

			//���̐���
			if(nNumThunder > 0) CThunderEmitter::CreateRound(nNumThunder + 1, 3, posWave, GetRot().y + D3DX_PI, 400.0f);
		}
		break;

	default:
		m_bValidAttack = false;
		break;
	}

	//�U���J�n��
	if (bBeginAttack) {
		AttackBegin();
	}
}

//=============================================================================
// �U���̓����蔻��
//=============================================================================
void CPlayer::AttackCollision(int nIdxModel, const int nNumCol, float fRadiusCol, D3DXVECTOR3 posOffsetStart, D3DXVECTOR3 posOffsetAdd, int nDamage) {
	D3DXVECTOR3* pPosColArray = new D3DXVECTOR3[nNumCol];	//�����蔻��̈ʒu�𐶐�
	int nNumKillEnemy = 0;	//�G��|������

	D3DXVECTOR3 posOffset = posOffsetStart;	//���f������̃I�t�Z�b�g

	for (int nCntCol = 0; nCntCol < nNumCol; nCntCol++)
	{
		//�����蔻��̈ʒu�̎擾
		GetPosCollision(&pPosColArray[nCntCol], posOffset, nIdxModel);
		//�I�t�Z�b�g�����Z
		posOffset += posOffsetAdd;
	}

	for (int nCntCol = 0; nCntCol < nNumCol; nCntCol++)
	{
		//�U��
		CObjectMotion::Attack(OBJTYPE_ENEMY | OBJTYPE_APPLETREE | OBJTYPE_SCAPEGOAT, pPosColArray[nCntCol], fRadiusCol, nDamage, DAMAGE_TYPE::PLAYER_PUNCH, &nNumKillEnemy);	//�G
	}

	//�U���̐�[�Ƀp�[�e�B�N���̐���
	CParticle::Create(pPosColArray[nNumCol - 1], 20, 55.0f, -1.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	delete[] pPosColArray;	//���I�m�ۂ��������蔻��̈ʒu�̃����������

	//�G��|�������̉��Z
	m_nNumKillEnemy += nNumKillEnemy;
}

//=============================================================================
// �U���J�n��
//=============================================================================
void CPlayer::AttackBegin(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//�U���ς݃��X�g�̏�����
	InitObjAttacked();	

	switch ((MOTION_TYPE)GetMotionType())
	{
	case MOTION_TYPE::PUNCH:
	case MOTION_TYPE::STAMP:
		//����U�鉹�̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_PUNCH);
		break;
	}
}

//=============================================================================
// ���[�V�����̋敪�̎擾
//=============================================================================
CPlayer::MOTION_CATEGORY CPlayer::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�ړ�
	case MOTION_TYPE::WALK:
		return MOTION_CATEGORY::MOVE;
		break;

		//�U��
	case MOTION_TYPE::PUNCH:
	case MOTION_TYPE::STAMP:
		return MOTION_CATEGORY::ATTACK;
			break;

		//���̑�
	default:
		return MOTION_CATEGORY::NONE;
		break;
	}
}