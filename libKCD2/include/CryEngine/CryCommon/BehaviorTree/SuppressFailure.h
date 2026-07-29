#pragma once

#include "Decorator.h"

namespace BehaviorTree
{
	class SuppressFailure : public Decorator
	{
	public:
		struct RuntimeData
		{
		};
		
		virtual Status Update(const UpdateContext& context)
		{
			Status s = m_child->Tick(context);

			if (s == Running)
				return Running;
			else
				return Success;
		}
	};

	GenerateBehaviorTreeNodeCreator(SuppressFailure);
}