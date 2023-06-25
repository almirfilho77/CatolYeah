#include "Logger.h"

#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace CatolYeah {

	std::shared_ptr<spdlog::logger> Logger::m_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::m_ClientLogger;

	void Logger::Init()
	{
		/* Creation of core logger */
		std::vector<spdlog::sink_ptr> core_sink;

		auto core_console_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
		//core_console_sink->set_level(spdlog::level::trace);
		
		auto core_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/core.txt", true);
		//core_file_sink->set_level(spdlog::level::trace);

		core_sink.push_back(core_console_sink);
		core_sink.push_back(core_file_sink);
		
		/* Creation of client logger */
		std::vector<spdlog::sink_ptr> client_sink;

		auto client_console_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
		//client_console_sink->set_level(spdlog::level::trace);

		auto client_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/client.txt", true);
		//client_file_sink->set_level(spdlog::level::trace);

		client_sink.push_back(client_console_sink);
		client_sink.push_back(client_file_sink);

		m_CoreLogger = std::make_shared<spdlog::logger>("CATOLYEAH", begin(core_sink), end(core_sink));
		m_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(client_sink), end(client_sink));

		//register it if you need to access it globally
		spdlog::register_logger(m_CoreLogger);
		spdlog::register_logger(m_ClientLogger);

		m_CoreLogger->set_pattern("%T:%e:%f  %L/%n [PID:%P][T:%t]: %s: %!(%#) > %v");
		m_CoreLogger->set_level(spdlog::level::trace);
		m_ClientLogger->set_pattern("%T:%e:%f  %L/%n [PID:%P][T:%t]: %s: %!(%#) > %v");
		m_ClientLogger->set_level(spdlog::level::trace);
	}
}