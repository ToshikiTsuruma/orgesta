//=============================================================================
//
// 3D�I�u�W�F�N�g���� [object3D.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// 3D�I�u�W�F�N�g�N���X
//*****************************************************************************
class CObject3D : public CObject
{
public:
	CObject3D();	//�f�t�H���g�R���X�g���N�^
	virtual ~CObject3D();	//�f�X�g���N�^
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��

	void SetPos(D3DXVECTOR3 pos);	//�ʒu���W�̐ݒ�
	D3DXVECTOR3 GetPos(void);		//�ʒu���W�̎擾
	void SetRot(D3DXVECTOR3 rot);	//�p�x�̐ݒ�
	D3DXVECTOR3 GetRot(void);		//�p�x�̎擾
	LPDIRECT3DVERTEXBUFFER9* GetPtrVtxBuff(void);	//���_�o�b�t�@�̃|�C���^�̃|�C���^���擾
	LPDIRECT3DINDEXBUFFER9* GetPtrIdxBuff(void);	//�C���f�b�N�X�o�b�t�@�̃|�C���^�̃|�C���^���擾
	void SetNumVtx(int nNumVtx);	//���_���̐ݒ�
	int GetNumVtx(void);			//���_���̎擾
	void SetNumIdx(int nNumVtx);	//�C���f�b�N�X���̐ݒ�
	int GetNumIdx(void);			//�C���f�b�N�X���̎擾
	void SetColor(D3DXCOLOR col) { m_col = col; }	//�F�̐ݒ�
	void AddColor(D3DXCOLOR col) { m_col += col; }	//�F�̉��Z
	D3DXCOLOR GetColor(void) { return m_col; }	//�F�̎擾

private:
	D3DXVECTOR3	m_pos;	//�|���S���̒��S���W
	D3DXVECTOR3 m_rot;	//�|���S���̊p�x
	D3DXMATRIX m_mtxWorld;//���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff; //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	int m_nNumVtx;	//���_��
	int m_nNumIdx;	//�C���f�b�N�X��
	D3DXCOLOR m_col;	//�}�e���A���̐F
};

#endif // !_OBJECT3D_H_