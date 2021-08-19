CFLAGS=  
LIBS = -lm -lpython3.8 -lminiimg




CC=/opt/v831/toolchain-sunxi-musl/toolchain/bin/arm-openwrt-linux-muslgnueabi-gcc
INCLUDE =  -Iomv/img/include -Iomv/include -I/opt/v831/usr/include -I/opt/v831/toolchain-sunxi-musl/toolchain/usr/include -I/opt/v831/toolchain-sunxi-musl/toolchain/include -I/opt/v831/toolchain-sunxi-musl/toolchain/include/fortify -I/opt/v831/usr/include/python3.8
LIBDIR = -L./omv -L/opt/v831/usr/lib -L/opt/v831/lib -L/opt/v831/toolchain-sunxi-musl/toolchain/usr/lib -L/opt/v831/toolchain-sunxi-musl/toolchain/lib




# CC = gcc
# INCLUDE =  -Iomv/img/include -Iomv/include -I/usr/include/python3.8
# LIBDIR += -L./omv





# OBJDIR=./build
project=./minicv.so
# C_SRC = $(wildcard *.c)  
C_SRC += py_modules/py_image.c
C_SRC += py_modules/tanstation.c
C_SRC += 




LDFLAGS += -fPIC  -shared -Wl,-rpath=./ -g
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
