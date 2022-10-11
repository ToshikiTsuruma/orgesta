//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "billboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �p�[�e�B�N���N���X
//*****************************************************************************
class CParticle : public CBillboard
{
public:
	CParticle();	//�f�t�H���g�R���X�g���N�^
	CParticle(int nLife, float fAddSize, D3DXVECTOR3 move, D3DXVECTOR3 addMove, D3DXCOLOR colStart, D3DXCOLOR colEnd);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CParticle();	//�f�X�g���N�^
	static CParticle* Create(D3DXVECTOR3 pos, int nLife, float fSizeStart, float fAddSize, D3DXVECTOR3 move, D3DXVECTOR3 addMove, D3DXCOLOR colStart, D3DXCOLOR colEnd);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

private:
	const int m_nLifeDef;	//���C�t�̏����l
	int m_nLife;			//���C�t
	float m_fAddSize;		//�T�C�Y�̒ǉ���
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_addMove;	//������
	D3DXCOLOR m_colStart;	//�������̐F
	D3DXCOLOR m_colEnd;		//���Ŏ��̐F
};

#endif // !_PARTICLE_H_