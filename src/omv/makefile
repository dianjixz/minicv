CFLAGS=  
LIBS = -lm

# CC = /opt/v831/toolchain-sunxi-musl/toolchain/bin/arm-openwrt-linux-muslgnueabi-gcc
# INCLUDE =  -Iomv/imlib -Iomv/alloc -Iomv/common -Iomv/config -I/opt/v831/usr/include -I/opt/v831/toolchain-sunxi-musl/toolchain/usr/include -I/opt/v831/toolchain-sunxi-musl/toolchain/include -I/opt/v831/toolchain-sunxi-musl/toolchain/include/fortify -I/opt/v831/usr/include/python3.8
# LIBDIR = -L./omv -L/opt/v831/usr/lib -L/opt/v831/lib -L/opt/v831/toolchain-sunxi-musl/toolchain/usr/lib -L/opt/v831/toolchain-sunxi-musl/toolchain/lib


CC = gcc
INCLUDE =  -I$(MY_DIR)img/include -I$(MY_DIR)include -I$(MY_DIR)common -I$(MY_DIR)config




mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
MY_DIR := $(dir $(mkfile_path))
# OBJDIR=./build
project=$(MY_DIR)libminiimg.so
# C_SRC = $(wildcard *.c)  
C_SRC += $(MY_DIR)fb_alloc.c
C_SRC += $(MY_DIR)umm_malloc.c
C_SRC += $(MY_DIR)xalloc.c
C_SRC += $(MY_DIR)framebuffer.c


C_SRC += $(MY_DIR)array.c
# C_SRC += $(MY_DIR)vfs_wrapper.c
C_SRC += $(MY_DIR)mutex.c
# C_SRC += $(MY_DIR)ringbuf.c


C_SRC += $(MY_DIR)img/agast.c
C_SRC += $(MY_DIR)img/edge.c    
C_SRC += $(MY_DIR)img/fmath.c        
C_SRC += $(MY_DIR)img/fsort.c  
C_SRC += $(MY_DIR)img/imlib.c        
C_SRC += $(MY_DIR)img/kmeans.c   
# C_SRC += $(MY_DIR)img/mjpeg.c             
C_SRC += $(MY_DIR)img/qrcode.c            
C_SRC += $(MY_DIR)img/stats.c
C_SRC += $(MY_DIR)img/apriltag.c  
C_SRC += $(MY_DIR)img/clahe.c        
C_SRC += $(MY_DIR)img/eye.c          
C_SRC += $(MY_DIR)img/lab_tab.c  
# C_SRC += $(MY_DIR)img/orb.c               
C_SRC += $(MY_DIR)img/qsort.c             
# C_SRC += $(MY_DIR)img/bayer.c    
C_SRC += $(MY_DIR)img/collections.c  
C_SRC += $(MY_DIR)img/fast.c    
# C_SRC += $(MY_DIR)img/font.c         
# C_SRC += $(MY_DIR)img/gif.c    
C_SRC += $(MY_DIR)img/integral.c     
# C_SRC += $(MY_DIR)img/lbp.c      
C_SRC += $(MY_DIR)img/phasecorrelation.c  
C_SRC += $(MY_DIR)img/rainbow_tab.c       
C_SRC += $(MY_DIR)img/template.c
C_SRC += $(MY_DIR)img/binary.c    
# C_SRC += $(MY_DIR)img/fft.c          
# C_SRC += $(MY_DIR)img/haar.c   
C_SRC += $(MY_DIR)img/integral_mw.c  
C_SRC += $(MY_DIR)img/line.c     
C_SRC += $(MY_DIR)img/point.c            
C_SRC += $(MY_DIR)img/rectangle.c         
C_SRC += $(MY_DIR)img/xyz_tab.c
C_SRC += $(MY_DIR)img/blob.c      
C_SRC += $(MY_DIR)img/dmtx.c         
C_SRC += $(MY_DIR)img/hog.c    
# C_SRC += $(MY_DIR)img/jpeg.c         
C_SRC += $(MY_DIR)img/lsd.c      
C_SRC += $(MY_DIR)img/pool.c              
C_SRC += $(MY_DIR)img/selective_search.c  
C_SRC += $(MY_DIR)img/zbar.c
# C_SRC += $(MY_DIR)img/bmp.c       
C_SRC += $(MY_DIR)img/draw.c         
C_SRC += $(MY_DIR)img/filter.c  
C_SRC += $(MY_DIR)img/hough.c  
# C_SRC += $(MY_DIR)img/jpegd.c        
C_SRC += $(MY_DIR)img/mathop.c   
# C_SRC += $(MY_DIR)img/ppm.c               
C_SRC += $(MY_DIR)img/sincos_tab.c
C_SRC += $(MY_DIR)img/rgb2rgb_tab.c
C_SRC += $(MY_DIR)img/yuv_tab.c

# C_SRC += $(MY_DIR)config/mini.c




LDFLAGS += -fPIC -shared -g
CFLAGS += -fPIC -Wall -g




C_OBJ = $(patsubst %c, %o, $(C_SRC))  
# CPP_SRC = $(wildcard *.cpp)
CPP_SRC =  
# CPP_OBJ = $(patsubst %cpp, %o, $(CPP_SRC))  
CPP_OBJ =  

$(project): $(C_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(notdir $(C_OBJ)) $(LIBS) $(INCLUDE)
	make clean_o

%.o:%.c
	$(CC) $(INCLUDE) $(CFLAGS) -c $<   
# %.o:%.cpp  
# 	$(CC)++ $(CFLAGS) -o $<  

clean_o:
	rm *.o
clean:  
	rm *~ *.o -f $(project) 
