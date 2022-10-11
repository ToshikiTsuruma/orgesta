//=============================================================================
//
// �Q�[�W���� [gauge.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "gauge.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGauge::CGauge() : m_bVertical(false), m_nMaxValueDefault(0)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CGauge::CGauge(int nMaxValue, bool bVertical) : m_bVertical(bVertical), m_nMaxValueDefault(nMaxValue)
{
	m_nMaxValue = nMaxValue;
	m_nValue = 0;
	m_nDrawLifeMax = 0;
	m_nCntDrawLife = 0;
	m_bExtend = false;

	m_colGaugeDefault = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_colGaugeDanger = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nDangerValue = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGauge::~CGauge()
{

}

//=============================================================================
// �Q�[�W�̏���������
//=============================================================================
HRESULT CGauge::Init(void) {
	SetGaugeValue(0);
	m_nCntDrawLife = 0;

	return S_OK;
}

//=============================================================================
// �Q�[�W�̏I������
//=============================================================================
void CGauge::Uninit(void) {
	//�Q�[�W�̔j��
	if (m_pGauge != nullptr) {
		m_pGauge->Uninit();
		m_pGauge = nullptr;
	}
	//�Q�[�W�̔w�i�̔j��
	if (m_pGaugeBG != nullptr) {
		m_pGaugeBG->Uninit();
		m_pGaugeBG = nullptr;
	}
	//�Q�[�W�̘g�̔j��
	if (m_pGaugeFrame != nullptr) {
		m_pGaugeFrame->Uninit();
		m_pGaugeFrame = nullptr;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �Q�[�W�̍X�V����
//=============================================================================
void CGauge::Update(void) {
	if (m_nCntDrawLife > 0) {
		//�`�悷�鎞�Ԃ����炷
		m_nCntDrawLife--;
		//�`�掞�Ԃ��I�������ꍇ
		if (m_nCntDrawLife == 0) {
			//�Q�[�W���\��
			SetGaugeDraw(false);
		}
	}
}

//=============================================================================
// �Q�[�W�̒l��ǉ�
//=============================================================================
void CGauge::AddGaugeValue(int nAddValue) {
	SetGaugeValue(m_nValue + nAddValue);
}

//=============================================================================
// �Q�[�W�̒l��ݒ�
//=============================================================================
void CGauge::SetGaugeValue(int nValue) {
	m_nValue = nValue;

	//����E�������ߎ��̒���
	if (m_nValue < 0) m_nValue = 0;
	if (m_nValue > m_nMaxValue) m_nValue = m_nMaxValue;

	m_nCntDrawLife = m_nDrawLifeMax;	//�`�掞�Ԃ��ő�l�ɐݒ�

	//�Q�[�W��\��
	SetGaugeDraw(true);

	if (m_pGauge == nullptr) return;

	if (m_nValue <= m_nDangerValue) {
		//�댯���̐F�ɕύX
		m_pGauge->SetColor(m_colGaugeDanger);
	}
	else {
		//�ʏ�F�֕ύX
		m_pGauge->SetColor(m_colGaugeDefault);
	}

	//�Q�[�W���ő�l�ɍ��킹�Ċg������ꍇ
	if (m_bExtend) {
		//�Q�[�W�̊�����ݒ�
		if (m_bVertical) {
			m_pGauge->SetRatioHeight((float)m_nValue / (float)m_nMaxValueDefault);
		}
		else {
			m_pGauge->SetRatioWidth((float)m_nValue / (float)m_nMaxValueDefault);
		}
	}
	else {
		//�Q�[�W�̊�����ݒ�
		if (m_bVertical) {
			m_pGauge->SetRatioHeight((float)m_nValue / (float)m_nMaxValue);
		}
		else
		{
			m_pGauge->SetRatioWidth((float)m_nValue / (float)m_nMaxValue);
		}
	}
}

//=============================================================================
// �Q�[�W�̍ő�l��ݒ�
//=============================================================================
void CGauge::SetMaxValue(int nValue) {
	//�ő�l��ݒ�
	m_nMaxValue = nValue;

	if (m_pGauge == nullptr) return;

	//�Q�[�W���ő�l�ɍ��킹�Ċg������ꍇ
	if (m_bExtend) {
		if (m_bVertical) {
			//�Q�[�W�̊�����ݒ�
			m_pGauge->SetRatioHeight((float)m_nValue / (float)m_nMaxValueDefault);
			//�Q�[�W�̔w�i�̊�����ݒ�
			m_pGaugeBG->SetRatioHeight((float)m_nMaxValue / (float)m_nMaxValueDefault);
			//�Q�[�W�̘g�̊�����ݒ�
			m_pGaugeFrame->SetRatioHeight((float)m_nMaxValue / (float)m_nMaxValueDefault);
		}
		else {
			//�Q�[�W�̊�����ݒ�
			m_pGauge->SetRatioWidth((float)m_nValue / (float)m_nMaxValueDefault);
			//�Q�[�W�̔w�i�̊�����ݒ�
			m_pGaugeBG->SetRatioWidth((float)m_nMaxValue / (float)m_nMaxValueDefault);
			//�Q�[�W�̘g�̊�����ݒ�
			m_pGaugeFrame->SetRatioWidth((float)m_nMaxValue / (float)m_nMaxValueDefault);
		}
	}
}

//=============================================================================
// �Q�[�W�̕`���Ԃ̐ݒ�
//=============================================================================
void CGauge::SetGaugeDraw(bool bDraw) {
	//�Q�[�W�̕\���؂�ւ�
	if (m_pGauge != nullptr) m_pGauge->SetDraw(bDraw);
	//�Q�[�W�̔w�i�̕\���؂�ւ�
	if (m_pGaugeBG != nullptr) m_pGaugeBG->SetDraw(bDraw);
	//�Q�[�W�̘g�̕\���؂�ւ�
	if (m_pGaugeFrame != nullptr) m_pGaugeFrame->SetDraw(bDraw);
}

//=============================================================================
// �Q�[�W�̂��ׂĂ̈ʒu�̐ݒ�
//=============================================================================
void CGauge::SetAllGaugePos(D3DXVECTOR3 pos) {
	if (m_pGauge != nullptr) m_pGauge->SetPos(pos);
	if (m_pGaugeBG != nullptr) m_pGaugeBG->SetPos(pos);
	if (m_pGaugeFrame != nullptr) m_pGaugeFrame->SetPos(pos);
}

//=============================================================================
// �Q�[�W�̈ʒu�̐ݒ�
//=============================================================================
void CGauge::SetGaugePos(D3DXVECTOR3 pos) {
	if (m_pGauge == nullptr) return;
	m_pGauge->SetPos(pos);
}

//=============================================================================
// �Q�[�W�̔w�i�̈ʒu�̐ݒ�
//=============================================================================
void CGauge::SetGaugeBGPos(D3DXVECTOR3 pos) {
	if (m_pGaugeBG == nullptr) return;
	m_pGaugeBG->SetPos(pos);
}

//=============================================================================
// �Q�[�W�̘g�̈ʒu�̐ݒ�
//=============================================================================
void CGauge::SetGaugeFramePos(D3DXVECTOR3 pos) {
	if (m_pGaugeFrame == nullptr) return;
	m_pGaugeFrame->SetPos(pos);
}

//=============================================================================
// �Q�[�W�̐F�̐ݒ�
//=============================================================================
void CGauge::SetGaugeColor(D3DXCOLOR col) {
	if (m_pGauge == nullptr) return;
	m_pGauge->SetColor(col);
	m_colGaugeDefault = col;
}

//=============================================================================
// �Q�[�W�̔w�i�̐F�̐ݒ�
//=============================================================================
void CGauge::SetGaugeBGColor(D3DXCOLOR col) {
	if (m_pGaugeBG == nullptr) return;
	m_pGaugeBG->SetColor(col);
}

//=============================================================================
// �Q�[�W�̘g�̐F�̐ݒ�
//=============================================================================
void CGauge::SetGaugeFrameColor(D3DXCOLOR col) {
	if (m_pGaugeFrame == nullptr) return;
	m_pGaugeFrame->SetColor(col);
}

//=============================================================================
//�Q�[�W�̃|�C���^��ݒ�
//=============================================================================
void CGauge::SetGaugePtr(CObject* pGauge) {
	//���łɐݒ肳��Ă���ꍇ�j��
	if (m_pGauge != nullptr) {
		m_pGauge->Uninit();
	}

	m_pGauge = pGauge;
}

//=============================================================================
//�Q�[�W�̔w�i�̃|�C���^��ݒ�
//=============================================================================
void CGauge::SetGaugeBGPtr(CObject* pGaugeBG) {
	//���łɐݒ肳��Ă���ꍇ�j��
	if (m_pGaugeBG != nullptr) {
		m_pGaugeBG->Uninit();
	}

	m_pGaugeBG = pGaugeBG;
}

//=============================================================================
//�Q�[�W�̘g�̃|�C���^��ݒ�
//=============================================================================
void CGauge::SetGaugeFramePtr(CObject* pGaugeFrame) {
	//���łɐݒ肳��Ă���ꍇ�j��
	if (m_pGaugeFrame != nullptr) {
		m_pGaugeFrame->Uninit();
	}

	m_pGaugeFrame = pGaugeFrame;
}