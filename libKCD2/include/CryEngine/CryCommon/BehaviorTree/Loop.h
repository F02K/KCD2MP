#pragma once

#include "Decorator.h"

namespace BehaviorTree
{
	// The loop fails when the child fails, otherwise the loop says it's running.
	//
	// If the child tick reports back that the child succeeded, the child will
	// immediately be ticked once more IF it was running _before_ the tick.
	// The effect of this is that if you have a child that runs over a period
	// of time, that child will be restarted immediately when it succeeds.
	// This behavior prevents the case where a non-instant loop's child could
	// mishandle an event if it was received during the window of time between
	// succeeding and being restarted by the loop node.
	//
	// However, if the child immediately succeeds, it will be ticked
	// only the next frame. Otherwise, it would get stuck in an infinite loop.

	class Loop : public Decorator
	{
		typedef Decorator BaseClass;

	public:
		struct RuntimeData
		{
			uint8 amountOfTimesChildSucceeded;
			bool childWasRunningLastTick;

			RuntimeData()
				: amountOfTimesChildSucceeded(0)
				, childWasRunningLastTick(false)
			{
			}
		};
		
		Loop()
			: m_desiredRepeatCount(0)
		{
		}

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context) OVERRIDE
		{
			IF_UNLIKELY (BaseClass::LoadFromXml(xml, context) == LoadFailure)
				return LoadFailure;

			m_desiredRepeatCount = 0; // 0 means infinite
			xml->getAttr("count", m_desiredRepeatCount);

			return LoadSuccess;
		}

	protected:
		virtual Status Update(const UpdateContext& context) OVERRIDE
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			
			Status childStatus = m_child->Tick(context);

			if (childStatus == Success)
			{
				const bool finiteLoop = (m_desiredRepeatCount > 0);
				if (finiteLoop)
				{
					if (runtimeData.amountOfTimesChildSucceeded + 1 >= m_desiredRepeatCount)
					{
						return Success;
					}

					++runtimeData.amountOfTimesChildSucceeded;
				}

				if (runtimeData.childWasRunningLastTick)
				{
					childStatus = m_child->Tick(context);
				}
			}

			runtimeData.childWasRunningLastTick = (childStatus == Running);

			if (childStatus == Failure)
			{
				return Failure;
			}

			return Running;
		}

	private:
		uint8 m_desiredRepeatCount; // 0 means infinite
	};

	GenerateBehaviorTreeNodeCreator(Loop);



	// Similar to the loop. It will tick the child node and keep running
	// while it's running. If the child succeeds, this node succeeds.
	// If the child node fails we try again, a defined amount of times.
	// The only reason this is its own node and not a configuration of
	// the Loop node is because of readability. It became hard to read.
	class LoopUntilSuccess : public Decorator
	{
		typedef Decorator BaseClass;

	public:
		struct RuntimeData
		{
			uint8 failedAttemptCountSoFar;
			bool childWasRunningLastTick;

			RuntimeData()
				: failedAttemptCountSoFar(0)
				, childWasRunningLastTick(false)
			{
			}
		};
		
		LoopUntilSuccess()
			: m_maxAttemptCount(0)
		{
		}

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context) OVERRIDE
		{
			IF_UNLIKELY (BaseClass::LoadFromXml(xml, context) == LoadFailure)
				return LoadFailure;

			m_maxAttemptCount = 0; // 0 means infinite
			xml->getAttr("attemptCount", m_maxAttemptCount);

			return LoadSuccess;
		}

	protected:
		virtual Status Update(const UpdateContext& context) OVERRIDE
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			
			Status childStatus = m_child->Tick(context);

			if (childStatus == Failure)
			{
				const bool finiteLoop = (m_maxAttemptCount > 0);
				if (finiteLoop)
				{
					if (runtimeData.failedAttemptCountSoFar + 1 >= m_maxAttemptCount)
					{
						return Failure;
					}

					++runtimeData.failedAttemptCountSoFar;
				}

				if (runtimeData.childWasRunningLastTick)
				{
					childStatus = m_child->Tick(context);
				}
			}

			runtimeData.childWasRunningLastTick = (childStatus == Running);

			if (childStatus == Success)
			{
				return Success;
			}

			return Running;
		}

	private:
		uint8 m_maxAttemptCount; // 0 means infinite
	};

 	GenerateBehaviorTreeNodeCreator(LoopUntilSuccess);
}