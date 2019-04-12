
/***********************************************************

							GI: Soft shader interface
		define some pure virtual function or something like that
		to allow users to override the interface like programmable 
		graphics pipeline do. Then we can write c++ functions while
		regarding them as 'soft shaders'.
		(2019.3.31)in D3D12/DXR, there are 5 shaders in
		ray tracing pipeline: 
		**1. ray generation shader (impl in PathTracer class)
		**2. any hit shader (for alpha mask, but doesn't intend to impl)
		**3. intersection shader (impl in CollisionTestor)
		**4. closest hit shader
		**5.  miss shader

**************************************************************/

#pragma once

namespace Noise3D
{
	namespace GI
	{
		class /*_declspec(dllexport)*/ IPathTracerSoftShader
		{
		public:

			IPathTracerSoftShader() :
				m_pFatherPathTracer(nullptr) {}

			//1. FireRay() (impl in PathTracer class)

			//2. AnyHit() (for alpha mask, but doesn't intend to impl)

			//3. Intersect() (impl in CollisionTestor)

			//4. closest hit, most material/lighting/texturing stuffs will happen here
			virtual void ClosestHit(int diffuseBounces, int specularBounces, float travelledDistance, const N_Ray& ray, const N_RayHitInfoForPathTracer& hitInfo, N_TraceRayPayload& in_out_payload)=0;

			//5. doesn't hit anything, might want to sample the skydome/skybox cubemap or sth
			virtual void Miss(N_Ray ray, N_TraceRayPayload& in_out_payload)=0;

		protected:

			friend void PathTracer::Render(Noise3D::SceneNode*, IPathTracerSoftShader*);

			void _InitInfrastructure(PathTracer* pt, CollisionTestor* ct) 
			{
				m_pFatherPathTracer = pt; 
				m_pCollisionTestor = ct;
			}

			void _TraceRay(int diffuseBounces, int specularBounces, float travelledDistance, const N_Ray& ray, N_TraceRayPayload& payload)
			{
				//distance will be accumulated automatically in PathTracer::TraceRay()
				//diffuse/specular bounces should be updated manually
				m_pFatherPathTracer->TraceRay(diffuseBounces, specularBounces, travelledDistance, ray, payload);
			}

			uint32_t _MaxDiffuseBounces()
			{
				return m_pFatherPathTracer->GetMaxDiffuseBounces();
			}

			uint32_t _MaxDiffuseSample()
			{
				return m_pFatherPathTracer->GetMaxDiffuseSampleCount();
			}

			uint32_t _MaxSpecularScatterBounces()
			{
				return m_pFatherPathTracer->GetMaxSpecularScatterBounces();
			}

			float _MaxDistance()
			{
				return m_pFatherPathTracer->GetRayMaxTravelDist();
			}

		private:

			PathTracer* m_pFatherPathTracer;

			CollisionTestor* m_pCollisionTestor;

		};
	}
}