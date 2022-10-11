//=============================================================================
//
// �t�H���g���� [font.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _FONT_H_
#define _FONT_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FONT_TEXT (256)	//�t�H���g�̕����̕\���ł���ő吔

//*****************************************************************************
// �t�H���g�N���X
//*****************************************************************************
class CFont : public CObject
{
public:
	CFont();		//�f�t�H���g�R���X�g���N�^
	~CFont();	//�f�X�g���N�^
	static CFont* Create(const char* pStrText, int nFlagAnchor, float fSide, float fTop, int nFontWidth);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetText(const char* pText);	//������̐ݒ�
	void SetRect(RECT rect);	//rect�̐ݒ�
	RECT GetRect(void);			//rect�̎擾
	void SetColor(D3DXCOLOR col) { m_col = col; }	//�t�H���g�̐F�̐ݒ�
	void SetFlagAnchor(int nFlag) { m_nFlagAnchor = nFlag; }	//������ʒu�̃t���O�̔ԍ��̐ݒ�

private:
	LPD3DXFONT m_pFont;		//�t�H���g�ւ̃|�C���^
	char m_sText[MAX_FONT_TEXT];	//�`�悷�镶����
	RECT m_rect;			//�`��͈�
	int m_nFontWidth;		//�t�H���g�̕����̕�
	D3DXCOLOR m_col;		//�t�H���g�̐F
	int m_nFlagAnchor;		//������ʒu�̃t���O�̔ԍ�

};

#endif // !_FONT_H_