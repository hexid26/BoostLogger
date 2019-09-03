#pragma once

#include "Logger.hpp"

class SimpleLogger : public logger_iface {
public:
  SimpleLogger() : m_level(log_level::error_level) {}
  ~SimpleLogger() {}
  void init() override {}
  void stop() override {}
  void set_log_level(log_level level) override { m_level = level; }

  log_level get_log_level() override { return m_level; }

  void debug_log(const std::string &msg) override { std::cout << msg << std::endl; }
  void info_log(const std::string &msg) override { std::cout << blue << msg << normal << std::endl; }
  void warn_log(const std::string &msg) override { std::cout << yellow << msg << normal << std::endl; }
  void error_log(const std::string &msg) override { std::cout << red << msg << normal << std::endl; }
  void critical_log(const std::string &msg) override { std::cout << red << msg << normal << std::endl; }

private:
  log_level m_level;
};