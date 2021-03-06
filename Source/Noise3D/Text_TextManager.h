/***********************************************************************

					h��NoiseFontLoader

************************************************************************/

#pragma once

namespace Noise3D
{
		struct N_Font_Bitmap
		{
			UINT width;
			UINT height;
			std::vector<NColor4u> bitmapBuffer;
		};

		struct N_FontObject
		{
			N_FontObject();

			~N_FontObject();

			FT_Face		mFtFace;
			UINT			mFontSize;
			float				mAspectRatio;
			std::string		mInternalTextureName;//name in font mgr texture mgr
			std::vector	<NVECTOR2>	mAsciiCharSizeList;//elements will be added in GetBitmapOfChar

		};


		class /*_declspec(dllexport)*/ TextManager
			:public IFactory<N_FontObject>,
			public IFactory<DynamicText>,
			public IFactory<StaticText>
		{
		public:

			static const int c_FontAsciiBitmapTableRowCount;

			static const int c_FontAsciiBitmapTableColumnCount ;

			bool		CreateFontFromFile(NFilePath filePath,N_UID fontName, UINT fontSize, float fontAspectRatio = 0.707f);

			bool		SetFontSize(N_UID fontName, UINT  fontSize);

			bool		IsFontExisted(N_UID fontName);

			StaticText*			CreateStaticTextA(N_UID fontName,N_UID textObjectName,std::string contentString, UINT boundaryWidth, UINT boundaryHeight, NVECTOR4 textColor=NVECTOR4(0,0,0,0), int wordSpacingOffset=0, int lineSpacingOffset=0);

			StaticText*			CreateStaticTextW(N_UID fontName,  N_UID textObjectName,std::wstring contentString, UINT boundaryWidth, UINT boundaryHeight, NVECTOR4 textColor = NVECTOR4(0, 0, 0, 0), int wordSpacingOffset=0, int lineSpacingOffset=0);

			DynamicText*		CreateDynamicTextA(N_UID fontName, N_UID textObjectName, std::string contentString, UINT boundaryWidth, UINT boundaryHeight, NVECTOR4 textColor = NVECTOR4(0, 0, 0, 0), int wordSpacingOffset=0, int lineSpacingOffset=0);

			NVECTOR2		GetFontSize(N_UID fontName);

			bool		DeleteFont(N_UID fontName);

			void			DeleteAllFont();

			bool		DeleteStaticText(N_UID textName);

			bool		DeleteStaticText(StaticText* pText);

			bool		DeleteDynamicText(N_UID textName);

			bool		DeleteDynamicText(DynamicText* pText);

			void			DeleteAllTexts();

			void			DeleteAllFonts();

		private:
			//init freetype library and internal objects , invoked by SceneManager
			bool	NOISE_MACRO_FUNCTION_EXTERN_CALL mFunction_Init(TextureManager* in_created_pTexMgr, GraphicObjectManager* in_created_pGObjMgr);
			//get bitmap of a single WCHAR
			void			mFunction_GetBitmapOfWChar(N_FontObject& fontObj, wchar_t targetWChar, N_Font_Bitmap& outFontBitmap, NVECTOR4 textColor);
			//create Bitmap Table of a w-char string (combining char pixel blocks)
			void			mFunction_GetBitmapOfWString(N_FontObject& fontObj, std::wstring targetString, UINT boundaryWidth, UINT boundaryHeight, NVECTOR4 textColor, N_Font_Bitmap & outFontBitmap, int wordSpacingOffset, int lineSpacingOffset);
			//Use GetBitmapOfString() to create ascii bitmap table, then Ascii char can be dynamically refered via texture coordinate
			bool		mFunction_CreateTexture_AsciiBitmapTable(N_FontObject& fontObj,std::string fontName, UINT charWidth, UINT charHeight);

		private:
			friend	class SceneManager;//create internal object
			friend  class IRenderModuleForText;
			friend  DynamicText;
			friend	IFactory<TextManager>;

			TextManager();

			~TextManager();

			TextureManager*				m_pTexMgr;//created by SceneManager, internal texture manager (ascii bitmap table/static Bitmap)
			
			GraphicObjectManager*	m_pGraphicObjMgr;//Created by SceneManager, assign GObj to every TextObj

			FT_Library					m_FTLibrary;
			bool							mIsFTInitialized;

		};
}