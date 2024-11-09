#ifndef __SCOPED_TIMER_H__
#define __SCOPED_TIMER_H__
#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace CatolYeah
{
	namespace Instrumentation
	{
		struct ProfileResult
		{
			std::string Name;
			std::chrono::microseconds StartTime;
			std::chrono::microseconds ElapsedTime;
			std::thread::id ThreadId;
		};

		struct InstrumentationSession
		{
			std::string Name;
		};
		
		class Instrumentor
		{
		public:
			Instrumentor(const Instrumentor &) = delete;
			Instrumentor(Instrumentor&&) = delete;

			void BeginSession(const std::string &name, const std::string &filepath = "results.json")
			{
				std::lock_guard lock(m_mutex);
				if (m_currentSession != nullptr)
				{
					return;
				}
				m_outputStream.open(filepath);
				if (m_outputStream.is_open())
				{
					m_currentSession = new InstrumentationSession({ name });
					m_WriteHeader();
				}
			}

			void EndSession()
			{
				std::lock_guard lock(m_mutex);
				if (m_currentSession)
				{
					m_WriteFooter();
					m_outputStream.close();
					delete m_currentSession;
					m_currentSession = nullptr;
				}
			}

			void WriteProfile(const ProfileResult& result)
			{
				std::stringstream json;

				json << std::setprecision(3) << std::fixed;
				json << ",{";
				json << "\"cat\":\"function\",";
				json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
				json << "\"name\":\"" << result.Name << "\",";
				json << "\"ph\":\"X\",";
				json << "\"pid\":0,";
				json << "\"tid\":" << result.ThreadId << ",";
				json << "\"ts\":" << result.StartTime.count();
				json << "}";

				std::lock_guard lock(m_mutex);
				if (m_currentSession)
				{
					m_outputStream << json.str();
					m_outputStream.flush();
				}
			}

			bool IsSessionValid() const
			{
				if (m_currentSession != nullptr)
				{
					return true;
				}
				return false;
			}

			static Instrumentor &Get()
			{
				static Instrumentor instance;
				return instance;
			}

		private:
			Instrumentor()
				: m_currentSession(nullptr)
			{
			}

			~Instrumentor()
			{
				EndSession();
			}

			void m_WriteHeader()
			{
				m_outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
				m_outputStream.flush();
			}

			void m_WriteFooter()
			{
				m_outputStream << "]}";
				m_outputStream.flush();
			}

		private:
			std::mutex m_mutex;
			std::ofstream m_outputStream;
			InstrumentationSession *m_currentSession;
		};

		using steady_clock_t = std::conditional<std::chrono::high_resolution_clock::is_steady,
			std::chrono::high_resolution_clock, std::chrono::steady_clock>::type;
		using timepoint_t = std::chrono::time_point<steady_clock_t>;

		class ScopedTimer
		{
		public:
			ScopedTimer(const std::string &name)
				: m_name(name), m_stopped(false)
			{
				m_startTimepoint = steady_clock_t::now();
			}

			void Stop()
			{
				if (Instrumentor::Get().IsSessionValid() == false)
				{
					m_stopped = true;
					return;
				}

				auto endTimepoint = steady_clock_t::now();

				std::chrono::microseconds start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch();
				std::chrono::microseconds end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch();
				std::chrono::microseconds elapsed = end - start;

				Instrumentor::Get().WriteProfile({ m_name, start, elapsed, std::this_thread::get_id() });
				
				m_stopped = true;
			}

			~ScopedTimer()
			{
				if (!m_stopped)
				{
					Stop();
				}
			}

		private:
			std::string m_name;
			timepoint_t m_startTimepoint;
			bool m_stopped;
		};
	}
}

#define CY_PROFILING 1
#if CY_PROFILING
	
#define CY_PROFILING_BEGIN_SESSION(name, filepath) ::CatolYeah::Instrumentation::Instrumentor::Get().BeginSession(name, filepath)
#define CY_PROFILING_END_SESSION() ::CatolYeah::Instrumentation::Instrumentor::Get().EndSession()
#define CY_PROFILING_SCOPED_TIMER(name) ::CatolYeah::Instrumentation::ScopedTimer timer##__LINE__(name)
#define CY_PROFILING_FUNCTION_TIMER() CY_PROFILING_SCOPED_TIMER(__FUNCSIG__)

#else

#define CY_PROFILING_BEGIN_SESSION(name, filepath) 
#define CY_PROFILING_END_SESSION() 
#define CY_PROFILING_SCOPED_TIMER(name) 
#define CY_PROFILING_FUNCTION_TIMER() 

#endif

#endif /*__SCOPED_TIMER_H__*/