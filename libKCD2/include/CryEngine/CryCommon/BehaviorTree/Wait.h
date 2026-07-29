#pragma once

#include "Action.h"
#include "Timer.h"

namespace BehaviorTree
{
	// Returns success after X seconds
 	class Wait : public Action
 	{
	public:
		struct RuntimeData
		{
			Timer timer;
		};
		
		Wait()
			: m_duration(0.0f)
			, m_variation(0.0f)
		{
		}

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context)
		{
			if (Action::LoadFromXml(xml, context) == LoadFailure)
			{
				// TODO: Report error
				return LoadFailure;
			}

			if(!xml->getAttr("duration",m_duration))
			{
				// TODO: Report error
				return LoadFailure;			
			}

			xml->getAttr("variation", m_variation);

			return LoadSuccess;
		}

		virtual void OnInitialize(const UpdateContext& context)
		{
			if (m_duration >= 0.0f)
			{
				RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
				runtimeData.timer.Reset(m_duration, m_variation);
			}
		}

		virtual Status Update(const UpdateContext& context)
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			return runtimeData.timer.Elapsed() ? Success : Running;
		}

// 		#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
// 		virtual void GetDebugTextForVisualizer(const UpdateContext& updateContext, stack_string& debugText) const
// 		{
// 			debugText.Format("%0.1f (%0.1f)", m_timer.GetSecondsLeft(), m_duration);
// 		}
// 		#endif

	private:
		float m_duration;
		float m_variation;
 	};

	GenerateBehaviorTreeNodeCreator(Wait);
}
