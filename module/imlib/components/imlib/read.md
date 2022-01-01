将库修改到标准linux c

#更新的文件
draw.c
imlib.c
blob.c
stats.c
zbar.c
line.c
apriltag.c
phasecorrelation.c
fft.c
yuv.c
xyz_tab.c
template.c
sincos_tab.c
selective_search.c


src_pixel   = 1111 1111 1111 1111     
dst_pixel   = 1111 1111 1111 1111
smuad_alpha = 1111 1111 1111 1111

mask_r = 0111 1100 0000 0000    0111 1100 0000 0000
mask_g = 0000 0111 1110 0000    0000 0111 1110 0000
mask_b = 0000 0000 0001 1111    0000 0000 0001 1111

rgb = (_src_pixel << 16) | _dst_pixel; 
rgb = src_pixel dst_pixel


rb