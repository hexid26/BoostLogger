# BoostLogger

## 基于 boost::log 封装的 log 库

大部分借鉴了 maxcong001 的项目 [boostlogger](https://github.com/maxcong001/boostlogger)

修改成基于同步机制的 logger 库，添加了 'stdout' 输出和 'file' 输出两种 log 记录方式（stream）。

## Build

Makefile 中的内容仅仅在 macOS 上测试，linux 和 Windows 未测试。

vscode 已经配置好 'OSX' 和 'Linux' 的 build function。

## Dependency

- boost 1.70.0
- C++11

Hexid26