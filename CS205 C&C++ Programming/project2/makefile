target = multiply
dir = ./src
source = $(wildcard ./src/*.c)
Include = -I./inc

OBJS = $(patsubst %.c,%.o,$(source))
CXX = gcc
CFLAGES = -c -Wall
CXXCFLAGES = $(CFLAGES) -O3

$(target) : $(OBJS)
	$(CXX) -o $@ $(OBJS)
%.o : %.c
	$(CXX) $(CXXCFLAGES) $^ -o $@ $(Include)

.PHONY: clean
clean:
	rm -f $(dir)/*.o $(target)