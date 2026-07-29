#pragma once

#include "Action.h"
#include "Timer.h"

namespace BehaviorTree
{
 	// Returns failure after X seconds
 	class Timeout : public Action
 	{
 	public:
  	struct RuntimeData
  	{
			Timer timer;
  	};
  	
  	Timeout()
			: m_duration(0.0f)
  	{
  	}

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context) OVERRIDE
		{
			if (Action::LoadFromXml(xml, context) == LoadFailure)
			{
				// TODO: Report error
				return LoadFailure;
			}

			if (!xml->getAttr("duration", m_duration))
			{
				// TODO: Report error
				return LoadFailure;
			}

			return LoadSuccess;
		}
 
 		virtual void OnInitialize(const UpdateContext& context)
 		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			runtimeData.timer.Reset(m_duration);
 		}
 
 		virtual Status Update(const UpdateContext& context)
 		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			return runtimeData.timer.Elapsed() ? Failure : Running;
 		}

// 		#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
// 		virtual void GetDebugTextForVisualizer(const UpdateContext& updateContext, stack_string& debugText) const
// 		{
// 			debugText.Format("%0.1f (%0.1f)", m_timer.GetSecondsLeft(), m_duration);
// 		}
// 		#endif
 
 	private:
		float m_duration;
	};

	GenerateBehaviorTreeNodeCreator(Timeout);
}