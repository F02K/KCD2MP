#pragma once

#if !defined(_RELEASE) && (defined(WIN32) || defined(WIN64))
# define DEBUG_MODULAR_BEHAVIOR_TREE
#endif

#ifdef DEBUG_MODULAR_BEHAVIOR_TREE
#  define STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
#  ifdef WIN32
#    define STORE_INFORMATION_FOR_BEHAVIOR_TREE_EVENTS
#  endif
#endif

#include <VectorMap.h>

struct IPipeUser;

namespace BehaviorTree
{
	struct INodeFactory;
	struct BehaviorTreeInstance;
	struct BehaviorTreeTemplate;

	enum Status
	{
		Invalid,
		Success,
		Failure,
		Running
	};

	enum LoadResult
	{
		LoadFailure,
		LoadSuccess,
	};

	class Event
	{
	public:
		Event()
			: m_nameCRC32(0)
			#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_EVENTS
			, m_name()
			#endif
		{
		}

		Event(uint32 nameCRC32)
			: m_nameCRC32(nameCRC32)
			#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_EVENTS
			, m_name()
			#endif
		{
		}

		#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_EVENTS
		Event(uint32 nameCRC32, const char* name)
			: m_nameCRC32(nameCRC32)
			, m_name(name)
		{
		}
		#endif

		Event(const char* name)
			#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_EVENTS
			: m_name(name)
			#endif
		{
			m_nameCRC32 = gEnv->pSystem->GetCrc32Gen()->GetCRC32(name);
		}

		bool operator == (const Event& rhs) const
		{
			return m_nameCRC32 == rhs.m_nameCRC32;
		}

		#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_EVENTS
		const char* GetName() const
		{
			return m_name.c_str();
		}
		#endif

	private:
		uint32 m_nameCRC32;

		#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_EVENTS
		string m_name;
		#endif
	};

	struct INodeFactory;

	struct LoadContext
	{
		LoadContext(
			INodeFactory& _nodeFactory,
			const char* _treeName,
			const BehaviorTreeTemplate& _behaviorTreeTemplate)
			: nodeFactory(_nodeFactory)
			, treeName(_treeName)
			, behaviorTreeTemplate(_behaviorTreeTemplate)
		{
		}

		INodeFactory& nodeFactory;
		const char* treeName;
		const BehaviorTreeTemplate& behaviorTreeTemplate;

		// Warning! If you're thinking about adding the entity id to the
		// LoadContext you need to keep one thing in mind:
		// The modular behavior tree will be loaded up and constructed before
		// the connection between the entity and the ai object has been made.
	};

	struct INode;



	struct DebugNode;

	typedef boost::shared_ptr<DebugNode> DebugNodePtr;

	struct DebugNode
	{
		typedef DynArray<DebugNodePtr> Children;

		const INode* node;
		Children children;

		DebugNode(const INode* _node) { node = _node; }
	};

	// Route an internal modular behavior tree log message
	struct ILogRouter
	{
		virtual ~ILogRouter() {}
		virtual void Log(const char* message) = 0;
	};

	class DebugTree
	{
	public:
		void Push(const INode* node)
		{
			DebugNodePtr debugNode(new DebugNode(node));

			if (!m_firstDebugNode)
				m_firstDebugNode = debugNode;

			if (!m_debugNodeStack.empty())
				m_debugNodeStack.back()->children.push_back(debugNode);

			m_debugNodeStack.push_back(debugNode);
		}

		void Pop(Status s)
		{
			IF_UNLIKELY (s == Failure || s == Success)
			{
				m_succeededAndFailedNodes.push_back(m_debugNodeStack.back());
			}

			m_debugNodeStack.pop_back();

			if (s != Running)
			{
				if (!m_debugNodeStack.empty())
					m_debugNodeStack.back()->children.pop_back();
				else
					m_firstDebugNode.reset();
			}
		}

		DebugNodePtr GetFirstNode() const
		{
			return m_firstDebugNode;
		}

		const DynArray<DebugNodePtr>& GetSucceededAndFailedNodes() const
		{
			return m_succeededAndFailedNodes;
		}

	private:
		DebugNodePtr m_firstDebugNode;
		DynArray<DebugNodePtr> m_debugNodeStack;
		DynArray<DebugNodePtr> m_succeededAndFailedNodes;
	};

	typedef uint32 NodeID;

	struct UpdateContext
	{
		UpdateContext(
			const EntityId _id
			, IPipeUser& _pipeUser
			, BehaviorTreeInstance& _behaviorTreeInstance
			, const bool _variablesWereChangedBeforeCurrentTick
			, void* _runtimeData
			#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
			, DebugTree* _debugTree = NULL
			, ILogRouter* _logRouter = NULL
			#endif
			#ifdef DEBUG_MODULAR_BEHAVIOR_TREE
			, bool _debugFailedActions = false
			#endif
		)
			: entityId(_id)
			, pipeUser(_pipeUser)
			, behaviorTreeInstance(_behaviorTreeInstance)
			, variablesWereChangedBeforeCurrentTick(_variablesWereChangedBeforeCurrentTick)
			, runtimeData(_runtimeData)
			#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
			, debugTree(_debugTree)
			, logRouter(_logRouter)
			#endif
			#ifdef DEBUG_MODULAR_BEHAVIOR_TREE
			, debugFailedActions(_debugFailedActions)
			#endif
		{
		}

		EntityId entityId;
		IPipeUser& pipeUser;
		BehaviorTreeInstance& behaviorTreeInstance;
		const bool variablesWereChangedBeforeCurrentTick;
		void* runtimeData;
		#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
		DebugTree* debugTree;
		ILogRouter* logRouter;
		#endif
		#ifdef DEBUG_MODULAR_BEHAVIOR_TREE
		bool debugFailedActions;
		#endif
	};

	struct EventContext
	{
		EventContext(
			const EntityId _id
			, IPipeUser& _pipeUser
			, BehaviorTreeInstance& _behaviorTreeInstance
		)
			: entityId(_id)
			, pipeUser(_pipeUser)
			, behaviorTreeInstance(_behaviorTreeInstance)
			, runtimeData(NULL)
		{
		}

		EntityId entityId;
		IPipeUser& pipeUser;
		BehaviorTreeInstance& behaviorTreeInstance;
		void* runtimeData;
	};

	struct INode
	{
		virtual ~INode() {}

		// When you "tick" a node the following rules apply:
		// 1. If the node was not previously running the node will first be
		//    initialized and then immediately updated.
		// 2. If the node was previously running, but no longer is, it will be
		//    terminated.
		virtual Status Tick(const UpdateContext& context) = 0;

		// Call this to explicitly terminate a node. The node will itself take
		// care of cleaning things up.
		// It's safe to call Terminate on an already terminated node,
		// although it's of course redundant.
		virtual void Terminate(const UpdateContext& context) = 0;

		// Load up a behavior tree node with information from an xml node.
		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const struct LoadContext& context) = 0;

		// Send an event to the node.
		// The event will be dispatched to the correct HandleEvent method
		// with validated runtime data.
		// Never override!
		virtual void SendEvent(const EventContext& context, const Event& event) = 0;
	};

	DECLARE_BOOST_POINTERS(INode);

	struct IBehaviorTreeManager
	{
		virtual ~IBehaviorTreeManager() {}
		virtual struct INodeFactory& GetNodeFactory() = 0;
		virtual std::auto_ptr<struct BehaviorTreeInstance> CreateBehaviorTreeInstance(const char* behaviorTreeName) = 0;
		virtual void LoadFromDiskIntoCache(const char* behaviorTreeName) = 0;
	};

	typedef uint64 RuntimeDataID;

	inline RuntimeDataID MakeRuntimeDataID(const EntityId entityID, const NodeID nodeID)
	{
		STATIC_ASSERT(sizeof(entityID) == 4, "Expected entity id to be 4 bytes");
		STATIC_ASSERT(sizeof(nodeID) == 4, "Expected node id to be 4 bytes");
		STATIC_ASSERT(sizeof(RuntimeDataID) == 8, "Expected runtime data id to be 8 bytes");

		const RuntimeDataID runtimeDataID = (uint64)entityID | (((uint64)nodeID) << 32);
		return runtimeDataID;
	}

	struct INodeCreator
	{
		virtual ~INodeCreator() {}
		virtual INodePtr Create() = 0;
		virtual void Trim() = 0;
		virtual const char* GetTypeName() const = 0;
		virtual size_t GetNodeClassSize() const = 0;
		virtual size_t GetSizeOfImmutableDataForAllAllocatedNodes() const = 0;
		virtual size_t GetSizeOfRuntimeDataForAllAllocatedNodes() const = 0;
		virtual void* AllocateRuntimeData(const RuntimeDataID runtimeDataID) = 0;
		virtual void* GetRuntimeData(const RuntimeDataID runtimeDataID) = 0;
		virtual void FreeRuntimeData(const RuntimeDataID runtimeDataID) = 0;
		virtual void SetNodeFactory(INodeFactory* nodeFactory) = 0;
	};

	struct INodeFactory
	{
		virtual ~INodeFactory() {}
		virtual INodePtr CreateNodeOfType(const char* typeName) = 0;
		virtual INodePtr CreateNodeFromXml(const XmlNodeRef& xml, const LoadContext& context) = 0;
		virtual void RegisterNodeCreator(struct INodeCreator* nodeCreator) = 0;
		virtual size_t GetSizeOfImmutableDataForAllAllocatedNodes() const = 0;
		virtual size_t GetSizeOfRuntimeDataForAllAllocatedNodes() const = 0;
		virtual void* AllocateRuntimeDataMemory(const size_t size) = 0;
		virtual void FreeRuntimeDataMemory(void* pointer) = 0;
		virtual void* AllocateNodeMemory(const size_t size) = 0;
		virtual void FreeNodeMemory(void* pointer) = 0;
	};

	template <typename NodeType, typename NodeCreatorType>
	class NodeDeleter
	{
	public:
		NodeDeleter(INodeFactory& nodeFactory, NodeCreatorType& creator)
			: m_nodeFactory(nodeFactory), m_creator(creator)
		{
		}

		void operator () (NodeType* node)
		{
			node->~NodeType();
			m_nodeFactory.FreeNodeMemory(node);
			--m_creator.m_nodeCount;
		}

	private:
		INodeFactory& m_nodeFactory;
		NodeCreatorType& m_creator;
	};

	template <typename NodeType> class NodeCreator : public INodeCreator
	{
		typedef NodeCreator<NodeType> ThisNodeCreatorType;
		typedef typename NodeType::RuntimeData RuntimeDataType;
		typedef VectorMap<RuntimeDataID, void*> RuntimeDataCollection;

		friend class NodeDeleter<NodeType, ThisNodeCreatorType>;

	public:
		NodeCreator(const char* typeName)
			: m_typeName(typeName)
			, m_nodeCount(0)
		{
		}

		virtual INodePtr Create() OVERRIDE
		{
			MEMSTAT_CONTEXT_FMT(EMemStatContextTypes::MSC_Other, 0, "Modular Behavior Tree Node Factory: %s", m_typeName);

			assert(m_nodeFactory != NULL);

			void* const pointer = m_nodeFactory->AllocateNodeMemory(sizeof(NodeType));
			
			if (pointer == NULL)
			{
				CryFatalError("Could not allocate the memory needed for a '%s' node.", GetTypeName());
			}

			++m_nodeCount;

			return INodePtr(new (pointer) NodeType(), NodeDeleter<NodeType, ThisNodeCreatorType>(*m_nodeFactory, *this));
		}

		virtual void Trim()
		{
			assert(m_runtimeDataCollection.empty());
			if (m_runtimeDataCollection.empty())
				stl::free_container(m_runtimeDataCollection);
			else
				gEnv->pLog->LogError("Failed to free runtime data collection. There were still %" PRISIZE_T " runtime data elements in '%s' node creator.", m_runtimeDataCollection.size(), m_typeName);
		}

		virtual const char* GetTypeName() const
		{
			return m_typeName;
		}

		virtual size_t GetNodeClassSize() const
		{
			return sizeof(NodeType);
		}

		virtual size_t GetSizeOfImmutableDataForAllAllocatedNodes() const OVERRIDE
		{
			return m_nodeCount * (sizeof(NodeType) + sizeof(INodePtr));
		}

		virtual size_t GetSizeOfRuntimeDataForAllAllocatedNodes() const OVERRIDE
		{
			return m_runtimeDataCollection.size() * sizeof(RuntimeDataType);
		}

		virtual void* AllocateRuntimeData(const RuntimeDataID runtimeDataID) OVERRIDE
		{
			void* pointer = m_nodeFactory->AllocateRuntimeDataMemory(sizeof(RuntimeDataType));
			RuntimeDataType* runtimeData = new (pointer) RuntimeDataType;
			assert(runtimeData != NULL);
			m_runtimeDataCollection.insert(std::make_pair(runtimeDataID, reinterpret_cast<void*>(runtimeData)));
			return runtimeData;
		}

		virtual void* GetRuntimeData(const RuntimeDataID runtimeDataID) OVERRIDE
		{
			typename RuntimeDataCollection::iterator it = m_runtimeDataCollection.find(runtimeDataID);
			if (it != m_runtimeDataCollection.end())
			{
				RuntimeDataType* runtimeData = reinterpret_cast<RuntimeDataType*>(it->second);
				return runtimeData;
			}

			return NULL;
		}

		virtual void FreeRuntimeData(const RuntimeDataID runtimeDataID) OVERRIDE
		{
			typename RuntimeDataCollection::iterator it = m_runtimeDataCollection.find(runtimeDataID);
			assert(it != m_runtimeDataCollection.end());
			if (it != m_runtimeDataCollection.end())
			{
				RuntimeDataType* runtimeData = reinterpret_cast<RuntimeDataType*>(it->second);
				runtimeData->~RuntimeDataType();
				m_nodeFactory->FreeRuntimeDataMemory(runtimeData);
				m_runtimeDataCollection.erase(it);
			}
		}

		virtual void SetNodeFactory(INodeFactory* nodeFactory) OVERRIDE
		{
			m_nodeFactory = nodeFactory;
		}

	private:
		const char* m_typeName;
		INodeFactory* m_nodeFactory;
		RuntimeDataCollection m_runtimeDataCollection;
		size_t m_nodeCount;
	};

	// Generates an object specialized to create a node of your type upon
	// request by the node factory. The macro drops a global variable here.
	#define GenerateBehaviorTreeNodeCreator(NodeType) namespace NodeCreators##NodeType { NodeCreator<NodeType> NodeType##NodeCreator(#NodeType); }

	// Register your node creator with the passed in factory. You need to
	// have made your creator available via GenerateBehaviorTreeNodeCreator.
	#define ExposeBehaviorTreeNodeToFactory(factory, nodetype) factory.RegisterNodeCreator(&NodeCreators##nodetype::nodetype##NodeCreator);
}