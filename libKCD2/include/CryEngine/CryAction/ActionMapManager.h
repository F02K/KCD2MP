/*************************************************************************
  Crytek Source File.
  Copyright (C), Crytek Studios, 2001-2004.
 -------------------------------------------------------------------------
  $Id$
  $DateTime$
  Description: Action manager and dispatcher.
  
 -------------------------------------------------------------------------
  History:
  - 7:9:2004   17:36 : Created by Márcio Martins
	- 15:9:2010  12:30 : Revised by Dean Claassen

*************************************************************************/
#ifndef __ACTIONMAPMANAGER_H__
#define __ACTIONMAPMANAGER_H__

#if _MSC_VER > 1000
# pragma once
#endif

#include "IActionMapManager.h"

class Crc32Gen;
class CActionMapAction;

typedef std::map<string, class CActionMap *>		TActionMapMap;
typedef std::map<string, class CActionFilter *>	TActionFilterMap;

//------------------------------------------------------------------------

class CConsoleActionListener : public IActionListener
{
public:
	void OnAction( const ActionId& actionId, int activationMode, float value )
	{
		IConsole * pCons = gEnv->pConsole;
		pCons->ExecuteString( actionId.c_str() );
	}
};

class CActionMapManager :
	public IActionMapManager,
	public IInputEventListener
{
public:
	CActionMapManager(IInput *pInput);

	void Release() { delete this; };

	// IInputEventListener
	virtual bool OnInputEvent(const SInputEvent &event);
	// ~IInputEventListener

	// IActionMapManager
	VIRTUAL void Update();
	VIRTUAL void Reset();
	VIRTUAL void Clear();

	VIRTUAL void SetLoadFromXMLPath(const char* szPath) { m_loadedXMLPath = szPath; }
	VIRTUAL const char* GetLoadFromXMLPath() const { return m_loadedXMLPath; }
	VIRTUAL bool LoadFromXML(const XmlNodeRef& node);
	VIRTUAL bool LoadRebindDataFromXML(const XmlNodeRef& node);
	VIRTUAL bool SaveRebindDataToXML(XmlNodeRef& node);

	VIRTUAL void AddExtraActionListener(IActionListener* pExtraActionListener);
	VIRTUAL void RemoveExtraActionListener(IActionListener* pExtraActionListener);
	VIRTUAL const TActionListeners& GetExtraActionListeners() const;

	VIRTUAL void AddAlwaysActionListener(TBlockingActionListener pActionListener); // TODO: Remove always action listeners and integrate into 1 prioritized type
	VIRTUAL void RemoveAlwaysActionListener(TBlockingActionListener pActionListener);
	VIRTUAL void RemoveAllAlwaysActionListeners();

	VIRTUAL IActionMap *CreateActionMap(const char *name);
	VIRTUAL bool RemoveActionMap(const char *name);
	VIRTUAL void RemoveAllActionMaps();
	VIRTUAL IActionMap *GetActionMap(const char *name);
	VIRTUAL const IActionMap *GetActionMap(const char *name) const;
	VIRTUAL IActionFilter *CreateActionFilter(const char *name, EActionFilterType type=eAFT_ActionFail);
	VIRTUAL IActionFilter *GetActionFilter(const char *name);
	VIRTUAL IActionMapIteratorPtr CreateActionMapIterator();
	VIRTUAL IActionFilterIteratorPtr CreateActionFilterIterator();

	VIRTUAL void Enable(const bool enable, const bool resetStateOnDisable = false);
	VIRTUAL void EnableActionMap(const char *name, bool enable);
	VIRTUAL void EnableFilter(const char *name, bool enable);
	VIRTUAL bool IsFilterEnabled(const char *name);
	VIRTUAL void ReleaseFilteredActions();
	VIRTUAL void ClearStoredCurrentInputData();

	VIRTUAL bool ReBindActionInput(const char* actionMapName, const ActionId& actionId, const char* szCurrentInput, const char* szNewInput);

	VIRTUAL const SActionInput* GetActionInput(const char* actionMapName, const ActionId& actionId, const EActionInputDevice device, const int iByDeviceIndex) const;

	VIRTUAL int GetVersion() const { return  m_version; }
	VIRTUAL void SetVersion(int version) { m_version = version; }
	VIRTUAL void EnumerateActions( IActionMapPopulateCallBack* pCallBack ) const;
	VIRTUAL int GetActionsCount() const;
	VIRTUAL int GetActionMapsCount() const {return m_actionMaps.size();}

	VIRTUAL bool AddInputDeviceMapping(const EActionInputDevice deviceType, const char* szDeviceTypeStr);
	VIRTUAL bool RemoveInputDeviceMapping(const EActionInputDevice deviceType);
	VIRTUAL void ClearInputDevicesMappings();
	VIRTUAL int GetNumInputDeviceData() const;
	VIRTUAL const SActionInputDeviceData* GetInputDeviceDataByIndex(const int iIndex);
	VIRTUAL const SActionInputDeviceData* GetInputDeviceDataByType(const EActionInputDevice deviceType);
	VIRTUAL const SActionInputDeviceData* GetInputDeviceDataByType(const char* szDeviceType);

	VIRTUAL void RemoveAllRefireData();
	// ~IActionMapManager

	bool ActionFiltered(const ActionId& action);

	void RemoveActionFilter(CActionFilter *pActionFilter);

	void ReleaseActionIfActive(const ActionId& actionId);

	bool AddBind(CActionMap* pActionMap, CActionMapAction* pAction, SActionInput* pActionInput);
	bool RemoveBind(CActionMap* pActionMap, CActionMapAction* pAction, SActionInput* pActionInput);
	void RemoveBind(CActionMap* pActionMap);
	void RemoveBind(CActionMapAction* pAction);
	bool HasBind(CActionMap* pActionMap, CActionMapAction* pAction, SActionInput* pActionInput) const;

	bool UpdateRefireData(const SInputEvent &event, CActionMap* pActionMap, CActionMapAction* pAction, SActionInput* pActionInput);
	bool RemoveRefireData(CActionMap* pActionMap, CActionMapAction* pAction, SActionInput* pActionInput);
	void RemoveRefireData(CActionMap* pActionMap);
	void RemoveRefireData(CActionMapAction* pAction);
	bool SetRefireDataDelayedPressNeedsRelease(const SInputEvent &event, CActionMap* pActionMap, CActionMapAction* pAction, SActionInput* pActionInput, const bool bDelayedPressNeedsRelease);
	void RemoveAllDelayedPressRefireData();
	int GetHighestPressDelayPriority() const;

	void GetMemoryStatistics(ICrySizer * s);

	ILINE bool IsCurrentlyRefiringInput() const { return m_bRefiringInputs; }
	ILINE bool IsIncomingInputRepeated() const { return m_bIncomingInputRepeated; }
	ILINE EKeyId GetIncomingInputKeyID() const { return m_currentInputKeyID; }
	ILINE void SetRepeatedInputHoldTriggerFired(const bool bFired) { m_bRepeatedInputHoldTriggerFired = bFired; }
	ILINE bool IsRepeatedInputHoldTriggerFired() const { return m_bRepeatedInputHoldTriggerFired; }

protected:
	virtual ~CActionMapManager();

private:

	TActionListeners m_ExtraActionListeners;

	struct SBindData
	{
		SBindData(CActionMap* pActionMap, CActionMapAction* pAction, SActionInput* pActionInput)
			: m_pActionMap(pActionMap)
			, m_pAction(pAction)
			, m_pActionInput(pActionInput)
		{
		}

		SActionInput*			m_pActionInput;
		CActionMapAction*	m_pAction;
		CActionMap*				m_pActionMap;
	};

	struct SRefireBindData
	{
		SRefireBindData(CActionMap* pActionMap, CActionMapAction* pAction, SActionInput* pActionInput)
			: m_bindData(pActionMap, pAction, pActionInput)
			, m_bIgnoreNextUpdate(false)
			, m_bDelayedPressNeedsRelease(false)
		{
		}

		SBindData					m_bindData;
		bool							m_bIgnoreNextUpdate; // Only used for refiring data since don't want to fire right after was added
		bool							m_bDelayedPressNeedsRelease;
	};

	typedef std::vector<SRefireBindData> TRefireBindData;
	struct SRefireData
	{
		SRefireData(const SInputEvent &event, CActionMap* pActionMap, CActionMapAction* pAction, SActionInput* pActionInput)
			: m_inputCRC(pActionInput->inputCRC)
			, m_inputEvent(event)
		{
			m_refireBindData.push_back(SRefireBindData(pActionMap, pAction, pActionInput));
		}

		uint32						m_inputCRC;
		SInputEvent				m_inputEvent; // Copy of actual event
		TRefireBindData		m_refireBindData;
	};

	typedef std::multimap<uint32, SBindData> TInputCRCToBind;
	typedef std::list<const SBindData*> TBindPriorityList;
	typedef std::list<TBlockingActionListener> TBlockingActionListeners;
	typedef std::vector<SActionInputDeviceData> TInputDeviceData;
	typedef std::map<uint32, SRefireData*> TInputCRCToRefireData;

	struct SRefireReleaseListData
	{
		SInputEvent m_inputEvent;
		TBindPriorityList	m_inputsList;
	};

	bool HandleAcceptedEvents(const SInputEvent &event, TBindPriorityList& priorityList);
	void HandleInputBlocking(const SInputEvent &event, const SActionInput* pActionInput, const float fCurrTime);
	SBindData* GetBindData(CActionMap* pActionMap, CActionMapAction* pAction, SActionInput* pActionInput);
	bool CreateEventPriorityList(const SInputEvent &inputEvent, TBindPriorityList& priorityList);
	bool CreateRefiredEventPriorityList(SRefireData* pRefireData, 
																			TBindPriorityList& priorityList, 
																			TBindPriorityList& removeList, 
																			TBindPriorityList& delayPressNeedsReleaseList);
	bool ProcessAlwaysListeners(const ActionId& action, int activationMode, float value, const SInputEvent &inputEvent);
	void SetCurrentlyRefiringInput(bool bRefiringInput) { m_bRefiringInputs = bRefiringInput; }
	void UpdateRefiringInputs();

	string										m_loadedXMLPath;
	IInput*										m_pInput;
	TActionMapMap							m_actionMaps;
	TActionFilterMap					m_actionFilters;
	TInputCRCToBind						m_inputCRCToBind;
	TInputCRCToRefireData			m_inputCRCToRefireData;
	TBlockingActionListeners	m_alwaysActionListeners; 
	TInputDeviceData					m_inputDeviceData;
	EKeyId										m_currentInputKeyID; // Keep track to determine if is a repeated input
	int												m_version;

#ifndef _RELEASE
	int                                             i_listActionMaps; // cvar
#endif

	bool											m_enabled;
	bool											m_bRefiringInputs;
	bool											m_bDelayedRemoveAllRefiringData; // This only gets set true while m_bRefiringInputs == true and something is disabling an action filter based on a refired input (Crashes if try to do this while iterating data)
	bool											m_bIncomingInputRepeated; // Input currently incoming is a repeated input
	bool											m_bRepeatedInputHoldTriggerFired; // Input currently incoming already fired initial hold trigger
};


#endif //__ACTIONMAPMANAGER_H__
