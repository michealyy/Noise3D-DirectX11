
/***********************************************************************

								Random Sample Generator

************************************************************************/

#pragma once

namespace Noise3D
{
	namespace GI
	{
		class RandomSampleGenerator
		{
		public:

			RandomSampleGenerator();

			//generate canonical real number(uniformly distribute in [0,1])
			float CanonicalReal();

			//generate uniform random unit vector which distributes on unit sphere
			NVECTOR3 UniformSpherical_Vector();

			//generate uniform random unit vector's( which distributes on unit sphere)azimulthal (vec.x==theta==pitch, vec.y==phi==yaw)
			NVECTOR2 UniformSpherical_Azimulthal();

		private:

			static std::default_random_engine mRandomEngine;

			static std::uniform_real_distribution<float> mCanonicalDist;
		};
	}
};
