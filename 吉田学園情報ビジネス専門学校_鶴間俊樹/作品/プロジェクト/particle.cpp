//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CParticle::CParticle() : m_nLifeDef(0)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CParticle::CParticle(int nLife, float fAddSize, D3DXVECTOR3 move, D3DXVECTOR3 addMove, D3DXCOLOR colStart, D3DXCOLOR colEnd) : m_nLifeDef(nLife)
{
	//�[�x�o�b�t�@�����̂܂܂ɂ���
	SetZtestAlways(false);
	//�`��\�ȋ����̍ő�l�ݒ�
	SetDistanceDrawMax(4000.0f);

	m_nLife = nLife;
	m_fAddSize = fAddSize;
	m_move = move;
	m_addMove = addMove;
	m_colStart = colStart;
	m_colEnd = colEnd;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// ��������
//=============================================================================
CParticle* CParticle::Create(D3DXVECTOR3 pos, int nLife, float fSizeStart, float fAddSize, D3DXVECTOR3 move, D3DXVECTOR3 addMove, D3DXCOLOR colStart, D3DXCOLOR colEnd) {
	CParticle* pParticle;
	pParticle = new CParticle(nLife, fAddSize, move, addMove, colStart, colEnd);
	if (pParticle == nullptr) return nullptr;

	pParticle->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_PARTICLE);
	pParticle->SetPos(pos);
	pParticle->SetSize(D3DXVECTOR3(fSizeStart, fSizeStart, 0.0f));
	pParticle->Init();
	pParticle->SetColor(colStart);

	return pParticle;
}

//=============================================================================
// �p�[�e�B�N���̏���������
//=============================================================================
HRESULT CParticle::Init(void) {
	//�r���{�[�h�̏�����
	CBillboard::Init();

	return S_OK;
}

//=============================================================================
// �p�[�e�B�N���̏I������
//=============================================================================
void CParticle::Uninit(void) {
	CBillboard::Uninit();
}

//=============================================================================
// �p�[�e�B�N���̍X�V����
//=============================================================================
void CParticle::Update(void) {
	//���C�t��0
	if (m_nLife == 0) {
		//�p�[�e�B�N���j��
		Uninit();
		return;
	}
	else {
		//���C�t�̌���
		m_nLife--;
	}

	//�r���{�[�h�̍X�V
	CBillboard::Update();

	//�ʒu�̍X�V
	D3DXVECTOR3 pos = GetPos();	//�ʒu�̎擾
	pos += m_move;	//�ʒu���ړ�
	SetPos(pos);	//�ʒu�̐ݒ�

	//�ړ��ʂ�ǉ�
	m_move += m_addMove;	

	D3DXVECTOR3 size = GetSize();	//�T�C�Y�̎擾
	//�T�C�Y�̌��Z
	size.x += m_fAddSize;
	size.y += m_fAddSize;
	//�T�C�Y���O�����ɂȂ�Ȃ��悤��
	if (size.x >= 0) {
		//�T�C�Y�̐ݒ�
		SetSize(size);
	}

	//�F�̐ݒ�
	D3DXCOLOR colDelta = m_colEnd - m_colStart;	//�ŏ��ƍŌ�̐F�̍���
	SetColor(colDelta * ((float)(m_nLifeDef - m_nLife) / (float)m_nLifeDef) + m_colStart);
}

//=============================================================================
// �p�[�e�B�N���̕`�揈��
//=============================================================================
void CParticle::Draw(void) {
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

	//���u�����f�B���O�����Z�����ɐݒ�
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�t�H�O�𖳌�
	pRenderer->SetEffectFogEnable(false);	

	CBillboard::Draw();

	//�t�H�O��L��
	pRenderer->SetEffectFogEnable(true);


	//���u�����f�B���O��ʏ�ɖ߂�
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}