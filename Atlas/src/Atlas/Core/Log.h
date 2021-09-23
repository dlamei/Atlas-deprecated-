#pragma once

#include "Core.h"

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)


#include "glm/gtx/string_cast.hpp"
#include "Atlas/Editor/AtlasLog.h"



namespace Atlas {


	class Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::ostringstream s_OStream;
		static AtlasLog m_Logger;

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_ClientLogger; };

		inline static std::ostringstream& GetCoreOStream() { return s_OStream; };
		inline static AtlasLog& GetAtlasLogger() { return m_Logger; }

		template<typename... Args>
		static void AtlasLog(bool Core, Utils::LogLevel logLevel, const Args&... msg)
		{
			auto& logger = s_CoreLogger;
			if (!Core) logger = s_ClientLogger;

			switch (logLevel)
			{
			case Atlas::Utils::LogLevel::ATL_TRACE:
				logger->trace(msg...);
				break;

			case Atlas::Utils::LogLevel::ATL_INFO:
				logger->info(msg...);
				break;

			case Atlas::Utils::LogLevel::ATL_WARN:
				logger->warn(msg...);
				break;

			case Atlas::Utils::LogLevel::ATL_ERROR:
				logger->error(msg...);
				break;

			default:
				logger->trace(msg...);
				break;
			}

			m_Logger.AddLog(s_OStream.str().c_str(), logLevel);
			s_OStream.str("");
			s_OStream.clear();
		}


	};



}

//#define ATL_WINDOW_APP
#ifdef ATL_WINDOW_APP
// Core log
#define ATL_CORE_TRACE(...)		::Atlas::Log::AtlasLog(true, ::Atlas::Utils::LogLevel::ATL_TRACE, __VA_ARGS__);
#define ATL_CORE_INFO(...)		::Atlas::Log::AtlasLog(true, ::Atlas::Utils::LogLevel::ATL_INFO, __VA_ARGS__);
#define ATL_CORE_WARN(...)		::Atlas::Log::AtlasLog(true, ::Atlas::Utils::LogLevel::ATL_WARN, __VA_ARGS__);
#define ATL_CORE_ERROR(...)		::Atlas::Log::AtlasLog(true, ::Atlas::Utils::LogLevel::ATL_ERROR, __VA_ARGS__);

// Client log
#define ATL_TRACE(...)		::Atlas::Log::AtlasLog(false, ::Atlas::Utils::LogLevel::ATL_TRACE, __VA_ARGS__);
#define ATL_INFO(...)		::Atlas::Log::AtlasLog(false, ::Atlas::Utils::LogLevel::ATL_INFO, __VA_ARGS__);
#define ATL_WARN(...)		::Atlas::Log::AtlasLog(false, ::Atlas::Utils::LogLevel::ATL_WARN, __VA_ARGS__);
#define ATL_ERROR(...)		::Atlas::Log::AtlasLog(false, ::Atlas::Utils::LogLevel::ATL_ERROR, __VA_ARGS__);
#else
#define ATL_CORE_TRACE(...)		::Atlas::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define ATL_CORE_INFO(...)		::Atlas::Log::GetCoreLogger()->info(__VA_ARGS__);
#define ATL_CORE_WARN(...)		::Atlas::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define ATL_CORE_ERROR(...)		::Atlas::Log::GetCoreLogger()->error(__VA_ARGS__);

// Client log
#define ATL_TRACE(...)		::Atlas::Log::GetLogger()->trace(__VA_ARGS__);
#define ATL_INFO(...)		::Atlas::Log::GetLogger()->info(__VA_ARGS__);
#define ATL_WARN(...)		::Atlas::Log::GetLogger()->warn(__VA_ARGS__);
#define ATL_ERROR(...)		::Atlas::Log::GetLogger()->error(__VA_ARGS__);
#endif
