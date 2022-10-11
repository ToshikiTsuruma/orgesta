//=============================================================================
//
// ���b�V���E�H�[������ [meshwall.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"
#include "object3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// ���b�V���E�H�[���N���X
//*****************************************************************************
class CMeshwall : public CObject3D
{
public:
	CMeshwall();	//�f�t�H���g�R���X�g���N�^
	virtual ~CMeshwall();	//�f�X�g���N�^
	static CMeshwall* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumX, int nNumY, float fWidth, float fHeight, CTexture::TEXTURE_TYPE typeTex);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

private:
	int m_nNumX;	//X�����̃|���S���̐�
	int m_nNumY;	//Y�����̃|���S���̐�
	float m_fWidth;	//�|���S���̕�
	float m_fHeight;//�|���S���̍���
};

#endif // !_MESHWALL_H_