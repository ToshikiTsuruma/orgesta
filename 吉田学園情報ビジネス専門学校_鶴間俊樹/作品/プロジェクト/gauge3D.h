//=============================================================================
//
// 3D�Q�[�W���� [gauge3D.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _GAUGE_3D_H_
#define _GAUGE_3D_H_

#include "gauge.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// 3D�Q�[�W�N���X
//*****************************************************************************
class CGauge3D : public CGauge
{
public:

	CGauge3D();		//�f�t�H���g�R���X�g���N�^
	CGauge3D(int nMaxValue, bool bVertical);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CGauge3D();		//�f�X�g���N�^
	static CGauge3D* Create(int nMaxValue, bool bVertical, int nValue, int nDrawLifeMax, bool bExtend);	//��������
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void CreateGauge(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight);			//�Q�[�W�𐶐�
	virtual void CreateGaugeBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight);		//�Q�[�W�̔w�i�𐶐�
	virtual void CreateGaugeFrame(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight);	//�Q�[�W�̘g�𐶐�

private:

};

#endif // !_GAUGE_3D_H_