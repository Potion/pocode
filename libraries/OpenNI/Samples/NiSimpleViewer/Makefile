OSTYPE := $(shell uname -s)

BIN_DIR = ../Bin

INC_DIRS = ../../Include =/usr/include/ni

SRC_FILES = ./*.cpp

EXE_NAME = Sample-NiSimpleViewer

ifeq ("$(OSTYPE)","Darwin")
	LDFLAGS += -framework OpenGL -framework GLUT
else
	USED_LIBS += glut
endif

USED_LIBS += OpenNI

LIB_DIRS += ../../Lib
include ../Build/Common/CommonCppMakefile

