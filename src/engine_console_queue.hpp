#pragma once

#include <algorithm>
#include <cstddef>
#include <deque>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

namespace big
{
	enum class engine_console_submit_status
	{
		queued,
		unavailable,
		empty,
		too_long,
		full
	};

	class engine_console_command_queue
	{
	public:
		explicit engine_console_command_queue(std::size_t capacity = 64, std::size_t maximum_command_length = 1024) :
		    m_capacity(capacity),
		    m_maximum_command_length(maximum_command_length)
		{
		}

		[[nodiscard]] engine_console_submit_status submit(std::string command, bool console_available)
		{
			if (!console_available)
			{
				return engine_console_submit_status::unavailable;
			}

			const auto first = command.find_first_not_of(" \t\r\n");
			if (first == std::string::npos)
			{
				return engine_console_submit_status::empty;
			}
			const auto last = command.find_last_not_of(" \t\r\n");
			command         = command.substr(first, last - first + 1);
			if (command.size() > m_maximum_command_length)
			{
				return engine_console_submit_status::too_long;
			}

			std::scoped_lock lock(m_mutex);
			if (m_queue.size() >= m_capacity)
			{
				return engine_console_submit_status::full;
			}
			m_queue.push_back(std::move(command));
			return engine_console_submit_status::queued;
		}

		[[nodiscard]] std::vector<std::string> drain(std::size_t maximum = 8)
		{
			std::scoped_lock lock(m_mutex);
			const auto count = std::min(maximum, m_queue.size());
			std::vector<std::string> result;
			result.reserve(count);
			for (std::size_t index = 0; index < count; ++index)
			{
				result.push_back(std::move(m_queue.front()));
				m_queue.pop_front();
			}
			return result;
		}

		[[nodiscard]] std::size_t size() const
		{
			std::scoped_lock lock(m_mutex);
			return m_queue.size();
		}

	private:
		std::size_t m_capacity;
		std::size_t m_maximum_command_length;
		mutable std::mutex m_mutex;
		std::deque<std::string> m_queue;
	};
} // namespace big
