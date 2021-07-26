CFLAGS=  
LIBS = -lm -lpython3.8 -lminiimg
CC=gcc
# OBJDIR=./build
project=./minicv.so
# C_SRC = $(wildcard *.c)  
C_SRC += py_modules/py_image.c
C_SRC += py_modules/tanstation.c
C_SRC += 

INCLUDE =  -Iomv/imlib -Iomv/alloc -Iomv/common -Iomv/config -I/usr/include/python3.8
LIBDIR = -L./omv

LDFLAGS += -fPIC  -shared -Wl,-rpath=./omv -g
CFLAGS += -fPIC -Wall -g




C_OBJ = $(patsubst %c, %o, $(C_SRC))  
# CPP_SRC = $(wildcard *.cpp)
CPP_SRC =  
# CPP_OBJ = $(patsubst %cpp, %o, $(CPP_SRC))  
CPP_OBJ =  




$(project): $(C_OBJ)
	$(CC) -o $@ $(notdir $(C_OBJ)) $(LIBS) $(LIBDIR) $(LDFLAGS)
	make clean_o

%.o:%.c
	$(CC) $(INCLUDE) $(CFLAGS) -c $<   
# %.o:%.cpp  
# 	$(CC)++ $(CFLAGS) -o $<  

clean_o:
	rm *.o
clean:  
	rm *~ *.o -f $(project) 
	make -f omv/Makefile clean

test:
	python3 test.py