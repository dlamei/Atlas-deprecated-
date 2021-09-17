#include "atlpch.h"
#include "Log.h"

#pragma warning(push, 0)
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/ostream_sink.h"
#pragma warning(pop)


namespace Atlas {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::ostringstream Log::s_OStream;
	AtlasLog Log::m_Logger;


	void Log::Init()
	{
		auto ostreamSink = std::make_shared<spdlog::sinks::ostream_sink_st>(s_OStream);

		spdlog::set_pattern("%^[%T] %n: %v%$");
#ifdef ATL_WINDOW_APP
		s_CoreLogger = std::make_shared<spdlog::logger>("ATLAS", ostreamSink);
		s_ClientLogger = std::make_shared<spdlog::logger>("APP", ostreamSink);
#else
		s_CoreLogger = spdlog::stdout_color_mt("ATLAS");
		s_ClientLogger = spdlog::stdout_color_mt("APP");
#endif

		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}
