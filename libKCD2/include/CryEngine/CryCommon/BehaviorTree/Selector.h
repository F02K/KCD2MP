#pragma once

#ifndef Selector_h
#define Selector_h

#include "Composite.h"

namespace BehaviorTree
{
	// Executes children one at a time in order.
	// Succeeds if child succeeds, otherwise tries next.
	// Fails when all children failed.
	class Selector : public CompositeWithChildLoader
	{
	public:
		typedef uint8 ChildIndexType;

		struct RuntimeData
		{
			ChildIndexType currentChildIndex;

			RuntimeData() : currentChildIndex(0) {}
		};
		
	protected:
		virtual void OnTerminate(const UpdateContext& context)
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			
			if (runtimeData.currentChildIndex < m_children.size())
			{
				m_children[runtimeData.currentChildIndex]->Terminate(context);
			}
		}

		virtual Status Update(const UpdateContext& context)
		{
			FUNCTION_PROFILER(gEnv->pSystem, PROFILE_AI);

			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);

			ChildIndexType& currentChildIndex = runtimeData.currentChildIndex;

			while (true)
			{
				const Status s = m_children[currentChildIndex]->Tick(context);

				if (s == Success || s == Running)
				{
					return s;
				}

				if (++currentChildIndex == m_children.size())
				{
					return Failure;
				}
			}

			assert(false);
			return Invalid;
		}

	private:
		virtual void HandleEvent(const EventContext& context, const Event& event) OVERRIDE
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			
			if (runtimeData.currentChildIndex < m_children.size())
			{
				m_children[runtimeData.currentChildIndex]->SendEvent(context, event);
			}
		}
	};

	GenerateBehaviorTreeNodeCreator(Selector);
}

#endif // Selector_h