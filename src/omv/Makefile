CFLAGS=  
LIBS = -lm

# CC = /opt/v831/toolchain-sunxi-musl/toolchain/bin/arm-openwrt-linux-muslgnueabi-gcc
# INCLUDE =  -Iomv/imlib -Iomv/alloc -Iomv/common -Iomv/config -I/opt/v831/usr/include -I/opt/v831/toolchain-sunxi-musl/toolchain/usr/include -I/opt/v831/toolchain-sunxi-musl/toolchain/include -I/opt/v831/toolchain-sunxi-musl/toolchain/include/fortify -I/opt/v831/usr/include/python3.8
# LIBDIR = -L./omv -L/opt/v831/usr/lib -L/opt/v831/lib -L/opt/v831/toolchain-sunxi-musl/toolchain/usr/lib -L/opt/v831/toolchain-sunxi-musl/toolchain/lib





CC = gcc
INCLUDE =  -I$(MY_DIR)imlib -I$(MY_DIR)alloc -I$(MY_DIR)common -I$(MY_DIR)config




mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
MY_DIR := $(dir $(mkfile_path))
# OBJDIR=./build
project=$(MY_DIR)libminiimg.so
# C_SRC = $(wildcard *.c)  
C_SRC += $(MY_DIR)alloc/fb_alloc.c
C_SRC += $(MY_DIR)alloc/umm_malloc.c
C_SRC += $(MY_DIR)alloc/xalloc.c


C_SRC += $(MY_DIR)common/array.c
C_SRC += $(MY_DIR)common/ff_wrapper.c
C_SRC += $(MY_DIR)common/ini.c
C_SRC += $(MY_DIR)common/mutex.c
C_SRC += $(MY_DIR)common/ringbuf.c


C_SRC += $(MY_DIR)imlib/agast.c
C_SRC += $(MY_DIR)imlib/edge.c    
C_SRC += $(MY_DIR)imlib/fmath.c        
C_SRC += $(MY_DIR)imlib/fsort.c  
C_SRC += $(MY_DIR)imlib/imlib.c        
C_SRC += $(MY_DIR)imlib/kmeans.c   
C_SRC += $(MY_DIR)imlib/mjpeg.c             
C_SRC += $(MY_DIR)imlib/qrcode.c            
C_SRC += $(MY_DIR)imlib/stats.c
C_SRC += $(MY_DIR)imlib/apriltag.c  
C_SRC += $(MY_DIR)imlib/clahe.c        
C_SRC += $(MY_DIR)imlib/eye.c          
C_SRC += $(MY_DIR)imlib/lab_tab.c  
C_SRC += $(MY_DIR)imlib/orb.c               
C_SRC += $(MY_DIR)imlib/qsort.c             
C_SRC += $(MY_DIR)imlib/bayer.c    
C_SRC += $(MY_DIR)imlib/collections.c  
C_SRC += $(MY_DIR)imlib/fast.c    
C_SRC += $(MY_DIR)imlib/font.c         
C_SRC += $(MY_DIR)imlib/gif.c    
C_SRC += $(MY_DIR)imlib/integral.c     
C_SRC += $(MY_DIR)imlib/lbp.c      
C_SRC += $(MY_DIR)imlib/phasecorrelation.c  
C_SRC += $(MY_DIR)imlib/rainbow_tab.c       
C_SRC += $(MY_DIR)imlib/template.c
C_SRC += $(MY_DIR)imlib/binary.c    
C_SRC += $(MY_DIR)imlib/fft.c          
C_SRC += $(MY_DIR)imlib/haar.c   
C_SRC += $(MY_DIR)imlib/integral_mw.c  
C_SRC += $(MY_DIR)imlib/line.c     
C_SRC += $(MY_DIR)imlib/point.c             
C_SRC += $(MY_DIR)imlib/rectangle.c         
C_SRC += $(MY_DIR)imlib/xyz_tab.c
C_SRC += $(MY_DIR)imlib/blob.c      
C_SRC += $(MY_DIR)imlib/dmtx.c         
C_SRC += $(MY_DIR)imlib/framebuffer.c  
C_SRC += $(MY_DIR)imlib/hog.c    
C_SRC += $(MY_DIR)imlib/jpeg.c         
C_SRC += $(MY_DIR)imlib/lsd.c      
C_SRC += $(MY_DIR)imlib/pool.c              
C_SRC += $(MY_DIR)imlib/selective_search.c  
C_SRC += $(MY_DIR)imlib/zbar.c
C_SRC += $(MY_DIR)imlib/bmp.c       
C_SRC += $(MY_DIR)imlib/draw.c         
C_SRC += $(MY_DIR)imlib/filter.c  
C_SRC += $(MY_DIR)imlib/hough.c  
C_SRC += $(MY_DIR)imlib/jpegd.c        
C_SRC += $(MY_DIR)imlib/mathop.c   
C_SRC += $(MY_DIR)imlib/ppm.c               
C_SRC += $(MY_DIR)imlib/sincos_tab.c



C_SRC += $(MY_DIR)config/mini.c








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
