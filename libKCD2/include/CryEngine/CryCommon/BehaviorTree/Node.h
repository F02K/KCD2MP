#pragma once

#ifndef BehaviorTreeNode_h
#define BehaviorTreeNode_h

#include "IBehaviorTree.h"

namespace BehaviorTree
{
	class Node : public INode
	{
	public:
		// When you "tick" a node the following rules apply:
		// 1. If the node was not previously running the node will first be
		//    initialized and then immediately updated.
		// 2. If the node was previously running, but no longer is, it will be
		//    terminated.
		virtual Status Tick(const UpdateContext& unmodifiedContext)
		{
			FUNCTION_PROFILER(gEnv->pSystem, PROFILE_AI);

			#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
			DebugTree* debugTree = unmodifiedContext.debugTree;
			ILogRouter* logRouter = unmodifiedContext.logRouter;
			if (debugTree)
				debugTree->Push(this);
			#endif

			UpdateContext context = unmodifiedContext;

			const RuntimeDataID runtimeDataID = MakeRuntimeDataID(context.entityId, m_id);
			context.runtimeData = GetCreator()->GetRuntimeData(runtimeDataID);
			const bool nodeNeedsToBeInitialized = (context.runtimeData == NULL);
			if (nodeNeedsToBeInitialized)
			{
				context.runtimeData = GetCreator()->AllocateRuntimeData(runtimeDataID);
			}

			if (nodeNeedsToBeInitialized)
			{
#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
				if (logRouter)
				{
					logRouter->Log(m_startLog.c_str());
				}
#endif
				OnInitialize(context);
			}

			const Status status = Update(context);

			if (status != Running)
			{
				OnTerminate(context);
				GetCreator()->FreeRuntimeData(runtimeDataID);
				context.runtimeData = NULL;

#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
				if (logRouter)
				{
					if (status == Success)
					{
						logRouter->Log(m_successLog.c_str());
					}
					else
					{
						logRouter->Log(m_failureLog.c_str());

						// if it's an Action, log its failure
						if(context.debugFailedActions && IsAction() && m_creator)
						{
							const char *actorName = gEnv->pEntitySystem->GetEntity(context.entityId)->GetName();
							CryWarning(VALIDATOR_MODULE_AI, VALIDATOR_WARNING, "MBT Action '%s' of '%s' failed [line #%i].", m_creator->GetTypeName(), actorName, GetXmlLine());
						}
					}
				}
#endif
			}

			#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
			if (debugTree)
				debugTree->Pop(status);
			#endif

			return status;
		}

		// Call this to explicitly terminate a node. The node will itself take
		// care of cleaning things up.
		// It's safe to call Terminate on an already terminated node,
		// although it's of course redundant.
		virtual void Terminate(const UpdateContext& unmodifiedContext)
		{
			UpdateContext context = unmodifiedContext;
			const RuntimeDataID runtimeDataID = MakeRuntimeDataID(context.entityId, m_id);
			context.runtimeData = GetCreator()->GetRuntimeData(runtimeDataID);
			if (context.runtimeData != NULL)
			{
				OnTerminate(context);
				GetCreator()->FreeRuntimeData(runtimeDataID);
				context.runtimeData = NULL;
			}
		}

		// Send an event to the node.
		// The event will be dispatched to the correct HandleEvent method
		// with validated runtime data.
		// Never override!
		virtual void SendEvent(const EventContext& unmodifiedContext, const Event& event) OVERRIDE
		{
			void* runtimeData = GetCreator()->GetRuntimeData(MakeRuntimeDataID(unmodifiedContext.entityId, m_id));
			if (runtimeData)
			{
				EventContext context = unmodifiedContext;
				context.runtimeData = runtimeData;
				HandleEvent(context, event);
			}
		}

		// Load up a behavior tree node with information from an xml node.
		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const struct LoadContext& context) OVERRIDE
		{
			#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
			m_startLog = xml->getAttr("_startLog");
			m_successLog = xml->getAttr("_successLog");
			m_failureLog = xml->getAttr("_failureLog");
			#endif

			return LoadSuccess;
		}

		void SetCreator(INodeCreator* creator) { m_creator = creator; }
		INodeCreator* GetCreator() { return m_creator; }
		const INodeCreator* GetCreator() const { return m_creator; }

		#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
		void SetXmlLine(const uint32 line) { m_xmlLine = line; }		
		uint32 GetXmlLine() const { return m_xmlLine; }
		virtual void GetDebugTextForVisualizer(const UpdateContext& updateContext, stack_string& debugText) const {}
		#endif

		template <typename RuntimeDataType>
		RuntimeDataType& GetRuntimeData(const EventContext& context)
		{
			assert(context.runtimeData);
			return *reinterpret_cast<RuntimeDataType*>(context.runtimeData);
		}

		template <typename RuntimeDataType>
		RuntimeDataType& GetRuntimeData(const UpdateContext& context)
		{
			assert(context.runtimeData);
			return *reinterpret_cast<RuntimeDataType*>(context.runtimeData);
		}

		NodeID GetNodeID() const { return m_id; }

		NodeID m_id; // TODO: Make this accessible only to the creator

	protected:

		Node()
			: m_id(0)
			, m_creator(NULL)
			#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
			, m_xmlLine(0)
			#endif
		{
		}

		// Called before the first call to Update.
		virtual void OnInitialize(const UpdateContext& context) {}

		// Called when a node is being terminated. Clean up after yourself here!
		// Not to be confused with Terminate which you call when you want to
		// terminate a node. :) Confused? Read it again.
		// OnTerminate is called in one of the following cases:
		// a) The node itself returned Success/Failure in Update.
		// b) Another node told this node to Terminate while this node was running.
		virtual void OnTerminate(const UpdateContext& context) {}

		// Do you node's work here.
		// - Note that OnInitialize will have been automatically called for you
		// before you get your first update.
		// - If you return Success or Failure the node will automatically
		// get OnTerminate called on itself.
		virtual Status Update(const UpdateContext& context) { return Running; }

	private:

		// This is where you would put your event handling code for your
		// node. It's always called with valid data. This method should never
		// be called directly.
		virtual void HandleEvent(const EventContext& context, const Event& event) = 0;

		// This is for debugging failed Action nodes.
		// Only the Action class will override this method.
		virtual bool IsAction() const { return false; }

		INodeCreator* m_creator;

#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
		string m_startLog;
		string m_successLog;
		string m_failureLog;
		uint32 m_xmlLine;
		#endif
	};

	DECLARE_BOOST_POINTERS(Node);

	// An object of this class will help out when reporting warnings and
	// errors from within the modular behavior tree code. If possible, it
	// automatically adds the node type, xml line and the name of the agent
	// using the node before routing the message along to gEnv->pLog.
	class ErrorReporter
	{
	public:
		ErrorReporter(const Node& node, const LoadContext& context)
		{
			#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
			const uint32 xmlLine = node.GetXmlLine();
			const char* nodeTypeName = node.GetCreator()->GetTypeName();
			m_prefixString.Format("%s(%d) [Tree='%s']", nodeTypeName, xmlLine, context.treeName);
			#else
			m_prefixString.Format("[Tree='%s']", context.treeName);
			#endif
		}

		ErrorReporter(const Node& node, const UpdateContext& context)
		{
			const IEntity* entity = gEnv->pEntitySystem->GetEntity(context.entityId);
			const char* agentName = entity ? entity->GetName() : "(null)";

			#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
			const uint32 xmlLine = node.GetXmlLine();
			const char* nodeTypeName = node.GetCreator()->GetTypeName();
			m_prefixString.Format("%s(%d) [%s]", nodeTypeName, xmlLine, agentName);
			#else
			m_prefixString.Format("[%s]", agentName);
			#endif
		}

		// Formats a warning message with node type, xml line and the name
		// of the agent using the node. This information is not accessible
		// in all configurations and is thus compiled in only when possible.
		// The message is routed through gEnv->pLog->LogWarning().
		void LogWarning(const char* format, ...) const
		{
			va_list	argumentList;
			char formattedLog[512];

			va_start(argumentList, format);
			vsprintf_s(formattedLog, 512, format, argumentList);
			va_end(argumentList);

			stack_string finalMessage;
			finalMessage.Format("%s %s", m_prefixString.c_str(), formattedLog);
			gEnv->pLog->LogWarning(finalMessage.c_str());
		}

		// Formats a error message with node type, xml line and the name
		// of the agent using the node. This information is not accessible
		// in all configurations and is thus compiled in only when possible.
		// The message is routed through gEnv->pLog->LogError().
		void LogError(const char* format, ...) const
		{
			va_list	argumentList;
			char formattedLog[512];

			va_start(argumentList, format);
			vsprintf_s(formattedLog, 512, format, argumentList);
			va_end(argumentList);

			stack_string finalMessage;
			finalMessage.Format("%s %s", m_prefixString.c_str(), formattedLog);
			gEnv->pLog->LogError(finalMessage.c_str());
		}

	private:
		CryFixedStringT<128> m_prefixString;
	};
}

#endif // BehaviorTreeNode_h
