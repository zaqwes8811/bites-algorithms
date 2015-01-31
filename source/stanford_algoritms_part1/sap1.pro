TARGET = a.app
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Libs
BOOST=/home/zaqwes/work/third_party/boost_1_55_0
DETAILS=..

# но лучше бы 11 убрать, т.к. не ясно нужно ли доопределять некоторые функции
# -std=c++11
QMAKE_CXXFLAGS += -O2  -Wall -Wno-deprecated -pg -std=c++11

SOURCES += \
  ../launcher.cc \
  ../../3rdparty/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
  ./w6_heap_hash/w6.cc \
  $$DETAILS/details/io_details.cc


INCLUDEPATH += \
  $$BOOST $$DETAILS $$DETAILS/.. \
  ../../3rdparty/gmock-1.6.0/fused-src/

LIBS += -lpthread -lrt -ltbb

OBJECTS_DIR = obj