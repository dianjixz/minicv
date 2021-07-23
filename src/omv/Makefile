CFLAGS=  
LIBS = -lm
CC=gcc
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
MY_DIR := $(dir $(mkfile_path))
# OBJDIR=./build
project=$(MY_DIR)libminiimg.so
# C_SRC = $(wildcard *.c)  
C_SRC += $(MY_DIR)imlib/blob.c
C_SRC += $(MY_DIR)imlib/bayer.c
C_SRC += $(MY_DIR)imlib/collections.c
C_SRC += $(MY_DIR)imlib/sincos_tab.c
C_SRC += $(MY_DIR)imlib/imlib.c
C_SRC += $(MY_DIR)imlib/xyz_tab.c 
C_SRC += $(MY_DIR)imlib/fmath.c 
C_SRC += $(MY_DIR)alloc/fb_alloc.c
C_SRC += 
C_SRC += 
C_SRC +=
C_SRC +=
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC +=
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC +=
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC += 
C_SRC +=
C_SRC += 
C_SRC +=
C_SRC += 
C_SRC +=
C_SRC +=
C_SRC +=
C_SRC +=
C_SRC +=
C_SRC += 





INCLUDE =  -I$(MY_DIR)imlib -I$(MY_DIR)alloc -I$(MY_DIR)common -I$(MY_DIR)config

LDFLAGS += -fPIC -shared
CFLAGS += -fPIC -Wall




C_OBJ = $(patsubst %c, %o, $(C_SRC))  
# CPP_SRC = $(wildcard *.cpp)
CPP_SRC =  
# CPP_OBJ = $(patsubst %cpp, %o, $(CPP_SRC))  
CPP_OBJ =  

$(project): $(C_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(notdir $(C_OBJ)) $(LIBS)
	make clean_o

%.o:%.c
	$(CC) $(INCLUDE) $(CFLAGS) -c $<   
# %.o:%.cpp  
# 	$(CC)++ $(CFLAGS) -o $<  

clean_o:
	rm *.o
clean:  
	rm *~ *.o -f $(project) 
