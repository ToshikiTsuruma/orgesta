//=============================================================================
//
// 地形処理 [terrain.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "terrain.h"
#include "manager.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTerrain::CTerrain(CModel::MODELTYPE typeModel) : CObjectModel(typeModel, false)
{
	SetObjType(OBJTYPE_TERRAIN);	//オブジェクトタイプの設定
	SetDrawPriority(DRAW_PRIORITY::BG);
}

//=============================================================================
// デストラクタ
//=============================================================================
CTerrain::~CTerrain()
{

}

//=============================================================================
// 地形の生成処理
//=============================================================================
CTerrain* CTerrain::Create(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot) {
	CTerrain* pTerrain;
	pTerrain = new CTerrain(typeModel);
	if (pTerrain == nullptr) return nullptr;

	pTerrain->SetPos(pos);
	pTerrain->SetRot(rot);
	pTerrain->Init();

	return pTerrain;
}

//=============================================================================
// 地形の初期化処理
//=============================================================================
HRESULT CTerrain::Init(void) {
	CObjectModel::Init();

	return S_OK;
}

//=============================================================================
// 地形の終了処理
//=============================================================================
void CTerrain::Uninit(void) {
	CObjectModel::Uninit();
}

//=============================================================================
// 地形の更新処理
//=============================================================================
void CTerrain::Update(void) {
	CObjectModel::Update();
}

//=============================================================================
// 地形の描画処理
//=============================================================================
void CTerrain::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// 地形との衝突判定
//=============================================================================
bool CTerrain::Collision(D3DXVECTOR3& posCollision, D3DXVECTOR3 vecStart, D3DXVECTOR3 vecEnd) {
	CObject* pObject;	//オブジェクトへのポインタ
	pObject = GetObjectTopAll();	//全オブジェクトのリストの先頭を取得

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//リストの次のオブジェクトのポインタを取得
	
		//オブジェクトタイプが異なる場合
		if (!(pObject->GetObjType() & OBJTYPE_TERRAIN)) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;
		}

		CTerrain* pTerrain = dynamic_cast<CTerrain*>(pObject);	//地形クラスにキャスト
		//キャスト失敗時
		if (pTerrain == nullptr) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;	//ループを飛ばす
		}
		//死亡フラグが立っている場合
		if (pTerrain->GetDeath()) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;	//ループを飛ばす
		}

		//地形のモデルデータの取得
		CModel::MODELTYPE modelType = pTerrain->GetModelType();	//モデルの種類の取得
		CModel::ModelData modelData = CModel::GetModelData(modelType);	//モデルのデータの取得
		LPD3DXMESH pMesh = modelData.pMesh;	//メッシュデータの取得
		//メッシュがnullの場合
		if (pMesh == nullptr) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;	//ループを飛ばす
		}

		D3DXVECTOR3 posTerrain = pTerrain->GetPos();//地形の位置				
		D3DXVECTOR3 rotTerrain = pTerrain->GetRot();//地形の回転				

		D3DXMATRIX mtxWorldTerrain;	//地形のワールドマトリックス
		D3DXMATRIX mtxTrans, mtxRot;	//計算用マトリックス
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorldTerrain);
		//地形の回転を行列にかける
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rotTerrain.y, rotTerrain.x, rotTerrain.z);
		D3DXMatrixMultiply(&mtxWorldTerrain, &mtxWorldTerrain, &mtxRot);
		//地形の位置座標を行列にかける
		D3DXMatrixTranslation(&mtxTrans, posTerrain.x, posTerrain.y, posTerrain.z);
		D3DXMatrixMultiply(&mtxWorldTerrain, &mtxWorldTerrain, &mtxTrans);

		D3DXMATRIX mtxInverseTerrain;	//地形のワールドマトリックスの逆行列
		//地形の行列の逆行列を計算
		D3DXMatrixInverse(&mtxInverseTerrain, nullptr, &mtxWorldTerrain);
		//レイに地形の逆行列をかける
		D3DXVec3TransformCoord(&vecStart, &vecStart, &mtxInverseTerrain);
		D3DXVec3TransformCoord(&vecEnd, &vecEnd, &mtxInverseTerrain);

		D3DXVECTOR3 vecDir = vecEnd - vecStart;	//レイの方向ベクトル

		BOOL bHit;		//衝突判定フラグ
		DWORD faceIdx;	//交差時に一番近い面のインデックス
		float fU, fV;	//交差した三角形の重心座標?

		//当たり判定
		D3DXIntersect(pMesh, &vecStart, &vecDir, &bHit, &faceIdx, &fU, &fV, nullptr, nullptr, nullptr);

		//レイが衝突していない場合
		if (bHit == FALSE) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;	//ループを飛ばす
		}

		//-------------------------------------------------
		//判定があった三角形の頂点のインデックスの取得
		//-------------------------------------------------
		int nIdx[3];	//三角形の頂点のインデックス
		WORD *pIdx;		//インデックス情報へのポインタ
		//インデックスバッファのロック
		pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIdx);
		for (int nCnt = 0; nCnt < 3; nCnt++) {
			nIdx[nCnt] = pIdx[(faceIdx * 3) + nCnt];
		}

		//インデックスバッファのアンロック
		pMesh->UnlockIndexBuffer();

		//-------------------------------------------------
		//判定があった三角形の頂点の位置の取得
		//-------------------------------------------------
		D3DXVECTOR3 posVtx[3];	//衝突した三角形の頂点の位置座標

		DWORD dwStride = pMesh->GetNumBytesPerVertex();	//頂点ごとのバイト数

		BYTE* pbVtx = nullptr;
		float* pfVtx = nullptr;

		//頂点バッファのロック
		pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pbVtx);

		for (int nCntVtx = 0; nCntVtx < 3; nCntVtx++) {
			//インデックスに応じた頂点座標を取得
			pfVtx = (float*)&pbVtx[dwStride * nIdx[nCntVtx]];

			posVtx[nCntVtx].x = pfVtx[0];
			posVtx[nCntVtx].y = pfVtx[1];
			posVtx[nCntVtx].z = pfVtx[2];
		}

		//頂点バッファのアンロック
		pMesh->UnlockVertexBuffer();

		D3DXVECTOR3 posCol = D3DXVECTOR3(posVtx[0] + fU * (posVtx[1] - posVtx[0]) + fV * (posVtx[2] - posVtx[0]));	//地形のローカル座標での衝突位置

		//地形との衝突位置をワールド座標に変換
		D3DXVec3TransformCoord(&posCol, &posCol, &mtxWorldTerrain);
		//衝突したオブジェクトの位置を設定
		posCollision = posCol;
		return true;
	
		pObject = pObjNext;	//リストの次のオブジェクトを代入
	}

	return false;
}

//=============================================================================
// 上向きベクトルの地形との衝突判定
//=============================================================================
bool CTerrain::Collision(D3DXVECTOR3& posCollision) {
	D3DXVECTOR3 vecStart, vecEnd;	//接触位置、開始ベクトル、終了ベクトル
	vecStart = posCollision;
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//上向きのベクトル

	//地形との当たり判定
	return CTerrain::Collision(posCollision, vecStart, vecEnd);
}