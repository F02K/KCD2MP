#pragma once

#include "Node.h"

namespace BehaviorTree
{
	class Action : public Node
	{
		typedef Node BaseClass;

	public:
		virtual void HandleEvent(const EventContext& context, const Event& event)
		{
		}

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context) OVERRIDE
		{
			IF_UNLIKELY (BaseClass::LoadFromXml(xml, context) == LoadFailure)
				return LoadFailure;

			if (xml->getChildCount() == 0)
			{
				return LoadSuccess;
			}
			else
			{
				ErrorReporter(*this, context).LogError("No children allowed but found %d.", xml->getChildCount());
				return LoadFailure;
			}
		}

		virtual void OnInitialize(const UpdateContext& context) OVERRIDE
		{
		}

		virtual void OnTerminate(const UpdateContext& context) OVERRIDE
		{
		}

	private:
		virtual bool IsAction() const OVERRIDE { return true; }
	};
}