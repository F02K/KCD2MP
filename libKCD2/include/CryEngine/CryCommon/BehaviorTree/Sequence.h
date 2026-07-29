#pragma once

#ifndef BehaviorTreeSequence_h
#define BehaviorTreeSequence_h

#include "Composite.h"

namespace BehaviorTree
{
	// Executes children one at a time in order.
	// Succeeds when all children succeeded.
	// Fails when a child failed.
	class Sequence : public CompositeWithChildLoader
	{
		typedef CompositeWithChildLoader BaseClass;

	public:
		typedef uint8 IndexType;

		struct RuntimeData
		{
			IndexType currentChildIndex;

			RuntimeData() : currentChildIndex(0) {}
		};

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context) OVERRIDE
		{
			IF_UNLIKELY (BaseClass::LoadFromXml(xml, context) == LoadFailure)
				return LoadFailure;

			const size_t maxChildCount = std::numeric_limits<IndexType>::max();
			IF_UNLIKELY ((size_t)xml->getChildCount() > maxChildCount)
			{
				ErrorReporter(*this, context).LogError("Too many children. Max %d children are supported.", maxChildCount);
				return LoadFailure;
			}

			return LoadSuccess;
		}

 		virtual void OnInitialize(const UpdateContext& context) OVERRIDE
 		{
 		}

	protected:
		virtual Status Update(const UpdateContext& context) OVERRIDE
		{
			FUNCTION_PROFILER(gEnv->pSystem, PROFILE_AI);

			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);

			IndexType& currentChildIndex = runtimeData.currentChildIndex;

			while (true)
			{
				assert(currentChildIndex < m_children.size());
				const Status s = m_children[currentChildIndex]->Tick(context);

				if (s == Running || s == Failure)
				{
					return s;
				}

				if (++currentChildIndex == m_children.size())
				{
					return Success;
				}
			}

			assert(false);
			return Invalid;
		}

		virtual void OnTerminate(const UpdateContext& context) OVERRIDE
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			const IndexType currentChildIndex = runtimeData.currentChildIndex;

			if (currentChildIndex < m_children.size())
			{
				m_children[currentChildIndex]->Terminate(context);
			}
		}

	private:
		virtual void HandleEvent(const EventContext& context, const Event& event) OVERRIDE
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			const IndexType currentChildIndex = runtimeData.currentChildIndex;
			if (currentChildIndex < m_children.size())
			{
				m_children[currentChildIndex]->SendEvent(context, event);
			}
		}
	};

	GenerateBehaviorTreeNodeCreator(Sequence);
}

#endif // BehaviorTree