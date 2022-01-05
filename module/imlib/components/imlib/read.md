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


src_pixel   = 1111 1111 1111 1111     
dst_pixel   = 1111 1111 1111 1111
smuad_alpha = 1111 1111 1111 1111

mask_r = 0111 1100 0000 0000    0111 1100 0000 0000
mask_g = 0000 0111 1110 0000    0000 0111 1110 0000
mask_b = 0000 0000 0001 1111    0000 0000 0001 1111

rgb = (_src_pixel << 16) | _dst_pixel; 
rgb = src_pixel dst_pixel

~~~ c
case PIXFORMAT_RGB565: {
    uint16_t *src16 = ((uint16_t *) data->row_buffer[!data->toggle]) + x_start;
    if (data->rgb_channel < 0) {
        if (data->smuad_alpha_palette) {
            const uint32_t *smuad_alpha_palette = data->smuad_alpha_palette;
            if (!data->color_palette) {
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = COLOR_RGB565_TO_Y(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel];
                        long smuad_pixel = (pixel << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = COLOR_RGB565_TO_Y(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel];
                        pixel = ((smuad_alpha * pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        int pixel_y = COLOR_RGB565_TO_Y(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel_y];
                        pixel = color_palette[pixel_y];
                        long smuad_pixel = (COLOR_RGB565_TO_Y(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        int pixel_y = COLOR_RGB565_TO_Y(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel_y];
                        pixel = color_palette[pixel_y];
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_Y(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            }
        } else if (data->alpha == 256) {
            if (!data->color_palette) {
                for (int x = x_start; x < x_end; x++) {
                    int pixel = *src16++;
                    pixel = COLOR_RGB565_TO_Y(pixel) > 127;
                    IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                for (int x = x_start; x < x_end; x++) {
                    int pixel = *src16++;
                    pixel = color_palette[COLOR_RGB565_TO_Y(pixel)];
                    pixel = COLOR_RGB565_TO_Y(pixel) > 127;
                    IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                }
            }
        } else {
            long smuad_alpha = data->smuad_alpha;
            if (!data->color_palette) {
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        long smuad_pixel = (COLOR_RGB565_TO_Y(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_Y(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = color_palette[COLOR_RGB565_TO_Y(pixel)];
                        long smuad_pixel = (COLOR_RGB565_TO_Y(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = color_palette[COLOR_RGB565_TO_Y(pixel)];
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_Y(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            }
        }
    } else if (data->rgb_channel == 0) {
        if (data->smuad_alpha_palette) {
            const uint32_t *smuad_alpha_palette = data->smuad_alpha_palette;
            if (!data->color_palette) {
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = COLOR_RGB565_TO_R8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel];
                        long smuad_pixel = (pixel << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = COLOR_RGB565_TO_R8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel];
                        pixel = ((smuad_alpha * pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        int pixel_y = COLOR_RGB565_TO_R8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel_y];
                        pixel = color_palette[pixel_y];
                        long smuad_pixel = (COLOR_RGB565_TO_Y(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        int pixel_y = COLOR_RGB565_TO_R8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel_y];
                        pixel = color_palette[pixel_y];
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_Y(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            }
        } else if (data->alpha == 256) {
            if (!data->color_palette) {
                for (int x = x_start; x < x_end; x++) {
                    int pixel = *src16++;
                    pixel = COLOR_RGB565_TO_R8(pixel) > 127;
                    IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                for (int x = x_start; x < x_end; x++) {
                    int pixel = *src16++;
                    pixel = color_palette[COLOR_RGB565_TO_R8(pixel)];
                    pixel = COLOR_RGB565_TO_Y(pixel) > 127;
                    IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                }
            }
        } else {
            long smuad_alpha = data->smuad_alpha;
            if (!data->color_palette) {
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        long smuad_pixel = (COLOR_RGB565_TO_R8(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_R8(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = color_palette[COLOR_RGB565_TO_R8(pixel)];
                        long smuad_pixel = (COLOR_RGB565_TO_Y(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = color_palette[COLOR_RGB565_TO_R8(pixel)];
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_Y(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            }
        }
    } else if (data->rgb_channel == 1) {
        if (data->smuad_alpha_palette) {
            const uint32_t *smuad_alpha_palette = data->smuad_alpha_palette;
            if (!data->color_palette) {
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = COLOR_RGB565_TO_G8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel];
                        long smuad_pixel = (pixel << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = COLOR_RGB565_TO_G8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel];
                        pixel = ((smuad_alpha * pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        int pixel_y = COLOR_RGB565_TO_G8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel_y];
                        pixel = color_palette[pixel_y];
                        long smuad_pixel = (COLOR_RGB565_TO_Y(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        int pixel_y = COLOR_RGB565_TO_G8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel_y];
                        pixel = color_palette[pixel_y];
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_Y(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            }
        } else if (data->alpha == 256) {
            if (!data->color_palette) {
                for (int x = x_start; x < x_end; x++) {
                    int pixel = *src16++;
                    pixel = COLOR_RGB565_TO_G8(pixel) > 127;
                    IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                for (int x = x_start; x < x_end; x++) {
                    int pixel = *src16++;
                    pixel = color_palette[COLOR_RGB565_TO_G8(pixel)];
                    pixel = COLOR_RGB565_TO_Y(pixel) > 127;
                    IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                }
            }
        } else {
            long smuad_alpha = data->smuad_alpha;
            if (!data->color_palette) {
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        long smuad_pixel = (COLOR_RGB565_TO_G8(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_G8(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = color_palette[COLOR_RGB565_TO_G8(pixel)];
                        long smuad_pixel = (COLOR_RGB565_TO_Y(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = color_palette[COLOR_RGB565_TO_G8(pixel)];
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_Y(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            }
        }
    } else if (data->rgb_channel == 2) {
        if (data->smuad_alpha_palette) {
            const uint32_t *smuad_alpha_palette = data->smuad_alpha_palette;
            if (!data->color_palette) {
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = COLOR_RGB565_TO_B8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel];
                        long smuad_pixel = (pixel << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = COLOR_RGB565_TO_B8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel];
                        pixel = ((smuad_alpha * pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        int pixel_y = COLOR_RGB565_TO_B8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel_y];
                        pixel = color_palette[pixel_y];
                        long smuad_pixel = (COLOR_RGB565_TO_Y(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        int pixel_y = COLOR_RGB565_TO_B8(pixel);
                        long smuad_alpha = smuad_alpha_palette[pixel_y];
                        pixel = color_palette[pixel_y];
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_Y(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            }
        } else if (data->alpha == 256) {
            if (!data->color_palette) {
                for (int x = x_start; x < x_end; x++) {
                    int pixel = *src16++;
                    pixel = COLOR_RGB565_TO_B8(pixel) > 127;
                    IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                for (int x = x_start; x < x_end; x++) {
                    int pixel = *src16++;
                    pixel = color_palette[COLOR_RGB565_TO_B8(pixel)];
                    pixel = COLOR_RGB565_TO_Y(pixel) > 127;
                    IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                }
            }
        } else {
            long smuad_alpha = data->smuad_alpha;
            if (!data->color_palette) {
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        long smuad_pixel = (COLOR_RGB565_TO_B8(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_B8(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            } else {
                const uint16_t *color_palette = data->color_palette;
                if (!data->black_background) {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = color_palette[COLOR_RGB565_TO_B8(pixel)];
                        long smuad_pixel = (COLOR_RGB565_TO_Y(pixel) << 16) | (IMAGE_GET_BINARY_PIXEL_FAST(dst32, x) ? COLOR_GRAYSCALE_BINARY_MAX : COLOR_GRAYSCALE_BINARY_MIN);
                        pixel = (__SMUAD(smuad_alpha, smuad_pixel) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                } else {
                    for (int x = x_start; x < x_end; x++) {
                        int pixel = *src16++;
                        pixel = color_palette[COLOR_RGB565_TO_B8(pixel)];
                        pixel = ((smuad_alpha * COLOR_RGB565_TO_Y(pixel)) >> 8) > 127;
                        IMAGE_PUT_BINARY_PIXEL_FAST(dst32, x, pixel);
                    }
                }
            }
        }
    }
    break;
}
~~~