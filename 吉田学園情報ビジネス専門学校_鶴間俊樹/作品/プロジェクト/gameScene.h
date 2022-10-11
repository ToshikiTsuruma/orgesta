//=============================================================================
//
// �Q�[���V�[������ [gameScene.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CStage;
class CAppleTree;
class CTimer;
class CPauseMenu;
class CSelectMenu2D;
class CEnemySpawner;
class CScore;
class CObject2D;
class CGameScoreManager;
class CScapegoatCreater;

//*****************************************************************************
// �Q�[���V�[���N���X
//*****************************************************************************
class CGameScene : public CScene
{
public:
	CGameScene();	//�f�t�H���g�R���X�g���N�^
	virtual ~CGameScene();	//�f�X�g���N�^
	void Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void DeletePauseMenu(void);	//�|�[�Y���j���[�̔j��
	void GameClear(void);	//�Q�[���N���A
	void GameOver(void);	//�Q�[���I�[�o�[

	bool GetGameClear(void) { return m_bGameClear; }	//�Q�[���N���A��Ԃ̃t���O
	bool GetGameOver(void) { return m_bGameOver; }		//�Q�[���I�[�o�[��Ԃ̃t���O

	CAppleTree* GetAppleTree(void) { return m_pAppleTree; }	//�ь�̖؂̎擾
	CTimer* GetTimer(void) { return m_pTimer; }	//�^�C�}�[�̎擾
	CEnemySpawner* GetEnemySpawner(void) { return m_pEnemySpawner; }	//�G�̃X�|�i�[���擾
	void AddGameScore(int nScore);	//�Q�[���̃X�R�A�����Z
	void AddNumKillEnemy(int nNum);	//�Q�[�����ɓG��|�������̉��Z

private:
	void UpdateGame(void);			//�Q�[�����̍X�V
	void UpdateGameClear(void);		//�Q�[���N���A���̍X�V
	void UpdateGameOver(void);		//�Q�[���I�[�o�[���̍X�V
	void CreateMenuEndGame(void);	//�Q�[���I�����̑I�����j���[�̐���
	void SaveAppleType(void);		//�Q�[���I�����̃����S�̎�ނ̕ۑ�

	bool m_bGameClear;	//�Q�[���I�[�o�[
	bool m_bGameOver;	//�Q�[���I�[�o�[

	CStage* m_pStage;				//�X�e�[�W�ւ̃|�C���^
	CAppleTree* m_pAppleTree;		//�ь�̖؂̃|�C���^
	CTimer* m_pTimer;				//�Q�[���̃^�C�}�[
	CObject2D* m_pTimerFrame;		//�^�C�}�[�̘g
	CScore* m_pScore;				//�X�R�A�ւ̃|�C���^
	CObject2D* m_pScoreFrame;		//�X�R�A�̘g
	CGameScoreManager* m_pGameScoreManager;	//�Q�[���̃X�R�A�̃}�l�[�W���[
	int m_nGameScore;				//�Q�[���I�����̃X�R�A
	CPauseMenu* m_pMenuPause;		//�|�[�Y���j���[
	CSelectMenu2D* m_pMenuGameEnd;	//�Q�[���I�����̑I�����j���[
	CEnemySpawner* m_pEnemySpawner;	//�G�̃X�|�i�[
	CScapegoatCreater* m_pScapegoatCreater;	//���ѐ����N���X

	int m_nCntGameClear;	//�Q�[���N���A��̃J�E���g
};

#endif // !_GAME_SCENE_H_