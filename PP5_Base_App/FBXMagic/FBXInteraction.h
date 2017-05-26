#pragma once
#ifdef FBXMAGIC_EXPORTS  
#define FBXINTERACTION_API __declspec(dllexport)   
#else  
#define FBXINTERACTION_API __declspec(dllimport)   
#endif

//#include <fbxsdk.h>
#include <vector>

//namespace fbxsdk
//{
	//class FBXManager;
//}

namespace FBXinteracts
{
	struct vert {
		float Position[4];
		float Color[4];
		float Normals[4];
		int BoneId[4];
		float Weights[4];
	};

	struct KeyFrame {

		double time;
		std::vector<vert> bones;
	};

	struct AnimClip{

		double lenght;
		std::vector<KeyFrame> keys;
	};
	
	AnimClip animation;
	//using namespace std;
	// This class is exported from the MathLibrary.dll  
	class Functions
	{
	private:
		
	public:
		
		// Returns a + b  
		static FBXINTERACTION_API int  GimmeSomething();

		static FBXINTERACTION_API void SetupFBX();


		static FBXINTERACTION_API float getverts(unsigned int indicesVec, unsigned int indicesArr);

		static FBXINTERACTION_API unsigned int getvertsSize();

		static FBXINTERACTION_API unsigned int getPositionsSize();

		FBXINTERACTION_API std::vector<uint32_t> getIndices();

		static FBXINTERACTION_API float getPositions(unsigned int indicesVec);

		static FBXINTERACTION_API AnimClip getAnimation() { return animation; }

		FBXINTERACTION_API std::vector<vert> getBecky();

	};
}

