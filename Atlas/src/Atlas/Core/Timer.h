#pragma once

#include <chrono>

#include "Core.h"

namespace Atlas
{
	template<typename Fn>
	class Timer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		const char* m_Name;
		bool m_Stopped;
		Fn m_Func;

	public:
		Timer(const char* name, Fn&& func)
			: m_Func(func), m_Name(name), m_StartTimepoint(std::chrono::high_resolution_clock::now()), m_Stopped(false) {}

		~Timer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			m_Stopped = true;

			float duration = (end - start) * 0.001f;

			m_Func({ m_Name, duration });
		}
	};
}

