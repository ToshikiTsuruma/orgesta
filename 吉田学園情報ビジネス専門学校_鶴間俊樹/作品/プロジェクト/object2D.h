//=============================================================================
//
// 2D�I�u�W�F�N�g���� [object2D.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// 2D�I�u�W�F�N�g�N���X
//*****************************************************************************
class CObject2D : public CObject
{
public:
	CObject2D();		//�f�t�H���g�R���X�g���N�^
	virtual ~CObject2D();	//�f�X�g���N�^
	static CObject2D* Create(void);	//�����ݒ�Ȃ��̐�������
	static CObject2D* Create(D3DXVECTOR3 pos, CTexture::TEXTURE_TYPE type, float fWidth, float fHeight);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

	void SetPos(D3DXVECTOR3 pos);	//�ʒu���W�̐ݒ�
	D3DXVECTOR3 GetPos(void);		//�ʒu���W�̎擾
	void SetMove(D3DXVECTOR3 move);	//�ړ��ʂ̐ݒ�
	D3DXVECTOR3 GetMove(void);		//�ړ��ʂ̎擾
	void SetSize(D3DXVECTOR3 size);	//�T�C�Y�̐ݒ�
	D3DXVECTOR3 GetSize(void);		//�T�C�Y�̎擾
	void SetRatioWidth(float fRatio);	//���̊����̐ݒ�
	void SetRatioHeight(float fRatio);	//�����̊����̐ݒ�
	void SetAngle(float fAngle);	//�p�x�̐ݒ�
	float GetAngle(void);			//�p�x�̎擾
	void SetColor(D3DXCOLOR col);	//�J���[�̐ݒ�
	D3DXCOLOR GetColor(void);		//�J���[�̎擾
	void SetTexPos(float startU, float startV, float endU, float endV);	//�e�N�X�`�����W�̐ݒ�
	void SetTexNumber(int nNumber);	//�����ɑΉ������e�N�X�`�����W�̐ݒ�

	void SetUseZBuffTexture(bool bUse) { m_bUseZBuffTexture = bUse; }

private:
	void SetVtxPos(void);	//�e���_�̈ʒu���X�V

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3	m_pos;	//�|���S���̒��S���W
	D3DXVECTOR3 m_move;	//�ړ���
	float m_fWidth;		//�|���S���̕�
	float m_fRatioWidth;//�|���S���̕��̊���
	float m_fHeight;	//�|���S���̍���
	float m_fRatioHeight;//�|���S���̍����̊���
	float m_fAngle;		//�|���S���̊p�x
	D3DXCOLOR m_col;	//�|���S���̐F

	bool m_bUseZBuffTexture;	//Z�o�b�t�@�̃e�N�X�`�����g��
};

#endif // !_OBJECT2D_H_
