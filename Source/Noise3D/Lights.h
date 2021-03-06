/***********************************************************************

                           h��ILight interafces && lightDesc 

************************************************************************/
#pragma once

namespace Noise3D
{
	//LIGHT description : just combination of data
	//LIGHT interfaces : provide interfaces for the user to interact


	struct N_CommonLightDesc
	{
		N_CommonLightDesc():
			ambientColor(NVECTOR3( 0, 0, 0 )),
			specularIntensity(0.0f),
			diffuseColor(NVECTOR3(0, 0, 0)),
			diffuseIntensity( 0.0f),
			specularColor(NVECTOR3(0, 0, 0))
			{}
		NVECTOR3	ambientColor;		float				specularIntensity;
		NVECTOR3	diffuseColor;		float				diffuseIntensity;
		NVECTOR3	specularColor;	//4 bytes left to pad to fulfill 128 bytes alignment
	};


	//don't forget graphic memory's 128 bit alignment
	struct N_DirLightDesc
		:public N_CommonLightDesc
	{
		N_DirLightDesc() {  };

		/*NVECTOR3	ambientColor;		float				specularIntensity;
		NVECTOR3	diffuseColor;				float				diffuseIntensity;
		NVECTOR3	specularColor;*/	float		mPad2;
		NVECTOR3 direction;				float		mPad3;
	};


	//don't forget graphic memory's 128 bit alignment
	struct N_PointLightDesc 
		:public N_CommonLightDesc
	{
		N_PointLightDesc() { }

		/*NVECTOR3	ambientColor;		float				specularIntensity;
		NVECTOR3	diffuseColor;				float				diffuseIntensity;
		NVECTOR3	specularColor;*/		float		attenuationFactor;
		NVECTOR3 position;					float		lightingRange;

	};


	//don't forget graphic memory's 128 bit alignment
	struct N_SpotLightDesc
		:public N_CommonLightDesc
	{
		N_SpotLightDesc(){}

		/*NVECTOR3 ambientColor;		float specularIntensity;
		NVECTOR3 diffuseColor;			float diffuseIntensity;
		NVECTOR3 specularColor;*/	float attenuationFactor;
		NVECTOR3 lookAt;					float lightingAngle;
		NVECTOR3 position;			float lightingRange;
	};


	//----------------------BASE LIGHT-------------------------------
	class IBaseLight
	{
	public:

		IBaseLight();

		void SetAmbientColor(const NVECTOR3& color);

		void SetDiffuseColor(const NVECTOR3& color);

		void SetSpecularColor(const NVECTOR3& color);

		void SetSpecularIntensity(float specInt);

		void SetDiffuseIntensity(float diffInt);


	protected:

		//invoked by derived Light,not by user
		void	SetDesc(const N_CommonLightDesc& desc);

		//invoked by derived Light,not by user
		void GetDesc(N_CommonLightDesc& outDesc);

	private:
		N_CommonLightDesc mBaseLightDesc;

	};


	//---------------------Dynamic Directional Light------------------
	class DirLight : 
		public IBaseLight,
		public IShadowCaster,//container of DSV of shadow map
		public ISceneObject
	{
	public:

		//set local direction (which can be rotated and transform to world space)
		void	SetDirection(const NVECTOR3& dir);

		//get local direction
		NVECTOR3 GetDirection();

		//get world space direction (which can be rotated to transform to world space)
		NVECTOR3 GetDirection_WorldSpace();

		//many CLAMP op happens in this
		void SetDesc(const N_DirLightDesc& desc);

		N_DirLightDesc GetDesc();

		//get desc with geometric info transformed to world space
		N_DirLightDesc GetDesc_TransformedToWorld();

		//ISceneObject::
		virtual	N_AABB GetLocalAABB() override;

		//ISceneObject::
		virtual	N_AABB ComputeWorldAABB_Accurate() override;

		//ISceneObject::
		virtual N_AABB ComputeWorldAABB_Fast() override;

		//ISceneObject::
		virtual	NOISE_SCENE_OBJECT_TYPE GetObjectType() override;

		//SceneNode::
		AffineTransform& GetLocalTransform()=delete;

		//SceneNode::
		NMATRIX EvalWorldAffineTransformMatrix() = delete;

		//SceneNode::
		void EvalWorldAffineTransformMatrix(NMATRIX& outWorldMat, NMATRIX& outWorldInvTranspose) = delete;


	protected:

		//override SM init function. invoked by LightManager
		virtual bool mFunction_InitShadowMap(N_SHADOW_MAPPING_PARAM smParam) override final;

	private:

		friend class LightManager;//to init
		friend IFactory<DirLight>;

		DirLight();

		~DirLight();

		N_DirLightDesc mLightDesc;

	};


	//-----------------------Dynamic Point Light--------------------
	class PointLight : 
		public IBaseLight,
		public  ISceneObject
		//shadow map init not implemented
	{
	public:

		//local space
		void SetPosition(const NVECTOR3& pos);

		//local space
		NVECTOR3 GetPostion();

		//world space
		NVECTOR3 GetPosition_WorldSpace();

		void SetAttenuationFactor(float attFactor);

		void	SetLightingRange(float range);

		void SetDesc(const N_PointLightDesc& desc);//many CLAMP op happens in this

		N_PointLightDesc GetDesc();

		//get desc with geometric info transformed to world space
		N_PointLightDesc GetDesc_TransformedToWorld();


		//ISceneObject::
		virtual	N_AABB GetLocalAABB() override;

		//ISceneObject::
		virtual	N_AABB ComputeWorldAABB_Accurate() override;

		//ISceneObject::
		virtual N_AABB ComputeWorldAABB_Fast() override;

		//ISceneObject::
		virtual	NOISE_SCENE_OBJECT_TYPE GetObjectType() override;

	private:

		friend LightManager;
		friend IFactory<PointLight>;

		PointLight();

		~PointLight();

		N_PointLightDesc mLightDesc;
	};


	//-----------------------Dynamic Spot Light------------------
	class SpotLight:
		public IBaseLight,
		public ISceneObject
	{
	public:

		void SetPosition(const NVECTOR3& pos);

		NVECTOR3 GetPosition();

		NVECTOR3 GetPosition_WorldSpace();

		void SetAttenuationFactor(float attFactor);

		void	SetLookAt(const NVECTOR3& vLitAt);

		NVECTOR3 GetLookAt();

		NVECTOR3 GetLookAt_WorldSpace();

		void	SetLightingAngle(float coneAngle_Rad);

		void	SetLightingRange(float range);

		void SetDesc(const N_SpotLightDesc& desc);//many CLAMP op happens in this

		N_SpotLightDesc GetDesc();

		//get desc with geometric info transformed to world space
		N_SpotLightDesc GetDesc_TransformedToWorld();

		//ISceneObject::
		virtual	N_AABB GetLocalAABB() override;

		//ISceneObject::
		virtual	N_AABB ComputeWorldAABB_Accurate() override;

		//ISceneObject::
		virtual N_AABB ComputeWorldAABB_Fast() override;

		//ISceneObject::
		virtual	NOISE_SCENE_OBJECT_TYPE GetObjectType() override;

		//SceneNode::
		AffineTransform& GetLocalTransform() = delete;

		//SceneNode::
		NMATRIX EvalWorldAffineTransformMatrix() = delete;

		//SceneNode::
		void EvalWorldAffineTransformMatrix(NMATRIX& outWorldMat, NMATRIX& outWorldInvTranspose) = delete;

	private:

		friend LightManager;
		friend IFactory<SpotLight>;

		SpotLight();

		~SpotLight();

		N_SpotLightDesc mLightDesc;

	};


};