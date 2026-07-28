#include "engine_console_queue.hpp"

#include <cassert>
#include <string>

int main()
{
	using big::engine_console_command_queue;
	using big::engine_console_submit_status;

	engine_console_command_queue queue(2, 8);
	assert(queue.submit("first", false) == engine_console_submit_status::unavailable);
	assert(queue.size() == 0);
	assert(queue.submit(" \t\r\n ", true) == engine_console_submit_status::empty);
	assert(queue.submit("123456789", true) == engine_console_submit_status::too_long);
	assert(queue.submit("12345678", true) == engine_console_submit_status::queued);
	const auto boundary_batch = queue.drain();
	assert(boundary_batch.size() == 1);
	assert(boundary_batch.front() == "12345678");
	assert(queue.submit("  first  ", true) == engine_console_submit_status::queued);
	assert(queue.submit("second", true) == engine_console_submit_status::queued);
	assert(queue.submit("third", true) == engine_console_submit_status::full);

	const auto first_batch = queue.drain(1);
	assert(first_batch.size() == 1);
	assert(first_batch.front() == "first");
	assert(queue.size() == 1);

	assert(queue.submit("third", true) == engine_console_submit_status::queued);
	const auto second_batch = queue.drain();
	assert(second_batch.size() == 2);
	assert(second_batch[0] == "second");
	assert(second_batch[1] == "third");
	assert(queue.size() == 0);

	return 0;
}
