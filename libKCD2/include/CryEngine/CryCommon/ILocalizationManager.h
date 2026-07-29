#include DEVIRTUALIZE_HEADER_FIX(ILocalizationManager.h)

#ifndef _CRY_ILOCALIZATIONMANAGER_H_
#define _CRY_ILOCALIZATIONMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <CrySizer.h>
//#include <platform.h> // Needed for LARGE_INTEGER (for consoles).

////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
////////////////////////////////////////////////////////////////////////////////////////////////

class XmlNodeRef;

//////////////////////////////////////////////////////////////////////////
// Localized strings manager interface.
//////////////////////////////////////////////////////////////////////////
	
// Localization Info structure
	struct SLocalizedInfoGame
	{

		SLocalizedInfoGame ()
			: szCharacterName(NULL)
			, bUseSubtitle(false)
		{

		}

		const char* szCharacterName;
		wstring swTranslatedText;

		bool bUseSubtitle;
	};

	struct SLocalizedAdvancesSoundEntry
	{
		string	sName;
		float		fValue;
		void GetMemoryUsage( ICrySizer *pSizer ) const
		{						
			pSizer->AddObject(sName);
		}
	};	

	// Localization Sound Info structure, containing sound related parameters.
	struct SLocalizedSoundInfoGame : public SLocalizedInfoGame
	{
		SLocalizedSoundInfoGame()
			: sSoundEvent(NULL)
			, fVolume(0.f)
			, fRadioRatio (0.f)
			, bIsDirectRadio(false)
			, bIsIntercepted(false)
			, nNumSoundMoods(0)
			, pSoundMoods (NULL)
			, nNumEventParameters(0)
			, pEventParameters(NULL)
		{
			
		}

		const char* sSoundEvent;
		float fVolume;
		float fRadioRatio;
		bool	bIsDirectRadio;
		bool	bIsIntercepted;

		// SoundMoods.
		int			nNumSoundMoods;
		SLocalizedAdvancesSoundEntry* pSoundMoods;

		// EventParameters.
		int			nNumEventParameters;
		SLocalizedAdvancesSoundEntry* pEventParameters;
	};

	// Localization Sound Info structure, containing sound related parameters.
	struct SLocalizedInfoEditor : public SLocalizedInfoGame
	{
		SLocalizedInfoEditor()
			: sKey(NULL)
			, sOriginalCharacterName(NULL)
			, sOriginalActorLine(NULL)
			, swTranslatedActorLine(NULL)
			, nRow(0)
		{

		}

		const char* sKey;
		const char* sOriginalCharacterName;
		const char* sOriginalActorLine;
		const wchar_t* swTranslatedActorLine;
		unsigned int nRow;
	};

UNIQUE_IFACE struct ITempLocStringStorage
{
	// <interfuscator:shuffle>
	virtual ~ITempLocStringStorage(){}
	virtual void Release() = 0;
	virtual const wchar_t* GetCStr() const = 0;
	// </interfuscator:shuffle>
};

// Summary: 
//		Interface to the Localization Manager.
UNIQUE_IFACE struct ILocalizationManager
{
	//Platform independent language IDs. These are used to map the platform specific language codes on the PS3 and 360 to localization pakfiles
	//Please ensure that each entry in this enum has a corresponding entry in the PLATFORM_INDEPENDENT_LANGUAGE_NAMES array which is defined in LocalizedStringManager.cpp currently.
	enum EPlatformIndependentLanguageID
	{
		ePILID_Japanese,
		ePILID_English,
		ePILID_French,
		ePILID_Spanish,
		ePILID_German,
		ePILID_Italian,
		ePILID_Dutch,
		ePILID_Portuguese,
		ePILID_Russian,
		ePILID_Korean,
		ePILID_ChineseT,	// Traditional Chinese
		ePILID_ChineseS,	// Simplified Chinese
		ePILID_Finnish,
		ePILID_Swedish,
		ePILID_Danish,
		ePILID_Norwegian,
		ePILID_Polish,
		ePILID_Arabic,		//Test value for PS3. Not currently supported by Sony on the PS3 as a system language
		ePILID_Czech,
		ePILID_Turkish,
		ePILID_MAX_OR_INVALID	//Not a language, denotes the maximum number of languages or an unknown language
	};

	typedef uint32 TLocalizatonBitfield;

	// <interfuscator:shuffle>
	virtual ~ILocalizationManager(){}
	virtual const char* LangNameFromPILID( const ILocalizationManager::EPlatformIndependentLanguageID id ) = 0;
	virtual ILocalizationManager::TLocalizatonBitfield MaskSystemLanguagesFromSupportedLocalizations( const ILocalizationManager::TLocalizatonBitfield systemLanguages ) = 0;
	virtual ILocalizationManager::TLocalizatonBitfield IsLanguageSupported( const ILocalizationManager::EPlatformIndependentLanguageID id ) = 0;
	virtual void SetAvailableLocalizationsBitField( const ILocalizationManager::TLocalizatonBitfield availableLocalizations ) = 0;
	virtual void SetAvailableLocalizationsCSVString( const char *sAvailableLocalizations ) = 0;
	virtual bool SetLanguage( const char* sLanguage ) = 0;
	virtual const char* GetLanguage() = 0;

	// load the descriptor file with tag information
	virtual bool InitLocalizationData( const char* sFileName, bool bReload=false) = 0;
	// request to load loca data by tag. Actual loading will happen during next level load begin event.
	virtual bool RequestLoadLocalizationDataByTag(const char* sTag) = 0;
	// direct load of loca data by tag
	virtual bool LoadLocalizationDataByTag(const char* sTag, bool bReload=false) = 0;
	virtual bool ReleaseLocalizationDataByTag(const char* sTag) = 0;

	virtual bool LoadExcelXmlSpreadsheet( const char* sFileName, bool bReload=false ) = 0;
	virtual void ReloadData() = 0;

	// Summary:
	//   Free localization data.
	virtual	void FreeData() = 0;

	// Summary:
	//   Translate a string into the currently selected language.
	// Description:
	//   Processes the input string and translates all labels contained into the currently selected language.
	// Parameters:
	//   sString             - String to be translated.
	//   outLocalizedString  - Translated version of the string.
	//   bEnglish            - if true, translates the string into the always present English language.
	// Returns:
	//   true if localization was successful, false otherwise
	virtual bool LocalizeString( const char* sString, wstring& outLocalizedString, bool bEnglish=false ) = 0;

	// Summary:
	//   Same as LocalizeString( const char* sString, wstring& outLocalizedString, bool bEnglish=false )
	//   but at the moment this is faster.
	virtual bool LocalizeString( const string& sString, wstring& outLocalizedString, bool bEnglish=false ) = 0;

	// Summary:
	//   Return the localized version corresponding to a label.
	// Description:
	//   A label has to start with '@' sign.
	// Parameters:
	//   sLabel              - Label to be translated, must start with '@' sign.
	//   outLocalizedString  - Localized version of the label.
	//   bEnglish            - if true, returns the always present English version of the label.
	// Returns:
	//   True if localization was successful, false otherwise.
	virtual bool LocalizeLabel( const char* sLabel, wstring& outLocalizedString, bool bEnglish=false, ITempLocStringStorage** ppTmpStorage = 0 ) = 0;

	// Summary:
	//   Get localization info structure corresponding to a key (key=label without the '@' sign).
	// Parameters:
	//   sKey    - Key to be looked up. Key = Label without '@' sign.
	//   outGameInfo - Reference to localization info structure to be filled in.
	//  Returns:
	//    True if info for key was found, false otherwise.
	virtual bool GetLocalizedInfoByKey( const char* sKey, SLocalizedInfoGame& outGameInfo ) = 0;

	// Summary:		
	//   Get the sound localization info structure corresponding to a key.
	// Parameters:
	//   sKey         - Key to be looked up. Key = Label without '@' sign.
	//   outSoundInfo - reference to sound info structure to be filled in
	//									pSoundMoods requires nNumSoundMoods-times allocated memory
	//									on return nNumSoundMoods will hold how many SoundsMood entries are needed
	//									pEventParameters requires nNumEventParameters-times allocated memory
	//									on return nNumEventParameters will hold how many EventParameter entries are needed
	//									Passing 0 in the Num fields will make the query ignore checking for allocated memory
	// Returns:
	//   True if successful, false otherwise (key not found, or not enough memory provided to write additional info).
	virtual bool GetLocalizedInfoByKey( const char* sKey, SLocalizedSoundInfoGame* pOutSoundInfo ) = 0;

	// Summary:
	//   Return number of localization entries.
	virtual int  GetLocalizedStringCount() = 0;
	
	// Summary:
	//   Get the localization info structure at index nIndex.
	// Parameters:
	//   nIndex  - Index.
	//   outEditorInfo - Reference to localization info structure to be filled in.
	// Returns:
	//   True if successful, false otherwise (out of bounds).
	virtual bool GetLocalizedInfoByIndex( int nIndex, SLocalizedInfoEditor& outEditorInfo ) = 0;

	// Summary:
	//   Get the localization info structure at index nIndex.
	// Parameters:
	//   nIndex  - Index.
	//   outGameInfo - Reference to localization info structure to be filled in.
	// Returns:
	//   True if successful, false otherwise (out of bounds).
	virtual bool GetLocalizedInfoByIndex( int nIndex, SLocalizedInfoGame& outGameInfo ) = 0;

	// Summary:
	//   Get the english localization info structure corresponding to a key.
	// Parameters:
	//   sKey         - Key to be looked up. Key = Label without '@' sign.
	//   sLocalizedString - Corresponding english language string.
	// Returns:
	//   True if successful, false otherwise (key not found).
	virtual bool GetEnglishString( const char *sKey, string &sLocalizedString ) = 0;
	
	// Summary:
	//   Get Subtitle for Key or Label .
	// Parameters:
	//   sKeyOrLabel    - Key or Label to be used for subtitle lookup. Key = Label without '@' sign.
	//   outSubtitle    - Subtitle (untouched if Key/Label not found).
	//   bForceSubtitle - If true, get subtitle (sLocalized or sEnglish) even if not specified in Data file.
	// Returns:
	//   True if subtitle found (and outSubtitle filled in), false otherwise.
	virtual bool GetSubtitle( const char* sKeyOrLabel, wstring& outSubtitle, bool bForceSubtitle = false) = 0;

	// Description:
	//		These methods format outString depending on sString with ordered arguments
	//		FormatStringMessage(outString, "This is %2 and this is %1", "second", "first");
	// Arguments:
	//		outString - This is first and this is second.
	virtual void FormatStringMessage( string& outString, const string& sString, const char** sParams, int nParams ) = 0;
	virtual void FormatStringMessage( string& outString, const string& sString, const char* param1, const char* param2=0, const char* param3=0, const char* param4=0 ) = 0;
	virtual void FormatStringMessage( wstring& outString, const wstring& sString, const wchar_t** sParams, int nParams ) = 0;
	virtual void FormatStringMessage( wstring& outString, const wstring& sString, const wchar_t* param1, const wchar_t* param2=0, const wchar_t* param3=0, const wchar_t* param4=0 ) = 0;

	virtual wchar_t ToUpperCase(wchar_t c) = 0;
	virtual wchar_t ToLowerCase(wchar_t c) = 0;
	virtual void LocalizeTime(time_t t, bool bMakeLocalTime, bool bShowSeconds, wstring& outTimeString) = 0;
	virtual void LocalizeDate(time_t t, bool bMakeLocalTime, bool bShort, bool bIncludeWeekday, wstring& outDateString) = 0;
	virtual void LocalizeDuration(int seconds, wstring& outDurationString) = 0;
	virtual void LocalizeNumber(int number, wstring& outNumberString) = 0;
	virtual void LocalizeNumber(float number, int decimals, wstring& outNumberString) = 0;
	// </interfuscator:shuffle>

	static ILINE TLocalizatonBitfield LocalizationBitfieldFromPILID( EPlatformIndependentLanguageID pilid )
	{
		assert( pilid >= 0 && pilid < ePILID_MAX_OR_INVALID );
		return ( 1 << pilid );
	}
};

#endif //_CRY_ILOCALIZATIONMANAGER_H_
