#pragma once

#include "Composite.h"
#include "Decorator.h"

#ifdef DEBUG_MODULAR_BEHAVIOR_TREE
#  ifdef WIN32
#    define STORE_INFORMATION_FOR_STATE_MACHINE_NODE
#  endif
#endif

namespace BehaviorTree
{
	typedef byte StateIndex;
	static const StateIndex StateIndexInvalid = ((StateIndex)~0);

	struct Transition
	{
		void LoadFromXml(const XmlNodeRef& transitionXml)
		{
			destinationStateCRC32 = 0;
			destinationStateIndex = StateIndexInvalid;
			#ifdef STORE_INFORMATION_FOR_STATE_MACHINE_NODE
			xmlLine = transitionXml->getLine();
			#endif

			const char* to;
			if(transitionXml->getAttr("to", &to))
			{
				destinationStateCRC32 = gEnv->pSystem->GetCrc32Gen()->GetCRC32Lowercase(to);

				#ifdef STORE_INFORMATION_FOR_STATE_MACHINE_NODE
				destinationState = to;
				#endif
			}

			onEvent = Event(transitionXml->getAttr("onEvent"));
		}

		#ifdef STORE_INFORMATION_FOR_STATE_MACHINE_NODE
		string destinationState;
		int xmlLine;
		#endif

		uint32 destinationStateCRC32;
		Event	onEvent;
		StateIndex destinationStateIndex;
	};

	typedef std::vector<Transition> Transitions;

	class State : public Decorator
	{
		friend class StateMachine;

	public:
		struct RuntimeData
		{
		};
		
		virtual LoadResult LoadFromXml(const XmlNodeRef& stateXml, const LoadContext& context) OVERRIDE
		{
			const char* name;
			if(stateXml->getAttr("name", &name))
			{
				m_nameCRC32 = gEnv->pSystem->GetCrc32Gen()->GetCRC32Lowercase(name);

				#ifdef STORE_INFORMATION_FOR_STATE_MACHINE_NODE
				m_name = name;
				#endif
			}
			else
			{
				gEnv->pLog->LogWarning("A state node must contain a valid 'name' attribute. The state node at the line %d does not.", stateXml->getLine());
				return LoadFailure;
			}

			const XmlNodeRef transitionsXml = stateXml->findChild("Transitions");
			if (transitionsXml)
				LoadTransitions(transitionsXml);

			const XmlNodeRef behaviorTreeXml = stateXml->findChild("BehaviorTree");
			if (behaviorTreeXml)
			{
				return LoadChildFromXml(behaviorTreeXml, context);
			}
			else
			{
				#ifdef STORE_INFORMATION_FOR_STATE_MACHINE_NODE
				gEnv->pLog->LogWarning("A state node must contain a 'BehaviorTree' child. The state node '%s' at %d does not.", m_name.c_str(), stateXml->getLine());
				#else
				gEnv->pLog->LogWarning("A state node must contain a 'BehaviorTree' child. The state node at the line %d does not.", stateXml->getLine());
				#endif

				return LoadFailure;
			}

			return LoadFailure;
		}

		const Transition* GetTransitionForEvent(const Event& event)
		{
			Transitions::iterator it = m_transitions.begin();
			Transitions::iterator end = m_transitions.end();

			for (; it != end; ++it)
			{
				const Transition& transition = *it;
				if (transition.onEvent == event)
					return &transition;
			}

			return NULL;
		}

		const unsigned int GetNameCRC32()
		{
			return m_nameCRC32;
		}

		#if defined(STORE_INFORMATION_FOR_STATE_MACHINE_NODE) && defined(STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER)
		virtual void GetDebugTextForVisualizer(const UpdateContext& updateContext, stack_string& debugText) const
		{
			debugText += m_name.c_str();
		}
		#endif

		Transitions& GetTransitions() { return m_transitions; }


	private:
		void LoadTransitions(const XmlNodeRef& transitionsXml)
		{
			const int childCount = transitionsXml->getChildCount();
			for (int i = 0; i < childCount; ++i)
				LoadTransition(transitionsXml->getChild(i));
		}

		void LoadTransition(const XmlNodeRef& transitionXml)
		{
			Transition transition;
			transition.LoadFromXml(transitionXml);
			m_transitions.push_back(transition);
		}
		
		Transitions m_transitions;
		uint32 m_nameCRC32;

		#ifdef STORE_INFORMATION_FOR_STATE_MACHINE_NODE
		string m_name;
		#endif
	};

	DECLARE_BOOST_POINTERS(State);

	// A state machine is a composite node that holds one or more children.
	// There is one selected child at any given time. Default is the first.
	//
	// A state machine's status is the same as that of it's currently
	// selected child: running, succeeded or failed.
	//
	// One state can transition to another state on a specified event.
	// The transition happens immediately in the sense of selection,
	// but the new state is only updated the next time the state machine
	// itself is being updated.
	//
	// Transitioning to the same state will cause the state to first be
	// terminated and then initialized and updated.
	class StateMachine : public Composite<StatePtr>
	{
	public:
		typedef uint8 ChildIndexType;

		struct RuntimeData
		{
			ChildIndexType currentChildIndex;
			ChildIndexType pendingChildIndex;
			bool transitionIsPending;

			RuntimeData()
				: currentChildIndex(0)
				, pendingChildIndex(0)
				, transitionIsPending(false)
			{
			}
		};
		
		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context) OVERRIDE
		{
			const size_t maxChildCount = std::numeric_limits<ChildIndexType>::max();

			IF_UNLIKELY ((size_t)xml->getChildCount() >= maxChildCount)
			{
				ErrorReporter(*this, context).LogError("Too many children. Max %d allowed.", maxChildCount);
				return LoadFailure;
			}

			for (int i = 0; i < xml->getChildCount(); ++i)
			{
				if (xml->getChild(i)->isTag("State"))
				{
					INodePtr node = context.nodeFactory.CreateNodeFromXml(xml->getChild(i), context);

					// TODO: Perform RTTI check on node before casting.
                    StatePtr child;
                    child = boost::static_pointer_cast<State>(node);
					if (child)
					{
						AddChild(child);
					}
					else
					{
						gEnv->pLog->LogError("Failed to create StateMachine 'Child' node at line %d.", xml->getLine());
						return LoadFailure;
					}
				}
				else
				{
					gEnv->pLog->LogError("StateMachine node must contain child nodes of the type 'State', and there is a child of the '%s' at line '%d'.", xml->getTag(), xml->getLine());
					return LoadFailure;
				}
			}

			return LinkStateTransitions();
		}

		virtual void HandleEvent(const EventContext& context, const Event& event)
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			
			ChildIndexType& pendingChildIndex = runtimeData.pendingChildIndex;

			if (pendingChildIndex >= m_children.size())
				return;

			const Transition* transition = m_children[pendingChildIndex]->GetTransitionForEvent(event);
			if (transition)
			{
				assert(transition->destinationStateIndex != StateIndexInvalid);
				pendingChildIndex = transition->destinationStateIndex;
				runtimeData.transitionIsPending = true;
			}

            StatePtr currentChild = boost::static_pointer_cast<State>(m_children[runtimeData.currentChildIndex]);
			currentChild->SendEvent(context, event);
		}

	protected:
		virtual void OnInitialize(const UpdateContext& context) OVERRIDE
		{
			assert(!m_children.empty());

			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			
			runtimeData.currentChildIndex = 0;
			runtimeData.pendingChildIndex = 0;

			runtimeData.transitionIsPending = false;
		}

		virtual void OnTerminate(const UpdateContext& context) OVERRIDE
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			
			if (runtimeData.currentChildIndex < m_children.size())
				m_children[runtimeData.currentChildIndex]->Terminate(context);
		}

		virtual Status Update(const UpdateContext& context)
		{
			FUNCTION_PROFILER(gEnv->pSystem, PROFILE_AI);

			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);

			ChildIndexType& currentChildIndex = runtimeData.currentChildIndex;
			ChildIndexType& pendingChildIndex = runtimeData.pendingChildIndex;
			
			if (runtimeData.transitionIsPending)
			{
				runtimeData.transitionIsPending = false;
				m_children[currentChildIndex]->Terminate(context);
				currentChildIndex = pendingChildIndex;
			}

			Status s = m_children[currentChildIndex]->Tick(context);

			return s;
		}


	private:

		LoadResult LinkStateTransitions()
		{
			ChildIterator iter;
			ChildIterator endIter = m_children.end();
			for (iter = m_children.begin(); iter != endIter ; ++iter)
			{
				StatePtr statePtr(*iter);
				LoadResult loadResult = LinkStateTransition(statePtr);
				if (loadResult != LoadSuccess)
				{
					return loadResult;
				}
			}

			return LoadSuccess;
		}


		LoadResult LinkStateTransition(StatePtr& state)
		{
			assert(state.get() != NULL);
			Transitions& transitions = state->GetTransitions();
			Transitions::iterator transitionIter;
			Transitions::iterator transitionEndIter = transitions.end();

			for (transitionIter = transitions.begin() ; transitionIter != transitionEndIter ; ++transitionIter)
			{
				StateIndex stateIndex = IndexOfStateTransition(transitionIter->destinationStateCRC32);
				if (stateIndex == StateIndexInvalid)
				{
					#ifdef STORE_INFORMATION_FOR_STATE_MACHINE_NODE
					gEnv->pLog->LogError("Cannot transition to unknown state '%s' at line %d.", transitionIter->destinationState, transitionIter->xmlLine);
					#endif

					return LoadFailure;
				}

				transitionIter->destinationStateIndex = stateIndex;
			}

			return LoadSuccess;
		}


		// Returns: The index of the found state transition in m_children or StateIndexInvalid if it could not be found.
		StateIndex IndexOfStateTransition(uint32 stateNameLowerCaseCRC32)
		{
			size_t index, size = m_children.size();
			assert(size < ((1 << (sizeof(StateIndex) * 8)) - 1)); // -1 because StateIndexInvalid is reserved.
			for (index = 0 ; index < size ; index++)
			{
                StatePtr state = boost::static_pointer_cast<State>(m_children[index]);
				if(state->GetNameCRC32() == stateNameLowerCaseCRC32)
				{
					return (int)index;
				}
			}

			return StateIndexInvalid;
		}
	};

	GenerateBehaviorTreeNodeCreator(StateMachine);
	GenerateBehaviorTreeNodeCreator(State);
}
