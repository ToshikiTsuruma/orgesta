//=============================================================================
//
// ���b�V���V�����_�[���� [meshcylinder.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"
#include "object3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// ���b�V���V�����_�[�N���X
//*****************************************************************************
class CMeshcylinder : public CObject3D
{
public:
	CMeshcylinder();	//�f�t�H���g�R���X�g���N�^
	CMeshcylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadiusTop, float fRadiusBottom, int nNumY, float fHeight, bool bOut);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CMeshcylinder();	//�f�X�g���N�^
	static CMeshcylinder* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadiusTop, float fRadiusBottom, int nNumY, float fHeight, bool bOut, CTexture::TEXTURE_TYPE typeTex);	//��������
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��
	float GetRadius(void);		//���a�̎擾�i���j
	float GetRadiusTop(void);	//�㔼�a�̎擾
	void AddRadius(float fRadius);	//���a��ǉ�
	void AddRadiusTop(float fRadius);		//�㔼�a��ǉ�
	void AddRadiusBottom(float fRadius);	//�����a��ǉ�
	void AddHeight(float fHeight);	//������ǉ�
	float GetHeight(void) { return m_fHeight; }	//�������擾

private:
	void SetVtxBuff(void);	//���_�o�b�t�@�̏����X�V
	void SetVtxBuffPos(void);	//���_�o�b�t�@�̈ʒu�����X�V

	const int m_nNumXZ;	//XZ�����̃|���S���̐�
	const int m_nNumY;	//Y�����̃|���S���̐�
	float m_fHeight;	//�|���S���̍���
	float m_fRadiusTop;		//�~���̔��a�̏�
	float m_fRadiusBottom;	//�~���̔��a�̉�
	bool m_bOut;		//�|���S�����O�������ǂ���
};

#endif // !_MESHCYLINDER_H_