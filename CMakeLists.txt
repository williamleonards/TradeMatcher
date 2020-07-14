cmake_minimum_required(VERSION 3.12)
project(TradeMatcher)

set(CMAKE_CXX_STANDARD 14)

add_executable(TradeMatcher main.cpp User.cpp User.h Order.cpp Order.h Trade.cpp Trade.h TradeEngine.cpp TradeEngine.h test.cpp test.h)
