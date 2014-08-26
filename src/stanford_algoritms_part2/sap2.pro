TARGET = a.app
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Libs
BOOST=/home/zaqwes/work/third_party/boost_1_55_0
DETAILS=..
ADOBE=/home/zaqwes/work/third_party/source_release

# но лучше бы 11 убрать, т.к. не ясно нужно ли доопределять некоторые функции
# -std=c++11
# Opt. keys:
# http://publib.boulder.ibm.com/infocenter/comphelp/v7v91/index.jsp?topic=%2Fcom.ibm.xlf91a.doc%2Fxlfug%2Fhu00509.htm
QMAKE_CXXFLAGS += -O3  -Wall -Wno-deprecated -pg -std=c++11

SOURCES += \
  ../launcher.cc \
  ../../3rdparty/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
  w1_greedy.cc \
  w3_dyn_pro.cc \
  w3_dyn_pro_parallel.cc \
  w3_lession_code.cc \
  $$DETAILS/details/io_details.cc


INCLUDEPATH += \
  $$BOOST $$DETAILS $$DETAILS/.. \
  $$ADOBE \
  ../../3rdparty/gmock-1.6.0/fused-src/

LIBS += -lpthread -lrt -ltbb

OBJECTS_DIR = obj