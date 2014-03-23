CC = mpic++
CC_C = mpicc
CFLAGS += #-Wall 

LIBS += -lm -I.
	
SRC_DIR = src
BUILD_DIR = build

#SRCS_C = $(shell ls src/*.c)
#SRCS_CPP = $(shell ls src/*.cpp)
#OBJS_t = ${SRCS_C:.c=.o}
#OBJS_t += ${SRCS_CPP:.cpp=.o}
#OBJS = $( patsubst $(SRC_DIR),   ) 

SOURCES = $(shell find $(SRC_DIR) -name "*.c" -printf "%f\n")
OBJS = $(SOURCES:%.c=$(BUILD_DIR)/%.out)

all: $(OBJS) 
	@echo "Finished"

debug: CFLAGS += -g
debug: all	

$(BUILD_DIR)/%.out: $(SRC_DIR)/%.c
	$(CC_C) $(CPPFLAGS) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.out: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) $@ -o $<

clean:
	rm -f *.o
