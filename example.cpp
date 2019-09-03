// ! 第一行一定需要这个东西
#define BOOST_LOG_DYN_LINK 0

// ! BoostLogger 和 SimpleLogger 二选一
#include "include/BoostLogger.hpp"
// #include "include/SimpleLogger.hpp"

#include <chrono>
#include <thread>
int main() {
  // ! BoostLogger 和 SimpleLogger 二选一
  INIT_LOGGER(new BoostLogger());
  // INIT_LOGGER(new SimpleLogger());
  // ! 设置日志记录的等级
  SET_LOG_LEVEL(debug);

  // * 使用方法
  __LOG(debug, "hello logger! this is critical log!");
  __LOG(error, "hello logger! this is error log!");
  __LOG(warn, "hello logger! this is warn log");
  __LOG(info, "hello logger! this is info log");
  __LOG(debug, "hello logger! this is debug log"/* << (void *)ACTIVE_LOGGER_INSTANCE*/);

  // // std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // ! 销毁 logger，使用 filestream 的时候一定要销毁，否则日志不完整
  DESTROY_LOGGER;
}
