#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"



namespace Atlas {

	class Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_ClientLogger; };
	};

}

// Core log
#define ATL_CORE_TRACE(...)		::Atlas::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ATL_CORE_INFO(...)		::Atlas::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ATL_CORE_WARN(...)		::Atlas::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ATL_CORE_ERROR(...)		::Atlas::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ATL_CORE_CRITICAL(...)	::Atlas::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log
#define ATL_TRACE(...)		::Atlas::Log::GetLogger()->trace(__VA_ARGS__)
#define ATL_INFO(...)		::Atlas::Log::GetLogger()->info(__VA_ARGS__)
#define ATL_WARN(...)		::Atlas::Log::GetLogger()->warn(__VA_ARGS__)
#define ATL_ERROR(...)		::Atlas::Log::GetLogger()->error(__VA_ARGS__)
#define ATL_CRITICAL(...)	::Atlas::Log::GetLogger()->critical(__VA_ARGS__)
