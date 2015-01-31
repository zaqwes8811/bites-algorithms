TARGET = a.app
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Libs
BOOST=/home/zaqwes/work/third_party/boost_1_55_0
DETAILS=../src

# но лучше бы 11 убрать, т.к. не ясно нужно ли доопределять некоторые функции
# 
QMAKE_CXXFLAGS += -O2  -Wall -Wno-deprecated -pg -std=c++11

SOURCES += \
  ds_apps.cc \
#  thread_safe_queue_test.cc \
#  some_lock_free_magic_test.cc \
  hash_tables/choose_test.cc \
#  $$DETAILS/details/io_details.cc \
#  thread_safe_ds_test.cc \
  skip_list_rb_alv_trees_test.cc \
  ../3rdparty/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
  ../src/launcher.cc

INCLUDEPATH += \
  $$BOOST \
  $$DETAILS \
  ../ \
  ../3rdparty/gmock-1.6.0/fused-src/ \
  /home/zaqwes/work/in-the-vicinity-cc/src

LIBS += -lpthread -lrt -ltbb
LIBS += -L$$BOOST/stage/lib -lboost_thread -lboost_system

OBJECTS_DIR = obj