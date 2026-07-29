#pragma once

#include "Composite.h"

namespace BehaviorTree
{
	// Composite node which executes it's children in parallel.
	// By default it returns Success when all children have succeeded
	// and Failure when any of any child has failed.
	// The behavior can be customized.
	class Parallel : public CompositeWithChildLoader
	{
	public:
		struct RuntimeData
		{
			uint32 runningChildren; // Each bit is a child
			uint32 successCount;
			uint32 failureCount;

			RuntimeData()
				: runningChildren(0)
				, successCount(0)
				, failureCount(0)
			{
			}
		};
		
		Parallel()
			: m_successMode(SuccessMode_All)
			, m_failureMode(FailureMode_Any)
		{
		}

		virtual LoadResult LoadFromXml(const XmlNodeRef& node, const LoadContext& context)
		{
			IF_UNLIKELY (node->getChildCount() > 32)
			{
				ErrorReporter(*this, context).LogError("Too many children. Max 32 children allowed.");
				return LoadFailure;
			}

			m_successMode = SuccessMode_All;
			m_failureMode = FailureMode_Any;

			stack_string failureMode = node->getAttr("failureMode");
			if (!failureMode.empty())
			{
				if (!failureMode.compare("all"))
				{
					m_failureMode = FailureMode_All;
				}
				else
				{
					gEnv->pLog->LogError("Error in the %s behavior tree : the parallel node at %d has an invalid value for the attribute failureMode.", context.treeName, node->getLine());
				}
			}

			stack_string successMode = node->getAttr("successMode");
			if (!successMode.empty())
			{
				if (!successMode.compare("any"))
				{
					m_successMode = SuccessMode_Any;
				}
				else
				{
					gEnv->pLog->LogError("Error in the %s behavior tree : the parallel node at %d has an invalid value for the attribute successMode.", context.treeName, node->getLine());
				}
			}

			return CompositeWithChildLoader::LoadFromXml(node, context);
		}

		virtual void OnInitialize(const UpdateContext& context)
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			
			uint32 runningChildren = 0;
			for (size_t childIndex = 0, n = m_children.size(); childIndex < n; ++childIndex)
			{
				runningChildren |= 1 << childIndex;
			}

			runtimeData.runningChildren = runningChildren;
			runtimeData.successCount = 0;
			runtimeData.failureCount = 0;
		}

		virtual void OnTerminate(const UpdateContext& context)
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			
			const uint32 runningChildren = runtimeData.runningChildren;
			for (size_t childIndex = 0, n = m_children.size(); childIndex < n; ++childIndex)
			{
				const bool childIsRunning = (runningChildren & (1 << childIndex)) != 0;
				if (childIsRunning)
				{
					m_children[childIndex]->Terminate(context);
				}
			}
		}

		virtual Status Update(const UpdateContext& context)
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);

			uint32& runningChildren = runtimeData.runningChildren;
			uint32& successCount = runtimeData.successCount;
			uint32& failureCount = runtimeData.failureCount;

			for (size_t childIndex = 0, n = m_children.size(); childIndex < n; ++childIndex)
			{
				const bool childIsRunning = (runningChildren & (1 << childIndex)) != 0;

				if (childIsRunning)
				{
					const Status s = m_children[childIndex]->Tick(context);

					if (s == Running)
						continue;
					else
					{
						if (s == Success)
							++successCount;
						else
							++failureCount;

						// Mark child as not running
						runningChildren = runningChildren & ~uint32(1u << childIndex);
					}
				}
			}

			if (m_successMode == SuccessMode_All)
			{
				if (successCount == m_children.size())
				{
					return Success;
				}
			}
			else if (m_successMode == SuccessMode_Any)
			{
				if (successCount > 0)
				{
					return Success;
				}
			}

			if (m_failureMode == FailureMode_All)
			{
				if (failureCount == m_children.size())
				{
					return Failure;
				}
			}
			else if (m_failureMode == FailureMode_Any)
			{
				if (failureCount > 0)
				{
					return Failure;
				}
			}

			return Running;
		}

	private:
		virtual void HandleEvent(const EventContext& context, const Event& event) OVERRIDE
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);

			const uint32 runningChildren = runtimeData.runningChildren;
			for (size_t childIndex = 0, n = m_children.size(); childIndex < n; ++childIndex)
			{
				const bool childIsRunning = (runningChildren & (1 << childIndex)) != 0;
				if (childIsRunning)
				{
					m_children[childIndex]->SendEvent(context, event);
				}
			}
		}

		enum SuccessMode
		{
			SuccessMode_Any,
			SuccessMode_All,
		};

		enum FailureMode
		{
			FailureMode_Any,
			FailureMode_All,
		};

		SuccessMode m_successMode;
		FailureMode m_failureMode;
	};

	GenerateBehaviorTreeNodeCreator(Parallel);
}