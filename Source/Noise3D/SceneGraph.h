
/***********************************************************************

								 h��Scene Node
				desc: a single node in a scene graph, storing 
				transformation and some geometry, object binding infos
				(similar to OGRE's idea)

************************************************************************/

#pragma once

namespace Noise3D
{

	class SceneGraph;
	class ISceneObject;

	//Scene Node is a base class that contains Tree operation and transformation info
	class SceneNode :
		public TreeNodeTemplate<SceneNode, SceneGraph>
	{
	public:

		SceneNode();

		~SceneNode();

		AffineTransform& GetLocalTransform();//relative to its father node (if current node is attached to root node, then local=world)

		//traverse through scene graph(from given node to root) and concatenate local transforms.
		//(2019.3.7)currently computed accumulated world matrix won't cache in ScenNode;
		NMATRIX EvalWorldAffineTransformMatrix();

		//only consider Rotation and Translation
		NMATRIX EvalWorldRigidTransformMatrix();

		//only consider Rotation
		NMATRIX EvalWorldRotationMatrix();

		void EvalWorldAffineTransformMatrix(NMATRIX& outWorldMat, NMATRIX& outWorldInvTranspose);

		void AttachSceneObject(ISceneObject* pObj);

		void DetachSceneObject(ISceneObject* pObj);

		bool IsAttachedSceneObject();

	protected:

		AffineTransform mLocalTransform;

		std::vector<ISceneObject*> mAttachedSceneObjectList;

		//(2019.3.7)manually update?
		//AffineTransform mWorldTransform;

		//accumulated world transform matrix CACHE(from current to path)
		//(2019.3.6) i decide not to do this optimization
		//NMATRIX mEvaluatedWorldTransform;

	};

	class SceneGraph :
		public TreeTemplate<SceneNode,SceneGraph>
	{
	public:


	private:

		friend class SceneManager;

		SceneGraph();

		~SceneGraph();

	};

};