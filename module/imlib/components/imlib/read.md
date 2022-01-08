将库修改到标准linux c
大端是人的思维逻辑，小端是机器的逻辑

大端，高位在前
小端低位在前



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

  uint8_t *op1_s = (uint8_t *) &op1, *op2_s = (uint8_t *) &op2;
  result = abs(op1_s[0] - op2_s[0]) + abs(op1_s[1] - op2_s[1]) + (op1_s[2] - op2_s[2]) + (op1_s[3] - op2_s[3]) + op3;

src_pixel   = 1111 1111 1111 1111     
dst_pixel   = 1111 1111 1111 1111
smuad_alpha = 1111 1111 1111 1111

mask_r = 0111 1100 0000 0000    0111 1100 0000 0000
mask_g = 0000 0111 1110 0000    0000 0111 1110 0000
mask_b = 0000 0000 0001 1111    0000 0000 0001 1111

rgb = (_src_pixel << 16) | _dst_pixel; 
rgb = src_pixel dst_pixel

long smuad_alpha = smuad_alpha_palette[COLOR_RGB888_TO_Y(src_pixel)];
const uint32_t *smuad_alpha_palette = data->smuad_alpha_palette;
imlib_draw_row_data_t *data


并行数据

long smlad_x_weight = (l_x_weight << 16) | r_x_weight;

long t_smlad_x_weight = smlad_x_weight * t_y_weight;
long b_smlad_x_weight = smlad_x_weight * b_y_weight;
long t_b_smlad_x_weight_sum = __QADD16(t_smlad_x_weight, b_smlad_x_weight);