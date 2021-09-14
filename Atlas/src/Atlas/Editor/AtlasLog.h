#pragma once

#include <string>
#include <array>

namespace Atlas {

	namespace Utils {
		enum class LogLevel
		{
			ATL_TRACE = 0,
			ATL_INFO,
			ATL_WARN,
			ATL_ERROR,
			NONE
		};
	}

	class AtlasLog
	{
	private:
		const size_t MAX_LOG_MESSAGES = 5000;
		bool m_AutoScroll;

	public:
		AtlasLog();

		void Clear();
		void AddLog(const std::string& log, Utils::LogLevel logLevel = Utils::LogLevel::ATL_TRACE);
		void Draw(const char* title, bool* p_open = NULL);

	};

}
