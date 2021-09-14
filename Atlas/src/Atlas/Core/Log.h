#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include "Atlas/Editor/AtlasLog.h"

#include <sstream>

namespace Atlas {


	class Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::ostringstream s_CoreOStream;
		static AtlasLog m_Logger;

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_ClientLogger; };

		inline static std::ostringstream& GetCoreOStream() { return s_CoreOStream; };
		inline static AtlasLog& GetAtlasLogger() { return m_Logger; }

		template<typename... Args>
		static void AtlasCoreTrace(const Args&... msg)
		{
			s_CoreLogger->trace(msg...);
			m_Logger.AddLog(s_CoreOStream.str().c_str(), Utils::LogLevel::ATL_TRACE);
			s_CoreOStream.str("");
			s_CoreOStream.clear();
		}

		template<typename... Args>
		static void AtlasCoreInfo(const Args&... msg)
		{
			s_CoreLogger->info(msg...);
			m_Logger.AddLog(s_CoreOStream.str().c_str(), Utils::LogLevel::ATL_INFO);
			s_CoreOStream.str("");
			s_CoreOStream.clear();
		}

		template<typename... Args>
		static void AtlasCoreWarn(const Args&... msg)
		{
			s_CoreLogger->warn(msg...);
			m_Logger.AddLog(s_CoreOStream.str().c_str(), Utils::LogLevel::ATL_WARN);
			s_CoreOStream.str("");
			s_CoreOStream.clear();
		}

		template<typename... Args>
		static void AtlasCoreError(const Args&... msg)
		{
			s_CoreLogger->error(msg...);
			m_Logger.AddLog(s_CoreOStream.str().c_str(), Utils::LogLevel::ATL_ERROR);
			s_CoreOStream.str("");
			s_CoreOStream.clear();
		}

	};

}

//#define ATL_CORE_TRACE(...)		::Atlas::Log::GetCoreLogger()->trace(__VA_ARGS__)
//#define ATL_CORE_INFO(...)		::Atlas::Log::GetCoreLogger()->info(__VA_ARGS__)
//#define ATL_CORE_WARN(...)		::Atlas::Log::GetCoreLogger()->warn(__VA_ARGS__)
//#define ATL_CORE_ERROR(...)		::Atlas::Log::GetCoreLogger()->error(__VA_ARGS__)
//#define ATL_CORE_CRITICAL(...)	::Atlas::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Core log
#define ATL_CORE_TRACE(...)		::Atlas::Log::AtlasCoreTrace(__VA_ARGS__);
#define ATL_CORE_INFO(...)		::Atlas::Log::AtlasCoreInfo(__VA_ARGS__);
#define ATL_CORE_WARN(...)		::Atlas::Log::AtlasCoreWarn(__VA_ARGS__);
#define ATL_CORE_ERROR(...)		::Atlas::Log::AtlasCoreError(__VA_ARGS__);

// Client log
#define ATL_TRACE(...)		::Atlas::Log::GetLogger()->trace(__VA_ARGS__)
#define ATL_INFO(...)		::Atlas::Log::GetLogger()->info(__VA_ARGS__)
#define ATL_WARN(...)		::Atlas::Log::GetLogger()->warn(__VA_ARGS__)
#define ATL_ERROR(...)		::Atlas::Log::GetLogger()->error(__VA_ARGS__)
#define ATL_CRITICAL(...)	::Atlas::Log::GetLogger()->critical(__VA_ARGS__)
