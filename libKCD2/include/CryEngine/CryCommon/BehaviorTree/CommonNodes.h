#pragma once

// Prerequisites
#include "Node.h"
#include "Action.h"

// Composites
#include "Parallel.h"
#include "Selector.h"
#include "Sequence.h"
#include "StateMachine.h"

// ------------------------------------------------------------------------

// Decorators
#include "Loop.h"
#include "SuppressFailure.h"

// Common decorators includes
#include "TimeValue.h"
// --------------------------
namespace BehaviorTree
{
	class WaitForEvent : public Action
	{
		typedef Action BaseClass;

	public:
		struct RuntimeData
		{
			bool eventReceieved;

			RuntimeData() : eventReceieved(false) {}
		};

		WaitForEvent() : m_statusToReturn(Success)
		{
		}

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context) OVERRIDE
		{
			IF_UNLIKELY (BaseClass::LoadFromXml(xml, context) == LoadFailure)
				return LoadFailure;

			const stack_string eventName = xml->getAttr("name");
			IF_UNLIKELY (eventName.empty())
			{
				gEnv->pLog->LogError("WaitForEvent could not find the 'name' attribute at line %d.", xml->getLine());
				return LoadFailure;
			}

			m_eventToWaitFor = Event(eventName);

			const stack_string resultString = xml->getAttr("result");
			if (!resultString.empty())
			{
				if (resultString == "Success")
					m_statusToReturn = Success;
				else if (resultString == "Failure")
					m_statusToReturn = Failure;
				else
				{
					ErrorReporter(*this, context).LogError("Invalid 'result' attribute. Expected 'Success' or 'Failure'.");
					return LoadFailure;
				}
			}

			return LoadSuccess;
		}

		virtual void HandleEvent(const EventContext& context, const Event& event)
		{
			if (m_eventToWaitFor == event)
			{
				RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
				runtimeData.eventReceieved = true;
			}
		}

		#if defined(STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER) && defined(STORE_INFORMATION_FOR_BEHAVIOR_TREE_EVENTS)
		virtual void GetDebugTextForVisualizer(const UpdateContext& updateContext, stack_string& debugText) const OVERRIDE
		{
			debugText = m_eventToWaitFor.GetName();
		}
		#endif

	protected:
		virtual Status Update(const UpdateContext& context) OVERRIDE
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);

			if (runtimeData.eventReceieved)
				return m_statusToReturn;
			else
				return Running;
		}

	private:
		Event m_eventToWaitFor;
		Status m_statusToReturn;
	};

	GenerateBehaviorTreeNodeCreator(WaitForEvent);

	//////////////////////////////////////////////////////////////////////////

	class Fail : public Action
	{
	public:
		struct RuntimeData
		{
		};
		
	protected:
		virtual Status Update(const UpdateContext& context)
		{
			return Failure;
		}
	};

	GenerateBehaviorTreeNodeCreator(Fail);

	//////////////////////////////////////////////////////////////////////////

	class Breakpoint : public Action
	{
	public:
		struct RuntimeData
		{
		};
		
	protected:
		virtual Status Update(const UpdateContext& context)
		{
			CryDebugBreak();
			return Success;
		}
	};

	GenerateBehaviorTreeNodeCreator(Breakpoint);

	//////////////////////////////////////////////////////////////////////////

	// This node effectively halts the execution of the modular behavior
	// tree because it never finishes.
	class Halt : public Action
	{
	public:
		struct RuntimeData
		{
		};
		
	protected:
		virtual Status Update(const UpdateContext& context) OVERRIDE
		{
			return Running;
		}
	};

	GenerateBehaviorTreeNodeCreator(Halt);
}

// ------------------------------------------------------------------------

// Actions
#include "Timeout.h"
#include "Wait.h"


// ------------------------------------------------------------------------
namespace BehaviorTree
{
	void ExposeCommonNodesSuchAsSequenceEtcTo(INodeFactory& factory)
	{
 		// Composites
 		ExposeBehaviorTreeNodeToFactory(factory, Sequence);
 		ExposeBehaviorTreeNodeToFactory(factory, Selector);
 		ExposeBehaviorTreeNodeToFactory(factory, Parallel);
 		ExposeBehaviorTreeNodeToFactory(factory, StateMachine);
 		ExposeBehaviorTreeNodeToFactory(factory, State);
 
 		// Decorators
 		ExposeBehaviorTreeNodeToFactory(factory, Loop);
 		ExposeBehaviorTreeNodeToFactory(factory, LoopUntilSuccess);
 		ExposeBehaviorTreeNodeToFactory(factory, SuppressFailure);
 
 		// Actions
 		ExposeBehaviorTreeNodeToFactory(factory, Timeout);
 		ExposeBehaviorTreeNodeToFactory(factory, Wait);
 		ExposeBehaviorTreeNodeToFactory(factory, WaitForEvent);
 		ExposeBehaviorTreeNodeToFactory(factory, Fail);
 		ExposeBehaviorTreeNodeToFactory(factory, Breakpoint);
 		ExposeBehaviorTreeNodeToFactory(factory, Halt);
	}
}