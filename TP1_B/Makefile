TARGET_ARCH = linux64

CC     = g++ 
OPT    = -O3
DEBUG  = -g
OTHER  = -Wall -Wno-deprecated -static
#CFLAGS = $(OPT) $(OTHER)
CFLAGS = $(DEBUG) $(OTHER)

MODULE = run
SRCS = main.cpp top.cpp bitgen.cpp bit2byte.cpp consumer.cpp fifo.cpp
OBJS = $(SRCS:.cpp=.o)

include Makefile.defs
