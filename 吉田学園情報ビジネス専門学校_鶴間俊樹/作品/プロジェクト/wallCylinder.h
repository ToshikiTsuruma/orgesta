//=============================================================================
//
// �~���̕ǂ̏��� [wallCylinder.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _WALL_CYLINDER_H_
#define _WALL_CYLINDER_H_

#include "main.h"
#include "meshcylinder.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �~���̕ǃN���X
//*****************************************************************************
class CWallCylinder : public CMeshcylinder
{
public:
	CWallCylinder();	//�f�t�H���g�R���X�g���N�^
	CWallCylinder(D3DXVECTOR3 pos, float fRadius, float fHeight, CTexture::TEXTURE_TYPE typeTex, bool bClear);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CWallCylinder();	//�f�X�g���N�^
	static CWallCylinder* Create(D3DXVECTOR3 pos, float fRadius, float fHeight, CTexture::TEXTURE_TYPE typeTex, bool bClear);	//��������
	static bool Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 lastPos, float fRadius);	//�����蔻��
	static void CrossPoint(D3DXVECTOR2* pPosCross, const D3DXVECTOR3* pPosStart, const D3DXVECTOR3* pPosEnd, float fDelta);	//�ǂƐ����̌�_
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

private:
	void ChangeColorAlpha(void);	//�����x�̕ύX

	bool m_bClear;	//�����ɂȂ邩�ǂ���
};

#endif // !_WALL_CYLINDER_H_