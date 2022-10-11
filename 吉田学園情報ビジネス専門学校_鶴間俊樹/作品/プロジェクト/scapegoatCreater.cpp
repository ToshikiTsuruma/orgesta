//=============================================================================
//
// ���ѐ������� [scapegoatCreater.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "scapegoatCreater.h"
#include "manager.h"
#include "input.h"
#include "gameScene.h"
#include "player.h"
#include "seedling.h"
#include "score.h"
#include "billboard.h"
#include "object2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_CREATE_SCAPEGOAT_DISTANCE (200.0f);	//���т𐶐��\�ȍő�̋���
#define DEFAULT_NUM_KILL_ENEMY (50)	//�ŏ����玝���Ă�G�̐�

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CScapegoatCreater::CScapegoatCreater()
{
	m_nNumKillEnemy = 0;

	m_pNumKillEnemyUI = CScore::Create(4, CTexture::TEXTURE_TYPE::NUMBER_002, D3DXVECTOR3(SCREEN_WIDTH - 20.0f, SCREEN_HEIGHT - 30.0f, 0.0f), 20.0f);
	m_pNumUseEnemyUI = nullptr;
	m_pInteractUI = nullptr;
	m_pNixSignUI = nullptr;
	m_pTextEnergy = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 130.0f, SCREEN_HEIGHT - 30.0f + 10.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_ENERGY, 80.0f, 20.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScapegoatCreater::~CScapegoatCreater()
{

}

//=============================================================================
// ���ѐ����N���X�̐�������
//=============================================================================
CScapegoatCreater* CScapegoatCreater::Create(void) {
	CScapegoatCreater* pScapegoatCreater;
	pScapegoatCreater = new CScapegoatCreater();
	if (pScapegoatCreater == nullptr) return nullptr;

	pScapegoatCreater->Init();

	return pScapegoatCreater;
}

//=============================================================================
// ���ѐ����N���X�̏���������
//=============================================================================
HRESULT CScapegoatCreater::Init(void) {
	m_nNumKillEnemy = DEFAULT_NUM_KILL_ENEMY;
	if (m_pNumKillEnemyUI != nullptr) m_pNumKillEnemyUI->SetScore(m_nNumKillEnemy);

	return S_OK;
}

//=============================================================================
// ���ѐ����N���X�̏I������
//=============================================================================
void CScapegoatCreater::Uninit(void) {
	//�G��|��������UI�̔j��
	if (m_pNumKillEnemyUI != nullptr) {
		m_pNumKillEnemyUI->Uninit();
		m_pNumKillEnemyUI = nullptr;
	}
	//�G���g�p���鐔��UI�̔j��
	if (m_pNumUseEnemyUI != nullptr) {
		m_pNumUseEnemyUI->Uninit();
		m_pNumUseEnemyUI = nullptr;
	}
	//�C���^���N�g�\���̃r���{�[�h�̔j��
	if (m_pInteractUI != nullptr) {
		m_pInteractUI->Uninit();
		m_pInteractUI = nullptr;
	}
	//�֎~�}�[�N�̃r���{�[�h�̔j��
	if (m_pNixSignUI != nullptr) {
		m_pNixSignUI->Uninit();
		m_pNixSignUI = nullptr;
	}
	//�G�l���M�[�e�L�X�g�̔j��
	if (m_pTextEnergy != nullptr) {
		m_pTextEnergy->Uninit();
		m_pTextEnergy = nullptr;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// ���ѐ����N���X�̍X�V����
//=============================================================================
void CScapegoatCreater::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	CInput* pInput = nullptr;
	CGameScene* pGame = nullptr;
	CPlayer* pPlayer = nullptr;

	//���݂̓��̓f�o�C�X�̎擾
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//�Q�[���V�[���̎擾
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//�v���C���[�̎擾
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	D3DXVECTOR3 posPlayer;	//�v���C���[�̈ʒu
	if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();
	float fDistSeedling;	//�v���C���[�ƕc�؂̋���

	D3DXVECTOR3 posSeedling;	//�c�؂̈ʒu
	//��ԋ߂��c�؂��擾
	CSeedling* pNearSeedling = dynamic_cast<CSeedling*>(GetNearObject(posPlayer, OBJTYPE_SEEDLING, 0, &posSeedling, &fDistSeedling, nullptr));
	if (pNearSeedling == nullptr) return;

	int nNumNeedEnemy;	//�c�؂��琶�т𐶂ݏo�����ɕK�v�ȓG�̗�
	if (pNearSeedling != nullptr) {
		nNumNeedEnemy = pNearSeedling->GetNumNeedEnemy();
	}

	bool bDistCreate = fDistSeedling < MAX_CREATE_SCAPEGOAT_DISTANCE;	//�����\�ȋ������ǂ���
	bool bCanCreate = m_nNumKillEnemy >= nNumNeedEnemy;	//�����\�ȗʂ̓G���������Ă��邩�ǂ���

	bool bEnableXInput = false;
	//���݂̓��̓f�o�C�X�̎�ނ��擾
	if (pManager != nullptr) {
		bEnableXInput = pManager->GetEnableXInput();
	}

	//�A�C�R���e�N�X�`���̐ݒ�
	CTexture::TEXTURE_TYPE typeTex = CTexture::TEXTURE_TYPE::ICON_INTERACT_KEYBOARD;	//�L�[�{�[�h�̃C���^���N�g�L�[�̃e�N�X�`��
	if (bEnableXInput) {
		typeTex = CTexture::TEXTURE_TYPE::ICON_INTERACT_GAMEPAD;	//�Q�[���p�b�h�̃C���^���N�g�{�^���̃e�N�X�`��
	}

	//�c�؂̏�̈ʒu�ɑf�ނ̕K�v�ʂ��r���{�[�h�ŕ\��
	if (bDistCreate) {
		//�G���g�p���鐔��UI�̐���
		if (m_pNumUseEnemyUI == nullptr) {
			m_pNumUseEnemyUI = CScore::Create(0, CTexture::TEXTURE_TYPE::NUMBER_006, D3DXVECTOR3(SCREEN_WIDTH - 20.0f, SCREEN_HEIGHT - 55.0f, 0.0f), 20.0f);
		}
		if (m_pNumUseEnemyUI != nullptr) {
			m_pNumUseEnemyUI->SetScore(nNumNeedEnemy);
		}

		//�C���^���N�g�\���r���{�[�h�̐���
		if (m_pInteractUI == nullptr) {
			m_pInteractUI = CBillboard::Create(posSeedling + D3DXVECTOR3(0.0f, 150.0f, 0.0f), typeTex, 40.0f, 40.0f);
		}
		//�r���{�[�h�̍X�V
		if (m_pInteractUI != nullptr) {
			//�e�N�X�`���̍X�V
			m_pInteractUI->SetTexType(typeTex);
		}
		//�����֎~�}�[�N�̐���
		if (!bCanCreate && m_pNixSignUI == nullptr) {
			m_pNixSignUI = CBillboard::Create(posSeedling + D3DXVECTOR3(0.0f, 150.0f, 0.0f), CTexture::TEXTURE_TYPE::ICON_NIX_SIGN, 70.0f, 70.0f);
			//�r���{�[�h�̏����ݒ�
			if (m_pNixSignUI != nullptr) {
				//m_pNixSignUI->SetOffsetPos(D3DXVECTOR3(0.0f, 0.0f, -1.0f));	//������O	�`�揇�I�ɗv��Ȃ�����
			}
		}
		//�֎~�}�[�N�̃r���{�[�h�̔j��
		if (bCanCreate && m_pNixSignUI != nullptr) {
			m_pNixSignUI->Uninit();
			m_pNixSignUI = nullptr;
		}
	}
	else {
		//�G���g�p���鐔��UI�̔j��
		if (m_pNumUseEnemyUI != nullptr) {
			m_pNumUseEnemyUI->Uninit();
			m_pNumUseEnemyUI = nullptr;
		}
		//�C���^���N�g�\���r���{�[�h�̔j��
		if (m_pInteractUI != nullptr) {
			m_pInteractUI->Uninit();
			m_pInteractUI = nullptr;
		}
		//�֎~�}�[�N�̃r���{�[�h�̔j��
		if (m_pNixSignUI != nullptr) {
			m_pNixSignUI->Uninit();
			m_pNixSignUI = nullptr;
		}
	}

	//���͏���
	if (pInput != nullptr) {
		//�C���^���N�g�{�^��������
		if (pInput->GetTrigger(CInput::CODE::INTERACT) && bDistCreate && bCanCreate && pNearSeedling != nullptr) {
			//�c��̑f�ޗʂ̍X�V
			m_nNumKillEnemy -= nNumNeedEnemy;
			if (m_pNumKillEnemyUI != nullptr) m_pNumKillEnemyUI->SetScore(m_nNumKillEnemy);
			//�C���^���N�g�\���r���{�[�h�̔j��	�������Ɉʒu��ݒ肵�Ă��邽�߁A�������ɋ߂��ɕc�؂��������ꍇ�ʒu���X�V����Ȃ�����
			if (m_pInteractUI != nullptr) {
				m_pInteractUI->Uninit();
				m_pInteractUI = nullptr;
			}
			//���т̐���
			pNearSeedling->CreateScapegoat();
			pNearSeedling = nullptr;	//���ѐ������ɔj������邽��
		}
	}
}

//=============================================================================
// �|�����G�̐��𑝂₷
//=============================================================================
void CScapegoatCreater::AddNumKillEnemy(int nNum) {
	m_nNumKillEnemy += nNum;
	if (m_pNumKillEnemyUI != nullptr) m_pNumKillEnemyUI->SetScore(m_nNumKillEnemy);
}