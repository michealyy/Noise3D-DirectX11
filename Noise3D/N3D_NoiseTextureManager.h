/***********************************************************************

                           h��NoiseTextureManager

************************************************************************/

#pragma once

struct N_TextureObject
{
	N_TextureObject() { ZeroMemory(this,sizeof(*this));}
	std::string	 mTexName;
	std::vector<NVECTOR4>	mPixelBuffer;
	BOOL	mIsPixelBufferInMemValid;
	ID3D11ShaderResourceView* m_pSRV;
};

public class _declspec(dllexport) NoiseTextureManager
{
public:
	friend NoiseScene;
	friend NoiseRenderer;
	friend NoiseMesh;

	//���캯��
	NoiseTextureManager();

	void		SelfDestruction();

	//BOOL	SetPixel(UINT texID, UINT x, UINT y, NVECTOR4 color);

	UINT		CreateTextureFromFile(const LPCWSTR filePath, const char* textureName, BOOL useDefaultSize, UINT pixelWidth, UINT pixelHeight, BOOL keepCopyInMemory=FALSE);

	UINT		CreateCubeMapFromFiles(const LPCWSTR fileName[6], const  char* cubeTextureName,NOISE_CUBEMAP_SIZE faceSize);

	UINT		CreateCubeMapFromDDS(const LPCWSTR dds_FileName, const char * cubeTextureName, NOISE_CUBEMAP_SIZE faceSize);

	BOOL	ConvertTextureToGreyMap(UINT texID);

	BOOL	ConvertTextureToGreyMapEx(UINT texID,float factorR,float factorG,float factorB);

	BOOL	ConvertHeightMapToNormalMap(UINT texID,float heightFieldScaleFactor=10.0f);

	UINT		GetIndexByName(const char* textureName);

	BOOL	SaveTextureToFile(UINT texID,const LPCWSTR filePath,NOISE_TEXTURE_SAVE_FORMAT picFormat);

	void		GetNameByIndex(UINT index, std::string* outTextureName);

	UINT		GetTextureWidth(UINT texID);

	UINT		GetTextureHeight(UINT texID);

	UINT		GetTextureCount();

	BOOL	DeleteTexture(UINT texID);

private:
	UINT		NOISE_MACRO_FUNCTION_EXTERN_CALL mFunction_ValidateTextureID(UINT texID, NOISE_TEXTURE_TYPE texType);

	UINT		mFunction_CreateTextureFromFile_DirectlyLoadToGpu(const LPCWSTR filePath, const  char* textureName, BOOL useDefaultSize, UINT pixelWidth, UINT pixelHeight);

	UINT		mFunction_CreateTextureFromFile_KeepACopyInMemory(const LPCWSTR filePath, const  char* textureName, BOOL useDefaultSize, UINT pixelWidth, UINT pixelHeight);

	UINT		mFunction_GetPixelIndexFromXY(UINT x, UINT y,UINT width);

private:
	NoiseScene*									m_pFatherScene;
	std::vector<N_TextureObject>*	m_pTextureObjectList;

};