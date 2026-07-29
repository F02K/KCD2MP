#pragma once

#include "Node.h"
#include "VectorMap.h"
#include "IBehaviorTree.h"
#include "BucketAllocator.h"
#include "BucketAllocatorImpl.h"

namespace BehaviorTree
{
	class NodeFactory : public INodeFactory
	{
	public:
		NodeFactory() : m_nextNodeID(0)
		{
			s_bucketAllocator.EnableExpandCleanups(false);
		}

		void CleanUpBucketAllocator()
		{
			s_bucketAllocator.cleanup();
		}

		virtual INodePtr CreateNodeOfType(const char* typeName)
		{
			INodePtr node;

			NodeCreators::iterator nodeCreatorIt = m_nodeCreators.find(stack_string(typeName));
			if (nodeCreatorIt != m_nodeCreators.end())
			{
				INodeCreator* creator = nodeCreatorIt->second;
				node = creator->Create();

				static_cast<Node*>(node.get())->m_id = m_nextNodeID++;
				static_cast<Node*>(node.get())->SetCreator(creator);
			}

			if (!node)
			{
				gEnv->pLog->LogError("Failed to create behavior tree node of type '%s'.", typeName);
			}

			return node;
		}

		virtual INodePtr CreateNodeFromXml(const XmlNodeRef& xml, const LoadContext& context)
		{
			INodePtr node = context.nodeFactory.CreateNodeOfType(xml->getTag());

			if (node)
			{
				MEMSTAT_CONTEXT_FMT(EMemStatContextTypes::MSC_Other, 0, "CreateNodeFromXml: %s", xml->getTag());

				#ifdef STORE_INFORMATION_FOR_BEHAVIOR_TREE_VISUALIZER
				static_cast<Node*>(node.get())->SetXmlLine(xml->getLine());
				#endif

				if (node->LoadFromXml(xml, context) == LoadSuccess)
				{
					return node;
				}
			}

			return INodePtr();
		}

		virtual void RegisterNodeCreator(INodeCreator* nodeCreator)
		{
			nodeCreator->SetNodeFactory(this);
			m_nodeCreators.insert(std::make_pair(stack_string(nodeCreator->GetTypeName()), nodeCreator));
			gEnv->pLog->Log("Modular behavior tree node '%s' has a class size of %" PRISIZE_T " bytes.", nodeCreator->GetTypeName(), nodeCreator->GetNodeClassSize());
		}

		virtual void TrimNodeCreators()
		{
			NodeCreators::const_iterator it = m_nodeCreators.begin();
			NodeCreators::const_iterator end = m_nodeCreators.end();

			for (; it != end; ++it)
			{
				INodeCreator* creator = it->second;
				creator->Trim();
			}
		}

		virtual size_t GetSizeOfImmutableDataForAllAllocatedNodes() const OVERRIDE
		{
			size_t total = 0;

			NodeCreators::const_iterator it = m_nodeCreators.begin();
			NodeCreators::const_iterator end = m_nodeCreators.end();

			for (; it != end; ++it)
			{
				INodeCreator* creator = it->second;
				total += creator->GetSizeOfImmutableDataForAllAllocatedNodes();
			}

			return total;
		}

		virtual size_t GetSizeOfRuntimeDataForAllAllocatedNodes() const OVERRIDE
		{
			size_t total = 0;

			NodeCreators::const_iterator it = m_nodeCreators.begin();
			NodeCreators::const_iterator end = m_nodeCreators.end();

			for (; it != end; ++it)
			{
				INodeCreator* creator = it->second;
				total += creator->GetSizeOfRuntimeDataForAllAllocatedNodes();
			}

			return total;
		}

		virtual void* AllocateRuntimeDataMemory(const size_t size) OVERRIDE
		{
			return s_bucketAllocator.allocate(size);
		}

		virtual void FreeRuntimeDataMemory(void* pointer) OVERRIDE
		{
			assert(s_bucketAllocator.IsInAddressRange(pointer));

			if (s_bucketAllocator.IsInAddressRange(pointer))
			{
				s_bucketAllocator.deallocate(pointer);
			}
		}

		// This will be called while loading a level or jumping into game
		// in the editor. The memory will remain until the level is unloaded
		// or we exit the game in the editor.
		virtual void* AllocateNodeMemory(const size_t size)
		{
			return new char[size];
		}

		// This will be called while unloading a level or exiting game
		// in the editor.
		virtual void FreeNodeMemory(void* pointer)
		{
			delete [] (char*)pointer;
		}

	private:
		typedef VectorMap<stack_string, INodeCreator*> NodeCreators;
		typedef BucketAllocator<BucketAllocatorDetail::DefaultTraits<(2*1024*1024), BucketAllocatorDetail::SyncPolicyUnlocked, false> > BehaviorTreeBucketAllocator;

		NodeCreators m_nodeCreators;
		NodeID m_nextNodeID;
		static BehaviorTreeBucketAllocator s_bucketAllocator;
	};

	NodeFactory::BehaviorTreeBucketAllocator NodeFactory::s_bucketAllocator(NULL, false);
}
