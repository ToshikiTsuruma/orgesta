//=============================================================================
//
// �Q�[�W���� [gauge.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"
#include "texture.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �Q�[�W�N���X
//*****************************************************************************
class CGauge : public CObject
{
public:

	CGauge();		//�f�t�H���g�R���X�g���N�^
	CGauge(int nMaxValue, bool bVertical);		//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CGauge();		//�f�X�g���N�^
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void) {}	//�`�揈��
	virtual void CreateGauge(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) = 0;			//�Q�[�W�𐶐�
	virtual void CreateGaugeBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) = 0;		//�Q�[�W�̔w�i�𐶐�
	virtual void CreateGaugeFrame(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) = 0;	//�Q�[�W�̘g�𐶐�

	void AddGaugeValue(int nAddValue);	//�Q�[�W�̒l��ǉ�
	void SetGaugeValue(int nValue);	//�Q�[�W�̒l��ݒ�
	void SetMaxValue(int nValue);	//�Q�[�W�̍ő�l��ݒ�
	void SetMaxDrawLife(int nLife) { m_nDrawLifeMax = nLife; }	//�`�掞�Ԃ̍ő�l��ݒ�
	void SetExtend(bool bExtend) { m_bExtend = bExtend; }	//�ő�l�ɍ��킹�ăQ�[�W�̕����g�����邩�ǂ����̐ݒ�

	void SetGaugeDraw(bool bDraw);	//�Q�[�W�̕`���Ԃ̐ݒ�

	void SetAllGaugePos(D3DXVECTOR3 pos);	//�Q�[�W�̂��ׂĂ̈ʒu�̐ݒ�
	void SetGaugePos(D3DXVECTOR3 pos);		//�Q�[�W�̈ʒu�̐ݒ�
	void SetGaugeBGPos(D3DXVECTOR3 pos);	//�Q�[�W�̔w�i�̈ʒu�̐ݒ�
	void SetGaugeFramePos(D3DXVECTOR3 pos);	//�Q�[�W�̘g�̈ʒu�̐ݒ�

	void SetGaugeColor(D3DXCOLOR col);		//�Q�[�W�̐F�̐ݒ�
	void SetGaugeColorDanger(D3DXCOLOR col) { m_colGaugeDanger = col; }	//�Q�[�W�̃f�t�H���g�J���[�̐ݒ�
	void SetDangerValue(int nValue) { m_nDangerValue = nValue; }	//�Q�[�W�̊댯���̐F�ɂȂ�l��ݒ�
	void SetGaugeBGColor(D3DXCOLOR col);	//�Q�[�W�̔w�i�̐F�̐ݒ�
	void SetGaugeFrameColor(D3DXCOLOR col);	//�Q�[�W�̘g�̐F�̐ݒ�

protected:
	//���̃I�u�W�F�N�g�����ȑO�̃I�u�W�F�N�g��ݒ肷��Ƃ����炭�I�u�W�F�N�g�S�j�����ɉ���̂Œ���
	void SetGaugePtr(CObject* pGauge);				//�Q�[�W�̃|�C���^��ݒ�
	void SetGaugeBGPtr(CObject* pGaugeBG);			//�Q�[�W�̔w�i�̃|�C���^��ݒ�
	void SetGaugeFramePtr(CObject* pGaugeFrame);	//�Q�[�W�̘g�̃|�C���^��ݒ�

private:
	const bool m_bVertical;		//�c�����̃Q�[�W���ǂ���
	const int m_nMaxValueDefault;	//�������̒l�̍ő�l
	int m_nDrawLifeMax;	//�`�掞�Ԃ̍ő�
	int m_nCntDrawLife;	//�`�掞��
	int m_nMaxValue;	//�Q�[�W�̒l�̍ő�
	int m_nValue;		//�Q�[�W�̒l
	bool m_bExtend;		//�ő�l�ɍ��킹�Ċg�����邩�ǂ���

	CObject* m_pGauge;		//�Q�[�W�̃|�C���^
	CObject* m_pGaugeBG;	//�Q�[�W�̔w�i�̃|�C���^
	CObject* m_pGaugeFrame;	//�Q�[�W�̘g�̃|�C���^

	D3DXCOLOR m_colGaugeDefault;	//�Q�[�W�̃f�t�H���g�F
	D3DXCOLOR m_colGaugeDanger;		//�Q�[�W�̊댯���̐F
	int m_nDangerValue;	//�Q�[�W���댯���̐F�ɂȂ�l
};

#endif // !_GAUGE_H_