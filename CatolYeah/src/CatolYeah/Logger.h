#ifndef __LOGGER_H__
#define __LOGGER_H__
#pragma once

#include "Core.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace CatolYeah {

	class CATOLYEAH_API Logger
	{
	public:
		Logger() {}
		~Logger() {}

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;
	};

}

/* Core macros */
#ifdef CY_CONFIG_DEBUG /* CORE Debug only log levels */
	#define CY_CORE_TRACE(...)   SPDLOG_LOGGER_TRACE(::CatolYeah::Logger::GetCoreLogger(),__VA_ARGS__)
	#define CY_CORE_DEBUG(...)   SPDLOG_LOGGER_DEBUG(::CatolYeah::Logger::GetCoreLogger(),__VA_ARGS__)
#else
	#define CY_CORE_TRACE(...)   
	#define CY_CORE_DEBUG(...)
#endif /* CORE Debug only log levels */

#define CY_CORE_INFO(...)    SPDLOG_LOGGER_INFO(::CatolYeah::Logger::GetCoreLogger(),__VA_ARGS__)
#define CY_CORE_WARN(...)    SPDLOG_LOGGER_WARN(::CatolYeah::Logger::GetCoreLogger(),__VA_ARGS__)
#define CY_CORE_ERROR(...)   SPDLOG_LOGGER_ERROR(::CatolYeah::Logger::GetCoreLogger(),__VA_ARGS__)
#define CY_CORE_FATAL(...)   SPDLOG_LOGGER_CRITICAL(::CatolYeah::Logger::GetCoreLogger(),__VA_ARGS__)

/* Client macros */
#ifdef CY_CONFIG_DEBUG /* CLIENT Debug only log levels */
	#define CY_TRACE(...)		 SPDLOG_LOGGER_TRACE(::CatolYeah::Logger::GetClientLogger(),__VA_ARGS__)
	#define CY_DEBUG(...)		 SPDLOG_LOGGER_DEBUG(::CatolYeah::Logger::GetClientLogger(),__VA_ARGS__)
#else
	#define CY_TRACE(...)
	#define CY_DEBUG(...)
#endif /* CLIENT Debug only log levels*/
#define CY_INFO(...)		 SPDLOG_LOGGER_INFO(::CatolYeah::Logger::GetClientLogger(),__VA_ARGS__)
#define CY_WARN(...)		 SPDLOG_LOGGER_WARN(::CatolYeah::Logger::GetClientLogger(),__VA_ARGS__)
#define CY_ERROR(...)		 SPDLOG_LOGGER_ERROR(::CatolYeah::Logger::GetClientLogger(),__VA_ARGS__)
#define CY_FATAL(...)		 SPDLOG_LOGGER_CRITICAL(::CatolYeah::Logger::GetClientLogger(),__VA_ARGS__)

#endif /* __LOGGER_H__ */
