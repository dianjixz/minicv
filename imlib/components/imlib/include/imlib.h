/* This file is part of the OpenMV project.
 * Copyright (c) 2013-2017 Ibrahim Abdelkader <iabdalkader@openmv.io> & Kwabena W. Agyeman <kwagyeman@openmv.io>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 */
/*
dianjixz change start 2021,12,20
one fun:
// Drawing Functions
*        int imlib_get_pixel(image_t *img, int x, int y);
*        void imlib_set_pixel(image_t *img, int x, int y, int p);
*        void imlib_draw_line(image_t *img, int x0, int y0, int x1, int y1, int c, int thickness);
*        void imlib_draw_rectangle(image_t *img, int rx, int ry, int rw, int rh, int c, int thickness, bool fill);
*        void imlib_draw_circle(image_t *img, int cx, int cy, int r, int c, int thickness, bool fill);
*        void imlib_draw_ellipse(image_t *img, int cx, int cy, int rx, int ry, int rotation, int c, int thickness, bool fill);
*        void imlib_draw_font(image_t *img, int x_off, int y_off, int c, float scale, uint8_t font_h, uint8_t font_w, const uint8_t *font);
*        // void imlib_draw_string(image_t *img, int x_off, int y_off, mp_obj_t str, int c, float scale, int x_spacing, int y_spacing, bool mono_space);
*        void imlib_draw_image(image_t *img, image_t *other, int x_off, int y_off, float x_scale, float y_scale, float alpha, image_t *mask);
*        size_t imlib_flood_fill(image_t *img, int x, int y,
*        float seed_threshold, float floating_threshold,
*        int c, bool invert, bool clear_background, image_t *mask);


*/

#ifndef __IMLIB_H__
#define __IMLIB_H__

#include <stdbool.h>
#include "imlib_config.h"

#include "imlib.h"
// //#include <bits/mathcalls.h>
// // #include <ff.h>
// #include "fb_alloc.h"
// #include "umm_malloc.h"
// #include "xalloc.h"
// #include "array.h"
// #include "fmath.h"
// #include "collections.h"

// // #include "py/obj.h"
#include "imdefs.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define rgb24_Color(_r8, _g8, _b8) \
({                                    \
    ((_r8 << 16) | (_g8 << 8) | _b8); \
})

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define PIXFORMAT_STRUCT            \
struct {                            \
  union {                           \
    struct {                        \
        uint32_t bpp            :8; \
        uint32_t subfmt_id      :8; \
        uint32_t pixfmt_id      :8; \
        uint32_t is_bayer       :1; \
        uint32_t is_compressed  :1; \
        uint32_t is_color       :1; \
        uint32_t is_mutable     :1; \
        uint32_t is_yuv         :1; \
        uint32_t /*reserved*/   :3; \
    };                              \
    uint32_t pixfmt;                \
  };                                \
  uint32_t size; /* for compressed images */ \
}
#elif defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define PIXFORMAT_STRUCT            \
struct {                            \
  union {                           \
    struct {                        \
        uint32_t /*reserved*/   :3; \
        uint32_t is_yuv         :1; \
        uint32_t is_mutable     :1; \
        uint32_t is_color       :1; \
        uint32_t is_compressed  :1; \
        uint32_t is_bayer       :1; \
        uint32_t pixfmt_id      :8; \
        uint32_t subfmt_id      :8; \
        uint32_t bpp            :8; \
    };                              \
    uint32_t pixfmt;                \
  };                                \
  uint32_t size; /* for compressed images */ \
}
#else
#error "Byte order is not defined."
#endif



typedef struct image {
    uint32_t w;
    uint32_t h;
    PIXFORMAT_STRUCT;
    union {
        uint8_t *pixels;
        uint8_t *data;
    };
} image_t;

typedef struct pixel_s {
    char red;
    char green;
    char blue;
} pixel24_t;

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#else
//cpu is little
//input pixel24_t，output uint32_t
#define pixel24232(_u24_t) \
({\
    __typeof__ (_u24_t) ___u24_t = _u24_t;\
    ((*((uint32_t*)((void*)&___u24_t))) & 0x00ffffff);\
})
//input_ uint32_t，output pixel24_t
#define pixel32224(_u32_t) \
({\
    __typeof__ (_u32_t) __u32_t = _u32_t;\
    __u32_t = __builtin_bswap32(__u32_t) >> 8;\
    (*((pixel24_t*)((void*)&__u32_t)));\
})

#endif //__BYTE_ORDER__














// /////////////////
// // Point Stuff //
// /////////////////

// typedef struct point {
//     int16_t x;
//     int16_t y;
// } __attribute__((aligned(8))) point_t;

// void point_init(point_t *ptr, int x, int y);
// void point_copy(point_t *dst, point_t *src);
// bool point_equal_fast(point_t *ptr0, point_t *ptr1);
// int point_quadrance(point_t *ptr0, point_t *ptr1);

// ////////////////
// // Line Stuff //
// ////////////////

// typedef struct line {
//     int16_t x1;
//     int16_t y1;
//     int16_t x2;
//     int16_t y2;
// } __attribute__((aligned(8))) line_t;

// bool lb_clip_line(line_t *l, int x, int y, int w, int h);

// /////////////////////
// // Rectangle Stuff //
// /////////////////////

// typedef struct rectangle {
//     int16_t x;
//     int16_t y;
//     int16_t w;
//     int16_t h;
// } __attribute__((aligned(8))) rectangle_t;

// void rectangle_init(rectangle_t *ptr, int x, int y, int w, int h);
// void rectangle_copy(rectangle_t *dst, rectangle_t *src);
// bool rectangle_equal_fast(rectangle_t *ptr0, rectangle_t *ptr1);
// bool rectangle_overlap(rectangle_t *ptr0, rectangle_t *ptr1);
// void rectangle_intersected(rectangle_t *dst, rectangle_t *src);
// void rectangle_united(rectangle_t *dst, rectangle_t *src);

// /////////////////
// // Color Stuff //
// /////////////////

// typedef struct color_thresholds_list_lnk_data
// {
//     uint8_t LMin, LMax; // or grayscale
//     int8_t AMin, AMax;
//     int8_t BMin, BMax;
// }__attribute__((aligned(8)))
// color_thresholds_list_lnk_data_t;






/////////////////
// Image Stuff //
/////////////////

typedef enum image_bpp
{
    IMAGE_BPP_BINARY,       // BPP = 0
    IMAGE_BPP_GRAYSCALE,    // BPP = 1
    IMAGE_BPP_RGB565,       // BPP = 2
    IMAGE_BPP_BAYER,        // BPP = 3
    IMAGE_BPP_JPEG,         // BPP = 4
    IMAGE_BPP_RGB888,       // BPP = 5
    IMAGE_BPP_RGBA8888,     // BPP = 6
}
image_bpp_t;


// void image_init(image_t *ptr, int w, int h, int bpp, void *data);
// void image_copy(image_t *dst, image_t *src);
// size_t image_size(image_t *ptr);
bool image_get_mask_pixel(image_t *ptr, int x, int y);


// typedef struct simple_color {
//     uint8_t G;          // Gray
//     union {
//         int8_t L;       // LAB L
//         uint8_t red;    // RGB888 Red
//     };
//     union {
//         int8_t A;       // LAB A
//         uint8_t green;  // RGB888 Green
//     };
//     union {
//         int8_t B;       // LAB B
//         uint8_t blue;   // RGB888 Blue
//     };
// } __attribute__((aligned(8))) simple_color_t;

// typedef struct integral_image {
//     int w;
//     int h;
//     uint32_t *data;
// } __attribute__((aligned(8))) i_image_t;

// typedef struct {
//     int w;
//     int h;
//     int y_offs;
//     int x_ratio;
//     int y_ratio;
//     uint32_t **data;
//     uint32_t **swap;
// } __attribute__((aligned(8))) mw_image_t;

// typedef struct _vector {
//     float x;
//     float y;
//     float m;
//     uint16_t cx,cy;
// } __attribute__((aligned(8))) vec_t;

// typedef struct cluster {
//     int x, y, w, h;
//     array_t *points;
// } __attribute__((aligned(8))) cluster_t;

// // Return the distance between a cluster centroid and some object.
// typedef float (*cluster_dist_t)(int cx, int cy, void *obj);

// /* Keypoint */
// typedef struct kp {
//     uint16_t x;
//     uint16_t y;
//     uint16_t score;
//     uint16_t octave;
//     uint16_t angle;
//     uint16_t matched;
//     uint8_t desc[32];
// } __attribute__((aligned(8))) kp_t;

// typedef struct size {
//     int w;
//     int h;
// } __attribute__((aligned(8))) wsize_t;

// /* Haar cascade struct */
// typedef struct cascade {
//     int std;                        // Image standard deviation.
//     int step;                       // Image scanning factor.
//     float threshold;                // Detection threshold.
//     float scale_factor;             // Image scaling factor.
//     int n_stages;                   // Number of stages in the cascade.
//     int n_features;                 // Number of features in the cascade.
//     int n_rectangles;               // Number of rectangles in the cascade.
//     struct size window;             // Detection window size.
//     struct image *img;              // Grayscale image.
//     mw_image_t *sum;                // Integral image.
//     mw_image_t *ssq;                // Squared integral image.
//     uint8_t *stages_array;          // Number of features per stage.
//     int16_t *stages_thresh_array;   // Stages thresholds.
//     int16_t *tree_thresh_array;     // Features threshold (1 per feature).
//     int16_t *alpha1_array;          // Alpha1 array (1 per feature).
//     int16_t *alpha2_array;          // Alpha2 array (1 per feature).
//     int8_t *num_rectangles_array;   // Number of rectangles per features (1 per feature).
//     int8_t *weights_array;          // Rectangles weights (1 per rectangle).
//     int8_t *rectangles_array;       // Rectangles array.
// } __attribute__((aligned(8))) cascade_t;

// typedef struct bmp_read_settings {
//     int32_t bmp_w;
//     int32_t bmp_h;
//     uint16_t bmp_bpp;
//     uint32_t bmp_fmt;
//     uint32_t bmp_row_bytes;
// } __attribute__((aligned(8))) bmp_read_settings_t;

// typedef struct ppm_read_settings {
//     uint8_t read_int_c;
//     bool read_int_c_valid;
//     uint8_t ppm_fmt;
// } __attribute__((aligned(8))) ppm_read_settings_t;

// typedef enum save_image_format {
//     FORMAT_DONT_CARE,
//     FORMAT_BMP,
//     FORMAT_PNM,
//     FORMAT_JPG,
//     FORMAT_RAW,
// } __attribute__((aligned(8))) save_image_format_t;

// typedef struct img_read_settings {
//     union
//     {
//         bmp_read_settings_t bmp_rs;
//         ppm_read_settings_t ppm_rs;
//     };
//     save_image_format_t format;
// } __attribute__((aligned(8))) img_read_settings_t;

// typedef void (*line_op_t)(image_t*, int, void*, void*, bool);
// typedef void (*flood_fill_call_back_t)(image_t *, int, int, int, void *);

// typedef enum descriptor_type {
//     DESC_LBP,
//     DESC_ORB,
// } __attribute__((aligned(8))) descriptor_t;

// typedef enum edge_detector_type {
//    EDGE_CANNY,
//    EDGE_SIMPLE,
// } __attribute__((aligned(8))) edge_detector_t;

// typedef enum template_match {
//     SEARCH_EX,  // Exhaustive search
//     SEARCH_DS,  // Diamond search
// } __attribute__((aligned(8))) template_match_t;

// typedef enum  jpeg_subsample {
//     JPEG_SUBSAMPLE_1x1 = 0x11,  // 1x1 chroma subsampling (No subsampling)
//     JPEG_SUBSAMPLE_2x1 = 0x21,  // 2x2 chroma subsampling
//     JPEG_SUBSAMPLE_2x2 = 0x22,  // 2x2 chroma subsampling
// } __attribute__((aligned(8))) jpeg_subsample_t;

// typedef enum corner_detector_type {
//     CORNER_FAST,
//     CORNER_AGAST
// } corner_detector_t;

// typedef struct histogram {
//     int LBinCount;
//     float *LBins;
//     int ABinCount;
//     float *ABins;
//     int BBinCount;
//     float *BBins;
// } __attribute__((aligned(8))) histogram_t;

// typedef struct percentile {
//     uint8_t LValue;
//     int8_t AValue;
//     int8_t BValue;
// } percentile_t;

// typedef struct threshold {
//     uint8_t LValue;
//     int8_t AValue;
//     int8_t BValue;
// } __attribute__((aligned(8))) threshold_t;

// typedef struct statistics {
//     uint8_t LMean, LMedian, LMode, LSTDev, LMin, LMax, LLQ, LUQ;
//     int8_t AMean, AMedian, AMode, ASTDev, AMin, AMax, ALQ, AUQ;
//     int8_t BMean, BMedian, BMode, BSTDev, BMin, BMax, BLQ, BUQ;
// } __attribute__((aligned(8))) statistics_t;

// typedef struct find_blobs_list_lnk_data {
//     rectangle_t rect;
//     uint32_t pixels;
//     point_t centroid;
//     float rotation;
//     uint16_t code, count;
// } __attribute__((aligned(8))) find_blobs_list_lnk_data_t;

// typedef struct find_lines_list_lnk_data {
//     line_t line;
//     uint32_t magnitude;
//     int16_t theta, rho;
// } __attribute__((aligned(8)))find_lines_list_lnk_data_t;

// typedef struct find_circles_list_lnk_data {
//     point_t p;
//     uint32_t r, magnitude;
// } __attribute__((aligned(8))) find_circles_list_lnk_data_t;

// typedef struct find_rects_list_lnk_data {
//     point_t corners[4];
//     rectangle_t rect;
//     uint32_t magnitude;
// } __attribute__((aligned(8))) find_rects_list_lnk_data_t;

// typedef struct find_qrcodes_list_lnk_data {
//     point_t corners[4];
//     rectangle_t rect;
//     size_t payload_len;
//     char *payload;
//     uint8_t version, ecc_level, mask, data_type;
//     uint32_t eci;
// } __attribute__((aligned(8))) find_qrcodes_list_lnk_data_t;

// typedef enum apriltag_families {
//     TAG16H5 = 1,
//     TAG25H7 = 2,
//     TAG25H9 = 4,
//     TAG36H10 = 8,
//     TAG36H11 = 16,
//     ARTOOLKIT = 32
// } __attribute__((aligned(8))) apriltag_families_t;

// typedef struct find_apriltags_list_lnk_data {
//     point_t corners[4];
//     rectangle_t rect;
//     uint16_t id;
//     uint8_t family, hamming;
//     point_t centroid;
//     float goodness, decision_margin;
//     float x_translation, y_translation, z_translation;
//     float x_rotation, y_rotation, z_rotation;
// } __attribute__((aligned(8))) find_apriltags_list_lnk_data_t;

// typedef struct find_datamatrices_list_lnk_data {
//     point_t corners[4];
//     rectangle_t rect;
//     size_t payload_len;
//     char *payload;
//     uint16_t rotation;
//     uint8_t rows, columns;
//     uint16_t capacity, padding;
// } __attribute__((aligned(8))) find_datamatrices_list_lnk_data_t;

// typedef enum barcodes {
//     BARCODE_EAN2,
//     BARCODE_EAN5,
//     BARCODE_EAN8,
//     BARCODE_UPCE,
//     BARCODE_ISBN10,
//     BARCODE_UPCA,
//     BARCODE_EAN13,
//     BARCODE_ISBN13,
//     BARCODE_I25,
//     BARCODE_DATABAR,
//     BARCODE_DATABAR_EXP,
//     BARCODE_CODABAR,
//     BARCODE_CODE39,
//     BARCODE_PDF417,
//     BARCODE_CODE93,
//     BARCODE_CODE128
// } barcodes_t;

// typedef struct find_barcodes_list_lnk_data {
//     point_t corners[4];
//     rectangle_t rect;
//     size_t payload_len;
//     char *payload;
//     uint16_t type, rotation;
//     int quality;
// } __attribute__((aligned(8))) find_barcodes_list_lnk_data_t;

// /* Color space functions */
// void imlib_rgb_to_lab(simple_color_t *rgb, simple_color_t *lab);
// void imlib_lab_to_rgb(simple_color_t *lab, simple_color_t *rgb);
// void imlib_rgb_to_grayscale(simple_color_t *rgb, simple_color_t *grayscale);
// void imlib_grayscale_to_rgb(simple_color_t *grayscale, simple_color_t *rgb);
// uint16_t imlib_yuv_to_rgb(uint8_t y, int8_t u, int8_t v);
// void imlib_bayer_to_rgb565(image_t *img, int w, int h, int xoffs, int yoffs, uint16_t *rgbbuf);
// void imlib_bayer_to_rgb888(image_t *img, int w, int h, int xoffs, int yoffs, uint32_t *rgbbuf);

// // /* Image file functions */
// // void ppm_read_geometry(mp_obj_t fp, image_t *img, const char *path, ppm_read_settings_t *rs);
// // void ppm_read_pixels(mp_obj_t fp, image_t *img, int line_start, int line_end, ppm_read_settings_t *rs);
// // void ppm_read(image_t *img, const char *path);
// // void ppm_write_subimg(image_t *img, const char *path, rectangle_t *r);
// // bool bmp_read_geometry(mp_obj_t fp, image_t *img, bmp_read_settings_t *rs);
// // bool bmp_read_pixels(mp_obj_t fp, image_t *img, int line_start, int line_end, bmp_read_settings_t *rs);
// // void bmp_read(image_t *img, const char *path);
// // void bmp_write_subimg(image_t *img, const char *path, rectangle_t *r);
// // bool jpeg_compress(image_t *src, image_t *dst, int quality, bool realloc);
// // void jpeg_read_geometry(mp_obj_t fp, image_t *img);
// // bool jpeg_read_pixels(mp_obj_t fp, image_t *img);
// // void jpeg_read(image_t *img, const char *path);
// // void jpeg_write(image_t *img, const char *path, int quality);
// // bool imlib_read_geometry(mp_obj_t fp, image_t *img, const char *path, img_read_settings_t *rs);
// // void imlib_image_operation(image_t *img, const char *path, image_t *other, int scalar, line_op_t op, void *data);
// // void imlib_load_image(image_t *img, const char *path, mp_obj_t file, uint8_t* buff, uint32_t buff_len);
// // void imlib_save_image(image_t *img, const char *path, rectangle_t *roi, int quality);

// // /* GIF functions */
// // void gif_open(mp_obj_t fp, int width, int height, bool color, bool loop);
// // void gif_add_frame(mp_obj_t fp, image_t *img, uint16_t delay);
// // void gif_close(mp_obj_t fp);

// // /* MJPEG functions */
// // void mjpeg_open(mp_obj_t fp, int width, int height);
// // void mjpeg_add_frame(mp_obj_t fp, uint32_t *frames, uint32_t *bytes, image_t *img, int quality);
// // void mjpeg_close(mp_obj_t fp, uint32_t *frames, uint32_t *bytes, float fps);

// /* Point functions */
// point_t *point_alloc(int16_t x, int16_t y);
// bool point_equal(point_t *p1, point_t *p2);
// float point_distance(point_t *p1, point_t *p2);

// /* Rectangle functions */
// rectangle_t *rectangle_alloc(int16_t x, int16_t y, int16_t w, int16_t h);
// bool rectangle_equal(rectangle_t *r1, rectangle_t *r2);
// bool rectangle_intersects(rectangle_t *r1, rectangle_t *r2);
// bool rectangle_subimg(image_t *img, rectangle_t *r, rectangle_t *r_out);
// array_t *rectangle_merge(array_t *rectangles);
// void rectangle_expand(rectangle_t *r, int x, int y);

// /* Separable 2D convolution */
// void imlib_sepconv3(image_t *img, const int8_t *krn, const float m, const int b);

// /* Image Statistics */
// int imlib_image_mean(image_t *src, int *r_mean, int *g_mean, int *b_mean);
// int imlib_image_std(image_t *src); // grayscale only

// /* Template Matching */
// void imlib_midpoint_pool(image_t *img_i, image_t *img_o, int x_div, int y_div, const int bias);
// void imlib_mean_pool(image_t *img_i, image_t *img_o, int x_div, int y_div);
// // float imlib_template_match_ds(image_t *image, image_t *template, rectangle_t *r);
// // float imlib_template_match_ex(image_t *image, image_t *template, rectangle_t *roi, int step, rectangle_t *r);

// /* Clustering functions */
// array_t *cluster_kmeans(array_t *points, int k, cluster_dist_t dist_func);

// /* Integral image functions */
// void imlib_integral_image_alloc(struct integral_image *sum, int w, int h);
// void imlib_integral_image_free(struct integral_image *sum);
// void imlib_integral_image(struct image *src, struct integral_image *sum);
// void imlib_integral_image_sq(struct image *src, struct integral_image *sum);
// void imlib_integral_image_scaled(struct image *src, struct integral_image *sum);
// uint32_t imlib_integral_lookup(struct integral_image *src, int x, int y, int w, int h);

// // Integral moving window
// void imlib_integral_mw_alloc(mw_image_t *sum, int w, int h);
// void imlib_integral_mw_free(mw_image_t *sum);
// void imlib_integral_mw_scale(rectangle_t *roi, mw_image_t *sum, int w, int h);
// void imlib_integral_mw(image_t *src, mw_image_t *sum);
// void imlib_integral_mw_sq(image_t *src, mw_image_t *sum);
// void imlib_integral_mw_shift(image_t *src, mw_image_t *sum, int n);
// void imlib_integral_mw_shift_sq(image_t *src, mw_image_t *sum, int n);
// void imlib_integral_mw_ss(image_t *src, mw_image_t *sum, mw_image_t *ssq, rectangle_t *roi);
// void imlib_integral_mw_shift_ss(image_t *src, mw_image_t *sum, mw_image_t *ssq, rectangle_t *roi, int n);
// long imlib_integral_mw_lookup(mw_image_t *sum, int x, int y, int w, int h);

// /* Haar/VJ */
// int imlib_load_cascade(struct cascade* cascade, const char *path);
// array_t *imlib_detect_objects(struct image *image, struct cascade *cascade, struct rectangle *roi);

// /* Corner detectors */
// void fast_detect(image_t *image, array_t *keypoints, int threshold, rectangle_t *roi);
// void agast_detect(image_t *image, array_t *keypoints, int threshold, rectangle_t *roi);

// /* ORB descriptor */
// array_t *orb_find_keypoints(image_t *image, bool normalized, int threshold,
//         float scale_factor, int max_keypoints, corner_detector_t corner_detector, rectangle_t *roi);
// int orb_match_keypoints(array_t *kpts1, array_t *kpts2, int *match, int threshold, rectangle_t *r, point_t *c, int *angle);
// int orb_filter_keypoints(array_t *kpts, rectangle_t *r, point_t *c);
// // int orb_save_descriptor(mp_obj_t fp, array_t *kpts);
// // int orb_load_descriptor(mp_obj_t fp, array_t *kpts);
// float orb_cluster_dist(int cx, int cy, void *kp);

// /* LBP Operator */
// uint8_t *imlib_lbp_desc(image_t *image, rectangle_t *roi);
// int imlib_lbp_desc_distance(uint8_t *d0, uint8_t *d1);
// // int imlib_lbp_desc_save(mp_obj_t fp, uint8_t *desc);
// // int imlib_lbp_desc_load(mp_obj_t fp, uint8_t **desc);

// /* Iris detector */
// void imlib_find_iris(image_t *src, point_t *iris, rectangle_t *roi);

// // Image filter functions
// void im_filter_bw(uint8_t *src, uint8_t *dst, int size, int bpp, void *args);
// void im_filter_skin(uint8_t *src, uint8_t *dst, int size, int bpp, void *args);

// // Edge detection
// void imlib_edge_simple(image_t *src, rectangle_t *roi, int low_thresh, int high_thresh);
// void imlib_edge_canny(image_t *src, rectangle_t *roi, int low_thresh, int high_thresh);

// // HoG
// void imlib_find_hog(image_t *src, rectangle_t *roi, int cell_size);

// Helper Functions
// size_t imlib_flood_fill_int(image_t *out, image_t *img, int x, int y,
//                           int seed_threshold, int floating_threshold,
//                           flood_fill_call_back_t cb, void *data);
// Drawing Functions
int imlib_get_pixel(image_t *img, int x, int y);
void imlib_set_pixel(image_t *img, int x, int y, int p);
void imlib_draw_line(image_t *img, int x0, int y0, int x1, int y1, int c, int thickness);
void imlib_draw_rectangle(image_t *img, int rx, int ry, int rw, int rh, int c, int thickness, bool fill);
void imlib_draw_circle(image_t *img, int cx, int cy, int r, int c, int thickness, bool fill);
void imlib_draw_ellipse(image_t *img, int cx, int cy, int rx, int ry, int rotation, int c, int thickness, bool fill);
void imlib_draw_font(image_t *img, int x_off, int y_off, int c, float scale, uint8_t font_h, uint8_t font_w, const uint8_t *font);
void imlib_draw_string(image_t *img, int x_off, int y_off, const char *arg_str, int c, float scale, int x_spacing, int y_spacing, bool mono_space);
// void imlib_draw_image(image_t *img, image_t *other, int x_off, int y_off, float x_scale, float y_scale, float alpha, image_t *mask);
// size_t imlib_flood_fill(image_t *img, int x, int y,
//                       float seed_threshold, float floating_threshold,
//                       int c, bool invert, bool clear_background, image_t *mask);
// // Binary Functions
// void imlib_binary(image_t *out, image_t *img, list_t *thresholds, bool invert, bool zero, image_t *mask);
// void imlib_invert(image_t *img);
// void imlib_b_and(image_t *img, const char *path, image_t *other, int scalar, image_t *mask);
// void imlib_b_nand(image_t *img, const char *path, image_t *other, int scalar, image_t *mask);
// void imlib_b_or(image_t *img, const char *path, image_t *other, int scalar, image_t *mask);
// void imlib_b_nor(image_t *img, const char *path, image_t *other, int scalar, image_t *mask);
// void imlib_b_xor(image_t *img, const char *path, image_t *other, int scalar, image_t *mask);
// void imlib_b_xnor(image_t *img, const char *path, image_t *other, int scalar, image_t *mask);
// void imlib_erode(image_t *img, int ksize, int threshold, image_t *mask);
// void imlib_dilate(image_t *img, int ksize, int threshold, image_t *mask);
// void imlib_open(image_t *img, int ksize, int threshold, image_t *mask);
// void imlib_close(image_t *img, int ksize, int threshold, image_t *mask);
// void imlib_top_hat(image_t *img, int ksize, int threshold, image_t *mask);
// void imlib_black_hat(image_t *img, int ksize, int threshold, image_t *mask);
// // Math Functions
// void imlib_gamma_corr(image_t *img, float gamma, float scale, float offset);
// void imlib_negate(image_t *img);
// void imlib_replace(image_t *img, const char *path, image_t *other, int scalar, bool hmirror, bool vflip, bool transpose, image_t *mask);
// void imlib_add(image_t *img, const char *path, image_t *other, int scalar, image_t *mask);
// void imlib_sub(image_t *img, const char *path, image_t *other, int scalar, bool reverse, image_t *mask);
// void imlib_mul(image_t *img, const char *path, image_t *other, int scalar, bool invert, image_t *mask);
// void imlib_div(image_t *img, const char *path, image_t *other, int scalar, bool invert, bool mod, image_t *mask);
// void imlib_min(image_t *img, const char *path, image_t *other, int scalar, image_t *mask);
// void imlib_max(image_t *img, const char *path, image_t *other, int scalar, image_t *mask);
// void imlib_difference(image_t *img, const char *path, image_t *other, int scalar, image_t *mask);
// void imlib_blend(image_t *img, const char *path, image_t *other, int scalar, float alpha, image_t *mask);
// // Filtering Functions
// void imlib_histeq(image_t *img, image_t *mask);
// void imlib_clahe_histeq(image_t *img, float clip_limit, image_t *mask);
// void imlib_mean_filter(image_t *img, const int ksize, bool threshold, int offset, bool invert, image_t *mask);
// void imlib_median_filter(image_t *img, const int ksize, float percentile, bool threshold, int offset, bool invert, image_t *mask);
// void imlib_mode_filter(image_t *img, const int ksize, bool threshold, int offset, bool invert, image_t *mask);
// void imlib_midpoint_filter(image_t *img, const int ksize, float bias, bool threshold, int offset, bool invert, image_t *mask);
// void imlib_morph(image_t *img, const int ksize, const int *krn, const float m, const int b, bool threshold, int offset, bool invert, image_t *mask);
// void imlib_bilateral_filter(image_t *img, const int ksize, float color_sigma, float space_sigma, bool threshold, int offset, bool invert, image_t *mask);
// void imlib_cartoon_filter(image_t *img, float seed_threshold, float floating_threshold, image_t *mask);
// // Image Correction
// void imlib_logpolar_int(image_t *dst, image_t *src, rectangle_t *roi, bool linear, bool reverse); // helper/internal
// void imlib_logpolar(image_t *img, bool linear, bool reverse);
// void imlib_remove_shadows(image_t *img, const char *path, image_t *other, int scalar, bool single);
// void imlib_chrominvar(image_t *img);
// void imlib_illuminvar(image_t *img);
// // Lens/Rotation Correction
// void imlib_lens_corr(image_t *img, float strength, float zoom);
// void imlib_rotation_corr(image_t *img, float x_rotation, float y_rotation,
//                          float z_rotation, float x_translation, float y_translation,
//                          float zoom, float fov, float *corners);

// // Statistics
// void imlib_get_similarity(image_t *img, const char *path, image_t *other, int scalar, float *avg, float *std, float *min, float *max);
// void imlib_get_histogram(histogram_t *out, image_t *ptr, rectangle_t *roi, list_t *thresholds, bool invert);
// void imlib_get_percentile(percentile_t *out, image_bpp_t bpp, histogram_t *ptr, float percentile);
// void imlib_get_threshold(threshold_t *out, image_bpp_t bpp, histogram_t *ptr);
// void imlib_get_statistics(statistics_t *out, image_bpp_t bpp, histogram_t *ptr);
// bool imlib_get_regression(find_lines_list_lnk_data_t *out, image_t *ptr, rectangle_t *roi, unsigned int x_stride, unsigned int y_stride,
//                           list_t *thresholds, bool invert, unsigned int area_threshold, unsigned int pixels_threshold, bool robust);
// // Color Tracking
// //原定义
// // void imlib_find_blobs(list_t *out, image_t *ptr, rectangle_t *roi, unsigned int x_stride, unsigned int y_stride,
// //                       list_t *thresholds, bool invert, unsigned int area_threshold, unsigned int pixels_threshold,
// //                       bool merge, int margin,
// //                       bool (*threshold_cb)(void*,find_blobs_list_lnk_data_t*), void *threshold_cb_arg,
// //                       bool (*merge_cb)(void*,find_blobs_list_lnk_data_t*,find_blobs_list_lnk_data_t*), void *merge_cb_arg);
// //现定义
// void imlib_find_blobs(list_t *out, image_t *ptr, rectangle_t *roi, unsigned int x_stride, unsigned int y_stride,
//                      list_t *thresholds, bool invert, unsigned int area_threshold, unsigned int pixels_threshold,
//                      bool merge, int margin);



// // Shape Detection
// size_t trace_line(image_t *ptr, line_t *l, int *theta_buffer, uint32_t *mag_buffer, point_t *point_buffer); // helper/internal
// void merge_alot(list_t *out, int threshold, int theta_threshold); // helper/internal
// void imlib_find_lines(list_t *out, image_t *ptr, rectangle_t *roi, unsigned int x_stride, unsigned int y_stride,
//                       uint32_t threshold, unsigned int theta_margin, unsigned int rho_margin);
// void imlib_lsd_find_line_segments(list_t *out, image_t *ptr, rectangle_t *roi, unsigned int merge_distance, unsigned int max_theta_diff);
// void imlib_find_line_segments(list_t *out, image_t *ptr, rectangle_t *roi, unsigned int x_stride, unsigned int y_stride,
//                               uint32_t threshold, unsigned int theta_margin, unsigned int rho_margin,
//                               uint32_t segment_threshold);
// void imlib_find_circles(list_t *out, image_t *ptr, rectangle_t *roi, unsigned int x_stride, unsigned int y_stride,
//                         uint32_t threshold, unsigned int x_margin, unsigned int y_margin, unsigned int r_margin,
//                         unsigned int r_min, unsigned int r_max, unsigned int r_step);
// void imlib_find_rects(list_t *out, image_t *ptr, rectangle_t *roi,
//                       uint32_t threshold);
// // 1/2D Bar Codes
// void imlib_find_qrcodes(list_t *out, image_t *ptr, rectangle_t *roi);
// void imlib_find_apriltags(list_t *out, image_t *ptr, rectangle_t *roi, apriltag_families_t families,
//                           float fx, float fy, float cx, float cy);
// void imlib_find_datamatrices(list_t *out, image_t *ptr, rectangle_t *roi, int effort);
// void imlib_find_barcodes(list_t *out, image_t *ptr, rectangle_t *roi);
// // Template Matching
// void imlib_phasecorrelate(image_t *img0, image_t *img1, rectangle_t *roi0, rectangle_t *roi1, bool logpolar, bool fix_rotation_scale,
//                           float *x_translation, float *y_translation, float *rotation, float *scale, float *response);

// array_t *imlib_selective_search(image_t *src, float t, int min_size, float a1, float a2, float a3);

// // MAIX conv acc
// // void imlib_conv3(image_t *img, float *krn);


// void pix_fill_8yuv(uint16_t* pixels, uint32_t ofs, int8_t* y, int8_t* u, int8_t* v);
// void pix_fill_8uv2(uint16_t* pixels, uint32_t ofs, int8_t* u, int8_t* v);
// void pix_fill_8y(uint16_t* pixels, uint32_t ofs, int8_t* y);
// void pix_fill_yuv(uint32_t idx, int8_t* y, int8_t* u, int8_t* v);


// void imlib_affine_getTansform(uint16_t *src, uint16_t *dst, uint16_t cnt, float* TT);
// int imlib_affine_ai(image_t* src_img, image_t* dst_img, float* TT);
// int imlib_affine(image_t* src_img, image_t* dst_img, float* TT);
// void imlib_zero(image_t *img, image_t *mask, bool invert);
// #define __BYTE_ORDER__ __ORDER_BIG_ENDIAN__

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__


#else
//cpu is little


#endif




#ifdef __cplusplus
}
#endif

#endif //__IMLIB_H__
