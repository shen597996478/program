CFLAGS = -static -I/usr/local/include -I./ -I/usr/include -Wall -std=c++0x
CFLAGS += -g   #调试日志开关，编译运行版本的时候注释掉该选项
CC=g++
SRC=$(wildcard *.cpp)
OBJ=$(addprefix ./, $(addsuffix .o, $(basename $(SRC))))
LIBVAR= -ldl -lpthread
LIBPATH= -L/usr/lib/x86_64-linux-gnu/
TARGET_BIN=../../../out/lib/libbase.a


all: $(TARGET_BIN)
$(TARGET_BIN): $(OBJ)
	$(AR) -rcs $@ $^ 
	rm -f $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< 


clean:
	rm -f $(TARGET) $(OBJ)
