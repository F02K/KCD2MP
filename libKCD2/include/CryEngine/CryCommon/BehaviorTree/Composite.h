#pragma once

#ifndef Composite_h
#define Composite_h

#include "Node.h"

namespace BehaviorTree
{
	template <typename ChildType = INodePtr>
	class Composite : public Node
	{
	public:
		void AddChild(const ChildType& child)
		{
			m_children.push_back(child);
		}

	protected:
		typedef std::vector<ChildType> Children;
		typedef typename Children::iterator ChildIterator;
		Children m_children;
	};

	class CompositeWithChildLoader : public Composite<INodePtr>
	{
		typedef Composite<INodePtr> BaseClass;

	public:
		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context) OVERRIDE
		{
			IF_UNLIKELY (BaseClass::LoadFromXml(xml, context) == LoadFailure)
				return LoadFailure;

			return ConstructChildNodesFromXml(xml, context);
		}

	protected:
		LoadResult ConstructChildNodesFromXml(const XmlNodeRef& xml, const LoadContext& context)
		{
			for (int i = 0; i < xml->getChildCount(); ++i)
			{
				INodePtr child = context.nodeFactory.CreateNodeFromXml(xml->getChild(i), context);

				if (child)
				{
					AddChild(child);
				}
				else
				{
					return LoadFailure;
				}
			}

			return !m_children.empty() ? LoadSuccess : LoadFailure;
		}
	};
}

#endif // Composite_h