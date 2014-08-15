TARGET = a.app
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Libs
BOOST=/home/zaqwes/work/third_party/boost_1_55_0
DETAILS=..

# но лучше бы 11 убрать, т.к. не ясно нужно ли доопределять некоторые функции
QMAKE_CXXFLAGS += -O2 -std=c++11 -Wall -Wno-deprecated -pg

SOURCES += w1_greedy.cc \
  $$DETAILS/details/io_details.cc
#\
#  ../../third_party/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
#  ../launcher.cc

INCLUDEPATH += $$BOOST $$DETAILS
#../../third_party/gmock-1.6.0/fused-src/

LIBS += -lpthread -lrt -ltbb

OBJECTS_DIR = obj