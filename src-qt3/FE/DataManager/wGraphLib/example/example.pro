TEMPLATE	= app
CONFIG		= qt thread release 
DEFINES         = LINUX 

INCLUDEPATH     = ../ \
                  ../../common

LIBS            = -L ../ -lwGraph 

MOC_DIR         = tmp

OBJECTS_DIR     = bin

HEADERS		= example.hpp
SOURCES		= example.cpp \
		  main.cpp

TARGET		= example
 

