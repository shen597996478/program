#CFLAGS = -static -I/usr/local/include -I./ -g -Wall -o2 -std=c++0x
#CFLAGS = -static -I/usr/local/include -I./ -g -Wall -std=c++0x
INC_PATH := -I/usr/local/include \
			-I./ \
			-I/usr/include \
			-I../../common

CFLAGS = -static $(INC_PATH) -g -Wall -std=c++0x
CFLAGS +=   #调试日志开关，编译运行版本的时候注释掉该选项
CC=g++
SRC=$(wildcard *.cpp)
OBJ=$(addprefix ./, $(addsuffix .o, $(basename $(SRC))))
LIBVAR= -ldl -lbase -lpthread 
LIBPATH= 	-L/usr/lib/x86_64-linux-gnu/ \
			-L../../../out/lib
TARGET=testserver
#TARGET_BIN=./libnet.a


all: $(TARGET)
 
$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBPATH) $(LIBVAR)
#$(TARGET_BIN): $(OBJ)
#	$(AR) -rcs $@ $^
#	rm -f $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< 


clean:
	rm -f $(TARGET) $(OBJ)
