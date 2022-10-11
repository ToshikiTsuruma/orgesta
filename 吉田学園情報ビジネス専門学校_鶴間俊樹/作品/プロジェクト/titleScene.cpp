//=============================================================================
//
// �^�C�g���V�[������ [titleScene.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "titleScene.h"
#include "manager.h"
#include "renderer.h"
#include "titleCamera.h"
#include "sound.h"
#include "input.h"
#include "fade.h"
#include "object2D.h"
#include "selectMenu2D.h"
#include "objectModel.h"
#include "glowApple.h"

#include "meshwall.h"
#include "player.h"
#include "appleTree.h"
//=============================================================================
// �}�N����`
//=============================================================================
#define FOG_COLOR (D3DXCOLOR(0.9f, 0.5f, 0.0f, 1.0f))	//�t�H�O�̐F
#define TEXT_FILE_NAME_APPLETYPE "data/TEXT/save_appletype.txt"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTitleScene::CTitleScene()
{
	m_pSelectMenuTitle = nullptr;

	m_pTutorial = nullptr;
	m_pCreadit = nullptr;
	m_bBeginFade = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitleScene::~CTitleScene()
{

}

//=============================================================================
// �^�C�g���V�[���̏���������
//=============================================================================
void CTitleScene::Init(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//�J�����̐ݒ�
	if (pManager != nullptr) pManager->SetCamera(CTitleCamera::Create());

	//------------------------------
	//���C�g�̏����ݒ�
	//------------------------------
	D3DXMATRIX mtxLightProj;   // ���C�g�̎ˉe�ϊ�
	//���C�g�̃v���W�F�N�V�����}�g���b�N�X�𐶐�
	D3DXMatrixPerspectiveFovLH(&mtxLightProj, D3DXToRadian(45.0f), 1.0f, 200.0f, 1800.0f);

	D3DXMATRIX mtxLightView;   // ���C�g�r���[�ϊ�
	D3DXVECTOR3 posLight = D3DXVECTOR3(0.0f, 1200.0f, -1000.0f);	//���C�g�̈ʒu
	D3DXVECTOR3 vecLight;	//���C�g�̃x�N�g��
	//���K������
	D3DXVec3Normalize(&vecLight, &posLight);
	//�t�����ɂ���
	vecLight *= -1;
	//�n��̈ʒu�ɍ��킹��
	posLight.y += APPLETREE_POS_Y;

	//���C�g�̃r���[�}�g���b�N�X�𐶐�
	D3DXMatrixLookAtLH(&mtxLightView, &posLight, &D3DXVECTOR3(posLight + vecLight), &D3DXVECTOR3(0, 1, 0));
	//�V�F�[�_�̃��C�g��ݒ�
	if (pRenderer != nullptr) {
		pRenderer->SetEffectLightMatrixView(mtxLightView);
		pRenderer->SetEffectLightVector(D3DXVECTOR4(vecLight, 1.0f));
		pRenderer->SetEffectLightMatrixProj(mtxLightProj);
	}

	//------------------------------
	//�t�H�O�̏����ݒ�
	//------------------------------
	if (pRenderer != nullptr) {
		pRenderer->SetEffectFogEnable(true);
		pRenderer->SetEffectFogColor(FOG_COLOR);
		pRenderer->SetEffectFogRange(200.0f, 4000.0f);
		//�o�b�N�o�b�t�@���t�H�O�̐F�ɍ��킹��
		pRenderer->SetBackBuffColor(FOG_COLOR);
	}

	//------------------------------
	//���[�V�������̃��[�h
	//------------------------------
	CPlayer::LoadMotionInfo();

	//------------------------------
	//3D���f���̐���
	//------------------------------
	//�n�`
	CObjectModel::Create(CModel::MODELTYPE::TERRAIN, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);	
	//�v���C���[
	CPlayer::Create(D3DXVECTOR3(0.0f, APPLETREE_POS_Y, -350.0f));	
	//�����S�̖�
	D3DXVECTOR3 posTree = D3DXVECTOR3(0.0f, APPLETREE_POS_Y, 0.0f);
	CObjectModel* pTree = CObjectModel::Create(CModel::MODELTYPE::OBJ_APPLE_TREE, posTree, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);
	if (pTree != nullptr) pTree->SetModelGlowColor(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));

	//�����S�̎�ނ̎擾
	int aTypeApple[MAX_NUM_CREATE_APPLE];
	memset(&aTypeApple, -1, sizeof(aTypeApple));

	FILE* pFile = fopen(TEXT_FILE_NAME_APPLETYPE, "r");
	if (pFile != nullptr) {
		//�����S�̎�ނ��擾
		for (int nCnt = 0; nCnt < MAX_NUM_CREATE_APPLE; nCnt++) {
			fscanf(pFile, "%d", &aTypeApple[nCnt]);
		}
		//�t�@�C�������
		fclose(pFile);
	}

	//�����S�̐���
	for (int nCnt = 0; nCnt < MAX_NUM_CREATE_APPLE; nCnt++)
	{
		if (aTypeApple[nCnt] == -1) continue;
		D3DXVECTOR3 posApple = posTree + CAppleTree::GetOffsetPosApple(nCnt);
		//�����S�̐���
		CAppleTree::CreateApple((CGlowApple::APPLE_TYPE)aTypeApple[nCnt], posApple, nullptr);
	}

	//------------------------------
	//UI�̐���
	//------------------------------
	CObject2D::Create(D3DXVECTOR3(120.0f, 40.0f, 0.0f), CTexture::TEXTURE_TYPE::QUIT_GAME, 200.0f, 40.0f);
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_TITLENAME, 800.0f, 200.0f);

	//�^�C�g���̑I�����j���[�̐���
	m_pSelectMenuTitle = CSelectMenu2D::Create(3, false);

	if (m_pSelectMenuTitle != nullptr) {
		//�c�I��
		m_pSelectMenuTitle->SetSelectType(CSelectMenu::SELECT_TYPE::VERTICAL);
		//�I����UI�̏ڍאݒ�
		m_pSelectMenuTitle->SetSelectUI(0, D3DXVECTOR3(SCREEN_WIDTH - 200.0f, 500.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_GAMESTART);
		m_pSelectMenuTitle->SetSelectUI(1, D3DXVECTOR3(SCREEN_WIDTH - 160.0f, 580.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_TUTORIAL);
		m_pSelectMenuTitle->SetSelectUI(2, D3DXVECTOR3(SCREEN_WIDTH - 140.0f, 660.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_CREDIT);
		//�I�����A�C�R���̐���
		m_pSelectMenuTitle->CreateSelectIcon(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), 40.0f, 40.0f, CTexture::TEXTURE_TYPE::SELECT_ICON);
		m_pSelectMenuTitle->SetIconPosOffset(1, D3DXVECTOR3(-160.0f, 0.0f, 0.0f));
		m_pSelectMenuTitle->SetIconPosOffset(2, D3DXVECTOR3(-140.0f, 0.0f, 0.0f));
	}

	//BGM�̍Đ�
	if (pSound != nullptr) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_TITLE);
		pSound->SetBGM(CSound::SOUND_LABEL::BGM_TITLE);
	}

	//�I�u�W�F�N�g�̃|�[�Y�������悤�ɐݒ�
	CObject::SetUpdatePauseLevel(0);


#ifdef _DEBUG
	//Z�o�b�t�@�e�N�X�`���̕\��
	CObject2D* pZBuff = CObject2D::Create(D3DXVECTOR3(70.0f, 120.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 100.0f, 100.0f);
	if (pZBuff != nullptr) {
		pZBuff->SetDrawPriority(CObject::DRAW_PRIORITY::FRONT);
		pZBuff->SetUseZBuffTexture(true);
	}
#endif

}

//=============================================================================
// �^�C�g���V�[���̏I������
//=============================================================================
void CTitleScene::Uninit(void) {
	//���[�V�������̃A�����[�h
	CPlayer::UnloadMotionInfo();

	//�V�[���̏I������
	CScene::Uninit();

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//���̒�~
	if (pSound != nullptr) {
		pSound->StopSound();
	}
}

//=============================================================================
// �^�C�g���V�[���̍X�V����
//=============================================================================
void CTitleScene::Update(void) {
	//�V�[���J�ڊJ�n��͍X�V�Ȃ�
	if (m_bBeginFade) return;

	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	CInput* pInput = nullptr;	//���̓f�o�C�X�ւ̃|�C���^
	CFade* pFade = nullptr;		//�t�F�[�h�ւ̃|�C���^
	CSound* pSound = nullptr;	//�T�E���h�ւ̃|�C���^

	if (pManager != nullptr) {
		//���݂̓��̓f�o�C�X�̎擾
		pInput = pManager->GetInputCur();
		//�t�F�[�h�̎擾
		pFade = pManager->GetFade();
		//�T�E���h�̎擾
		pSound = pManager->GetSound();
	}

	if (pInput == nullptr || pFade == nullptr) return;


	//����L�[���������Ƃ�
	if (pInput->GetTrigger(CInput::CODE::SELECT) && m_pSelectMenuTitle != nullptr) {
		//�t�F�[�h���������ꍇ
		if (pFade->GetFade()) {
			//�t�F�[�h���X�L�b�v
			pFade->SkipFade();
		}
		//�`���[�g���A����ʂ��\������Ă���Ƃ�
		else if (m_pTutorial != nullptr) {
			//�`���[�g���A����ʂ����
			m_pTutorial->Uninit();
			m_pTutorial = nullptr;
			//�I���̃��b�N������
			m_pSelectMenuTitle->SetLockChangeSelect(false);
			//���艹�̍Đ�
			if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
		}
		//�N���W�b�g��ʂ��\������Ă���Ƃ�
		else if (m_pCreadit != nullptr) {
			//�N���W�b�g��ʂ����
			m_pCreadit->Uninit();
			m_pCreadit = nullptr;
			//�I���̃��b�N������
			m_pSelectMenuTitle->SetLockChangeSelect(false);
			//���艹�̍Đ�
			if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
		}
		//���̉�ʂ��\������Ă��Ȃ��Ƃ�
		else {
			//�I�����j���[�̌��݂̑I�������擾
			int nIdxSelect = m_pSelectMenuTitle->GetIdxCurSelect();
			switch (nIdxSelect)
			{
				//�Q�[���X�^�[�g
			case 0:
				//�V�[���J�ڊJ�n
				m_bBeginFade = true;
				if (pFade != nullptr) pFade->SetFade(CScene::SCENE_TYPE::GAME, 0.02f, 60);
				//���艹�̍Đ�
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_START);
				//���j���[�̑I�������b�N
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;

				//�`���[�g���A�����
			case 1:
				//�`���[�g���A����ʂ̐���
				m_pTutorial = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::TUTORIAL, SCREEN_WIDTH, SCREEN_HEIGHT);
				//���艹�̍Đ�
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
				//���j���[�̑I�������b�N
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;

				//�N���W�b�g���
			case 2:
				//�N���W�b�g��ʂ̐���
				m_pTutorial = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::CREDIT, SCREEN_WIDTH, SCREEN_HEIGHT);
				//���艹�̍Đ�
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
				//���j���[�̑I�������b�N
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;
			}
		}
	}
}