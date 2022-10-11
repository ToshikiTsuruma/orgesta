//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define COLOR_FADE_DEFAULT (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CFade::CFade()
{
	m_pVtxBuff = nullptr;
	m_fFadeSpeed = 0.0f;
	m_bFadein = false;
	m_nextScene = CScene::SCENE_TYPE::TITLE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// �t�F�[�h�̏���������
//=============================================================================
HRESULT CFade::Init(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return S_OK;

	VERTEX_2D *pVtx;
	m_bFadein = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		NULL,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = COLOR_FADE_DEFAULT;
	pVtx[1].col = COLOR_FADE_DEFAULT;
	pVtx[2].col = COLOR_FADE_DEFAULT;
	pVtx[3].col = COLOR_FADE_DEFAULT;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �t�F�[�h�̏I������
//=============================================================================
void CFade::Uninit(void) {
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// �t�F�[�h�̍X�V����
//=============================================================================
void CFade::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	

	//�t�F�[�h�؂�ւ����̏I��
	if (m_bChangeFade) m_bChangeFade = false;

	//�t�F�[�h�A�E�g
	if (!m_bFadein && m_colA > 0.0f) {
		//�t�F�[�h�A�E�g
		m_colA -= m_fFadeSpeed;
		if (m_colA < 0.0f) {
			m_colA = 0.0f;
		}
	}

	//�t�F�[�h�C��
	if (m_bFadein && m_colA <= 1.0f) {
		m_colA += m_fFadeSpeed;
		//�t�F�[�h������
		if (m_colA >= 1.0f) {
			m_colA = 1.0f;
			//��~��Ԃ̔���
			if (m_nCntStop < m_nStopTime) {
				//��~�J�E���g��i�߂�
				m_nCntStop++;
			}
			else {
				m_bFadein = false;
				//�V�[���J��
				if (pManager != nullptr) {
					m_bChangeFade = true;	//�t�F�[�h�؂�ւ���
					//�V�[���̃^�C�v�����݂��Ă���ꍇ�̂݃V�[���J��
					if (m_nextScene >= (CScene::SCENE_TYPE)0 && m_nextScene < CScene::SCENE_TYPE::ENUM_MAX) pManager->ChangeScene((int)m_nextScene);
				}
			}
		}
	}

	//�t�F�[�h���̂ݍX�V
	if (m_bFadein || m_colA > 0.0f) {
		VERTEX_2D *pVtx;
		D3DXCOLOR colFade = COLOR_FADE_DEFAULT;
		colFade.a = m_colA;

		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_����ݒ�
		pVtx[0].col = colFade;
		pVtx[1].col = colFade;
		pVtx[2].col = colFade;
		pVtx[3].col = colFade;
		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �t�F�[�h�̕`�揈��
//=============================================================================
void CFade::Draw(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return;

	//�t�F�[�h���ł͂Ȃ��ꍇ�`�悵�Ȃ�
	if (m_colA <= 0.0f) return;

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	//���_��`��ݒ�
	pRenderer->SetVtxDecl2D();
	//�e�N�X�`���̐ݒ�
	pRenderer->SetEffectTexture(nullptr);

	pRenderer->BeginPassEffect(PASS_2D);	//�p�X�̊J�n

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0,
		2);

	pRenderer->EndPassEffect();	//�p�X�̏I��

}

//=============================================================================
//�t�F�[�h�̐ݒ�
//=============================================================================
void CFade::SetFade(CScene::SCENE_TYPE typeScene, float fFadeSpeed, int nStopTime) {
	//�t�F�[�h���̏ꍇ�I��
	if (m_bFadein || m_colA > 0.0f) return;

	m_bFadein = true;
	m_nextScene = typeScene;
	m_fFadeSpeed = fFadeSpeed;
	m_nStopTime = nStopTime;
	m_nCntStop = 0;
}

//=============================================================================
// �t�F�[�h�A�E�g���X�L�b�v������
//=============================================================================
void CFade::SkipFade(void) {
	m_colA = 0.0f;
	m_bFadein = false;
}

//=============================================================================
// �t�F�[�h�����ǂ������擾
//=============================================================================
bool CFade::GetFade(void) {
	if (m_bFadein || m_colA > 0.0f) {
		return true;
	}
	return false;
}

//=============================================================================
// �t�F�[�h�C������t�F�[�h�A�E�g�ɐ؂�ւ�����^�C�~���O���ǂ���
//=============================================================================
bool CFade::GetChangeFade(void) {
	return m_bChangeFade;
}