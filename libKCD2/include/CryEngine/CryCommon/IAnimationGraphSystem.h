#ifndef __IANIMATIONGRAPHSYSTEM_H__
#define __IANIMATIONGRAPHSYSTEM_H__

#pragma once

#include <IEntitySystem.h> // <> required for Interfuscator


struct IAnimationGraphCategoryIterator
{
	struct SCategory
	{
		const char * name;
		bool overridable;
    bool usableWithTemplate;
	};
	
	// <interfuscator:shuffle>
	virtual	~IAnimationGraphCategoryIterator(){}
	virtual void AddRef() = 0;
	virtual void Release() = 0;
	
	virtual bool Next( SCategory& ) = 0;
	// </interfuscator:shuffle>
};
typedef _smart_ptr<IAnimationGraphCategoryIterator> IAnimationGraphCategoryIteratorPtr;

typedef bool (*AnimationGraphUpgradeFunction)(XmlNodeRef node);

struct IAnimationGraphStateFactoryIterator
{
	struct SStateFactoryParameter
	{
		bool required;
		const char * type;
		const char * name;
		const char * humanName;
		const char * defaultValue;
		AnimationGraphUpgradeFunction upgradeFunction;
	};

	struct SStateFactory
	{
		const char * name;
		const char * humanName;
		const char * category;
		const SStateFactoryParameter * pParams;
	};
	
	// <interfuscator:shuffle>
	virtual	~IAnimationGraphStateFactoryIterator(){}
	virtual void AddRef() = 0;
	virtual void Release() = 0;

	virtual bool Next( SStateFactory& ) = 0;
	// </interfuscator:shuffle>
};
typedef _smart_ptr<IAnimationGraphStateFactoryIterator> IAnimationGraphStateFactoryIteratorPtr;

struct IAnimationGraphStateIterator
{
	struct SState
	{
		const char * name;
	};

	// <interfuscator:shuffle>
	virtual	~IAnimationGraphStateIterator(){}
	virtual void AddRef() = 0;
	virtual void Release() = 0;

	virtual bool Next( SState& ) = 0;
	// </interfuscator:shuffle>
};
typedef _smart_ptr<IAnimationGraphStateIterator> IAnimationGraphStateIteratorPtr;

struct IAnimationGraphInputs
{
	enum InputType
	{
		eAGIT_Integer,
		eAGIT_Float,
		eAGIT_String
	};

	struct SInput
	{
		// <interfuscator:shuffle>
		virtual	~SInput(){}
		virtual InputType GetType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetValueCount() const = 0;
		virtual const char* GetValue(int) const= 0;
		// </interfuscator:shuffle>
	};

	// <interfuscator:shuffle>
	virtual	~IAnimationGraphInputs(){}
	virtual int GetNumInputs() = 0;
	virtual const SInput* GetInput(int index) = 0;

	virtual void AddRef() = 0;
	virtual void Release() = 0;
	// </interfuscator:shuffle>
};
typedef _smart_ptr<IAnimationGraphInputs> IAnimationGraphInputsPtr;

struct IAnimationGraphDeadInputReportCallback
{
	// <interfuscator:shuffle>
	virtual	~IAnimationGraphDeadInputReportCallback(){}
	virtual void OnDeadInputValues( std::pair<const char*, const char*>* pInputValues ) = 0;
	// </interfuscator:shuffle>
};

struct IAnimationGraphQueryResults
{
	struct IResult
	{
		// <interfuscator:shuffle>
		virtual	~IResult(){}
		virtual const char* GetValue() const = 0;
		virtual const char* GetValueNoMixIns() const = 0;
		virtual int GetRanking() const = 0;
		// </interfuscator:shuffle>
	};

	// <interfuscator:shuffle>
	virtual	~IAnimationGraphQueryResults(){}
	virtual int GetHighestRankingValue() const = 0;
	virtual int GetNumResults() const = 0;
	virtual const IResult * GetResult( int index ) const = 0;
	virtual const IResult * GetResult( const char * value ) const = 0;
	virtual bool Contains( const char * value, bool bIgnoreMixIns=true ) const = 0;

	virtual void AddRef() = 0;
	virtual void Release() = 0;
	// </interfuscator:shuffle>
};
typedef _smart_ptr<IAnimationGraphQueryResults> IAnimationGraphQueryResultsPtr;

struct IAnimationGraphQueryInputs
{
	enum InputType
	{
		eAGQIT_Invalid,
		eAGQIT_Integer,
		eAGQIT_Float,
		eAGQIT_String
	};

	struct SInput
	{
		SInput( const char *name ) : eType( eAGQIT_Invalid ), iValue( 0 ), fValue( 0.f ), bAnyValue( false ) 
		{
			strcpy_s(	sName, name );
			sValue[0] = '\0'; // empty string
		}

		InputType eType;
		char sName[64];
		char sValue[64];
		int iValue;
		float fValue;
		bool bAnyValue;
	};

	// <interfuscator:shuffle>
	virtual	~IAnimationGraphQueryInputs(){}
	virtual int GetNumInputs() const = 0;
	virtual const SInput* GetInput(int index) const  = 0;
	virtual void AddInput( const SInput& input ) = 0;
	virtual void Clear() = 0;

	virtual void AddRef() = 0;
	virtual void Release() = 0;
	// </interfuscator:shuffle>
};
typedef _smart_ptr<IAnimationGraphQueryInputs> IAnimationGraphQueryInputsPtr;

struct IAnimationGraphSystem
{
	// <interfuscator:shuffle>
	virtual	~IAnimationGraphSystem(){}
	virtual IAnimationGraphCategoryIteratorPtr CreateCategoryIterator() = 0;
	virtual IAnimationGraphStateFactoryIteratorPtr CreateStateFactoryIterator() = 0;
	virtual bool TrialAnimationGraph( const char * name, XmlNodeRef animGraph, bool loadBinary = false) = 0;
	virtual void FindDeadInputValues( IAnimationGraphDeadInputReportCallback * pCallback, XmlNodeRef animGraph ) = 0;

	virtual void SetAnimationGraphActivation( EntityId id, bool bActivation ) = 0;

	virtual IAnimationGraphStateIteratorPtr CreateStateIterator( EntityId id) = 0;
	virtual IAnimationGraphInputsPtr RetrieveInputs( EntityId id) = 0;
	virtual void ExportXMLToBinary( const char * filename, XmlNodeRef animGraph ) = 0;

	// We allow a separate name and filename so the animgraph can be loaded without overriding any graphs
	// currently being used in the engine
	virtual bool LoadAnimationGraph( const char * name, const char * filename, XmlNodeRef animGraph, bool loadBinary=false ) = 0;
	virtual bool IsAnimationGraphLoaded( const char * name ) = 0;

	virtual IAnimationGraphQueryInputsPtr GetAGQueryInputsObject() const = 0;
	virtual IAnimationGraphQueryResultsPtr QueryAnimationGraph( const char * name, const IAnimationGraphQueryInputs * inputs ) = 0;
	// </interfuscator:shuffle>
};


#endif
