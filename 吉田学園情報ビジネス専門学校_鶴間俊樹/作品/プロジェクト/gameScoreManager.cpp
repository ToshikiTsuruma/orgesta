//=============================================================================
//
//�Q�[���X�R�A�}�l�[�W���[���� [gameScoreManager.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "gameScoreManager.h"
#include "gauge2D.h"
#include "object2D.h"
#include "score.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TIME_COMBO_DEFAULT (10 * FPS)	//�����̃R���{�̍ő厞��

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGameScoreManager::CGameScoreManager()
{
	m_nKillCombo = 0;
	m_nTimeCombo = 0;
	m_nTimeComboMax = 0;
	m_fScoreRate = 1.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGameScoreManager::~CGameScoreManager()
{

}

//=============================================================================
// �Q�[���X�R�A�}�l�[�W���[�̐�������
//=============================================================================
CGameScoreManager* CGameScoreManager::Create(D3DXVECTOR3 pos, float fSize) {
	CGameScoreManager* pGameScoreManager;
	pGameScoreManager = new CGameScoreManager();
	if (pGameScoreManager == nullptr) return nullptr;

	pGameScoreManager->m_nKillCombo = 0;
	pGameScoreManager->m_nTimeCombo = 0;
	pGameScoreManager->m_fScoreRate = 1.0f;
	pGameScoreManager->m_pScoreRate = CObject2D::Create(pos + D3DXVECTOR3(fSize * 0.5f, fSize * 0.1f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_X1, fSize * 0.8f, fSize * 0.8f);
	pGameScoreManager->m_pTextCombo = CObject2D::Create(pos + D3DXVECTOR3(fSize * 0.5f, fSize * 0.8f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_COMBO, fSize, fSize * 0.3f);
	pGameScoreManager->m_pComboTimeGauge = CGauge2D::Create(MAX_TIME_COMBO_DEFAULT, false, 0, 0, false);
	//���ԃQ�[�W�̏����ݒ�
	if (pGameScoreManager->m_pComboTimeGauge != nullptr) {
		//�w�i�ݒ�
		pGameScoreManager->m_pComboTimeGauge->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, pos + D3DXVECTOR3(fSize * 0.5f, fSize * 0.6f, 0.0f), fSize, fSize * 0.1f);
		pGameScoreManager->m_pComboTimeGauge->SetGaugeBGColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
		//�Q�[�W�ݒ�
		pGameScoreManager->m_pComboTimeGauge->CreateGauge(CTexture::TEXTURE_TYPE::GAUGE_HORIZONTAL, pos + D3DXVECTOR3(fSize * 0.5f, fSize * 0.6f, 0.0f), fSize - fSize * 0.05f, fSize * 0.1f - fSize * 0.05f);
		pGameScoreManager->m_pComboTimeGauge->SetGaugeColor(D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f));
		pGameScoreManager->m_pComboTimeGauge->SetGaugeColorDanger(D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f));
		pGameScoreManager->m_pComboTimeGauge->SetDangerValue(FPS * 2);
		pGameScoreManager->m_pComboTimeGauge->SetGaugeValue(0);

	}
	//�R���{��
	pGameScoreManager->m_pComboScore = CScore::Create(1, CTexture::TEXTURE_TYPE::NUMBER_002, pos + D3DXVECTOR3(0.0f, fSize * 0.5f, 0.0f), fSize * 0.5f);

	pGameScoreManager->Init();

	return pGameScoreManager;
}

//=============================================================================
// �Q�[���X�R�A�}�l�[�W���[�̏���������
//=============================================================================
HRESULT CGameScoreManager::Init(void) {
	m_nKillCombo = 0;
	m_nTimeCombo = 0;
	m_nTimeComboMax = MAX_TIME_COMBO_DEFAULT;
	m_fScoreRate = 1.0f;

	return S_OK;
}

//=============================================================================
// �Q�[���X�R�A�}�l�[�W���[�̏I������
//=============================================================================
void CGameScoreManager::Uninit(void) {
	//�X�R�A�̔{���\���̔j��
	if (m_pScoreRate != nullptr) {
		m_pScoreRate->Uninit();
		m_pScoreRate = nullptr;
	}
	//�R���{�̎c�莞�ԃQ�[�W�̔j��
	if (m_pComboTimeGauge != nullptr) {
		m_pComboTimeGauge->Uninit();
		m_pComboTimeGauge = nullptr;
	}
	//�R���{���̔j��
	if (m_pComboScore != nullptr) {
		m_pComboScore->Uninit();
		m_pComboScore = nullptr;
	}
	//�R���{�̃e�L�X�g�̔j��
	if (m_pTextCombo != nullptr) {
		m_pTextCombo->Uninit();
		m_pTextCombo = nullptr;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �Q�[���X�R�A�}�l�[�W���[�̍X�V����
//=============================================================================
void CGameScoreManager::Update(void) {
	if (m_nTimeCombo > 0) {
		//�R���{�L�����Ԃ����炷
		m_nTimeCombo--;
		//�Q�[�W�̍X�V
		if (m_pComboTimeGauge != nullptr) {
			//�R���{�Q�[�W�̒l�̐ݒ�
			m_pComboTimeGauge->SetGaugeValue(m_nTimeCombo);
		}

		//�R���{���r�؂ꂽ�ꍇ
		if (m_nTimeCombo <= 0) {
			m_nTimeCombo = 0;
			//������Ԃɖ߂�
			m_nTimeComboMax = MAX_TIME_COMBO_DEFAULT;
			m_nKillCombo = 0;
			m_fScoreRate = 1.0f;
			//�{���\�����P�{�ɂ���
			if (m_pScoreRate != nullptr) {
				m_pScoreRate->SetTexType(CTexture::TEXTURE_TYPE::TEXT_X1);
			}
			//�R���{���̐ݒ�
			if (m_pComboScore != nullptr) {
				m_pComboScore->SetScore(0);
			}
		}
	}
}

//=============================================================================
// �|�����R���{�̉��Z
//=============================================================================
void CGameScoreManager::AddKillCombo(void) {
	//�R���{�ǉ�
	m_nKillCombo++;
	//�R���{���̐ݒ�
	if (m_pComboScore != nullptr) {
		m_pComboScore->SetScore(m_nKillCombo);
	}

	//����̃R���{���ɒB�������̐ݒ�
	switch (m_nKillCombo)
	{
	case 10:
		//�R���{�̍ő�L�����Ԃ�����
		m_nTimeComboMax = 6 * FPS;
		//�X�R�A�̔{���̑���
		m_fScoreRate = 2.0f;
		//�{���\����ύX
		if (m_pScoreRate != nullptr) {
			m_pScoreRate->SetTexType(CTexture::TEXTURE_TYPE::TEXT_X2);
		}
		break;
	case 30:
		//�R���{�̍ő�L�����Ԃ�����
		m_nTimeComboMax = 4 * FPS;
		//�X�R�A�̔{���̑���
		m_fScoreRate = 3.0f;
		//�{���\����ύX
		if (m_pScoreRate != nullptr) {
			m_pScoreRate->SetTexType(CTexture::TEXTURE_TYPE::TEXT_X3);
		}
		break;
	case 50:
		//�R���{�̍ő�L�����Ԃ�����
		m_nTimeComboMax = (float)(2.5f * FPS);
		//�X�R�A�̔{���̑���
		m_fScoreRate = 5.0f;
		//�{���\����ύX
		if (m_pScoreRate != nullptr) {
			m_pScoreRate->SetTexType(CTexture::TEXTURE_TYPE::TEXT_X5);
		}
		break;
	}

	//�R���{�̎��Ԃ�ݒ�
	m_nTimeCombo = m_nTimeComboMax;
	//�R���{�Q�[�W�̐ݒ�
	if (m_pComboTimeGauge != nullptr) {
		//�R���{�Q�[�W�̍ő�l�̐ݒ�
		m_pComboTimeGauge->SetMaxValue(m_nTimeComboMax);
		//�R���{�Q�[�W�̒l�̐ݒ�
		m_pComboTimeGauge->SetGaugeValue(m_nTimeCombo);
	}
}