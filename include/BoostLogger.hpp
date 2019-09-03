#pragma once

#include <boost/core/null_deleter.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/attribute_cast.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <fstream>
#include <iostream>
#include <string>

// Related headers
#include <boost/log/sinks/block_on_overflow.hpp>
#include <boost/log/sinks/bounded_fifo_queue.hpp>
#include <boost/log/sinks/bounded_ordering_queue.hpp>
#include <boost/log/sinks/drop_on_overflow.hpp>
#include <boost/log/sinks/unbounded_fifo_queue.hpp>
#include <boost/log/sinks/unbounded_ordering_queue.hpp>

#include "Logger.hpp"
#include <chrono>
#include <thread>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;

thread_local static boost::log::sources::severity_logger<log_level> lg;

typedef sinks::synchronous_sink<sinks::text_ostream_backend> sink_t;
static std::ostream &operator<<(std::ostream &strm, log_level level) {
  static const char *strings[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

  if (static_cast<std::size_t>(level) < sizeof(strings) / sizeof(*strings))
    strm << strings[level];
  else
    strm << static_cast<int>(level);

  return strm;
}

class BoostLogger : public logger_iface {
public:
  BoostLogger() : m_level(log_level::error_level) {}
  ~BoostLogger() {}

  void init() override {
    logging::add_common_attributes();
    core = logging::core::get();
    // boost::shared_ptr<sinks::text_ostream_backend> backend = boost::make_shared<sinks::text_ostream_backend>();
    boost::shared_ptr<sinks::text_ostream_backend> backend = boost::make_shared<sinks::text_ostream_backend>();
    if (!_sink) {
      _sink.reset(new sink_t(backend));
      core->add_sink(_sink);
    }

    core->add_global_attribute("ThreadID", attrs::current_thread_id());
    core->add_global_attribute("Process", attrs::current_process_name());
    _sink->set_filter(expr::attr<log_level>("Severity") >= m_level);
    _sink->set_formatter(
        expr::stream << "[" << expr::attr<unsigned int>("LineID") << "]"
                     << "<" << expr::attr<log_level>("Severity") << ">{"
                     << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "}"
                     << "[" << expr::attr<std::string>("Process") << ":"
                     << expr::attr<attrs::current_thread_id::value_type>("ThreadID") << "]" << expr::smessage);
    sink_t::locked_backend_ptr p = _sink->locked_backend();
    // ! 文件 log 的指定
    p->add_stream(boost::make_shared<std::ofstream>("example.log"));
    p->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
  }
  void stop() override {
    warn_log("boost logger stopping");
    _sink->flush();
    // _sink->stop();
    core->remove_sink(_sink);
  }
  void set_log_level(log_level level) override {
    m_level = level;

    if (_sink) {
      _sink->set_filter(expr::attr<log_level>("Severity") >= m_level);
    }
  }

  log_level get_log_level() override { return m_level; }

  // ! 被注释的代码是带颜色输出 msg，但是会在文件 log 中加入额外符号。可在不适用文件 log 时开启颜色
  void debug_log(const std::string &msg) override { BOOST_LOG_SEV(lg, debug_level) << msg << std::endl; }
  void info_log(const std::string &msg) override {
    // BOOST_LOG_SEV(lg, info_level) << blue << msg << normal << std::endl;
    BOOST_LOG_SEV(lg, info_level) << msg << std::endl;
  }
  void warn_log(const std::string &msg) override {
    // BOOST_LOG_SEV(lg, warn_level) << yellow << msg << normal << std::endl;
    BOOST_LOG_SEV(lg, warn_level) << msg << std::endl;
  }
  void error_log(const std::string &msg) override {
    // BOOST_LOG_SEV(lg, error_level) << red << msg << normal << std::endl;
    BOOST_LOG_SEV(lg, error_level) << msg << std::endl;
  }
  void critical_log(const std::string &msg) override {
    // BOOST_LOG_SEV(lg, critical_level) << red << msg << normal << std::endl;
    BOOST_LOG_SEV(lg, critical_level) << msg << std::endl;
  }

private:
  log_level m_level;
  boost::shared_ptr<logging::core> core;
  boost::shared_ptr<sink_t> _sink;
};