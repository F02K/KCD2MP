#pragma once

#include "Node.h"

namespace BehaviorTree
{
	// A decorator is a a node that adds functionality to another node
	// without knowing about its internals.
	// Must have exactly one child.
	class Decorator : public Node
	{
		typedef Node BaseClass;

	public:
		void SetChild(const INodePtr& child)
		{
			m_child = child;
		}

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context) OVERRIDE
		{
			IF_UNLIKELY (BaseClass::LoadFromXml(xml, context) == LoadFailure)
				return LoadFailure;

			return LoadChildFromXml(xml, context);
		}

		virtual void HandleEvent(const EventContext& context, const Event& event) OVERRIDE
		{
			assert(m_child.get() != NULL);
			if (m_child)
			{
				m_child->SendEvent(context, event);
			}
		}

	protected:

		LoadResult LoadChildFromXml(const XmlNodeRef& xml, const LoadContext& context)
		{
			IF_UNLIKELY (BaseClass::LoadFromXml(xml, context) == LoadFailure)
				return LoadFailure;

			if (xml->getChildCount() != 1)
			{
				gEnv->pLog->LogError("A decorator must have exactly one child, but the decorator node %s (line %d) has %d nodes", xml->getTag(), xml->getLine(), xml->getChildCount());
				return LoadFailure;
			}

			INodePtr node = context.nodeFactory.CreateNodeFromXml(xml->getChild(0), context);
			if (node)
			{
				SetChild(node);
				return LoadSuccess;
			}

			return LoadFailure;
		}

		virtual void OnInitialize(const UpdateContext& context) OVERRIDE
		{
			BaseClass::OnInitialize(context);
		}

		virtual void OnTerminate(const UpdateContext& context) OVERRIDE
		{
			m_child->Terminate(context);
			BaseClass::OnTerminate(context);
		}

		virtual Status Update(const UpdateContext& context) OVERRIDE
		{
			return m_child->Tick(context);
		}

		INodePtr m_child;
	};
}