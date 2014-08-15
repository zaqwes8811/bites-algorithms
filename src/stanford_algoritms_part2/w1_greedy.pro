TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# -std=c++11 -Wall -Wno-deprecated -c -pg

APP=w1_greedy

SOURCES += ./$$APP.cc 
#\
#  ../../third_party/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
#  ../launcher.cc

#INCLUDEPATH += ../../third_party/gmock-1.6.0/fused-src/

LIBS += -lpthread -lrt -ltbb