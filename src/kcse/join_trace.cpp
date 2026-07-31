#include "kcse/join_trace.hpp"

#include <Windows.h>

#include <atomic>
#include <filesystem>
#include <format>
#include <mutex>
#include <string>

namespace kcd2mp::kcse::join_trace
{
	namespace
	{
		std::atomic_bool g_active{};
		std::atomic_uint64_t g_session{};
		std::mutex g_write_mutex;
		thread_local thread_role g_thread_role{thread_role::unknown};

		std::filesystem::path trace_path()
		{
			HMODULE module{};
			const auto address = reinterpret_cast<LPCWSTR>(
			    reinterpret_cast<std::uintptr_t>(&trace_path));
			if (GetModuleHandleExW(
			        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
			            | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			        address,
			        &module))
			{
				std::wstring path(32768, L'\0');
				const auto length = GetModuleFileNameW(
				    module,
				    path.data(),
				    static_cast<DWORD>(path.size()));
				if (length != 0 && length < path.size())
				{
					path.resize(length);
					return std::filesystem::path(path).parent_path()
					    / L"KCD2MP-join.log";
				}
			}
			return std::filesystem::current_path() / L"KCD2MP-join.log";
		}

		std::string_view filename(std::string_view path)
		{
			const auto slash = path.find_last_of("/\\");
			return slash == std::string_view::npos
			    ? path
			    : path.substr(slash + 1);
		}

		void append(std::string_view line) noexcept
		{
			try
			{
				std::scoped_lock lock(g_write_mutex);
				const auto path = trace_path();
				const auto file = CreateFileW(
				    path.c_str(),
				    FILE_APPEND_DATA,
				    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				    nullptr,
				    OPEN_ALWAYS,
				    FILE_ATTRIBUTE_NORMAL,
				    nullptr);
				if (file == INVALID_HANDLE_VALUE)
				{
					OutputDebugStringA(std::string(line).c_str());
					return;
				}
				DWORD written{};
				WriteFile(
				    file,
				    line.data(),
				    static_cast<DWORD>(line.size()),
				    &written,
				    nullptr);
				FlushFileBuffers(file);
				CloseHandle(file);
			}
			catch (...)
			{
				// Diagnostics must never become a second crash source.
			}
		}
	}

	void set_thread_role(thread_role role) noexcept
	{
		g_thread_role = role;
	}

	thread_role current_thread_role() noexcept
	{
		return g_thread_role;
	}

	const char *thread_role_name(thread_role role) noexcept
	{
		switch (role)
		{
		case thread_role::abi:
			return "abi";
		case thread_role::network:
			return "network";
		case thread_role::kcse_post_update:
			return "kcse-post-update";
		case thread_role::unknown:
		default:
			return "unknown";
		}
	}

	std::uint64_t begin_join(
	    std::string_view server_target,
	    std::source_location location) noexcept
	{
		const auto id = g_session.fetch_add(1, std::memory_order_acq_rel) + 1;
		g_active.store(true, std::memory_order_release);
		try
		{
			write(
			    "join.begin",
			    std::format("target=\"{}\"", server_target),
			    location);
		}
		catch (...)
		{
			write("join.begin", "target formatting failed", location);
		}
		return id;
	}

	void finish_join(
	    std::string_view outcome,
	    std::source_location location) noexcept
	{
		if (!active())
			return;
		write("join.finish", outcome, location);
		g_active.store(false, std::memory_order_release);
	}

	bool active() noexcept
	{
		return g_active.load(std::memory_order_acquire);
	}

	std::uint64_t session_id() noexcept
	{
		return g_session.load(std::memory_order_acquire);
	}

	void write(
	    std::string_view event,
	    std::string_view detail,
	    std::source_location location) noexcept
	{
		if (!active())
			return;
		try
		{
			SYSTEMTIME time{};
			GetLocalTime(&time);
			const auto line = std::format(
			    "[{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}] "
			    "[join={}] [pid={}] [tid={} role={}] [{}:{} {}] {}{}{}\r\n",
			    time.wYear,
			    time.wMonth,
			    time.wDay,
			    time.wHour,
			    time.wMinute,
			    time.wSecond,
			    time.wMilliseconds,
			    session_id(),
			    GetCurrentProcessId(),
			    GetCurrentThreadId(),
			    thread_role_name(current_thread_role()),
			    filename(location.file_name()),
			    location.line(),
			    location.function_name(),
			    event,
			    detail.empty() ? "" : " | ",
			    detail);
			append(line);
		}
		catch (...)
		{
			// Diagnostics must never become a second crash source.
		}
	}

#ifdef _WIN32
	long seh_filter(
	    EXCEPTION_POINTERS *exception,
	    std::string_view event,
	    std::source_location location) noexcept
	{
		try
		{
			const auto *record = exception ? exception->ExceptionRecord : nullptr;
			write(
			    event,
			    std::format(
			        "SEH code=0x{:08X} address={} flags=0x{:08X}",
			        record ? record->ExceptionCode : 0,
			        record ? record->ExceptionAddress : nullptr,
			        record ? record->ExceptionFlags : 0),
			    location);
		}
		catch (...)
		{
		}
		return EXCEPTION_EXECUTE_HANDLER;
	}
#endif
}
