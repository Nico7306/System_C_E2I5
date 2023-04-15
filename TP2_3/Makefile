TARGET_ARCH = linux64

CC     =  g++
OPT    = -O3 
DEBUG  = -g
OTHER  = -Wall  -Wno-deprecated
#CFLAGS = $(OPT) $(OTHER)
CFLAGS = $(DEBUG) $(OTHER)

MODULE = run
SRCS = main.cpp cpu.cpp kernel.cpp os.cpp processes.cpp top.cpp timer.cpp
OBJS = $(SRCS:.cpp=.o)

include Makefile.defs
