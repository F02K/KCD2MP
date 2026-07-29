
=================
Behavior Tree
=================

> Memory

The memory model is pretty naive right now. It's using shared pointers
to pass the nodes around.
We could easily construct them in a contiguous block of memory so that we
go through them in a cache-friendly manner.

> Level of abstraction

I'm not too happy about how aware the behavior tree is of the
usage. It's very aware of the fact that it's working on an entity.

> Examples

------------------------------------------------------
Example - Perform one action (smallest tree possible)
------------------------------------------------------
<BehaviorTree>
	<Root>
		<Log message="This is a log message."/>
	</Root>
</BehaviorTree>



------------------------------------------------------
Example - Perform actions in a sequence
------------------------------------------------------
<BehaviorTree>
	<Root>
		<Sequence>
			<Log message="This is a log message."/>
			<Wait duration="1.0"/>
			<Log message="This is another log message."/>
			<Wait duration="2.0"/>
		</Sequence>
	</Root>
</BehaviorTree>



------------------------------------------------------
Example - Loop Decorator
------------------------------------------------------
<BehaviorTree>
	<Root>
		<Loop>
			<Sequence>
				<Log message="This is a log message."/>
				<Wait duration="2.0"/>
			</Sequence>
		</Loop>
	</Root>
</BehaviorTree>