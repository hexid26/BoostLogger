cc = clang++
target = example
objects = example.o
CXXstd = -std=c++11
perargs = -Wall -O0
src = include/boost_logger.hpp include/logger.hpp include/simpleLogger.hpp
# OS = $(shell echo $$OS)
ifeq ($(OS),Windows_NT)
	#Windows stuff
	LC_OS_NAME = windows
else
	# Linux & OSX stuff
	OS_NAME = $(shell uname -s)
	LC_OS_NAME = $(shell echo $(OS_NAME) | tr '[A-Z]' '[a-z]')
	# * LC_OS_NAME 代表了系统的类型：darwin 为 macOS，linux 为 linux
endif

ifeq ($(LC_OS_NAME),darwin)
	CFLAGS = -lboost_log-mt -lboost_log_setup-mt -lboost_system -lboost_thread-mt
endif
ifeq ($(LC_OS_NAME),linux)
	CFLAGS = -lboost_log -lboost_log_setup -lboost_system -lboost_thread
endif

example_cpp: example.o
	$(cc) example.o -o release/example_cpp $(CFLAGS)
	rm example.o

example.o: example.cpp
	$(cc) ${CXXstd} ${perargs} -c example.cpp

.PHONY : clean checkos
clean:
	rm -rf example_cpp $(objects) *.log
	rm -rf release/*

checkos:
	@echo System Name : $(LC_OS_NAME)