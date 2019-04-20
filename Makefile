# Makefile

TARGET = scan
WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -pthread -std=c99 -g $(WARNING) $(ERROR)

SRCS = main.c utils.c bmpfunc.c histogram.c

OBJS = $(SRCS:%.c=%.o)

VALGRIND = valgrind --tool=memcheck --verbose --leak-check=full --log-file=valgrind.log ./scan exampleImages/RV2Small.bmp outputImages/outputRV2Small.bmp 3

$(TARGET): $(OBJS) 
	$(GCC) $(OBJS) -o $(TARGET)

.c.o: 
	$(GCC) -c $*.c 

val: scan 
	$(VALGRIND)

testall: test1 test2 test3 test4

test1: scan
	./scan exampleImages/RV2.bmp outputImages/outputRV2.bmp 3

test2: scan
	./scan exampleImages/RV.bmp outputImages/outputRV.bmp 5

test3: scan
	./scan exampleImages/RV2Small.bmp outputImages/outputRV2Small.bmp 3

test4: scan
	./scan exampleImages/CondProb.bmp outputImages/outputCondProb.bmp 1

clean: # remove all machine generated files
	rm -f $(OBJS) $(TARGET)
