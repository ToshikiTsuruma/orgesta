//=============================================================================
//
// �V�[������ [scene.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;

//*****************************************************************************
// �V�[���N���X
//*****************************************************************************
class CScene
{
public:
	//�V�[���̎��
	enum class SCENE_TYPE {
		TITLE = 0,
		GAME,
		ENUM_MAX
	};

	CScene();	//�f�t�H���g�R���X�g���N�^
	virtual ~CScene();	//�f�X�g���N�^
	virtual void Init(void) = 0;	//����������
	virtual void Uninit(void) = 0;	//�I������
	virtual void Update(void) = 0;	//�X�V����
	static void ChangeScene(CScene*& pScene, SCENE_TYPE typeScene);	//�V�[���̕ύX

	//�v���C���[�̃Z�b�^�[�E�Q�b�^�[
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* GetPlayer(void) { return m_pPlayer; }
private:
	CPlayer* m_pPlayer;	//�v���C���[�ւ̃|�C���^

};

#endif // !_SCENE_H_