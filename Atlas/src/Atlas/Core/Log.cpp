#include "atlpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/ostream_sink.h"

namespace Atlas {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::ostringstream Log::s_CoreOStream;
	AtlasLog Log::m_Logger;


	void Log::Init()
	{
		auto ostreamSink = std::make_shared<spdlog::sinks::ostream_sink_st>(s_CoreOStream);

		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = std::make_shared<spdlog::logger>("ATLAS", ostreamSink);
		//s_CoreLogger = spdlog::stdout_color_mt("ATLAS");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}
