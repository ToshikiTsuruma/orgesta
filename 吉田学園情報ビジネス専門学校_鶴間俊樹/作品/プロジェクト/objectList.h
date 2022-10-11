//=============================================================================
//
// オブジェクトリスト処理 [objectList.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _OBJECT_LIST_H_
#define _OBJECT_LIST_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// オブジェクトリストクラス
//*****************************************************************************
class CObjectList
{
public:
	CObjectList();	//デフォルトコンストラクタ
	~CObjectList();	//デストラクタ
	void Init(void);	//初期化処理
	void AppendNode(CObject* pObject);				//ノードの追加
	bool DeleteNode(int nIdx, CObject** ppObject);	//ノードの削除
	bool MatchObject(CObject* pObject);	//一致するオブジェクトがあるかどうか
	CObject* GetObjectIndex(int nIdx);	//リストの上からn番目のオブジェクトの取得
	int GetNumNode(void);		//リストにあるノードの数の取得
	void DeleteAllObject(void);	//リストのすべてのオブジェクトを削除する
	static void ReleaseDeleteNode(CObject* pDeleteObj);	//全リストのノードのオブジェクトのポインタと引数のオブジェクトのポインタが一致する場合ノードを破棄

private:
	//リストのノード
	struct ListNode
	{
		ListNode* pPrevNode;	//前のノード
		ListNode* pNextNode;	//次のノード

		CObject* pObject;		//オブジェクトのポインタ
	};

	void ReleaseNode(ListNode* pReleaseNode);	//ノードの破棄

	ListNode* m_pTopNode;	//先頭のノード
	ListNode* m_pCurNode;	//最後尾のノード
	int m_nNumNode;			//リストにあるノードの数

	//全オブジェクトリストのリスト
	static CObjectList* m_pTopAllList;	//先頭のリストのポインタ
	static CObjectList* m_pCurAllList;	//最後尾のリストのポインタ
	CObjectList* m_pPrevAllList;		//前のリストのポインタ
	CObjectList* m_pNextAllList;		//次のリストのポインタ
};

#endif // !_OBJECT_LIST_H_