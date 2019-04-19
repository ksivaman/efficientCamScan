# Makefile

TARGET = scan
WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR) 

SRCS = main.c utils.c bmpfunc.c

OBJS = $(SRCS:%.c=%.o)

VALGRIND = valgrind --tool=memcheck --verbose --leak-check=full --log-file=valgrind.log ./scan exampleImages/RV2.bmp expectedImages/expectedRV2.bmp 3 2

$(TARGET): $(OBJS) 
	$(GCC) $(OBJS) -o $(TARGET)

.c.o: 
	$(GCC) $(TESTFALGS) -c $*.c 

val: scan 
	$(VALGRIND)

testall: test1 test2 test3 test4

test1: scan
	./scan exampleImages/RV2.bmp outputImages/outputRV2.bmp 3 2

test2: scan
	./scan exampleImages/RV.bmp outputImages/outputRV.bmp 5 2

test3: scan
	./scan exampleImages/RV2Small.bmp outputImages/outputRV2Small.bmp 11 2   

test4: scan
	./scan exampleImages/CondProb.bmp outputImages/outputCondProb.bmp 1 2

clean: # remove all machine generated files
	rm -f $(OBJS) $(TARGET)
