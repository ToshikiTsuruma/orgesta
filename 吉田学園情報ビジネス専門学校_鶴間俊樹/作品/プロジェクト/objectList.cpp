//=============================================================================
//
// オブジェクトリスト処理 [objectList.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "objectList.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CObjectList* CObjectList::m_pTopAllList = nullptr;
CObjectList* CObjectList::m_pCurAllList = nullptr;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CObjectList::CObjectList()
{
	m_pTopNode = nullptr;
	m_pCurNode = nullptr;
	m_nNumNode = 0;

	//全オブジェクトリストのリストの初期設定
	m_pPrevAllList = m_pCurAllList;	//前のポインタの設定
	m_pNextAllList = nullptr;		//次のポインタの設定
	if (m_pTopAllList == nullptr) m_pTopAllList = this;				//topが存在しない場合、このリストをtopにする
	if (m_pCurAllList != nullptr) m_pCurAllList->m_pNextAllList = this;	//curが存在する場合、curのpNextをこのリストのポインタにする
	m_pCurAllList = this;		//最後尾のポインタの設定
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjectList::~CObjectList()
{
	ListNode* pListNode = m_pTopNode;	//先頭のノードを取得

	while (pListNode != nullptr)
	{
		//次のノードを保存
		ListNode* pNextNode = pListNode->pNextNode;
		//ノードの破棄
		delete pListNode;
		//次のノードを設定
		pListNode = pNextNode;
	}

	//全オブジェクトリスト
	if (m_pTopAllList == this) m_pTopAllList = m_pNextAllList;	//このリストがtopだった場合、次のリストをtopにする
	if (m_pCurAllList == this) m_pCurAllList = m_pPrevAllList;	//このvがcurだった場合、前のリストをcurにする
	if (m_pPrevAllList != nullptr) m_pPrevAllList->m_pNextAllList = m_pNextAllList;	//前のリストのpNextに、このリストのpNextを代入
	if (m_pNextAllList != nullptr) m_pNextAllList->m_pPrevAllList = m_pPrevAllList;	//次のリストのpPrevに、このリストのpPrevを代入
}

//=============================================================================
// オブジェクトリストの初期化処理
//=============================================================================
void CObjectList::Init(void) {
	ListNode* pListNode = m_pTopNode;	//先頭のノードを取得

	while (pListNode != nullptr)
	{
		//次のノードを保存
		ListNode* pNextNode = pListNode->pNextNode;
		//ノードの破棄
		delete pListNode;
		//次のノードを設定
		pListNode = pNextNode;
	}

	m_pTopNode = nullptr;
	m_pCurNode = nullptr;

	m_nNumNode = 0;
}

//=============================================================================
// オブジェクトリストにノードを追加
//=============================================================================
void CObjectList::AppendNode(CObject* pObject) {
	ListNode* pNewNode = new ListNode;	//新しく追加するノード
	//ノードを生成できていない場合終了
	if (pNewNode == nullptr) return;

	//リストの設定
	pNewNode->pPrevNode = m_pCurNode;	//前のノードの設定
	pNewNode->pNextNode = nullptr;		//次のノードの設定
	if (m_pTopNode == nullptr) m_pTopNode = pNewNode;				//先頭のノードが存在しない場合、このノードを先頭にする
	if (m_pCurNode != nullptr) m_pCurNode->pNextNode = pNewNode;	//最後尾のノードが存在する場合、最後尾の次をこのノードにする
	m_pCurNode = pNewNode;		//最後尾のノードの設定

	//ノードに保存するオブジェクトを設定
	pNewNode->pObject = pObject;	
	//ノードの数を加算
	m_nNumNode++;
}

//=============================================================================
// オブジェクトリストのノードの削除
//=============================================================================
bool CObjectList::DeleteNode(int nIdx, CObject** ppObject) {
	ListNode* pListNode = m_pTopNode;	//先頭のノードを取得
	int nCntLoop = 0;	//ループした回数

	//ノードがnullではない場合
	while (pListNode != nullptr)
	{
		//削除する番号に達した場合
		if (nIdx == nCntLoop) {
			//引数のオブジェクトのポインタにノードのオブジェクトのポインタを渡す
			if (ppObject != nullptr) {
				*ppObject = pListNode->pObject;
			}

			//ノードの破棄
			ReleaseNode(pListNode);

			//ノードの数を減算
			m_nNumNode--;
			return true;
		}
		//次のノードにする
		pListNode = pListNode->pNextNode;

		nCntLoop++;
	}

	return false;
}

//=============================================================================
// オブジェクトリストに一致するオブジェクトがあるかどうか
//=============================================================================
bool CObjectList::MatchObject(CObject* pObject) {
	ListNode* pListNode = m_pTopNode;	//先頭のノードを取得

	//ノードがnullではない場合
	while (pListNode != nullptr)
	{
		//オブジェクトリストに一致するオブジェクトがある場合
		if (pListNode->pObject == pObject) {
			return true;
		}
		//次のノードにする
		pListNode = pListNode->pNextNode;
	}

	return false;
}

//=============================================================================
// オブジェクトリストの上からn番目のオブジェクトを取得
//=============================================================================
CObject* CObjectList::GetObjectIndex(int nIdx) {
	ListNode* pListNode = m_pTopNode;	//先頭のノードを取得
	//先頭ノードがnullの場合終了
	if (pListNode == nullptr) return nullptr;

	for (int nCnt = 0; nCnt < nIdx; nCnt++)
	{
		//次のノードにする
		pListNode = pListNode->pNextNode;
		//ノードがnullの場合終了
		if (pListNode == nullptr) return nullptr;
	}

	return pListNode->pObject;
}

//=============================================================================
// リストにあるノードの数の取得
//=============================================================================
int CObjectList::GetNumNode(void) {
	return m_nNumNode;
}

//=============================================================================
// リストのすべてのオブジェクトを削除する
//=============================================================================
void CObjectList::DeleteAllObject(void) {
	ListNode* pListNode = m_pTopNode;	//先頭のノードを取得

	while (pListNode != nullptr)
	{
		//次のノードを保存
		ListNode* pNextNode = pListNode->pNextNode;

		//オブジェクトの終了処理
		if (pListNode->pObject != nullptr) {
			pListNode->pObject->Uninit();
			pListNode->pObject = nullptr;
		}

		//ノードの破棄
		delete pListNode;
		//次のノードを設定
		pListNode = pNextNode;
	}

	m_pTopNode = nullptr;
	m_pCurNode = nullptr;

	m_nNumNode = 0;
}

//=============================================================================
// 全リストのノードのオブジェクトのポインタと引数のオブジェクトのポインタが一致する場合ノードを破棄
//=============================================================================
//※動作未チェック-----多少の動作確認では問題ない---------------------------------------------------------------------------------------※
void CObjectList::ReleaseDeleteNode(CObject* pDeleteObj) {
	if (m_pTopAllList == nullptr || pDeleteObj == nullptr) return;

	CObjectList* pObjList = m_pTopAllList;	//オブジェクトリストのポインタ

	while (pObjList != nullptr)
	{
		ListNode* pNode = pObjList->m_pTopNode;	//ノードのポインタ
		while (pNode != nullptr)
		{
			ListNode* pNextNode = pNode->pNextNode;	//次のノードのポインタ
			CObject* pObject = pNode->pObject;	//オブジェクトへのポインタ

			//ノードが保持しているオブジェクトが削除するオブジェクトだった場合
			if (pObject == pDeleteObj) {
				pObjList->ReleaseNode(pNode);
			}

			//次のノードを代入
			pNode = pNextNode;
		}

		//次のリストを代入
		pObjList = pObjList->m_pNextAllList;
	}
}

//=============================================================================
// ノードの破棄
//=============================================================================
void CObjectList::ReleaseNode(ListNode* pReleaseNode) {
	//リストの更新
	if (m_pTopNode == pReleaseNode) m_pTopNode = pReleaseNode->pNextNode;	//このノードが先頭だった場合、次のノードを先頭にする
	if (m_pCurNode == pReleaseNode) m_pCurNode = pReleaseNode->pPrevNode;	//このノードが最後尾だった場合、前のノードを最後尾にする
	if (pReleaseNode->pPrevNode != nullptr) pReleaseNode->pPrevNode->pNextNode = pReleaseNode->pNextNode;	//前のノードのpNextに、このノードのpNextを代入
	if (pReleaseNode->pNextNode != nullptr) pReleaseNode->pNextNode->pPrevNode = pReleaseNode->pPrevNode;	//次のノードのpPrevに、このノードのpPrevを代入

	//ノードを削除
	delete pReleaseNode;
}