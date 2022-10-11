//=============================================================================
//
// �r���{�[�h���� [billboard.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �r���{�[�h�N���X
//*****************************************************************************
class CBillboard : public CObject
{
public:
	CBillboard();	//�f�t�H���g�R���X�g���N�^
	virtual ~CBillboard();	//�f�X�g���N�^
	static CBillboard* Create(D3DXVECTOR3 pos, CTexture::TEXTURE_TYPE type, float fWidth, float fHeight);	//��������
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��
	void SetPos(D3DXVECTOR3 pos);	//�ʒu���W�̐ݒ�
	D3DXVECTOR3 GetPos(void);		//�ʒu���W�̎擾
	void SetMove(D3DXVECTOR3 move);	//�ړ��ʂ̐ݒ�
	D3DXVECTOR3 GetMove(void);		//�ړ��ʂ̎擾
	void AddSize(D3DXVECTOR3 size) { SetSize(GetSize() + size); }	//�T�C�Y�̐ݒ�
	void SetSize(D3DXVECTOR3 size);	//�T�C�Y�̐ݒ�
	D3DXVECTOR3 GetSize(void);		//�T�C�Y�̎擾
	void SetRatioWidth(float fRatio);	//���̊����̐ݒ�
	void SetRatioHeight(float fRatio);	//�����̊����̐ݒ�
	void SetOffsetPos(D3DXVECTOR3 pos);	//�ʒu�̃I�t�Z�b�g�̐ݒ�
	void SetTexPos(float startU, float startV, float endU, float endV);	//�e�N�X�`�����W�̐ݒ�
	void SetColor(D3DXCOLOR col);	//�F�̐ݒ�
	void SetRotateYOnly(bool bFlag) { m_bRotateYOnly = bFlag; }	//�J�����ɑ΂���Y��]�݂̂ɂ��邩�ǂ����̐ݒ�

	void SetEnableFog(bool bEnable);	//�t�H�O��L���ɂ��邩�ǂ����̐ݒ�
	void SetZtestAlways(bool bAlways);	//�[�x�֌W�Ȃ��`��ł��邩�ǂ����̐ݒ�
	void SetZwriteEnable(bool bEnable);	//�[�x�������|���S�����Ղ邩�ǂ����̐ݒ�
	void SetDrawAllDistance(bool bDraw);	//�S�Ă̋����ŕ`��\���ǂ����̐ݒ�
	void SetDistanceDrawMax(float fDist);	//�`��\�ȋ����̍ő�l�̐ݒ�

private:
	D3DXVECTOR3	m_pos;	//�|���S���̒��S���W
	D3DXVECTOR3 m_move;	//�ړ���
	D3DXMATRIX m_mtxWorld;//���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�̃|�C���^
	float m_fWidth;			//�|���S���̕�
	float m_fRatioWidth;	//�|���S���̕��̊���
	float m_fHeight;		//�|���S���̍���
	float m_fRatioHeight;	//�|���S���̍����̊���
	D3DXVECTOR3 m_offsetPos;//�ʒu�̃I�t�Z�b�g
	bool m_bRotateYOnly;	//Y��]�̂�
	D3DXCOLOR m_col;	//���_�F

	bool m_bEnableFog;		//�t�H�O��L���ɂ��邩�ǂ���
	bool m_bZtestAlways;	//�[�x�֌W�Ȃ��`��ł��邩�ǂ���
	bool m_bZwriteEnable;	//�[�x�������|���S�����Ղ邩�ǂ���
	bool m_bDrawAllDist;	//�S�Ă̋����ŕ`��\���ǂ���
	float m_fDistDrawMax;	//�`��\�ȋ����̍ő�l
};

#endif // !_BILLBOARD_H_