/***********************************************************

					SceneNode & SceneGraph

		Each Node's transform are based on its parent node,
		so hierarchical transformation is available.
		World transform evaluation might need to traverse a
		path in scene graph from leaf to root, and concatenate
		the local transforms into one.

***********************************************************/

#include "Noise3D.h"

using namespace Noise3D;

Noise3D::SceneNode::SceneNode()
{
}

Noise3D::SceneNode::~SceneNode()
{
}

AffineTransform& Noise3D::SceneNode::GetLocalTransform()
{
	return mLocalTransform;
}

NMATRIX Noise3D::SceneNode::EvalWorldAffineTransformMatrix()
{
	SceneGraph* pSG = this->GetHostTree();
	std::vector<SceneNode*> path;//path to root
	pSG->TraversePathToRoot(this, path);

	//concatenate leaf node's transform first, then level-by-level to root
	//traversePathToRoot's result have the leaf node at the beginning, and root at the end
	NMATRIX result = XMMatrixIdentity();
	for (auto pNode: path)
	{
		//WARNING: plz be careful about ROW/COLUMN major 
		//(2019.3.7)Noise3D uses ROW major like DXMath do. refer to AffineTransform for related info
		NMATRIX tmpMat = XMMatrixIdentity();
		AffineTransform& localTrans = pNode->GetLocalTransform();
		localTrans.GetTransformMatrix(tmpMat);

		// world_vec = local_vec *  Mat_n * Mat_(n-1) * .... Mat_1 * Mat_root
		result = result * tmpMat;
	}

	return result;
}

NMATRIX Noise3D::SceneNode::EvalWorldRigidTransformMatrix()
{
	//similar to SceneNode::EvalWorldAffineTransformMatrix()
	SceneGraph* pSG = this->GetHostTree();
	std::vector<SceneNode*> path;//path to root
	pSG->TraversePathToRoot(this, path);

	NMATRIX result = XMMatrixIdentity();
	for (auto pNode : path)
	{
		NMATRIX tmpMat = XMMatrixIdentity();
		AffineTransform& localTrans = pNode->GetLocalTransform();
		localTrans.GetRigidTransformMatrix(tmpMat);

		// world_vec = local_vec *  Mat_n * Mat_(n-1) * .... Mat_1 * Mat_root
		result = result * tmpMat;
	}

	return result;
}

NMATRIX Noise3D::SceneNode::EvalWorldRotationMatrix()
{
	//similar to SceneNode::EvalWorldAffineTransformMatrix()
	SceneGraph* pSG = this->GetHostTree();
	std::vector<SceneNode*> path;//path to root
	pSG->TraversePathToRoot(this, path);

	NMATRIX result = XMMatrixIdentity();
	for (auto pNode : path)
	{
		NMATRIX tmpMat = XMMatrixIdentity();
		AffineTransform& localTrans = pNode->GetLocalTransform();
		tmpMat = localTrans.GetRotationMatrix();

		// world_vec = local_vec *  Mat_n * Mat_(n-1) * .... Mat_1 * Mat_root
		result = result * tmpMat;
	}

	return result;
}

void Noise3D::SceneNode::EvalWorldAffineTransformMatrix(NMATRIX & outWorldMat, NMATRIX & outWorldInvTranspose)
{
	outWorldMat = SceneNode::EvalWorldAffineTransformMatrix();

	//world inv transpose for normal's transformation
	NMATRIX worldInvMat = XMMatrixInverse(nullptr, outWorldMat);
	if (XMMatrixIsInfinite(worldInvMat))
	{
		//WARNING_MSG("world matrix Inv not exist! determinant == 0 ! ");
		outWorldInvTranspose = XMMatrixIdentity();
		return;
	}
	else
	{
		outWorldInvTranspose = worldInvMat.Transpose();
	}
}

void Noise3D::SceneNode::AttachSceneObject(ISceneObject * pObj)
{
	//should be compatible with ISceneObject::AttachToSceneNode(SceneNode * pNode)

	//similar to the impl of TreeNodeTemplate's AttachChildNode()
	if (pObj != nullptr)
	{
		SceneNode* pOriginalNode = pObj->GetAttachedSceneNode();
		//if attach to different node, we must detach its original connection first
		if (pOriginalNode != nullptr && pOriginalNode != this)
		{
			//delete pObj's original father's ref to this scene object
			std::vector<ISceneObject*>& originalNodeSOList = pOriginalNode->SceneNode::mAttachedSceneObjectList;
			auto iter_ChildObjRef = std::find(originalNodeSOList.begin(),	originalNodeSOList.end(),pObj);

			if (iter_ChildObjRef != originalNodeSOList.end())
			{
				originalNodeSOList.erase(iter_ChildObjRef);
			}
		}
		pObj->m_pAttachedSceneNode = this;
		mAttachedSceneObjectList.push_back(pObj);
	}
}

void Noise3D::SceneNode::DetachSceneObject(ISceneObject * pObj)
{
	if (pObj != nullptr)
	{
		//delete pObj's original father's ref to current node
		auto iter_ChildObjRef = std::find(
				mAttachedSceneObjectList.begin(),
				mAttachedSceneObjectList.end(),pObj);

		if (iter_ChildObjRef != mAttachedSceneObjectList.end())
		{
			mAttachedSceneObjectList.erase(iter_ChildObjRef);
		}
	}
	pObj->m_pAttachedSceneNode = nullptr;
}

bool Noise3D::SceneNode::IsAttachedSceneObject()
{
	return (mAttachedSceneObjectList.size()!=0);
}


/******************************************
					
						Scene Graph

*******************************************/

Noise3D::SceneGraph::SceneGraph()
{
}

Noise3D::SceneGraph::~SceneGraph()
{
}
