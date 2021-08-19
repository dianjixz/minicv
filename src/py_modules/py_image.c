#define PY_SSIZE_T_CLEAN
#include <Python.h>
// #include "collections.h"
// #include "imdefs.h"
#include "imlib.h"
#include "tanstation.h"
#define debug_line printf("[%s %s] %s:%d: %s\n", __DATE__, __TIME__, __FILE__, __LINE__, __func__)

// #define debug_line

image_t self_image_img;

static PyObject *
py_image_img_data_load(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *o_data = Py_None;
    PyObject *o_w = Py_None;
    PyObject *o_h = Py_None;
    PyObject *o_bpp = Py_None;
    uint8_t *r24_pixel;
    uint16_t *r16_pixel;
    static char *kwlist[] = {"img_data", "w", "h", "bpp", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|OOO", kwlist,
                                     &o_data, &o_w, &o_h, &o_bpp))
        return NULL;
    if (self_image_img.data != NULL)
    {
        free(self_image_img.data);
        self_image_img.data = NULL;
    }
    return Py_BuildValue("i", r24to_imgr16(o_data, o_w, o_h, o_bpp, &self_image_img));
}

static PyObject *
py_image_img_data_free(PyObject *self, PyObject *args)
{
    free(self_image_img.data);
    self_image_img.data = NULL;
}

static PyObject *
py_img_torgb24(PyObject *self, PyObject *args)
{
    return back_img(&self_image_img);
    // #if _BIG
    // 	uint8_t cha;
    // 	unsigned short *nihao;
    // 	nihao = (unsigned short *)arg_img.data;
    // 	for (int i = 0; i < arg_img.w * arg_img.h; i++)
    // 	{
    // 		cha = nihao[i];
    // 		nihao[i] = nihao[i] >> 8;
    // 		nihao[i] |= cha << 8;
    // 	}
    // #endif
}

static PyObject *
py_image_print(PyObject *self, PyObject *args)
{
    if (self_image_img.data == NULL)
    {
        printf("minicv no image!\rplease run \"img_bytes_load\" funaction input image!\r\n");
        return Py_BuildValue("i", -1);
    }
    switch (self_image_img.bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        printf("{\"w\":%d, \"h\":%d, \"type\"=\"binary\", \"size\":%d}",
               self_image_img.w, self_image_img.h,
               ((self_image_img.w + UINT32_T_MASK) >> UINT32_T_SHIFT) * self_image_img.h);
        break;
    }
    case IMAGE_BPP_GRAYSCALE:
    {
        printf("{\"w\":%d, \"h\":%d, \"type\"=\"grayscale\", \"size\":%d}",
               self_image_img.w, self_image_img.h,
               (self_image_img.w * self_image_img.h) * sizeof(uint8_t));
        break;
    }
    case IMAGE_BPP_RGB565:
    {
        printf("{\"w\":%d, \"h\":%d, \"type\"=\"rgb565\", \"size\":%d}",
               self_image_img.w, self_image_img.h,
               (self_image_img.w * self_image_img.h) * sizeof(uint16_t));
        break;
    }
    case IMAGE_BPP_BAYER:
    {
        printf("{\"w\":%d, \"h\":%d, \"type\"=\"bayer\", \"size\":%d}",
               self_image_img.w, self_image_img.h,
               (self_image_img.w * self_image_img.h) * sizeof(uint8_t));
        break;
    }
    default:
    {
        if ((self_image_img.data[0] == 0xFE) && (self_image_img.data[self_image_img.bpp - 1] == 0xFE))
        { // for ide
            // print->print_strn(print->data, (const char *) self_image_img.data, self_image_img.bpp);
        }
        else
        { // not for ide
            printf("{\"w\":%d, \"h\":%d, \"type\"=\"jpeg\", \"size\":%d}",
                   self_image_img.w, self_image_img.h,
                   self_image_img.bpp);
        }
        break;
    }
    }
    return Py_BuildValue("i", 0);
}

static PyObject *
py_image_binary_to_grayscale(PyObject *self, PyObject *args, PyObject *keywds)
{
    int b;
    debug_line;
    bool nihao = 0;
    static char *kwlist[] = {"binary_image_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &b))
        return NULL;

    return Py_BuildValue("i", b ? 255 : 0);
}

static PyObject *
py_image_binary_to_rgb(PyObject *self, PyObject *args, PyObject *keywds)
{
    int b = 0;
    static char *kwlist[] = {"binary_image_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &b))
        return NULL;
    uint16_t rgb565 = COLOR_BINARY_TO_RGB565(b);

    return Py_BuildValue("iii", COLOR_RGB565_TO_R8(rgb565), COLOR_RGB565_TO_G8(rgb565), COLOR_RGB565_TO_B8(rgb565));
}

static PyObject *
py_image_binary_to_lab(PyObject *self, PyObject *args, PyObject *keywds)
{
    int b = 0;
    static char *kwlist[] = {"binary_image_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &b))
        return NULL;
    uint16_t rgb565 = COLOR_BINARY_TO_RGB565(b);
    int ll = COLOR_RGB565_TO_L(rgb565);
    int aa = COLOR_RGB565_TO_A(rgb565);
    int bb = COLOR_RGB565_TO_B(rgb565);

    return Py_BuildValue("iii", ll, aa, bb);
}

static PyObject *
py_image_binary_to_yuv(PyObject *self, PyObject *args, PyObject *keywds)
{
    int8_t b = 0;
    static char *kwlist[] = {"binary_image_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &b))
        return NULL;
    uint16_t rgb565 = COLOR_BINARY_TO_RGB565(b);
    int YY = COLOR_RGB565_TO_Y(rgb565);
    int UU = COLOR_RGB565_TO_U(rgb565);
    int VV = COLOR_RGB565_TO_V(rgb565);
    return Py_BuildValue("iii", YY, UU, VV);
}

static PyObject *
py_image_grayscale_to_binary(PyObject *self, PyObject *args, PyObject *keywds)
{
    int g = 0;
    static char *kwlist[] = {"grayscale_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &g))
        return NULL;
    return Py_BuildValue("i", COLOR_GRAYSCALE_TO_BINARY(g));
}

static PyObject *
py_image_grayscale_to_rgb(PyObject *self, PyObject *args, PyObject *keywds)
{
    int g = 0;
    static char *kwlist[] = {"grayscale_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &g))
        return NULL;

    uint16_t rgb565 = COLOR_GRAYSCALE_TO_RGB565(g);
    return Py_BuildValue("iii", COLOR_RGB565_TO_R8(rgb565), COLOR_RGB565_TO_G8(rgb565), COLOR_RGB565_TO_B8(rgb565));
}

static PyObject *
py_image_grayscale_to_lab(PyObject *self, PyObject *args, PyObject *keywds)
{
    int g = 0;
    static char *kwlist[] = {"grayscale_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &g))
        return NULL;
    uint16_t rgb565 = COLOR_GRAYSCALE_TO_RGB565(g);
    return Py_BuildValue("iii", COLOR_RGB565_TO_L(rgb565), COLOR_RGB565_TO_A(rgb565), COLOR_RGB565_TO_B(rgb565));
}

static PyObject *
py_image_grayscale_to_yuv(PyObject *self, PyObject *args, PyObject *keywds)
{
    int g = 0;
    static char *kwlist[] = {"grayscale_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &g))
        return NULL;
    uint16_t rgb565 = COLOR_GRAYSCALE_TO_RGB565(g);
    return Py_BuildValue("iii", COLOR_RGB565_TO_Y(rgb565), COLOR_RGB565_TO_U(rgb565), COLOR_RGB565_TO_V(rgb565));
}

static PyObject *
py_image_rgb_to_binary(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *rgb_tuple;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    static char *kwlist[] = {"rgb_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &rgb_tuple))
        return NULL;
    r = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 0));
    g = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 1));
    b = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 2));

    uint16_t rgb565 = COLOR_R8_G8_B8_TO_RGB565(r, g, b);

    return Py_BuildValue("i", COLOR_RGB565_TO_BINARY(rgb565));
}

static PyObject *
py_image_rgb_to_grayscale(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *rgb_tuple;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    static char *kwlist[] = {"rgb_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &rgb_tuple))
        return NULL;
    r = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 0));
    g = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 1));
    b = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 2));
    uint16_t rgb565 = COLOR_R8_G8_B8_TO_RGB565(r, g, b);
    return Py_BuildValue("i", COLOR_RGB565_TO_GRAYSCALE(rgb565));
}

static PyObject *
py_image_rgb_to_lab(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *rgb_tuple;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    static char *kwlist[] = {"rgb_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &rgb_tuple))
        return NULL;
    r = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 0));
    g = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 1));
    b = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 2));
    uint16_t rgb565 = COLOR_R8_G8_B8_TO_RGB565(r, g, b);
    return Py_BuildValue("iii", COLOR_RGB565_TO_L(rgb565), COLOR_RGB565_TO_A(rgb565), COLOR_RGB565_TO_B(rgb565));
}

static PyObject *
py_image_rgb_to_yuv(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *rgb_tuple;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    static char *kwlist[] = {"rgb_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &rgb_tuple))
        return NULL;
    r = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 0));
    g = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 1));
    b = PyLong_AsLong(PyTuple_GetItem(rgb_tuple, 2));
    uint16_t rgb565 = COLOR_R8_G8_B8_TO_RGB565(r, g, b);
    return Py_BuildValue("iii", COLOR_RGB565_TO_Y(rgb565), COLOR_RGB565_TO_U(rgb565), COLOR_RGB565_TO_V(rgb565));
}

static PyObject *
py_image_lab_to_binary(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *lab_tuple;
    uint8_t l = 0;
    uint8_t a = 0;
    uint8_t b = 0;
    static char *kwlist[] = {"lab_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &lab_tuple))
        return NULL;
    l = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 0));
    a = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 1));
    b = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 2));

    uint16_t rgb565 = COLOR_LAB_TO_RGB565(l, a, b);
    return Py_BuildValue("i", COLOR_RGB565_TO_BINARY(rgb565));
}

static PyObject *
py_image_lab_to_grayscale(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *lab_tuple;
    uint8_t l = 0;
    uint8_t a = 0;
    uint8_t b = 0;
    static char *kwlist[] = {"lab_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &lab_tuple))
        return NULL;
    l = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 0));
    a = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 1));
    b = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 2));
    uint16_t rgb565 = COLOR_LAB_TO_RGB565(l, a, b);
    return Py_BuildValue("i", COLOR_RGB565_TO_GRAYSCALE(rgb565));
}

static PyObject *
py_image_lab_to_rgb(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *lab_tuple;
    uint8_t l = 0;
    uint8_t a = 0;
    uint8_t b = 0;
    static char *kwlist[] = {"lab_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &lab_tuple))
        return NULL;
    l = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 0));
    a = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 1));
    b = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 2));
    uint16_t rgb565 = COLOR_LAB_TO_RGB565(l, a, b);
    return Py_BuildValue("iii", COLOR_RGB565_TO_R8(rgb565), COLOR_RGB565_TO_G8(rgb565), COLOR_RGB565_TO_B8(rgb565));
}

static PyObject *
py_image_lab_to_yuv(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *lab_tuple;
    uint8_t l = 0;
    uint8_t a = 0;
    uint8_t b = 0;
    static char *kwlist[] = {"lab_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &lab_tuple))
        return NULL;
    l = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 0));
    a = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 1));
    b = PyLong_AsLong(PyTuple_GetItem(lab_tuple, 2));
    uint16_t rgb565 = COLOR_LAB_TO_RGB565(l, a, b);
    return Py_BuildValue("iii", COLOR_RGB565_TO_Y(rgb565), COLOR_RGB565_TO_U(rgb565), COLOR_RGB565_TO_V(rgb565));
}

static PyObject *
py_image_yuv_to_binary(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *yuv_tuple;
    uint8_t y = 0;
    uint8_t u = 0;
    uint8_t v = 0;
    static char *kwlist[] = {"yuv_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &yuv_tuple))
        return NULL;
    y = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 0));
    u = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 1));
    v = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 2));

    uint16_t rgb565 = COLOR_YUV_TO_RGB565(y, u, v);
    return Py_BuildValue("i", COLOR_RGB565_TO_BINARY(rgb565));
}

static PyObject *
py_image_yuv_to_grayscale(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *yuv_tuple;
    uint8_t y = 0;
    uint8_t u = 0;
    uint8_t v = 0;
    static char *kwlist[] = {"yuv_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &yuv_tuple))
        return NULL;
    y = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 0));
    u = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 1));
    v = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 2));
    uint16_t rgb565 = COLOR_YUV_TO_RGB565(y, u, v);
    return Py_BuildValue("i", COLOR_RGB565_TO_GRAYSCALE(rgb565));
}

static PyObject *
py_image_yuv_to_rgb(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *yuv_tuple;
    uint8_t y = 0;
    uint8_t u = 0;
    uint8_t v = 0;
    static char *kwlist[] = {"yuv_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &yuv_tuple))
        return NULL;
    y = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 0));
    u = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 1));
    v = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 2));

    uint16_t rgb565 = COLOR_YUV_TO_RGB565(y, u, v);
    return Py_BuildValue("iii", COLOR_RGB565_TO_R8(rgb565), COLOR_RGB565_TO_G8(rgb565), COLOR_RGB565_TO_B8(rgb565));
}

static PyObject *
py_image_yuv_to_lab(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *yuv_tuple;

    int py = 0;
    int pu = 0;
    int pv = 0;

    static char *kwlist[] = {"yuv_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &yuv_tuple))
        return NULL;
    py = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 0));
    pu = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 1));
    pv = PyLong_AsLong(PyTuple_GetItem(yuv_tuple, 2));

    uint8_t y = py;
    uint8_t u = pu;
    uint8_t v = pv;
    uint16_t rgb565 = COLOR_YUV_TO_RGB565(y, u, v);
    return Py_BuildValue("iii", COLOR_RGB565_TO_L(rgb565), COLOR_RGB565_TO_A(rgb565), COLOR_RGB565_TO_B(rgb565));
}

//////////////
// Get Methods
//////////////

// #ifdef IMLIB_ENABLE_GET_SIMILARITY
// // Similarity Object //
// #define py_similarity_obj_size 4

// // static void py_image_similarity_print(PyObject *self, PyObject *args)
// // {
// //     py_similarity_obj_t *self = self_in;
// //     mp_printf(print,
// //               "{\"mean\":%f, \"stdev\":%f, \"min\":%f, \"max\":%f}",
// //               (double)mp_obj_get_float(self->avg),
// //               (double)mp_obj_get_float(self->std),
// //               (double)mp_obj_get_float(self->min),
// //               (double)mp_obj_get_float(self->max));
// // }

// // static PyObject *
// // py_similarity_subscr(PyObject *self, PyObject *args, PyObject *keywds)
// // {
// //     if (value == MP_OBJ_SENTINEL)
// //     { // load
// //         py_similarity_obj_t *self = self_in;
// //         if (MP_OBJ_IS_TYPE(index, &mp_type_slice))
// //         {
// //             mp_bound_slice_t slice;
// //             if (!mp_seq_get_fast_slice_indexes(py_similarity_obj_size, index, &slice))
// //             {
// //                 mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("only slices with step=1 (aka None) are supported"));
// //             }
// //             mp_obj_tuple_t *result = mp_obj_new_tuple(slice.stop - slice.start, NULL);
// //             mp_seq_copy(result->items, &(self->avg) + slice.start, result->len, mp_obj_t);
// //             return result;
// //         }
// //         switch (mp_get_index(self->base.type, py_similarity_obj_size, index, false))
// //         {
// //         case 0:
// //             return self->avg;
// //         case 1:
// //             return self->std;
// //         case 2:
// //             return self->min;
// //         case 3:
// //             return self->max;
// //         }
// //     }
// //     return MP_OBJ_NULL; // op not supported
// // }

// // mp_obj_t py_similarity_mean(mp_obj_t self_in) { return ((py_similarity_obj_t *)self_in)->avg; }
// // mp_obj_t py_similarity_stdev(mp_obj_t self_in) { return ((py_similarity_obj_t *)self_in)->std; }
// // mp_obj_t py_similarity_min(mp_obj_t self_in) { return ((py_similarity_obj_t *)self_in)->min; }
// // mp_obj_t py_similarity_max(mp_obj_t self_in) { return ((py_similarity_obj_t *)self_in)->max; }

// static mp_obj_t py_image_get_similarity(mp_obj_t img_obj, mp_obj_t other_obj)
// {
//     image_t *arg_img = py_helper_arg_to_image_mutable(img_obj);
//     float avg, std, min, max;

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(other_obj))
//     {
//         imlib_get_similarity(arg_img, mp_obj_str_get_str(other_obj), NULL, 0, &avg, &std, &min, &max);
//     }
//     else if (MP_OBJ_IS_TYPE(other_obj, &py_image_type))
//     {
//         imlib_get_similarity(arg_img, NULL, py_helper_arg_to_image_mutable(other_obj), 0, &avg, &std, &min, &max);
//     }
//     else
//     {
//         imlib_get_similarity(arg_img, NULL, NULL,
//                              py_helper_keyword_color(arg_img, 1, &other_obj, 0, NULL, 0),
//                              &avg, &std, &min, &max);
//     }

//     fb_alloc_free_till_mark();

//     py_similarity_obj_t *o = m_new_obj(py_similarity_obj_t);
//     o->base.type = &py_similarity_type;
//     o->avg = mp_obj_new_float(avg);
//     o->std = mp_obj_new_float(std);
//     o->min = mp_obj_new_float(min);
//     o->max = mp_obj_new_float(max);
//     return o;
// }
// STATIC MP_DEFINE_CONST_FUN_OBJ_2(py_image_get_similarity_obj, py_image_get_similarity);
// #endif // IMLIB_ENABLE_GET_SIMILARITY

#ifdef IMLIB_ENABLE_GET_HISTOGRAM

static PyObject *
py_image_get_histogram(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    list_t thresholds;
    list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
    py_helper_keyword_thresholds(n_args, args, 1, kw_args, &thresholds);
    bool invert = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_invert), false);
    image_t *other = py_helper_keyword_to_image_mutable(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_difference), NULL);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 3, kw_args, &roi);

    histogram_t hist;
    switch (arg_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        int bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                         (COLOR_BINARY_MAX - COLOR_BINARY_MIN + 1));
        PY_ASSERT_TRUE_MSG(bins >= 2, "bins must be >= 2");
        hist.LBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), bins);
        PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
        hist.ABinCount = 0;
        hist.BBinCount = 0;
        fb_alloc_mark();
        hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
        hist.ABins = NULL;
        hist.BBins = NULL;
        imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert, other);
        list_free(&thresholds);
        break;
    }
    case IMAGE_BPP_GRAYSCALE:
    {
        int bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                         (COLOR_GRAYSCALE_MAX - COLOR_GRAYSCALE_MIN + 1));
        PY_ASSERT_TRUE_MSG(bins >= 2, "bins must be >= 2");
        hist.LBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), bins);
        PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
        hist.ABinCount = 0;
        hist.BBinCount = 0;
        fb_alloc_mark();
        hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
        hist.ABins = NULL;
        hist.BBins = NULL;
        imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert, other);
        list_free(&thresholds);
        break;
    }
    case IMAGE_BPP_RGB565:
    {
        int l_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                           (COLOR_L_MAX - COLOR_L_MIN + 1));
        PY_ASSERT_TRUE_MSG(l_bins >= 2, "bins must be >= 2");
        hist.LBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), l_bins);
        PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
        int a_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                           (COLOR_A_MAX - COLOR_A_MIN + 1));
        PY_ASSERT_TRUE_MSG(a_bins >= 2, "bins must be >= 2");
        hist.ABinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_a_bins), a_bins);
        PY_ASSERT_TRUE_MSG(hist.ABinCount >= 2, "a_bins must be >= 2");
        int b_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                           (COLOR_B_MAX - COLOR_B_MIN + 1));
        PY_ASSERT_TRUE_MSG(b_bins >= 2, "bins must be >= 2");
        hist.BBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_b_bins), b_bins);
        PY_ASSERT_TRUE_MSG(hist.BBinCount >= 2, "b_bins must be >= 2");
        fb_alloc_mark();
        hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
        hist.ABins = fb_alloc(hist.ABinCount * sizeof(float), FB_ALLOC_NO_HINT);
        hist.BBins = fb_alloc(hist.BBinCount * sizeof(float), FB_ALLOC_NO_HINT);
        imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert, other);
        list_free(&thresholds);
        break;
    }
    default:
    {
        return MP_OBJ_NULL;
    }
    }

    py_histogram_obj_t *o = m_new_obj(py_histogram_obj_t);
    o->base.type = &py_histogram_type;
    o->bpp = arg_img->bpp;

    o->LBins = mp_obj_new_list(hist.LBinCount, NULL);
    o->ABins = mp_obj_new_list(hist.ABinCount, NULL);
    o->BBins = mp_obj_new_list(hist.BBinCount, NULL);

    for (int i = 0; i < hist.LBinCount; i++)
    {
        ((mp_obj_list_t *)o->LBins)->items[i] = mp_obj_new_float(hist.LBins[i]);
    }

    for (int i = 0; i < hist.ABinCount; i++)
    {
        ((mp_obj_list_t *)o->ABins)->items[i] = mp_obj_new_float(hist.ABins[i]);
    }

    for (int i = 0; i < hist.BBinCount; i++)
    {
        ((mp_obj_list_t *)o->BBins)->items[i] = mp_obj_new_float(hist.BBins[i]);
    }

    fb_alloc_free_till_mark();

    return o;
}
#endif // IMLIB_ENABLE_GET_HISTOGRAM

#ifdef IMLIB_ENABLE_GET_PERCENTILE

static PyObject *
py_histogram_get_percentile(PyObject *self, PyObject *args, PyObject *keywds)
{
    histogram_t hist;
    hist.LBinCount = ((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->LBins)->len;
    hist.ABinCount = ((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->ABins)->len;
    hist.BBinCount = ((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->BBins)->len;
    fb_alloc_mark();
    hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
    hist.ABins = fb_alloc(hist.ABinCount * sizeof(float), FB_ALLOC_NO_HINT);
    hist.BBins = fb_alloc(hist.BBinCount * sizeof(float), FB_ALLOC_NO_HINT);

    for (int i = 0; i < hist.LBinCount; i++)
    {
        hist.LBins[i] = mp_obj_get_float(((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->LBins)->items[i]);
    }

    for (int i = 0; i < hist.ABinCount; i++)
    {
        hist.ABins[i] = mp_obj_get_float(((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->ABins)->items[i]);
    }

    for (int i = 0; i < hist.BBinCount; i++)
    {
        hist.BBins[i] = mp_obj_get_float(((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->BBins)->items[i]);
    }

    percentile_t p;
    imlib_get_percentile(&p, ((py_histogram_obj_t *)self_in)->bpp, &hist, mp_obj_get_float(percentile));
    fb_alloc_free_till_mark();

    py_percentile_obj_t *o = m_new_obj(py_percentile_obj_t);
    o->base.type = &py_percentile_type;
    o->bpp = ((py_histogram_obj_t *)self_in)->bpp;

    o->LValue = mp_obj_new_int(p.LValue);
    o->AValue = mp_obj_new_int(p.AValue);
    o->BValue = mp_obj_new_int(p.BValue);

    return o;
}

#endif // IMLIB_ENABLE_GET_PERCENTILE

#ifdef IMLIB_ENABLE_GET_THRESHOLD
static PyObject *
py_histogram_get_threshold(PyObject *self, PyObject *args, PyObject *keywds)
{
    histogram_t hist;
    hist.LBinCount = ((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->LBins)->len;
    hist.ABinCount = ((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->ABins)->len;
    hist.BBinCount = ((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->BBins)->len;
    fb_alloc_mark();
    hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
    hist.ABins = fb_alloc(hist.ABinCount * sizeof(float), FB_ALLOC_NO_HINT);
    hist.BBins = fb_alloc(hist.BBinCount * sizeof(float), FB_ALLOC_NO_HINT);

    for (int i = 0; i < hist.LBinCount; i++)
    {
        hist.LBins[i] = mp_obj_get_float(((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->LBins)->items[i]);
    }

    for (int i = 0; i < hist.ABinCount; i++)
    {
        hist.ABins[i] = mp_obj_get_float(((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->ABins)->items[i]);
    }

    for (int i = 0; i < hist.BBinCount; i++)
    {
        hist.BBins[i] = mp_obj_get_float(((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->BBins)->items[i]);
    }

    threshold_t t;
    imlib_get_threshold(&t, ((py_histogram_obj_t *)self_in)->bpp, &hist);
    fb_alloc_free_till_mark();

    py_threshold_obj_t *o = m_new_obj(py_threshold_obj_t);
    o->base.type = &py_threshold_type;
    o->bpp = ((py_threshold_obj_t *)self_in)->bpp;

    o->LValue = mp_obj_new_int(t.LValue);
    o->AValue = mp_obj_new_int(t.AValue);
    o->BValue = mp_obj_new_int(t.BValue);

    return o;
}

#endif //IMLIB_ENABLE_GET_THRESHOLD


#ifdef IMLIB_ENABLE_FIND_BLOBS

// static PyObject *
// py_image_find_blobs(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     image_t *arg_img = &self_image_img;

//     list_t thresholds;
//     list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
//     py_helper_arg_to_thresholds(args[1], &thresholds);
//     if (!list_size(&thresholds))
//         return mp_obj_new_list(0, NULL);
//     bool invert = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_invert), false);

//     rectangle_t roi;
//     py_helper_keyword_rectangle_roi(arg_img, n_args, args, 3, kw_args, &roi);

//     unsigned int x_stride =
//         py_helper_keyword_int(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_stride), 2);
//     PY_ASSERT_TRUE_MSG(x_stride > 0, "x_stride must not be zero.");
//     unsigned int y_stride =
//         py_helper_keyword_int(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_stride), 1);
//     PY_ASSERT_TRUE_MSG(y_stride > 0, "y_stride must not be zero.");
//     unsigned int area_threshold =
//         py_helper_keyword_int(n_args, args, 6, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_area_threshold), 10);
//     unsigned int pixels_threshold =
//         py_helper_keyword_int(n_args, args, 7, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_pixels_threshold), 10);
//     bool merge =
//         py_helper_keyword_int(n_args, args, 8, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_merge), false);
//     int margin =
//         py_helper_keyword_int(n_args, args, 9, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_margin), 0);
//     mp_obj_t threshold_cb =
//         py_helper_keyword_object(n_args, args, 10, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold_cb), NULL);
//     mp_obj_t merge_cb =
//         py_helper_keyword_object(n_args, args, 11, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_merge_cb), NULL);
//     unsigned int x_hist_bins_max =
//         py_helper_keyword_int(n_args, args, 12, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_hist_bins_max), 0);
//     unsigned int y_hist_bins_max =
//         py_helper_keyword_int(n_args, args, 13, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_hist_bins_max), 0);

//     list_t out;
//     fb_alloc_mark();
//     imlib_find_blobs(&out, arg_img, &roi, x_stride, y_stride, &thresholds, invert,
//                      area_threshold, pixels_threshold, merge, margin,
//                      py_image_find_blobs_threshold_cb, threshold_cb, py_image_find_blobs_merge_cb, merge_cb, x_hist_bins_max, y_hist_bins_max);
//     fb_alloc_free_till_mark();
//     list_free(&thresholds);

//     mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
//     for (size_t i = 0; list_size(&out); i++)
//     {
//         find_blobs_list_lnk_data_t lnk_data;
//         list_pop_front(&out, &lnk_data);

//         py_blob_obj_t *o = m_new_obj(py_blob_obj_t);
//         o->base.type = &py_blob_type;
//         o->corners = mp_obj_new_tuple(4, (mp_obj_t[]){mp_obj_new_tuple(2, (mp_obj_t[]){mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION * 0) / 4].x),
//                                                                                        mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION * 0) / 4].y)}),
//                                                       mp_obj_new_tuple(2, (mp_obj_t[]){mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION * 1) / 4].x),
//                                                                                        mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION * 1) / 4].y)}),
//                                                       mp_obj_new_tuple(2, (mp_obj_t[]){mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION * 2) / 4].x),
//                                                                                        mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION * 2) / 4].y)}),
//                                                       mp_obj_new_tuple(2, (mp_obj_t[]){mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION * 3) / 4].x),
//                                                                                        mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION * 3) / 4].y)})});
//         point_t min_corners[4];
//         point_min_area_rectangle(lnk_data.corners, min_corners, FIND_BLOBS_CORNERS_RESOLUTION);
//         o->min_corners = mp_obj_new_tuple(4, (mp_obj_t[]){mp_obj_new_tuple(2, (mp_obj_t[]){mp_obj_new_int(min_corners[0].x), mp_obj_new_int(min_corners[0].y)}),
//                                                           mp_obj_new_tuple(2, (mp_obj_t[]){mp_obj_new_int(min_corners[1].x), mp_obj_new_int(min_corners[1].y)}),
//                                                           mp_obj_new_tuple(2, (mp_obj_t[]){mp_obj_new_int(min_corners[2].x), mp_obj_new_int(min_corners[2].y)}),
//                                                           mp_obj_new_tuple(2, (mp_obj_t[]){mp_obj_new_int(min_corners[3].x), mp_obj_new_int(min_corners[3].y)})});
//         o->x = mp_obj_new_int(lnk_data.rect.x);
//         o->y = mp_obj_new_int(lnk_data.rect.y);
//         o->w = mp_obj_new_int(lnk_data.rect.w);
//         o->h = mp_obj_new_int(lnk_data.rect.h);
//         o->pixels = mp_obj_new_int(lnk_data.pixels);
//         o->cx = mp_obj_new_float(lnk_data.centroid_x);
//         o->cy = mp_obj_new_float(lnk_data.centroid_y);
//         o->rotation = mp_obj_new_float(lnk_data.rotation);
//         o->code = mp_obj_new_int(lnk_data.code);
//         o->count = mp_obj_new_int(lnk_data.count);
//         o->perimeter = mp_obj_new_int(lnk_data.perimeter);
//         o->roundness = mp_obj_new_float(lnk_data.roundness);
//         o->x_hist_bins = mp_obj_new_list(lnk_data.x_hist_bins_count, NULL);
//         o->y_hist_bins = mp_obj_new_list(lnk_data.y_hist_bins_count, NULL);

//         for (int i = 0; i < lnk_data.x_hist_bins_count; i++)
//         {
//             ((mp_obj_list_t *)o->x_hist_bins)->items[i] = mp_obj_new_int(lnk_data.x_hist_bins[i]);
//         }

//         for (int i = 0; i < lnk_data.y_hist_bins_count; i++)
//         {
//             ((mp_obj_list_t *)o->y_hist_bins)->items[i] = mp_obj_new_int(lnk_data.y_hist_bins[i]);
//         }

//         objects_list->items[i] = o;
//         if (lnk_data.x_hist_bins)
//             xfree(lnk_data.x_hist_bins);
//         if (lnk_data.y_hist_bins)
//             xfree(lnk_data.y_hist_bins);
//     }

//     return objects_list;
// }
static PyObject *
py_image_find_blobs(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    struct RECE_STA
    {
        PyObject *thresholds;
        PyObject *roi;
        int x_stride;
        int y_stride;
        PyObject *invert;
        int area_threshold;
        int pixels_threshold;
        PyObject *merge;
        int margin;
    } rece_sta;
    PyObject *thr;
    PyObject *thr2;
    PyObject *thr3;

    list_t out;        //定义输出列表
    list_t thresholds; //定义一个LAB阈值列表
    // list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t)); //列表初始化
    // color_thresholds_list_lnk_data_t thrm;
    rectangle_t roi; //定义感兴趣区域
    bool invert;     //反相标志
    bool merge;      //合并标志

    find_blobs_list_lnk_data_t lnk_blob;

    static char *kwlist[] = {"thresholds", "roi", "x_stride", "y_stride", "invert", "area_threshold", "pixels_threshold", "merge", "margin", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|OiiOiiOi", kwlist,
                                     &rece_sta.thresholds, &rece_sta.roi, &rece_sta.x_stride, &rece_sta.y_stride, &rece_sta.invert, &rece_sta.area_threshold, &rece_sta.pixels_threshold, &rece_sta.merge, &rece_sta.margin))
        return NULL;

    if (!PyList_Check(rece_sta.thresholds))
        return NULL;
    if (!PyList_Check(rece_sta.roi))
        return NULL;
    if (!PyBool_Check(rece_sta.invert))
        return NULL;
    if (!PyBool_Check(rece_sta.merge))
        return NULL;

    if (thresholds_tan(rece_sta.thresholds, &thresholds) == 0)
    {
        return PyList_New(0);
    }

    roi_tan(rece_sta.roi, &roi, arg_img->w, arg_img->h);

    if (rece_sta.invert == Py_True)
        invert = 1;
    else
        invert = 0;

    if (rece_sta.merge == Py_True)
        merge = 1;
    else
        merge = 0;
    fb_alloc_mark();
    imlib_find_blobs(&out, &arg_img, &roi, rece_sta.x_stride, rece_sta.y_stride, &thresholds, invert,
                     rece_sta.area_threshold, rece_sta.pixels_threshold, merge, rece_sta.margin);
    fb_alloc_free_till_mark();
    thr = PyList_New(0);

    while (out.size > 0)
    {
        list_pop_front(&out, &lnk_blob);
        thr2 = PyDict_New();
        PyDict_SetItemString(thr2, "x", PyLong_FromLong(lnk_blob.rect.x));
        PyDict_SetItemString(thr2, "y", PyLong_FromLong(lnk_blob.rect.y));
        PyDict_SetItemString(thr2, "w", PyLong_FromLong(lnk_blob.rect.w));
        PyDict_SetItemString(thr2, "h", PyLong_FromLong(lnk_blob.rect.h));
        PyDict_SetItemString(thr2, "pixels", PyLong_FromLong(lnk_blob.pixels));
        PyDict_SetItemString(thr2, "cx", PyLong_FromLong(lnk_blob.centroid.x));
        PyDict_SetItemString(thr2, "cy", PyLong_FromLong(lnk_blob.centroid.y));
        PyDict_SetItemString(thr2, "rotation", PyFloat_FromDouble(lnk_blob.rotation));
        PyDict_SetItemString(thr2, "code", PyLong_FromLong(lnk_blob.code));
        PyDict_SetItemString(thr2, "count", PyLong_FromLong(lnk_blob.count));
        PyList_Append(thr, thr2);
    }
    return thr;
}
#endif //IMLIB_ENABLE_FIND_BLOBS

#ifdef IMLIB_ENABLE_FIND_LINES
static PyObject *
py_image_find_lines(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;

    unsigned int x_stride = 2;
    // PY_ASSERT_TRUE_MSG(x_stride > 0, "x_stride must not be zero.");
    unsigned int y_stride = 1;
    // PY_ASSERT_TRUE_MSG(y_stride > 0, "y_stride must not be zero.");
    uint32_t threshold = 1000;
    unsigned int theta_margin = 25;
    unsigned int rho_margin = 25;

    PyObject *py_roi = Py_None;
    debug_line;
    static char *kwlist[] = {"roi", "x_stride", "y_stride", "threshold", "theta_margin", "rho_margin", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|Oiiiii", kwlist,
                                     &py_roi, &x_stride, &y_stride, &threshold, &theta_margin, &rho_margin))
        return NULL;
    debug_line;
    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);
    list_t out;
    fb_alloc_mark();
    debug_line;
    imlib_find_lines(&out, arg_img, &roi, x_stride, y_stride, threshold, theta_margin, rho_margin);
    debug_line;
    fb_alloc_free_till_mark();
    debug_line;
    PyObject *objects_list = PyList_New(list_size(&out));
    PyObject *o;
    debug_line;
    for (size_t i = 0; list_size(&out); i++)
    {
        find_lines_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);
        o = PyDict_New();
        PyDict_SetItem(o, Py_BuildValue("s", "x1"), Py_BuildValue("i", lnk_data.line.x1));
        PyDict_SetItem(o, Py_BuildValue("s", "y1"), Py_BuildValue("i", lnk_data.line.y1));
        PyDict_SetItem(o, Py_BuildValue("s", "x2"), Py_BuildValue("i", lnk_data.line.x2));
        PyDict_SetItem(o, Py_BuildValue("s", "y2"), Py_BuildValue("i", lnk_data.line.y2));
        int x_diff = lnk_data.line.x2 - lnk_data.line.x1;
        int y_diff = lnk_data.line.y2 - lnk_data.line.y1;
        PyDict_SetItem(o, Py_BuildValue("s", "length"), Py_BuildValue("i", fast_roundf(fast_sqrtf((x_diff * x_diff) + (y_diff * y_diff)))));
        PyDict_SetItem(o, Py_BuildValue("s", "magnitude"), Py_BuildValue("i", lnk_data.magnitude));
        PyDict_SetItem(o, Py_BuildValue("s", "theta"), Py_BuildValue("i", lnk_data.theta));
        PyDict_SetItem(o, Py_BuildValue("s", "rho"), Py_BuildValue("i", lnk_data.rho));
        PyList_SetItem(objects_list, i, o);
        debug_line;
    }
    debug_line;
    return objects_list;
}
#endif // IMLIB_ENABLE_FIND_LINES

#ifdef IMLIB_ENABLE_FIND_LINE_SEGMENTS
static PyObject *
py_image_find_line_segments(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;

    unsigned int merge_distance = 0;
    unsigned int max_theta_diff = 15;

    PyObject *py_roi = Py_None;

    static char *kwlist[] = {"roi", "merge_distance", "max_theta_difference", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|Oii", kwlist,
                                     &py_roi, &merge_distance, &max_theta_diff))
        return NULL;

    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    list_t out;
    fb_alloc_mark();
    imlib_lsd_find_line_segments(&out, arg_img, &roi, merge_distance, max_theta_diff);
    fb_alloc_free_till_mark();

    PyObject *objects_list = PyList_New(list_size(&out));
    PyObject *o;
    for (size_t i = 0; list_size(&out); i++)
    {
        find_lines_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        o = PyDict_New();
        PyDict_SetItem(o, Py_BuildValue("s", "x1"), Py_BuildValue("i", lnk_data.line.x1));
        PyDict_SetItem(o, Py_BuildValue("s", "y1"), Py_BuildValue("i", lnk_data.line.y1));
        PyDict_SetItem(o, Py_BuildValue("s", "x2"), Py_BuildValue("i", lnk_data.line.x2));
        PyDict_SetItem(o, Py_BuildValue("s", "y2"), Py_BuildValue("i", lnk_data.line.y2));

        int x_diff = lnk_data.line.x2 - lnk_data.line.x1;
        int y_diff = lnk_data.line.y2 - lnk_data.line.y1;

        PyDict_SetItem(o, Py_BuildValue("s", "length"), Py_BuildValue("i", fast_roundf(fast_sqrtf((x_diff * x_diff) + (y_diff * y_diff)))));
        PyDict_SetItem(o, Py_BuildValue("s", "magnitude"), Py_BuildValue("i", lnk_data.magnitude));
        PyDict_SetItem(o, Py_BuildValue("s", "theta"), Py_BuildValue("i", lnk_data.theta));
        PyDict_SetItem(o, Py_BuildValue("s", "rho"), Py_BuildValue("i", lnk_data.rho));

        PyList_SetItem(objects_list, i, o);
    }

    return objects_list;
}
#endif // IMLIB_ENABLE_FIND_LINE_SEGMENTS

#define IMLIB_ENABLE_GET_REGRESSION
#ifdef IMLIB_ENABLE_GET_REGRESSION
static PyObject *
py_image_get_regression(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    list_t thresholds;
    PyObject *py_thresholds;
    list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
    // if (!list_size(&thresholds)) return mp_const_none;
    bool invert = 0;

    rectangle_t roi;

    unsigned int x_stride = 2;
    // PY_ASSERT_TRUE_MSG(x_stride > 0, "x_stride must not be zero.");
    unsigned int y_stride = 1;
    // PY_ASSERT_TRUE_MSG(y_stride > 0, "y_stride must not be zero.");
    unsigned int area_threshold = 10;
    unsigned int pixels_threshold = 10;
    bool robust = 0;

    PyObject *py_roi = Py_None;

    static char *kwlist[] = {"thresholds", "invert", "roi", "x_stride", "y_stride", "area_threshold", "pixels_threshold", "robust", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|iOiiiii", kwlist,
                                     &py_roi, &invert, &py_roi, &x_stride, &y_stride, &area_threshold, &pixels_threshold, &robust))
        return NULL;

    thresholds_tan(py_thresholds, &thresholds);
    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    find_lines_list_lnk_data_t out;
    fb_alloc_mark();
    bool result = imlib_get_regression(&out, arg_img, &roi, x_stride, y_stride, &thresholds, invert, area_threshold, pixels_threshold, robust);
    fb_alloc_free_till_mark();
    list_free(&thresholds);
    if (!result)
    {
        return Py_None;
    }

    PyObject *o = PyDict_New();
    PyDict_SetItem(o, Py_BuildValue("s", "x1"), Py_BuildValue("i", out.line.x1));
    PyDict_SetItem(o, Py_BuildValue("s", "y1"), Py_BuildValue("i", out.line.y1));
    PyDict_SetItem(o, Py_BuildValue("s", "x2"), Py_BuildValue("i", out.line.x2));
    PyDict_SetItem(o, Py_BuildValue("s", "y2"), Py_BuildValue("i", out.line.y2));

    int x_diff = out.line.x2 - out.line.x1;
    int y_diff = out.line.y2 - out.line.y1;

    PyDict_SetItem(o, Py_BuildValue("s", "length"), Py_BuildValue("i", fast_roundf(fast_sqrtf((x_diff * x_diff) + (y_diff * y_diff)))));
    PyDict_SetItem(o, Py_BuildValue("s", "magnitude"), Py_BuildValue("i", out.magnitude));
    PyDict_SetItem(o, Py_BuildValue("s", "theta"), Py_BuildValue("i", out.theta));
    PyDict_SetItem(o, Py_BuildValue("s", "rho"), Py_BuildValue("i", out.rho));
    return o;
}

#endif // IMLIB_ENABLE_GET_REGRESSION

#ifdef IMLIB_ENABLE_FIND_CIRCLES
static PyObject *
py_image_find_circles(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;

    unsigned int x_stride = 2;
    // PY_ASSERT_TRUE_MSG(x_stride > 0, "x_stride must not be zero.");
    unsigned int y_stride = 1;
    // PY_ASSERT_TRUE_MSG(y_stride > 0, "y_stride must not be zero.");
    uint32_t threshold = 2000;
    unsigned int x_margin = 10;
    unsigned int y_margin = 10;
    unsigned int r_margin = 10;
    unsigned int r_min = 2;
    unsigned int r_max = IM_MIN((roi.w / 2), (roi.h / 2));
    unsigned int r_step = 2;

    PyObject *py_roi = Py_None;

    static char *kwlist[] = {"roi", "x_stride", "y_stride", "threshold", "x_margin", "y_margin", "r_margin", "r_min", "r_max", "r_step", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|Oiiiiiiiii", kwlist,
                                     &py_roi, &x_stride, &y_stride, &threshold, &x_margin, &y_margin, &r_margin, &r_min, &r_max, &r_step))
        return NULL;

    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    list_t out;
    fb_alloc_mark();
    imlib_find_circles(&out, arg_img, &roi, x_stride, y_stride, threshold, x_margin, y_margin, r_margin,
                       r_min, r_max, r_step);
    fb_alloc_free_till_mark();

    PyObject *objects_list = PyList_New(list_size(&out));
    PyObject *o;
    for (size_t i = 0; list_size(&out); i++)
    {
        find_circles_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        o = PyDict_New();

        PyDict_SetItem(o, Py_BuildValue("s", "x"), Py_BuildValue("i", lnk_data.p.x));
        PyDict_SetItem(o, Py_BuildValue("s", "y"), Py_BuildValue("i", lnk_data.p.y));
        PyDict_SetItem(o, Py_BuildValue("s", "r"), Py_BuildValue("i", lnk_data.r));
        PyDict_SetItem(o, Py_BuildValue("s", "magnitude"), Py_BuildValue("i", lnk_data.magnitude));

        PyList_SetItem(objects_list, i, o);
    }

    return objects_list;
}
#endif // IMLIB_ENABLE_FIND_CIRCLES

#ifdef IMLIB_ENABLE_FIND_RECTS
static PyObject *
py_image_find_rects(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;

    uint32_t threshold = 1000;
    PyObject *py_roi = Py_None;
    static char *kwlist[] = {"roi", "threshold", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|Oi", kwlist,
                                     &py_roi, &threshold))
        return NULL;

    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    list_t out;
    fb_alloc_mark();
    imlib_find_rects(&out, arg_img, &roi, threshold);
    fb_alloc_free_till_mark();

    PyObject *objects_list = PyList_New(list_size(&out));
    PyObject *o;
    PyObject *ot;
    PyObject *ott;

    for (size_t i = 0; list_size(&out); i++)
    {
        find_rects_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);
        o = PyDict_New();
        ot = PyTuple_New(4);
        ott = PyTuple_New(2);
        PyTuple_SetItem(ott, 0, Py_BuildValue("i", lnk_data.corners[0].x));
        PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[0].y));
        PyTuple_SetItem(ot, 0, ott);
        PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[1].x));
        PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[1].y));
        PyTuple_SetItem(ot, 1, ott);
        PyTuple_SetItem(ott, 2, Py_BuildValue("i", lnk_data.corners[2].x));
        PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[2].y));
        PyTuple_SetItem(ot, 2, ott);
        PyTuple_SetItem(ott, 3, Py_BuildValue("i", lnk_data.corners[3].x));
        PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[3].y));
        PyTuple_SetItem(ot, 3, ott);
        PyDict_SetItem(o, Py_BuildValue("s", "corners"), ot);
        ott = PyTuple_New(4);
        PyTuple_SetItem(ott, 0, Py_BuildValue("i", lnk_data.rect.x));
        PyTuple_SetItem(ott, 0, Py_BuildValue("i", lnk_data.rect.y));
        PyTuple_SetItem(ott, 0, Py_BuildValue("i", lnk_data.rect.w));
        PyTuple_SetItem(ott, 0, Py_BuildValue("i", lnk_data.rect.h));
        PyDict_SetItem(o, Py_BuildValue("s", "corners"), ott);
        PyDict_SetItem(o, Py_BuildValue("s", "magnitude"), Py_BuildValue("i", lnk_data.magnitude));

        PyList_SetItem(objects_list, 0, o);
    }

    return objects_list;
}

#endif // IMLIB_ENABLE_FIND_RECTS

#ifdef IMLIB_ENABLE_QRCODES
static PyObject *
py_image_find_qrcodes(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;

    PyObject *py_roi = Py_None;
    static char *kwlist[] = {"roi", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|O", kwlist,
                                     &py_roi))
        return NULL;

    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    list_t out;
    fb_alloc_mark();
    imlib_find_qrcodes(&out, arg_img, &roi);
    fb_alloc_free_till_mark();
    debug_line;
    PyObject *objects_list = PyList_New(list_size(&out));
    PyObject *o = Py_None;
    // PyObject *ot;
    for (size_t i = 0; list_size(&out); i++)
    {
        find_qrcodes_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);
        debug_line;
        o = PyDict_New();
        // o = m_new_obj(py_qrcode_obj_t);
        // o->base.type = &py_qrcode_type;

        // ot = Py_BuildValue("((ii)(ii)(ii)(ii))",
        //                                         lnk_data.corners[0].x,
        //                                         lnk_data.corners[0].y,
        //                                         lnk_data.corners[1].x,
        //                                         lnk_data.corners[1].y,
        //                                         lnk_data.corners[2].x,
        //                                         lnk_data.corners[2].y,
        //                                         lnk_data.corners[3].x,
        //                                         lnk_data.corners[3].y

        // );

        // ot = PyTuple_New(4);
        // ott = PyTuple_New(2);
        // PyTuple_SetItem(ott, 0, Py_BuildValue("i", lnk_data.corners[0].x));
        // PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[0].y));

        // PyTuple_SetItem(ot, 0, ott);
        // PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[1].x));
        // PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[1].y));
        // PyTuple_SetItem(ot, 1, ott);
        // PyTuple_SetItem(ott, 2, Py_BuildValue("i", lnk_data.corners[2].x));
        // PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[2].y));
        // PyTuple_SetItem(ot, 2, ott);
        // PyTuple_SetItem(ott, 3, Py_BuildValue("i", lnk_data.corners[3].x));
        // PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[3].y));
        // PyTuple_SetItem(ot, 3, ott);

        PyDict_SetItem(o, Py_BuildValue("s", "corners"), Py_BuildValue("((ii)(ii)(ii)(ii))", lnk_data.corners[0].x, lnk_data.corners[0].y, lnk_data.corners[1].x, lnk_data.corners[1].y, lnk_data.corners[2].x, lnk_data.corners[2].y, lnk_data.corners[3].x, lnk_data.corners[3].y

                                                                       ));
        debug_line;

        // ot = PyTuple_New(4);

        // PyTuple_SetItem(ot, 0, Py_BuildValue("i", lnk_data.rect.x));
        // PyTuple_SetItem(ot, 1, Py_BuildValue("i", lnk_data.rect.y));
        // PyTuple_SetItem(ot, 2, Py_BuildValue("i", lnk_data.rect.w));
        // PyTuple_SetItem(ot, 3, Py_BuildValue("i", lnk_data.rect.h));

        // ot = Py_BuildValue("(iiii)", lnk_data.rect.x,
        //                              lnk_data.rect.y,
        //                              lnk_data.rect.w,
        //                              lnk_data.rect.h
        //                                             );
        PyDict_SetItem(o, Py_BuildValue("s", "rect"), Py_BuildValue("(iiii)", lnk_data.rect.x, lnk_data.rect.y, lnk_data.rect.w, lnk_data.rect.h));
        debug_line;
        PyDict_SetItem(o, Py_BuildValue("s", "payload"), Py_BuildValue("s", lnk_data.payload));
        debug_line;
        // o->payload = mp_obj_new_str(lnk_data.payload, lnk_data.payload_len);
        PyDict_SetItem(o, Py_BuildValue("s", "version"), Py_BuildValue("i", lnk_data.version));
        debug_line;
        PyDict_SetItem(o, Py_BuildValue("s", "ecc_level"), Py_BuildValue("i", lnk_data.mask));
        debug_line;
        PyDict_SetItem(o, Py_BuildValue("s", "mask"), Py_BuildValue("i", lnk_data.version));
        debug_line;
        PyDict_SetItem(o, Py_BuildValue("s", "data_type"), Py_BuildValue("i", lnk_data.data_type));
        debug_line;
        PyDict_SetItem(o, Py_BuildValue("s", "eci"), Py_BuildValue("i", lnk_data.eci));
        debug_line;

        PyList_SetItem(objects_list, i, o);
        // objects_list->items[i] = o;
        debug_line;
        xfree(lnk_data.payload);
        debug_line;
    }
    debug_line;
    return objects_list;
}

#endif // IMLIB_ENABLE_QRCODES

#ifdef IMLIB_ENABLE_APRILTAGS
static PyObject *
py_image_find_apriltags(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;
    // #ifndef IMLIB_ENABLE_HIGH_RES_APRILTAGS
    //     // PY_ASSERT_TRUE_MSG((roi.w * roi.h) < 65536, "The maximum supported resolution for find_apriltags() is < 64K pixels.");
    // #endif
    if ((roi.w < 4) || (roi.h < 4))
    {
        return PyList_New(0);
    }

    apriltag_families_t families = TAG36H11;
    // 2.8mm Focal Length w/ OV7725 sensor for reference.
    float fx = (2.8 / 3.984) * arg_img->w;
    // 2.8mm Focal Length w/ OV7725 sensor for reference.
    float fy = (2.8 / 2.952) * arg_img->h;
    // Use the image versus the roi here since the image should be projected from the camera center.
    float cx = arg_img->w * 0.5;
    // Use the image versus the roi here since the image should be projected from the camera center.
    float cy = arg_img->h * 0.5;

    PyObject *py_roi = Py_None;

    static char *kwlist[] = {"roi", "families", "fx", "fy", "cx", "cy", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|Oiffff", kwlist,
                                     &py_roi, &families, &fx, &fy, &cx, &cy))
        return NULL;
    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    list_t out;
    fb_alloc_mark();
    imlib_find_apriltags(&out, arg_img, &roi, families, fx, fy, cx, cy);
    fb_alloc_free_till_mark();

    PyObject *objects_list = PyList_New(list_size(&out));
    PyObject *o;
    PyObject *ot;
    // PyObject *ott;

    for (size_t i = 0; list_size(&out); i++)
    {
        find_apriltags_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        //     o = PyDict_New();
        //     // o = m_new_obj(py_qrcode_obj_t);
        //     // o->base.type = &py_qrcode_type;
        //     // ot = PyTuple_New(4);
        //     // ott = PyTuple_New(2);

        //     // PyTuple_SetItem(ott, 0, Py_BuildValue("i", lnk_data.corners[0].x));
        //     // PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[0].y));
        //     // PyTuple_SetItem(ot, 0, ott);
        //     // PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[1].x));
        //     // PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[1].y));
        //     // PyTuple_SetItem(ot, 1, ott);
        //     // PyTuple_SetItem(ott, 2, Py_BuildValue("i", lnk_data.corners[2].x));
        //     // PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[2].y));
        //     // PyTuple_SetItem(ot, 2, ott);
        //     // PyTuple_SetItem(ott, 3, Py_BuildValue("i", lnk_data.corners[3].x));
        //     // PyTuple_SetItem(ott, 1, Py_BuildValue("i", lnk_data.corners[3].y));
        //     // PyTuple_SetItem(ot, 3, ott);

        ot = Py_BuildValue("(ii)(ii)(ii)(ii)", lnk_data.corners[0].x, lnk_data.corners[0].y, lnk_data.corners[1].x, lnk_data.corners[1].y, lnk_data.corners[2].x, lnk_data.corners[2].y, lnk_data.corners[3].x, lnk_data.corners[3].y);
        PyDict_SetItem(o, Py_BuildValue("s", "corners"), ot);

        ot = Py_BuildValue("iiii", lnk_data.rect.x, lnk_data.rect.y, lnk_data.rect.w, lnk_data.rect.h);
        //     // PyTuple_New(4);

        //     // PyTuple_SetItem(ot, 0, Py_BuildValue("i", lnk_data.rect.x));
        //     // PyTuple_SetItem(ot, 1, Py_BuildValue("i", lnk_data.rect.y));
        //     // PyTuple_SetItem(ot, 2, Py_BuildValue("i", lnk_data.rect.w));
        //     // PyTuple_SetItem(ot, 3, Py_BuildValue("i", lnk_data.rect.h));

        PyDict_SetItem(o, Py_BuildValue("s", "rect"), ot);

        PyDict_SetItem(o, Py_BuildValue("s", "id"), Py_BuildValue("i", lnk_data.id));

        PyDict_SetItem(o, Py_BuildValue("s", "family"), Py_BuildValue("i", lnk_data.family));

        PyDict_SetItem(o, Py_BuildValue("s", "cx"), Py_BuildValue("i", lnk_data.centroid.x));

        PyDict_SetItem(o, Py_BuildValue("s", "cy"), Py_BuildValue("i", lnk_data.centroid.y));

        PyDict_SetItem(o, Py_BuildValue("s", "rotation"), Py_BuildValue("f", lnk_data.z_rotation));

        PyDict_SetItem(o, Py_BuildValue("s", "decision_margin"), Py_BuildValue("f", lnk_data.decision_margin));

        PyDict_SetItem(o, Py_BuildValue("s", "hamming"), Py_BuildValue("f", lnk_data.hamming));

        PyDict_SetItem(o, Py_BuildValue("s", "goodness"), Py_BuildValue("f", lnk_data.goodness));

        PyDict_SetItem(o, Py_BuildValue("s", "x_translation"), Py_BuildValue("f", lnk_data.x_translation));

        PyDict_SetItem(o, Py_BuildValue("s", "y_translation"), Py_BuildValue("f", lnk_data.y_translation));

        PyDict_SetItem(o, Py_BuildValue("s", "z_translation"), Py_BuildValue("f", lnk_data.z_translation));

        PyDict_SetItem(o, Py_BuildValue("s", "x_rotation"), Py_BuildValue("f", lnk_data.x_rotation));

        PyDict_SetItem(o, Py_BuildValue("s", "y_rotation"), Py_BuildValue("f", lnk_data.y_rotation));

        PyDict_SetItem(o, Py_BuildValue("s", "z_rotation"), Py_BuildValue("f", lnk_data.z_rotation));

        PyList_SetItem(objects_list, i, o);

        //     // objects_list->items[i] = o;
    }

    return objects_list;
}

#endif // IMLIB_ENABLE_APRILTAGS

#ifdef IMLIB_ENABLE_DATAMATRICES
static PyObject *
py_image_find_datamatrices(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;
    int effort = 200;
    PyObject *py_roi = Py_None;

    static char *kwlist[] = {"roi", "effort", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|Oi", kwlist,
                                     &py_roi, &effort))
        return NULL;
    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    list_t out;
    fb_alloc_mark();
    imlib_find_datamatrices(&out, arg_img, &roi, effort);
    fb_alloc_free_till_mark();

    PyObject *objects_list = PyList_New(list_size(&out));
    PyObject *o;
    PyObject *ot;
    PyObject *ott;

    for (size_t i = 0; list_size(&out); i++)
    {
        find_datamatrices_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        o = PyDict_New();
        // o = m_new_obj(py_qrcode_obj_t);
        // o->base.type = &py_qrcode_type;
        // ot = PyTuple_New(4);
        // ott = PyTuple_New(2);

        // PyTuple_SetItem(ott,0,Py_BuildValue("i", lnk_data.corners[0].x));PyTuple_SetItem(ott,1,Py_BuildValue("i", lnk_data.corners[0].y));PyTuple_SetItem(ot,0,ott);
        // PyTuple_SetItem(ott,1,Py_BuildValue("i", lnk_data.corners[1].x));PyTuple_SetItem(ott,1,Py_BuildValue("i", lnk_data.corners[1].y));PyTuple_SetItem(ot,1,ott);
        // PyTuple_SetItem(ott,2,Py_BuildValue("i", lnk_data.corners[2].x));PyTuple_SetItem(ott,1,Py_BuildValue("i", lnk_data.corners[2].y));PyTuple_SetItem(ot,2,ott);
        // PyTuple_SetItem(ott,3,Py_BuildValue("i", lnk_data.corners[3].x));PyTuple_SetItem(ott,1,Py_BuildValue("i", lnk_data.corners[3].y));PyTuple_SetItem(ot,3,ott);

        PyDict_SetItem(o, Py_BuildValue("s", "corners"), Py_BuildValue("((ii)(ii)(ii)(ii))", lnk_data.corners[0].x, lnk_data.corners[0].y, lnk_data.corners[1].x, lnk_data.corners[1].y, lnk_data.corners[2].x, lnk_data.corners[2].y, lnk_data.corners[3].x, lnk_data.corners[3].y));

        // ot = PyTuple_New(4);

        // PyTuple_SetItem(ot,0,Py_BuildValue("i", lnk_data.rect.x));
        // PyTuple_SetItem(ot,1,Py_BuildValue("i", lnk_data.rect.y));
        // PyTuple_SetItem(ot,2,Py_BuildValue("i", lnk_data.rect.w));
        // PyTuple_SetItem(ot,3,Py_BuildValue("i", lnk_data.rect.h));

        PyDict_SetItem(o, Py_BuildValue("s", "rect"), Py_BuildValue("(iiii)", lnk_data.rect.x, lnk_data.rect.y, lnk_data.rect.w, lnk_data.rect.h));

        PyDict_SetItem(o, Py_BuildValue("s", "payload"), Py_BuildValue("s", lnk_data.payload));

        PyDict_SetItem(o, Py_BuildValue("s", "rotation"), Py_BuildValue("f", IM_DEG2RAD(lnk_data.rotation)));

        PyDict_SetItem(o, Py_BuildValue("s", "rows"), Py_BuildValue("i", lnk_data.rows));

        PyDict_SetItem(o, Py_BuildValue("s", "columns"), Py_BuildValue("i", lnk_data.columns));

        PyDict_SetItem(o, Py_BuildValue("s", "capacity"), Py_BuildValue("i", lnk_data.capacity));

        PyDict_SetItem(o, Py_BuildValue("s", "padding"), Py_BuildValue("i", lnk_data.padding));

        PyList_SetItem(objects_list, i, o);
        // objects_list->items[i] = o;
        xfree(lnk_data.payload);
    }

    return objects_list;
}
#endif // IMLIB_ENABLE_DATAMATRICES

#ifdef IMLIB_ENABLE_BARCODES
static PyObject *
py_image_find_barcodes(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;

    PyObject *py_roi = Py_None;

    static char *kwlist[] = {"roi", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|O", kwlist,
                                     &py_roi))
        return NULL;
    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    list_t out;
    fb_alloc_mark();
    imlib_find_barcodes(&out, arg_img, &roi);
    fb_alloc_free_till_mark();

    PyObject *objects_list = PyList_New(list_size(&out));
    PyObject *o;
    // PyObject *ot;
    // PyObject *ott;

    for (size_t i = 0; list_size(&out); i++)
    {
        find_barcodes_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        o = PyDict_New();

        PyDict_SetItem(o, Py_BuildValue("s", "corners"), Py_BuildValue("((ii)(ii)(ii)(ii))", lnk_data.corners[0].x, lnk_data.corners[0].y, lnk_data.corners[1].x, lnk_data.corners[1].y, lnk_data.corners[2].x, lnk_data.corners[2].y, lnk_data.corners[3].x, lnk_data.corners[3].y));
        PyDict_SetItem(o, Py_BuildValue("s", "rect"), Py_BuildValue("(iiii)", lnk_data.rect.x, lnk_data.rect.y, lnk_data.rect.w, lnk_data.rect.h));

        PyDict_SetItem(o, Py_BuildValue("s", "payload"), Py_BuildValue("s", lnk_data.payload));

        PyDict_SetItem(o, Py_BuildValue("s", "type"), Py_BuildValue("i", lnk_data.type));

        PyDict_SetItem(o, Py_BuildValue("s", "rotation"), Py_BuildValue("f", IM_DEG2RAD(lnk_data.rotation)));

        PyDict_SetItem(o, Py_BuildValue("s", "quality"), Py_BuildValue("i", lnk_data.quality));

        PyList_SetItem(objects_list, i, o);

        // objects_list->items[i] = o;
        xfree(lnk_data.payload);
    }

    return objects_list;
}
#endif // IMLIB_ENABLE_BARCODES
// #define IMLIB_ENABLE_FIND_DISPLACEMENT

#ifdef IMLIB_ENABLE_FIND_DISPLACEMENT
static PyObject *
py_image_find_displacement(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    image_t *arg_template_img = py_helper_arg_to_image_mutable(args[1]);

    rectangle_t roi;

    rectangle_t template_roi;
    // py_helper_keyword_rectangle(arg_template_img, n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_template_roi), &template_roi);

    // PY_ASSERT_FALSE_MSG((roi.w != template_roi.w) || (roi.h != template_roi.h), "ROI(w,h) != TEMPLATE_ROI(w,h)");

    bool logpolar = 0;
    bool fix_rotation_scale = 0;

    PyObject *py_template;
    PyObject *py_roi = Py_None;
    PyObject *py_template_roi;

    static char *kwlist[] = {"template", "roi", "template_roi", "logpolar", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|OOOi", kwlist,
                                     &py_roi, &effort))
        return NULL;
    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    float x, y, r, s, response;
    fb_alloc_mark();
    imlib_phasecorrelate(arg_img, arg_template_img, &roi, &template_roi, logpolar, fix_rotation_scale, &x, &y, &r, &s, &response);
    fb_alloc_free_till_mark();

    py_displacement_obj_t *o = m_new_obj(py_displacement_obj_t);
    o->base.type = &py_displacement_type;
    o->x_translation = mp_obj_new_float(x);
    o->y_translation = mp_obj_new_float(y);
    o->rotation = mp_obj_new_float(r);
    o->scale = mp_obj_new_float(s);
    o->response = mp_obj_new_float(response);

    return o;
}

#endif // IMLIB_ENABLE_FIND_DISPLACEMENT

#ifdef IMLIB_ENABLE_DESCRIPTOR

static PyObject *
py_image_match_descriptor(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *match_obj = Py_None;
    PyObject *py_descritor0;
    PyObject *py_descriptor1;
    // PY_ASSERT_TRUE_MSG((desc1_type == desc2_type), "Descriptors have different types!");

    if (desc1_type == &py_lbp_type)
    {
        py_lbp_obj_t *lbp1 = ((py_lbp_obj_t *)args[0]);
        py_lbp_obj_t *lbp2 = ((py_lbp_obj_t *)args[1]);

        // Sanity checks
        PY_ASSERT_TYPE(lbp1, &py_lbp_type);
        PY_ASSERT_TYPE(lbp2, &py_lbp_type);

        // Match descriptors
        match_obj = mp_obj_new_int(imlib_lbp_desc_distance(lbp1->hist, lbp2->hist));
    }
    else if (desc1_type == &py_kp_type)
    {
        py_kp_obj_t *kpts1 = ((py_kp_obj_t *)args[0]);
        py_kp_obj_t *kpts2 = ((py_kp_obj_t *)args[1]);
        int threshold = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold), 85);
        int filter_outliers = py_helper_keyword_int(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_filter_outliers), false);

        // Sanity checks
        PY_ASSERT_TYPE(kpts1, &py_kp_type);
        PY_ASSERT_TYPE(kpts2, &py_kp_type);
        PY_ASSERT_TRUE_MSG((threshold >= 0 && threshold <= 100), "Expected threshold between 0 and 100");

        int theta = 0;       // Estimated angle of rotation
        int count = 0;       // Number of matches
        point_t c = {0};     // Centroid
        rectangle_t r = {0}; // Bounding rectangle
        // List of matching keypoints indices
        mp_obj_t match_list = mp_obj_new_list(0, NULL);

        if (array_length(kpts1->kpts) && array_length(kpts1->kpts))
        {
            int *match = fb_alloc(array_length(kpts1->kpts) * sizeof(int) * 2);

            // Match the two keypoint sets
            count = orb_match_keypoints(kpts1->kpts, kpts2->kpts, match, threshold, &r, &c, &theta);

            // Add matching keypoints to Python list.
            for (int i = 0; i < count * 2; i += 2)
            {
                mp_obj_t index_obj[2] = {
                    mp_obj_new_int(match[i + 0]),
                    mp_obj_new_int(match[i + 1]),
                };
                mp_obj_list_append(match_list, mp_obj_new_tuple(2, index_obj));
            }

            // Free match list
            fb_free();

            if (filter_outliers == true)
            {
                count = orb_filter_keypoints(kpts2->kpts, &r, &c);
            }
        }

        py_kptmatch_obj_t *o = m_new_obj(py_kptmatch_obj_t);
        o->base.type = &py_kptmatch_type;
        o->cx = mp_obj_new_int(c.x);
        o->cy = mp_obj_new_int(c.y);
        o->x = mp_obj_new_int(r.x);
        o->y = mp_obj_new_int(r.y);
        o->w = mp_obj_new_int(r.w);
        o->h = mp_obj_new_int(r.h);
        o->count = mp_obj_new_int(count);
        o->theta = mp_obj_new_int(theta);
        o->match = match_list;
        match_obj = o;
    }
    else
    {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "Descriptor type is not supported"));
    }

    return match_obj;
}

// static PyObject *
// py_image_match_descriptor(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     PyObject *match_obj = Py_None;
//     PyObject *py_descritor0;
//     PyObject *py_descriptor1;

//     // const mp_obj_type_t *desc1_type;
//     // const mp_obj_type_t *desc2_type;

//     // PY_ASSERT_TRUE_MSG((desc1_type == desc2_type), "Descriptors have different types!");

//     if (0)
//     {
// #if defined(IMLIB_ENABLE_FIND_LBP)
//     }
//     else if (desc1_type == &py_lbp_type)
//     {
//         py_lbp_obj_t *lbp1 = ((py_lbp_obj_t *)args[0]);
//         py_lbp_obj_t *lbp2 = ((py_lbp_obj_t *)args[1]);

//         // Sanity checks
//         PY_ASSERT_TYPE(lbp1, &py_lbp_type);
//         PY_ASSERT_TYPE(lbp2, &py_lbp_type);

//         // Match descriptors
//         match_obj = mp_obj_new_int(imlib_lbp_desc_distance(lbp1->hist, lbp2->hist));
// #endif //IMLIB_ENABLE_FIND_LBP
// #if defined(IMLIB_ENABLE_FIND_KEYPOINTS)
//     }
//     else if (desc1_type == &py_kp_type)
//     {
//         py_kp_obj_t *kpts1 = ((py_kp_obj_t *)args[0]);
//         py_kp_obj_t *kpts2 = ((py_kp_obj_t *)args[1]);
//         int threshold = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold), 85);
//         int filter_outliers = py_helper_keyword_int(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_filter_outliers), false);

//         // Sanity checks
//         PY_ASSERT_TYPE(kpts1, &py_kp_type);
//         PY_ASSERT_TYPE(kpts2, &py_kp_type);
//         PY_ASSERT_TRUE_MSG((threshold >= 0 && threshold <= 100), "Expected threshold between 0 and 100");

//         int theta = 0;       // Estimated angle of rotation
//         int count = 0;       // Number of matches
//         point_t c = {0};     // Centroid
//         rectangle_t r = {0}; // Bounding rectangle
//         // List of matching keypoints indices
//         mp_obj_t match_list = mp_obj_new_list(0, NULL);

//         if (array_length(kpts1->kpts) && array_length(kpts1->kpts))
//         {
//             fb_alloc_mark();
//             int *match = fb_alloc(array_length(kpts1->kpts) * sizeof(int) * 2, FB_ALLOC_NO_HINT);

//             // Match the two keypoint sets
//             count = orb_match_keypoints(kpts1->kpts, kpts2->kpts, match, threshold, &r, &c, &theta);

//             // Add matching keypoints to Python list.
//             for (int i = 0; i < count * 2; i += 2)
//             {
//                 mp_obj_t index_obj[2] = {
//                     mp_obj_new_int(match[i + 0]),
//                     mp_obj_new_int(match[i + 1]),
//                 };
//                 mp_obj_list_append(match_list, mp_obj_new_tuple(2, index_obj));
//             }

//             // Free match list
//             fb_alloc_free_till_mark();

//             if (filter_outliers == true)
//             {
//                 count = orb_filter_keypoints(kpts2->kpts, &r, &c);
//             }
//         }

//         py_kptmatch_obj_t *o = m_new_obj(py_kptmatch_obj_t);
//         o->base.type = &py_kptmatch_type;
//         o->cx = mp_obj_new_int(c.x);
//         o->cy = mp_obj_new_int(c.y);
//         o->x = mp_obj_new_int(r.x);
//         o->y = mp_obj_new_int(r.y);
//         o->w = mp_obj_new_int(r.w);
//         o->h = mp_obj_new_int(r.h);
//         o->count = mp_obj_new_int(count);
//         o->theta = mp_obj_new_int(theta);
//         o->match = match_list;
//         match_obj = o;
// #endif //IMLIB_ENABLE_FIND_KEYPOINTS
//     }
//     else
//     {
//         mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("Descriptor type is not supported"));
//     }

//     return match_obj;
// }

#endif //IMLIB_ENABLE_DESCRIPTOR

////////////////
// Basic Methods
////////////////

static PyObject *
py_image_width(PyObject *self, PyObject *args)
{
    return Py_BuildValue("i", self_image_img.w);
}

static PyObject *
py_image_height(PyObject *self, PyObject *args, PyObject *keywds)
{
    return Py_BuildValue("i", self_image_img.h);
}

// IMAGE_BPP_BINARY,       // BPP = 0
// IMAGE_BPP_GRAYSCALE,    // BPP = 1
// IMAGE_BPP_RGB565,       // BPP = 2
// IMAGE_BPP_BAYER,        // BPP = 3
// IMAGE_BPP_JPEG,         // BPP = 4

static PyObject *
py_image_format(PyObject *self, PyObject *args)
{
    switch (self_image_img.bpp)
    {
    case IMAGE_BPP_BINARY:
        return Py_BuildValue("i", IMAGE_BPP_BINARY);
    case IMAGE_BPP_GRAYSCALE:
        return Py_BuildValue("i", IMAGE_BPP_GRAYSCALE);
    case IMAGE_BPP_RGB565:
        return Py_BuildValue("i", IMAGE_BPP_RGB565);
    case IMAGE_BPP_BAYER:
        return Py_BuildValue("i", IMAGE_BPP_BAYER);
    default:
        return Py_BuildValue("i", IMAGE_BPP_JPEG);
    }
}

static PyObject *
py_image_size(PyObject *self, PyObject *args)
{
    return Py_BuildValue("ii", self_image_img.w, self_image_img.h);
}

static PyObject *
py_image_get_pixel(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    PyObject *py_arg_x;
    PyObject *py_arg_y;
    PyObject *py_rgbtuple;

    int arg_x = 0;
    int arg_y = 0;

    bool arg_rgbtuple = 0;
    static char *kwlist[] = {"x", "y", "rgbtuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|OO", kwlist,
                                     &py_arg_x, &py_arg_y, &py_rgbtuple))
        return NULL;

    // if ((!IM_X_INSIDE(arg_img, arg_x)) || (!IM_Y_INSIDE(arg_img, arg_y))) {
    //     return mp_const_none;
    // }
    if (PyTuple_Check(py_arg_x))
    {
        arg_x = PyLong_AsLong(PyTuple_GetItem(py_arg_x, 0));
        arg_y = PyLong_AsLong(PyTuple_GetItem(py_arg_x, 1));
    }
    else
    {
        arg_x = PyLong_AsLong(py_arg_x);
        arg_y = PyLong_AsLong(py_arg_y);
    }
    if (py_rgbtuple == Py_True)
        arg_rgbtuple = 1;
    else
        arg_rgbtuple = 0;
    if ((!IM_X_INSIDE(arg_img, arg_x)) || (!IM_Y_INSIDE(arg_img, arg_y)))
    {
        return Py_None;
    }

    switch (arg_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        if (arg_rgbtuple)
        {
            int pixel = IMAGE_GET_BINARY_PIXEL(arg_img, arg_x, arg_y);
            return Py_BuildValue("iii", COLOR_RGB565_TO_R8(COLOR_BINARY_TO_RGB565(pixel)), COLOR_RGB565_TO_G8(COLOR_BINARY_TO_RGB565(pixel)), COLOR_RGB565_TO_B8(COLOR_BINARY_TO_RGB565(pixel)));
            // mp_obj_t pixel_tuple[3];
            // pixel_tuple[0] = mp_obj_new_int(COLOR_RGB565_TO_R8(COLOR_BINARY_TO_RGB565(pixel)));
            // pixel_tuple[1] = mp_obj_new_int(COLOR_RGB565_TO_G8(COLOR_BINARY_TO_RGB565(pixel)));
            // pixel_tuple[2] = mp_obj_new_int(COLOR_RGB565_TO_B8(COLOR_BINARY_TO_RGB565(pixel)));
            // return mp_obj_new_tuple(3, pixel_tuple);
        }
        else
        {
            return Py_BuildValue("i", IMAGE_GET_BINARY_PIXEL(arg_img, arg_x, arg_y));
            // return mp_obj_new_int(IMAGE_GET_BINARY_PIXEL(arg_img, arg_x, arg_y));
        }
    }
    case IMAGE_BPP_GRAYSCALE:
    {
        if (arg_rgbtuple)
        {
            int pixel = IMAGE_GET_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y);
            return Py_BuildValue("iii", COLOR_RGB565_TO_R8(COLOR_GRAYSCALE_TO_RGB565(pixel)), COLOR_RGB565_TO_G8(COLOR_GRAYSCALE_TO_RGB565(pixel)), COLOR_RGB565_TO_B8(COLOR_GRAYSCALE_TO_RGB565(pixel)));
            // mp_obj_t pixel_tuple[3];
            // pixel_tuple[0] = mp_obj_new_int(COLOR_RGB565_TO_R8(COLOR_GRAYSCALE_TO_RGB565(pixel)));
            // pixel_tuple[1] = mp_obj_new_int(COLOR_RGB565_TO_G8(COLOR_GRAYSCALE_TO_RGB565(pixel)));
            // pixel_tuple[2] = mp_obj_new_int(COLOR_RGB565_TO_B8(COLOR_GRAYSCALE_TO_RGB565(pixel)));
            // return mp_obj_new_tuple(3, pixel_tuple);
        }
        else
        {
            return Py_BuildValue("i", IMAGE_GET_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y));
            // return mp_obj_new_int(IMAGE_GET_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y));
        }
    }
    case IMAGE_BPP_RGB565:
    {
        if (arg_rgbtuple)
        {
            int pixel = IMAGE_GET_RGB565_PIXEL(arg_img, arg_x, arg_y);
            return Py_BuildValue("iii", COLOR_R5_TO_R8(COLOR_RGB565_TO_R5(pixel)), COLOR_G6_TO_G8(COLOR_RGB565_TO_G6(pixel)), COLOR_B5_TO_B8(COLOR_RGB565_TO_B5(pixel)));
            // mp_obj_t pixel_tuple[3];
            // pixel_tuple[0] = mp_obj_new_int(COLOR_R5_TO_R8(COLOR_RGB565_TO_R5(pixel)));
            // pixel_tuple[1] = mp_obj_new_int(COLOR_G6_TO_G8(COLOR_RGB565_TO_G6(pixel)));
            // pixel_tuple[2] = mp_obj_new_int(COLOR_B5_TO_B8(COLOR_RGB565_TO_B5(pixel)));
            // return mp_obj_new_tuple(3, pixel_tuple);
        }
        else
        {
            return Py_BuildValue("i", IMAGE_GET_RGB565_PIXEL(arg_img, arg_x, arg_y));
            // return mp_obj_new_int(IMAGE_GET_RGB565_PIXEL(arg_img, arg_x, arg_y));
        }
    }
    case IMAGE_BPP_BAYER:
        if (arg_rgbtuple)
        {
            int pixel = IMAGE_GET_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y); // Correct!
            return Py_BuildValue("iii", COLOR_RGB565_TO_R8(COLOR_GRAYSCALE_TO_RGB565(pixel)), COLOR_RGB565_TO_G8(COLOR_GRAYSCALE_TO_RGB565(pixel)), COLOR_RGB565_TO_B8(COLOR_GRAYSCALE_TO_RGB565(pixel)));
            // mp_obj_t pixel_tuple[3];
            // pixel_tuple[0] = mp_obj_new_int(COLOR_RGB565_TO_R8(COLOR_GRAYSCALE_TO_RGB565(pixel)));
            // pixel_tuple[1] = mp_obj_new_int(COLOR_RGB565_TO_G8(COLOR_GRAYSCALE_TO_RGB565(pixel)));
            // pixel_tuple[2] = mp_obj_new_int(COLOR_RGB565_TO_B8(COLOR_GRAYSCALE_TO_RGB565(pixel)));
            // return mp_obj_new_tuple(3, pixel_tuple);
        }
        else
        {
            return Py_BuildValue("i", IMAGE_GET_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y)); // Correct!
            // return mp_obj_new_int(IMAGE_GET_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y)); // Correct!
        }
    default:
        return Py_None;
    }
}

static PyObject *
py_image_set_pixel(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    int arg_x = 0;
    int arg_y = 0;
    int arg_c = 0xffffff;

    static char *kwlist[] = {"x", "y", "c", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iii", kwlist,
                                     &arg_x, &arg_y, &arg_c))
        return NULL;

    if ((!IM_X_INSIDE(arg_img, arg_x)) || (!IM_Y_INSIDE(arg_img, arg_y)))
    {
        return Py_None;
    }

    switch (arg_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        IMAGE_PUT_BINARY_PIXEL(arg_img, arg_x, arg_y, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)));
        return Py_None;
    }
    case IMAGE_BPP_GRAYSCALE:
    {
        IMAGE_PUT_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)));
        return Py_None;
    }
    case IMAGE_BPP_RGB565:
    {
        IMAGE_PUT_RGB565_PIXEL(arg_img, arg_x, arg_y, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)));
        return Py_None;
    }
    case IMAGE_BPP_BAYER:
    {
        IMAGE_PUT_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff))); // Correct!
        return Py_None;
    }
    default:
        return Py_None;
    }
}

#include "framebuffer.h"
#include "omv_boardconfig.h"
// extern framebuffer_t *fb_framebuffer;
// #define MAIN_FB()           fb_framebuffer

bool is_img_data_in_main_fb(uint8_t *data)
{
#if CONFIG_MAIXPY_OMV_DOUBLE_BUFF
    uint8_t buff_size = SENSOR_BUFFER_NUM;
    if (sensor.double_buff)
        buff_size = 2;
    for (uint8_t i = 0; i < buff_size; ++i)
    {
        if ((MAIN_FB()->pixels[i] != NULL) &&
            (data >= MAIN_FB()->pixels[i]) &&
            (data < (MAIN_FB()->pixels[i] + MAIN_FB()->w_max * MAIN_FB()->h_max * OMV_INIT_BPP)))
        {
            return true;
        }
    }
#else
    if ((MAIN_FB()->pixels != NULL) &&
        (data >= MAIN_FB()->pixels) &&
        (data < (MAIN_FB()->pixels + MAIN_FB()->w_max * MAIN_FB()->h_max * OMV_INIT_BPP)))
    {
        return true;
    }
#endif
    return false;
}

static PyObject *
py_image_mean_pool(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    int x_div;
    // PY_ASSERT_TRUE_MSG(x_div >= 1, "Width divisor must be greater than >= 1");
    // PY_ASSERT_TRUE_MSG(x_div <= arg_img->w, "Width divisor must be less than <= img width");
    int y_div;
    // PY_ASSERT_TRUE_MSG(y_div >= 1, "Height divisor must be greater than >= 1");
    // PY_ASSERT_TRUE_MSG(y_div <= arg_img->h, "Height divisor must be less than <= img height");

    static char *kwlist[] = {"x_div", "y_div", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ii", kwlist,
                                     &x_div, &y_div))
        return NULL;
    image_t out_img;
    out_img.w = arg_img->w / x_div;
    out_img.h = arg_img->h / y_div;
    out_img.bpp = arg_img->bpp;
    out_img.pixels = arg_img->pixels;

    imlib_mean_pool(arg_img, &out_img, x_div, y_div);
    arg_img->w = out_img.w;
    arg_img->h = out_img.h;

    if (is_img_data_in_main_fb(arg_img->data))
    {
        MAIN_FB()->w = out_img.w;
        MAIN_FB()->h = out_img.h;
    }
    // PyObject *tmp = back_img(&out_img);

    return back_img(&out_img);
}

static PyObject *
py_image_mean_pooled(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    int x_div;
    // PY_ASSERT_TRUE_MSG(x_div >= 1, "Width divisor must be greater than >= 1");
    // PY_ASSERT_TRUE_MSG(x_div <= arg_img->w, "Width divisor must be less than <= img width");
    int y_div;
    // PY_ASSERT_TRUE_MSG(y_div >= 1, "Height divisor must be greater than >= 1");
    // PY_ASSERT_TRUE_MSG(y_div <= arg_img->h, "Height divisor must be less than <= img height");

    static char *kwlist[] = {"x_div", "y_div", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ii", kwlist,
                                     &x_div, &y_div))
        return NULL;
    image_t out_img;
    out_img.w = arg_img->w / x_div;
    out_img.h = arg_img->h / y_div;
    out_img.bpp = arg_img->bpp;
    out_img.pixels = xalloc(image_size(&out_img));

    imlib_mean_pool(arg_img, &out_img, x_div, y_div);
    PyObject *tmp = back_img(&out_img);
    xfree(out_img.pixels);
    return tmp;
    // return py_image_from_struct(&out_img);
}

static PyObject *
py_image_midpoint_pool(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    int x_div;
    // PY_ASSERT_TRUE_MSG(x_div >= 1, "Width divisor must be greater than >= 1");
    // PY_ASSERT_TRUE_MSG(x_div <= arg_img->w, "Width divisor must be less than <= img width");
    int y_div;
    // PY_ASSERT_TRUE_MSG(y_div >= 1, "Height divisor must be greater than >= 1");
    // PY_ASSERT_TRUE_MSG(y_div <= arg_img->h, "Height divisor must be less than <= img height");

    float bias_s = 0.5;

    static char *kwlist[] = {"x_div", "y_div", "bias", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ii|f", kwlist,
                                     &x_div, &y_div, bias_s))
        return NULL;
    int bias = IM_MAX(IM_MIN(bias_s * 256, 256), 0);

    image_t out_img;
    out_img.w = arg_img->w / x_div;
    out_img.h = arg_img->h / y_div;
    out_img.bpp = arg_img->bpp;
    out_img.pixels = arg_img->pixels;

    imlib_midpoint_pool(arg_img, &out_img, x_div, y_div, bias);
    arg_img->w = out_img.w;
    arg_img->h = out_img.h;
    if (is_img_data_in_main_fb(arg_img->data))
    {
        MAIN_FB()->w = out_img.w;
        MAIN_FB()->h = out_img.h;
    }

    return back_img(&out_img);
}

static PyObject *
py_image_midpoint_pooled(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    int x_div;
    // PY_ASSERT_TRUE_MSG(x_div >= 1, "Width divisor must be greater than >= 1");
    // PY_ASSERT_TRUE_MSG(x_div <= arg_img->w, "Width divisor must be less than <= img width");
    int y_div;
    // PY_ASSERT_TRUE_MSG(y_div >= 1, "Height divisor must be greater than >= 1");
    // PY_ASSERT_TRUE_MSG(y_div <= arg_img->h, "Height divisor must be less than <= img height");
    float bias_s = 0.5;

    static char *kwlist[] = {"x_div", "y_div", "bias", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ii|f", kwlist,
                                     &x_div, &y_div, bias_s))
        return NULL;
    int bias = IM_MAX(IM_MIN(bias_s * 256, 256), 0);

    image_t out_img;
    out_img.w = arg_img->w / x_div;
    out_img.h = arg_img->h / y_div;
    out_img.bpp = arg_img->bpp;
    out_img.pixels = xalloc(image_size(&out_img));

    imlib_midpoint_pool(arg_img, &out_img, x_div, y_div, bias);
    PyObject *tmp = back_img(&out_img);
    xfree(out_img.pixels);
    return tmp;
}

static PyObject *
py_image_to_grayscale(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    bool copy = false;

    static char *kwlist[] = {"copy", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist,
                                     &copy))
        return NULL;
    image_t out;
    out.w = arg_img->w;
    out.h = arg_img->h;
    out.bpp = IMAGE_BPP_GRAYSCALE;
    out.data = copy ? xalloc(image_size(&out)) : arg_img->data;
    out.pix_ai = copy ? xalloc(out.w * out.h * 3) : arg_img->pix_ai;

    switch (arg_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        // PY_ASSERT_TRUE_MSG((out.w == 1) || copy,
        //                    "Can't convert to grayscale in place!");
        for (int y = 0, yy = out.h; y < yy; y++)
        {
            uint32_t *row_ptr = IMAGE_COMPUTE_BINARY_PIXEL_ROW_PTR(arg_img, y);
            uint8_t *out_row_ptr = IMAGE_COMPUTE_GRAYSCALE_PIXEL_ROW_PTR(&out, y);
            for (int x = 0, xx = out.w; x < xx; x++)
            {
                IMAGE_PUT_GRAYSCALE_PIXEL_FAST(out_row_ptr, x,
                                               COLOR_BINARY_TO_GRAYSCALE(IMAGE_GET_BINARY_PIXEL_FAST(row_ptr, x)));
            }
        }
        break;
    }
    case IMAGE_BPP_GRAYSCALE:
    {
        if (copy)
            memcpy(out.data, arg_img->data, image_size(&out));
        break;
    }
    case IMAGE_BPP_RGB565:
    {
        for (int y = 0, yy = out.h; y < yy; y++)
        {
            uint16_t *row_ptr = IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(arg_img, y);
            uint8_t *out_row_ptr = IMAGE_COMPUTE_GRAYSCALE_PIXEL_ROW_PTR(&out, y);
            for (int x = 0, xx = out.w; x < xx; x++)
            {
                IMAGE_PUT_GRAYSCALE_PIXEL_FAST(out_row_ptr, x,
                                               COLOR_RGB565_TO_GRAYSCALE(IMAGE_GET_RGB565_PIXEL_FAST(row_ptr, x)));
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }

    if (!copy)
    {
        arg_img->bpp = out.bpp;
        if (is_img_data_in_main_fb(out.data))
        {
            MAIN_FB()->bpp = out.bpp;
        }
    }

    PyObject *tmp = back_img(&out);
    xfree(out.pixels);
    return tmp;
}

static PyObject *
py_image_to_rgb565(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    bool copy = false;

    static char *kwlist[] = {"copy", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist,
                                     &copy))
        return NULL;
    image_t out;
    out.w = arg_img->w;
    out.h = arg_img->h;
    out.bpp = IMAGE_BPP_RGB565;
    out.data = copy ? xalloc(image_size(&out)) : arg_img->data;
    out.pix_ai = copy ? xalloc(out.w * out.h * 3) : arg_img->pix_ai;

    switch (arg_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        // PY_ASSERT_TRUE_MSG((out.w == 1) || copy,
        //     "Can't convert to grayscale in place!");
        for (int y = 0, yy = out.h; y < yy; y++)
        {
            uint32_t *row_ptr = IMAGE_COMPUTE_BINARY_PIXEL_ROW_PTR(arg_img, y);
            uint16_t *out_row_ptr = IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(&out, y);
            for (int x = 0, xx = out.w; x < xx; x++)
            {
                IMAGE_PUT_RGB565_PIXEL_FAST(out_row_ptr, x,
                                            imlib_yuv_to_rgb(IMAGE_GET_BINARY_PIXEL_FAST(row_ptr, x) * COLOR_GRAYSCALE_MAX, 0, 0));
            }
        }
        break;
    }
    case IMAGE_BPP_GRAYSCALE:
    {
        // PY_ASSERT_TRUE_MSG(copy,
        //     "Can't convert to rgb565 in place!");
        for (int y = 0, yy = out.h; y < yy; y++)
        {
            uint8_t *row_ptr = IMAGE_COMPUTE_GRAYSCALE_PIXEL_ROW_PTR(arg_img, y);
            uint16_t *out_row_ptr = IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(&out, y);
            for (int x = 0, xx = out.w; x < xx; x++)
            {
                IMAGE_PUT_RGB565_PIXEL_FAST(out_row_ptr, x,
                                            imlib_yuv_to_rgb(IMAGE_GET_GRAYSCALE_PIXEL_FAST(row_ptr, x), 0, 0));
            }
        }
        break;
    }
    case IMAGE_BPP_RGB565:
    {
        if (copy)
            memcpy(out.data, arg_img->data, image_size(&out));
        break;
    }
    default:
    {
        break;
    }
    }

    if ((!copy) && is_img_data_in_main_fb(out.data))
    {
        MAIN_FB()->bpp = out.bpp;
    }

    PyObject *tmp = back_img(&out);
    xfree(out.pixels);
    return tmp;
}
extern const uint16_t rainbow_table[256];
static PyObject *
py_image_to_rainbow(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    bool copy = false;

    static char *kwlist[] = {"copy", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist,
                                     &copy))
        return NULL;
    image_t out;
    out.w = arg_img->w;
    out.h = arg_img->h;
    out.bpp = IMAGE_BPP_RGB565;
    out.data = copy ? xalloc(image_size(&out)) : arg_img->data;
    out.pix_ai = copy ? xalloc(out.w * out.h * 3) : arg_img->pix_ai;

    switch (arg_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        // PY_ASSERT_TRUE_MSG((out.w == 1) || copy,
        //     "Can't convert to rainbow in place!");
        for (int y = 0, yy = out.h; y < yy; y++)
        {
            uint32_t *row_ptr = IMAGE_COMPUTE_BINARY_PIXEL_ROW_PTR(arg_img, y);
            uint16_t *out_row_ptr = IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(&out, y);
            for (int x = 0, xx = out.w; x < xx; x++)
            {
                IMAGE_PUT_RGB565_PIXEL_FAST(out_row_ptr, x,
                                            rainbow_table[IMAGE_GET_BINARY_PIXEL_FAST(row_ptr, x) * COLOR_GRAYSCALE_MAX]);
            }
        }
        break;
    }
    case IMAGE_BPP_GRAYSCALE:
    {
        // PY_ASSERT_TRUE_MSG(copy,
        //     "Can't convert to rainbow in place!");
        for (int y = 0, yy = out.h; y < yy; y++)
        {
            uint8_t *row_ptr = IMAGE_COMPUTE_GRAYSCALE_PIXEL_ROW_PTR(arg_img, y);
            uint16_t *out_row_ptr = IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(&out, y);
            for (int x = 0, xx = out.w; x < xx; x++)
            {
                IMAGE_PUT_RGB565_PIXEL_FAST(out_row_ptr, x,
                                            rainbow_table[IMAGE_GET_GRAYSCALE_PIXEL_FAST(row_ptr, x)]);
            }
        }
        break;
    }
    case IMAGE_BPP_RGB565:
    {
        for (int y = 0, yy = out.h; y < yy; y++)
        {
            uint16_t *row_ptr = IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(arg_img, y);
            uint16_t *out_row_ptr = IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(&out, y);
            for (int x = 0, xx = out.w; x < xx; x++)
            {
                IMAGE_PUT_RGB565_PIXEL_FAST(out_row_ptr, x,
                                            rainbow_table[COLOR_RGB565_TO_GRAYSCALE(IMAGE_GET_RGB565_PIXEL_FAST(row_ptr, x))]);
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }

    if ((!copy) && is_img_data_in_main_fb(out.data))
    {
        MAIN_FB()->bpp = out.bpp;
    }

    PyObject *tmp = back_img(&out);
    xfree(out.pixels);
    return tmp;
}

// static PyObject *
// py_image_compress(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     image_t *arg_img = &self_image_img;
//     int arg_q =50;
//     // PY_ASSERT_TRUE_MSG((1 <= arg_q) && (arg_q <= 100), "Error: 1 <= quality <= 100!");

//     static char *kwlist[] = {"quality",NULL};
//     if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist,
//                                      &arg_q))

//     uint64_t size;
//     fb_alloc_mark();
//     uint8_t *buffer = fb_alloc_all(&size);
//     image_t out = { .w=arg_img->w, .h=arg_img->h, .bpp=size, .data=buffer };
//     jpeg_compress(arg_img, &out, arg_q, false)
//     // PY_ASSERT_FALSE_MSG(jpeg_compress(arg_img, &out, arg_q, false), "Out of Memory!");
//     // PY_ASSERT_TRUE_MSG(out.bpp <= image_size(arg_img), "Can't compress in place!");
//     memcpy(arg_img->data, out.data, out.bpp);
//     arg_img->bpp = out.bpp;
//     fb_free();
//     fb_alloc_free_till_mark();

//     if (is_img_data_in_main_fb(arg_img->data)) {
//         MAIN_FB()->bpp = arg_img->bpp;
//     }

//     return Py_BuildValue("i",0);
// }

// static PyObject *
// py_image_compressed(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     image_t *arg_img = &self_image_img;
//     int arg_q = 50;
//     // PY_ASSERT_TRUE_MSG((1 <= arg_q) && (arg_q <= 100), "Error: 1 <= quality <= 100!");

//     static char *kwlist[] = {"quality",NULL};
//     if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist,
//                                      &arg_q))

//     uint64_t size;
//     fb_alloc_mark();
//     uint8_t *buffer = fb_alloc_all(&size);
//     image_t out = { .w=arg_img->w, .h=arg_img->h, .bpp=size, .data=buffer };
//     // PY_ASSERT_FALSE_MSG(jpeg_compress(arg_img, &out, arg_q, false), "Out of Memory!");
//     jpeg_compress(arg_img, &out, arg_q, false)
//     uint8_t *temp = xalloc(out.bpp);
//     memcpy(temp, out.data, out.bpp);
//     out.data = temp;
//     fb_free();
//     fb_alloc_free_till_mark();

//     PyObject *tmp = back_img(&out);
//     xfree(out.pixels);
//     return tmp;
// }

static PyObject *
py_image_copy(PyObject *self, PyObject *args, PyObject *keywds)
{
    // mode == false -> copy behavior
    // mode == true -> crop/scale behavior
    image_t *arg_img = &self_image_img;

    rectangle_t roi;
    PyObject *py_roi = Py_None;
    
    // py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    float arg_x_scale = 1.0f;
    // py_helper_keyword_float(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_scale), 1.0f);
    // PY_ASSERT_TRUE_MSG((0.0f <= arg_x_scale), "Error: 0.0 <= x_scale!");

    float arg_y_scale = 1.0f;
    //     py_helper_keyword_float(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_scale), 1.0f);
    // PY_ASSERT_TRUE_MSG((0.0f <= arg_y_scale), "Error: 0.0 <= y_scale!");

    // mp_obj_t copy_to_fb_obj = py_helper_keyword_object(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(mode ? MP_QSTR_copy : MP_QSTR_copy_to_fb));

    // bool copy_to_fb = false;

    static char *kwlist[] = {"roi", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist,
                                     &py_roi))
        return NULL;
    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

        image_t *arg_other = arg_img;

    // if (copy_to_fb_obj)
    // {
    //     if (mp_obj_is_integer(copy_to_fb_obj))
    //     {
    //         if (!mode)
    //         {
    //             copy_to_fb = mp_obj_get_int(copy_to_fb_obj);
    //         }
    //         else if (mp_obj_get_int(copy_to_fb_obj))
    //         {
    //             arg_other = NULL;
    //         }
    //     }
    //     else
    //     {
    //         arg_other = py_helper_arg_to_image_mutable(copy_to_fb_obj);
    //     }
    // }

    // if (copy_to_fb) {
    //     // fb_update_jpeg_buffer();     #暂时不做处理
    // }

    image_t image;
    image.w = fast_floorf(roi.w * arg_x_scale);
    // PY_ASSERT_TRUE_MSG(image.w >= 1, "Output image width is 0!");
    image.h = fast_floorf(roi.h * arg_y_scale);
    // PY_ASSERT_TRUE_MSG(image.h >= 1, "Output image height is 0!");
    image.bpp = arg_img->bpp;
    image.data = NULL;

    // if (copy_to_fb)
    // {
    //     // MAIN_FB()->w = 0;
    //     // MAIN_FB()->h = 0;
    //     // MAIN_FB()->bpp = 0;
    //     // // PY_ASSERT_TRUE_MSG((image_size(&image) <= fb_avail()), "The new image won't fit in the main frame buffer!");
    //     // MAIN_FB()->w = image.w;
    //     // MAIN_FB()->h = image.h;
    //     // MAIN_FB()->bpp = image.bpp;
    //     // image.data = MAIN_FB()->pixels;
    // }
    // else
    if (arg_other)
    {
        // PY_ASSERT_TRUE_MSG((image_size(&image) <= image_size(arg_other)), "The new image won't fit in the target frame buffer!");
        image.data = arg_other->data;
    }
    else
    {
        image.data = xalloc(image_size(&image));
    }

    bool in_place = arg_img->data == image.data;
    image_t temp;

    if (in_place)
    {
        memcpy(&temp, arg_img, sizeof(image_t));
        fb_alloc_mark();
        temp.data = fb_alloc(image_size(&temp));
        memcpy(temp.data, arg_img->data, image_size(&temp));
        arg_img = &temp;
        // if (copy_to_fb)
        // {
        //     MAIN_FB()->w = 0;
        //     MAIN_FB()->h = 0;
        //     MAIN_FB()->bpp = 0;
        //     // PY_ASSERT_TRUE_MSG((image_size(&image) <= fb_avail()), "The new image won't fit in the main frame buffer!");
        //     MAIN_FB()->w = image.w;
        //     MAIN_FB()->h = image.h;
        //     MAIN_FB()->bpp = image.bpp;
        // }
    }

    float over_xscale = IM_DIV(1.0, arg_x_scale), over_yscale = IM_DIV(1.0f, arg_y_scale);

    switch (arg_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        for (int y = 0, yy = image.h; y < yy; y++)
        {
            uint32_t *row_ptr = IMAGE_COMPUTE_BINARY_PIXEL_ROW_PTR(arg_img, fast_floorf(y * over_yscale) + roi.y);
            uint32_t *row_ptr_2 = IMAGE_COMPUTE_BINARY_PIXEL_ROW_PTR(&image, y);
            for (int x = 0, xx = image.w; x < xx; x++)
            {
                IMAGE_PUT_BINARY_PIXEL_FAST(row_ptr_2, x,
                                            IMAGE_GET_BINARY_PIXEL_FAST(row_ptr, fast_floorf(x * over_xscale) + roi.x));
            }
        }
        break;
    }
    case IMAGE_BPP_GRAYSCALE:
    {
        for (int y = 0, yy = image.h; y < yy; y++)
        {
            uint8_t *row_ptr = IMAGE_COMPUTE_GRAYSCALE_PIXEL_ROW_PTR(arg_img, fast_floorf(y * over_yscale) + roi.y);
            uint8_t *row_ptr_2 = IMAGE_COMPUTE_GRAYSCALE_PIXEL_ROW_PTR(&image, y);
            for (int x = 0, xx = image.w; x < xx; x++)
            {
                IMAGE_PUT_GRAYSCALE_PIXEL_FAST(row_ptr_2, x,
                                               IMAGE_GET_GRAYSCALE_PIXEL_FAST(row_ptr, fast_floorf(x * over_xscale) + roi.x));
            }
        }
        break;
    }
    case IMAGE_BPP_RGB565:
    {
        for (int y = 0, yy = image.h; y < yy; y++)
        {
            uint16_t *row_ptr = IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(arg_img, fast_floorf(y * over_yscale) + roi.y);
            uint16_t *row_ptr_2 = IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(&image, y);
            for (int x = 0, xx = image.w; x < xx; x++)
            {
                IMAGE_PUT_RGB565_PIXEL_FAST(row_ptr_2, x,
                                            IMAGE_GET_RGB565_PIXEL_FAST(row_ptr, fast_floorf(x * over_xscale) + roi.x));
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }

    if (in_place)
    {
        fb_alloc_free_till_mark();
    }

    if (MAIN_FB()->pixels == image.data)
    {
        MAIN_FB()->w = image.w;
        MAIN_FB()->h = image.h;
        MAIN_FB()->bpp = image.bpp;
    }

    // if (copy_to_fb)
    // {
    //     // image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    //     // if (MAIN_FB()->pixels == arg_img->data)
    //     // {
    //     //     arg_img->w = image.w;
    //     //     arg_img->h = image.h;
    //     //     arg_img->bpp = image.bpp;
    //     // }
    // }
    // else
    // {
    if (arg_other)
    {
        arg_other->w = image.w;
        arg_other->h = image.h;
        arg_other->bpp = image.bpp;
    }
    // }
    PyObject *tmp = back_img(&image);
    xfree(image.pixels);
    return tmp;
    // return py_image_from_struct(&image);
}

// static mp_obj_t py_image_save(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args)
// {
//     image_t *arg_img = py_image_cobj(args[0]);
//     const char *path = mp_obj_str_get_str(args[1]);

//     rectangle_t roi;
//     py_helper_keyword_rectangle_roi(arg_img, n_args, args, 2, kw_args, &roi);

//     int arg_q = py_helper_keyword_int(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_quality), 50);
//     PY_ASSERT_TRUE_MSG((1 <= arg_q) && (arg_q <= 100), "Error: 1 <= quality <= 100!");

//     fb_alloc_mark();
//     imlib_save_image(arg_img, path, &roi, arg_q);
//     fb_alloc_free_till_mark();
//     return args[0];
// }

static PyObject *
py_image_clear(PyObject *self, PyObject *args)
{
    image_t *arg_img = &self_image_img;
    // image_t *arg_img = py_helper_arg_to_image_mutable_bayer(img_obj);
    memset(arg_img->data, 0, image_size(arg_img));
    return Py_BuildValue("i", 0);
}

//此函数可能出现问题，请注意
//暂时不做修复
// static PyObject *
// py_image_mean_pool(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     image_t *arg_img = &self_image_img;
//     PyObject *img_tuple;
//     PyObject *img_data;
//     PyObject *img_w;
//     PyObject *img_h;
//     int arg_x_div = 0;
//     int arg_y_div = 0;
//     static char *kwlist[] = {"x_div", "y_div", NULL};
//     if (!PyArg_ParseTupleAndKeywords(args, keywds, "ii", kwlist,
//                                      &arg_x_div, &arg_y_div))
//         return NULL;
//     image_t out_img;
//     out_img.w = arg_img->w / arg_x_div;
//     out_img.h = arg_img->h / arg_y_div;
//     out_img.bpp = arg_img->bpp;
//     out_img.pixels = arg_img->pixels;
//     imlib_mean_pool(arg_img, &out_img, arg_x_div, arg_y_div);
//     arg_img->w = out_img.w;
//     arg_img->h = out_img.h;
//     img_w = PyLong_FromLong(out_img.w);
//     img_h = PyLong_FromLong(out_img.h);
//     data1 = PyBytes_FromStringAndSize(_888_data, out_img.w * out_img.h * 3);
//     return img_tuple;
// }
//此函数可能出现问题，请注意
//暂时不做修复
// static PyObject * py_image_mean_pooled(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     image_t *arg_img = py_helper_arg_to_image_mutable(img_obj);
//     int arg_x_div = mp_obj_get_int(x_div_obj);
//     PY_ASSERT_TRUE_MSG(arg_x_div >= 1, "Width divisor must be greater than >= 1");
//     PY_ASSERT_TRUE_MSG(arg_x_div <= arg_img->w, "Width divisor must be less than <= img width");
//     int arg_y_div = mp_obj_get_int(y_div_obj);
//     PY_ASSERT_TRUE_MSG(arg_y_div >= 1, "Height divisor must be greater than >= 1");
//     PY_ASSERT_TRUE_MSG(arg_y_div <= arg_img->h, "Height divisor must be less than <= img height");
//     image_t out_img;
//     out_img.w = arg_img->w / arg_x_div;
//     out_img.h = arg_img->h / arg_y_div;
//     out_img.bpp = arg_img->bpp;
//     out_img.pixels = xalloc(image_size(&out_img));
//     imlib_mean_pool(arg_img, &out_img, arg_x_div, arg_y_div);
//     return py_image_from_struct(&out_img);
// }

// py_image_to(IMAGE_BPP_BINARY, NULL, false, n_args, args, kw_args);

// (int bpp, const uint16_t *default_color_palette, bool copy_to_fb,uint n_args, const mp_obj_t *args, mp_map_t *kw_args)

// image.to_bitmap             ([x_scale=1.0[, y_scale=1.0[, roi=None[, rgb_channel=-1[, alpha=256[, color_palette=None[,                   alpha_palette=None[, hint=0[, x_size=None[, y_size=None[, copy=False]]]]]]]]]]]
// image.to_grayscale          ([x_scale=1.0[, y_scale=1.0[, roi=None[, rgb_channel=-1[, alpha=256[, color_palette=None[,                   alpha_palette=None[, hint=0[, x_size=None[, y_size=None[, copy=False]]]]]]]]]]])
// image.to_rgb565             ([                                                                                                                                                                     copy=False[,             rgb_channel=-1]])
// image.to_rainbow            ([x_scale=1.0[, y_scale=1.0[, roi=None[, rgb_channel=-1[, alpha=256[, color_palette=sensor.PALETTE_RAINBOW[, alpha_palette=None[, hint=0[, x_size=None[, y_size=None[, copy=False]]]]]]]]]]])

// void py_helper_set_to_framebuffer(image_t *img)
// {
//     #if MICROPY_PY_SENSOR
//     sensor_set_framebuffers(1);
//     #else
//     framebuffer_set_buffers(1);
//     #endif
//     debug_line;
//     // PY_ASSERT_TRUE_MSG((image_size(img) <= framebuffer_get_buffer_size()),
//     //         "The image doesn't fit in the frame buffer!");
//     framebuffer_set(img->w, img->h, img->bpp);
//     img->data = framebuffer_get_buffer(framebuffer->head)->data;
// }
// bool py_helper_is_equal_to_framebuffer(image_t *img)
// {
//     return framebuffer_get_buffer(framebuffer->head)->data == img->data;
// }
// void py_helper_update_framebuffer(image_t *img)
// {
//     if (py_helper_is_equal_to_framebuffer(img)) {
//         framebuffer_set(img->w, img->h, img->bpp);
//     }
// }
// image_t *py_helper_arg_to_image_mutable_bayer(const mp_obj_t arg)
// {
//     image_t *arg_img = py_image_cobj(arg);
//     if(!IMAGE_IS_MUTABLE_BAYER(arg_img))
//     {
//         debug_line;
//         return NULL;
//     }
//     // PY_ASSERT_TRUE_MSG(IMAGE_IS_MUTABLE_BAYER(arg_img), "Image is not mutable!");
//     return arg_img;
// }
// static PyObject *
// py_image_to(int bpp, const uint16_t *default_color_palette, bool copy_to_fb, PyObject *self, PyObject *args, PyObject *keywds)
// {
//     image_t *src_img = &self_image_img;

//     float arg_x_scale = 1.f;
//     // bool got_x_scale = py_helper_keyword_float_maybe(n_args, args, 1, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_scale), &arg_x_scale);

//     float arg_y_scale = 1.f;
//     // bool got_y_scale = py_helper_keyword_float_maybe(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_scale), &arg_y_scale);

//     rectangle_t arg_roi;
//     PyObject *py_roi = Py_None;
//     // py_helper_keyword_rectangle_roi(src_img, n_args, args, 3, kw_args, &arg_roi);

//     int arg_rgb_channel = -1;

//     int arg_alpha = 256;

//     const uint16_t *color_palette = default_color_palette;
//     const uint8_t *alpha_palette = NULL;
//     PyObject *py_color_palette = Py_None;
//     PyObject *py_alpha_palette = Py_None;

//     image_hint_t hint;

//     int arg_x_size = 0;
//     // bool got_x_size = &arg_x_size;

//     int arg_y_size = 0;
//     // bool got_y_size = &arg_y_size;
//     bool copy = false;
//     static char *kwlist[] = {"x_scale", "y_scale", "roi", "rgb_channel", "alpha", "color_palette", "alpha_palette", "hint", "x_size", "y_size", "copy", NULL};
//     if (!PyArg_ParseTupleAndKeywords(args, keywds, "|ffOiiOOiiii", kwlist,
//                                      &arg_x_scale, &arg_y_scale, &py_roi, &arg_rgb_channel, &arg_alpha, &py_color_palette, &py_alpha_palette, &hint, &arg_x_size, &arg_y_size, &copy))
//         return NULL;

//     roi_tan(py_roi, &arg_roi, src_img->w, src_img->h);
//     bool got_x_scale = (arg_x_scale == 1.f) ? 0 : 1;
//     bool got_y_scale = (arg_y_scale == 1.f) ? 0 : 1;
//     bool got_x_size = (arg_x_size == 0) ? 0 : 1;
//     bool got_y_size = (arg_y_size == 0) ? 0 : 1;

//     // copy = false;

//     if ((arg_rgb_channel < -1) || (2 < arg_rgb_channel))
//     {
//         debug_line;
//         return NULL;
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("-1 <= rgb_channel <= 2!"));
//     }
//     if ((arg_alpha < 0) || (256 < arg_alpha))
//     {
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("0 <= alpha <= 256!"));
//         debug_line;
//         return NULL;
//     }

//     if (got_x_scale && got_x_size)
//     {
//         debug_line;return NULL;
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Choose either x_scale or x_size not both!"));
//     }

//     if (got_y_scale && got_y_size)
//     {
//         debug_line;return NULL;
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Choose either y_scale or y_size not both!"));
//     }

//     if (got_x_size)
//     {
//         arg_x_scale = arg_x_size / ((float)arg_roi.w);
//     }

//     if (got_y_size)
//     {
//         arg_y_scale = arg_y_size / ((float)arg_roi.h);
//     }

//     if ((!got_x_scale) && (!got_x_size) && got_y_size)
//     {
//         arg_x_scale = arg_y_scale;
//     }

//     if ((!got_y_scale) && (!got_y_size) && got_x_size)
//     {
//         arg_y_scale = arg_x_scale;
//     }

//     // mp_obj_t copy_obj = py_helper_keyword_object(n_args, args, 11, kw_args,
//                                                 //  MP_OBJ_NEW_QSTR(copy_to_fb ? MP_QSTR_copy_to_fb : MP_QSTR_copy), NULL);
//     bool copy = false;
//     image_t *arg_other = copy_to_fb ? NULL : src_img;

//     // if (copy_obj)
//     // {
//     //     if (mp_obj_is_integer(copy_obj))
//     //     {
//     //         copy = mp_obj_get_int(copy_obj);
//     //     }
//     //     else
//     //     {
//     //         arg_other = py_helper_arg_to_image_mutable_bayer(copy_obj);
//     //     }
//     // }

//     // if (copy_to_fb && copy)
//     // {
//     //     framebuffer_update_jpeg_buffer();
//     // }

//     image_t dst_img;
//     dst_img.w = fast_floorf(arg_roi.w * arg_x_scale);
//     dst_img.h = fast_floorf(arg_roi.h * arg_y_scale);
//     dst_img.bpp = (bpp >= 0) ? bpp : src_img->bpp;

//     if (dst_img.bpp == IMAGE_BPP_BAYER)
//     {
//         if (((arg_x_scale != 1) && (arg_x_scale != -1)) ||
//             ((arg_y_scale != 1) && (arg_y_scale != -1)) ||
//             (arg_rgb_channel != -1) ||
//             (arg_alpha != 256) ||
//             (color_palette != NULL) ||
//             (alpha_palette != NULL))
//         {
//             mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Only bayer copying/cropping is supported!"));
//         }
//         else
//         {
//             hint &= ~(IMAGE_HINT_AREA |
//                       IMAGE_HINT_BICUBIC |
//                       IMAGE_HINT_BILINEAR |
//                       IMAGE_HINT_EXTRACT_RGB_CHANNEL_FIRST |
//                       IMAGE_HINT_APPLY_COLOR_PALETTE_FIRST);
//         }
//     }
//     else if (dst_img.bpp >= IMAGE_BPP_JPEG)
//     {
//         if ((arg_x_scale != 1) ||
//             (arg_y_scale != 1) ||
//             (arg_roi.x != 0) ||
//             (arg_roi.y != 0) ||
//             (arg_roi.w != src_img->w) ||
//             (arg_roi.h != src_img->h) ||
//             (arg_rgb_channel != -1) ||
//             (arg_alpha != 256) ||
//             (color_palette != NULL) ||
//             (alpha_palette != NULL))
//         {
//             mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Only jpeg copying is supported!"));
//         }
//     }

//     if (copy)
//     {
//         if (copy_to_fb)
//         {
//             py_helper_set_to_framebuffer(&dst_img);
//         }
//         else
//         {
//             dst_img.data = xalloc(image_size(&dst_img));
//         }
//     }
//     else if (arg_other)
//     {
//         bool fb = py_helper_is_equal_to_framebuffer(arg_other);
//         size_t size = fb ? framebuffer_get_buffer_size() : image_size(arg_other);
//         PY_ASSERT_TRUE_MSG((image_size(&dst_img) <= size),
//                            "The new image won't fit in the target frame buffer!");
//         // DO NOT MODIFY arg_other YET (as it could point to src_img)!
//         dst_img.data = arg_other->data;
//         py_helper_update_framebuffer(&dst_img);
//     }
//     else
//     {
//         dst_img.data = xalloc(image_size(&dst_img));
//     }

//     if (dst_img.bpp >= IMAGE_BPP_JPEG)
//     {
//         if (dst_img.data != src_img->data)
//         {
//             memcpy(dst_img.data, src_img->data, dst_img.bpp);
//         }
//     }
//     else
//     {
//         fb_alloc_mark();
//         imlib_draw_image(&dst_img, src_img, 0, 0, arg_x_scale, arg_y_scale, &arg_roi,
//                          arg_rgb_channel, arg_alpha, color_palette, alpha_palette,
//                          (hint & (~IMAGE_HINT_CENTER)) | IMAGE_HINT_BLACK_BACKGROUND, NULL, NULL);
//         fb_alloc_free_till_mark();
//     }

//     if (arg_other)
//     {
//         arg_other->w = dst_img.w;
//         arg_other->h = dst_img.h;
//         arg_other->bpp = dst_img.bpp;
//     }

//     return py_image_from_struct(&dst_img);
// }
//不允许使用复制功能
// static PyObject *
// py_image_to(int bpp, const uint16_t *default_color_palette, bool copy_to_fb,PyObject *self, PyObject *args, PyObject *keywds)
// {
//     image_t *src_img = &self_image_img;
//     // PyObject *img_tuple ;
//     // PyObject *img_data;
//     // PyObject *img_w;
//     // PyObject *img_h;
//     float arg_x_scale = 1.f;
//     float arg_y_scale = 1.f;
//     rectangle_t arg_roi;
//     PyObject *py_roi = Py_None;
//     int arg_rgb_channel = 0;
//     int arg_alpha = 0;
//     const uint16_t *color_palette = NULL;
//     const uint8_t *alpha_palette = NULL;
//     PyObject *py_color_palette = Py_None;
//     PyObject *py_alpha_palette = Py_None;
//     image_hint_t hint ;
//     int arg_x_size = 0;
//     int arg_y_size = 0;
//     // mp_obj_t copy_obj ;
//     bool copy = false;
//     static char *kwlist[] = {"x_scale", "y_scale", "roi", "rgb_channel", "alpha", "color_palette","alpha_palette", "hint","x_size","y_size","copy",NULL};
//     if (!PyArg_ParseTupleAndKeywords(args, keywds, "|ffOiiOOiiii", kwlist,
//                                      &arg_x_scale, &arg_y_scale, &py_roi, &arg_rgb_channel, &arg_alpha, &py_color_palette,&py_alpha_palette,&hint,&arg_x_size,&arg_y_size,&copy))
//         return NULL;
//     // Py_DECREF(py_color_palette);
//     // Py_DECREF(py_alpha_palette);
//     debug_line;
//     roi_tan(py_roi, &arg_roi, src_img->w, src_img->h);

//     debug_line;
//     bool got_x_scale = (arg_x_scale == 1.f) ? 0 : 1 ;
//     bool got_y_scale = (arg_y_scale == 1.f) ? 0 : 1 ;

// debug_line;
//     bool got_x_size =(arg_x_size == 0) ? 0 : 1 ;
//     bool got_y_size = (arg_y_size == 0) ? 0 : 1 ;

//     copy = false;

// debug_line;

//     if ((arg_rgb_channel < -1) || (2 < arg_rgb_channel)) {
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("-1 <= rgb_channel <= 2!"));
//         debug_line;
//         return NULL;
//     }

//     if ((arg_alpha < 0) || (256 < arg_alpha)) {
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("0 <= alpha <= 256!"));
//         debug_line;
//         return NULL;
//     }

// debug_line;
//     if (got_x_scale && got_x_size) {
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Choose either x_scale or x_size not both!"));
//         debug_line;
//         return NULL;
//     }

//     if (got_y_scale && got_y_size) {
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Choose either y_scale or y_size not both!"));
//         debug_line;
//         return NULL;
//     }
// debug_line;
//     if (got_x_size)
//     {
//         arg_x_scale = arg_x_size / ((float)arg_roi.w);
//     }

//     if (got_y_size)
//     {
//         arg_y_scale = arg_y_size / ((float)arg_roi.h);
//     }

//     if ((!got_x_scale) && (!got_x_size) && got_y_size)
//     {
//         arg_x_scale = arg_y_scale;
//     }
// debug_line;
//     if ((!got_y_scale) && (!got_y_size) && got_x_size)
//     {
//         arg_y_scale = arg_x_scale;
//     }

//     image_t *arg_other = copy_to_fb ? NULL : src_img;

//     // if (copy_obj)
//     // {
//     //     if (mp_obj_is_integer(copy_obj))
//     //     {
//     //         copy = mp_obj_get_int(copy_obj);
//     //     }
//     //     else
//     //     {
//     //         arg_other = py_helper_arg_to_image_mutable_bayer(copy_obj);
//     //     }
//     // }

//     // if (copy_to_fb && copy) {
//     //     framebuffer_update_jpeg_buffer();
//     // }

//     image_t dst_img;
//     dst_img.w = fast_floorf(arg_roi.w * arg_x_scale);
//     dst_img.h = fast_floorf(arg_roi.h * arg_y_scale);
//     dst_img.bpp = (bpp >= 0) ? bpp : src_img->bpp;
// debug_line;
//     if (dst_img.bpp == IMAGE_BPP_BAYER)
//     {
//         if (((arg_x_scale != 1) && (arg_x_scale != -1)) ||
//             ((arg_y_scale != 1) && (arg_y_scale != -1)) ||
//             (arg_rgb_channel != -1) ||
//             (arg_alpha != 256) ||
//             (color_palette != NULL) ||
//             (alpha_palette != NULL))
//         {
//             debug_line;
//             // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Only bayer copying/cropping is supported!"));
//             return NULL;
//         }
//         else
//         {
//             hint &= ~(IMAGE_HINT_AREA |
//                       IMAGE_HINT_BICUBIC |
//                       IMAGE_HINT_BILINEAR |
//                       IMAGE_HINT_EXTRACT_RGB_CHANNEL_FIRST |
//                       IMAGE_HINT_APPLY_COLOR_PALETTE_FIRST);
//         }
//     }
//     else if (dst_img.bpp >= IMAGE_BPP_JPEG)
//     {
//         if ((arg_x_scale != 1) ||
//             (arg_y_scale != 1) ||
//             (arg_roi.x != 0) ||
//             (arg_roi.y != 0) ||
//             (arg_roi.w != src_img->w) ||
//             (arg_roi.h != src_img->h) ||
//             (arg_rgb_channel != -1) ||
//             (arg_alpha != 256) ||
//             (color_palette != NULL) ||
//             (alpha_palette != NULL))
//         {
//             // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Only jpeg copying is supported!"));
//             debug_line;
//             return NULL;
//         }
//     }
// debug_line;
//     if (copy)
//     {
//                 debug_line;
//         if (copy_to_fb)
//         {
//             py_helper_set_to_framebuffer(&dst_img);
//         }
//         else
//         {
//             dst_img.data = xalloc(image_size(&dst_img));
//         }
//     }
//     else if (arg_other)
//     {
//                 debug_line;
//         bool fb = py_helper_is_equal_to_framebuffer(arg_other);
//         size_t size = fb ? framebuffer_get_buffer_size() : image_size(arg_other);

//         // PY_ASSERT_TRUE_MSG((image_size(&dst_img) <= size),
//         //         "The new image won't fit in the target frame buffer!");
//         if (!(image_size(&dst_img) <= size))
//         {
//             debug_line;
//             return NULL;
//         }
//         // DO NOT MODIFY arg_other YET (as it could point to src_img)!
//         dst_img.data = arg_other->data;
//         py_helper_update_framebuffer(&dst_img);
//                 debug_line;
//     }
//     else
//     {
//                 debug_line;
//         dst_img.data = xalloc(image_size(&dst_img));
//     }
//     if (dst_img.bpp >= IMAGE_BPP_JPEG)
//     {
//         if (dst_img.data != src_img->data)
//         {
//             memcpy(dst_img.data, src_img->data, dst_img.bpp);
//         }
//     }
//     else
//     {
//         // for (int i = 0; i < 1000; i++)
//         // {
//         //     printf("%x", src_img->data[i]);
//         // }
//         // printf("\r\n");
//         fb_alloc_mark();
//         imlib_draw_image(&dst_img, &src_img, 0, 0, arg_x_scale, arg_y_scale, &arg_roi,
//                          arg_rgb_channel, arg_alpha, color_palette, alpha_palette,
//                          (hint & (~IMAGE_HINT_CENTER)) | IMAGE_HINT_BLACK_BACKGROUND, NULL, NULL);
//         fb_alloc_free_till_mark();
//         debug_line;
//         // for (int i = 0;i < 1000; i++)
//         // {
//         //     printf("%x", dst_img.data[i]);
//         //     //                 if(src_img->data[i] != dst_img.data[i])
//         //     //                 {
//         //     //                     printf("err:%d\r\n",i);
//         //     //                 }
//         // }
//         // printf("\r\n");
//     }
// debug_line;
//     // if (arg_other)
//     // {
//     //     debug_line;
//     //     arg_other->w = dst_img.w;
//     //     arg_other->h = dst_img.h;
//     //     arg_other->bpp = dst_img.bpp;
//     // }
// debug_line;
//     // return Py_BuildValue("i", 0);
//     return back_img(&dst_img);
//     // return py_image_from_struct(&dst_img);
// }

// // static PyObject * py_image_to_bitmap(PyObject *self, PyObject *args, PyObject *keywds)
// // {
// //     return py_image_to(IMAGE_BPP_BINARY, NULL, false, n_args, args, kw_args);
// // }

/*
static PyObject *
py_image_to_grayscale(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *src_img = &self_image_img;
    image_t dst_img;
    bool copy = false;
    static char *kwlist[] = {"copy", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist,
                                     &copy))
        return NULL;
    dst_img.w = src_img->w;
    dst_img.h = src_img->h;
    dst_img.bpp = src_img->bpp;
    switch (src_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        if (copy)
        {
            src_img->bpp = IMAGE_BPP_GRAYSCALE;
            PyObject *bac = back_img(&src_img->data);
            src_img->bpp = dst_img.bpp;
            return bac;
        }
        else
        {
            self_image_img.bpp = IMAGE_BPP_GRAYSCALE;
            return Py_BuildValue("i", 0);
        }
    }
    break;
    case IMAGE_BPP_GRAYSCALE:
    {
        if (copy)
        {
            PyObject *bac = back_img(&src_img->data);
            return bac;
        }
        else
        {
            return Py_BuildValue("i", 0);
        }
    }
    break;
    case IMAGE_BPP_RGB565:
    {
        dst_img.data = (uint8_t *)malloc(src_img->w * src_img->h);
        uint8_t *dstpicx = dst_img.data;
        uint16_t *srcpicx = (uint16_t *)src_img->data;
        for (int i = 0; i < src_img->w * src_img->h; i++)
        {
            *dstpicx = COLOR_RGB565_TO_GRAYSCALE(*srcpicx);
            dstpicx++;
            srcpicx++;
        }
        dst_img.bpp = IMAGE_BPP_GRAYSCALE;
        if (copy)
        {
            PyObject *bac = back_img(&dst_img);
            free(dst_img.data);
            return bac;
        }
        else
        {
            free(self_image_img.data);
            self_image_img.data = (uint8_t *)malloc(src_img->w * src_img->h);
            memcpy(self_image_img.data, dst_img.data, dst_img.w * dst_img.h);
            self_image_img.bpp = IMAGE_BPP_GRAYSCALE;
            free(dst_img.data);
            return Py_BuildValue("i", 0);
        }
    }
    break;
    case IMAGE_BPP_BAYER:
    {
        return Py_BuildValue("i", -1);
    }
    break;
    case IMAGE_BPP_JPEG:
    {
        return Py_BuildValue("i", -1);
    }
    break;
    default:
        return Py_BuildValue("i", -1);
        break;
    }
}
static PyObject *
py_image_to_rgb565(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *src_img = &self_image_img;
    image_t dst_img;
    bool copy = false;
    static char *kwlist[] = {"copy", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist,
                                     &copy))
        return NULL;
    dst_img.w = src_img->w;
    dst_img.h = src_img->h;
    dst_img.bpp = src_img->bpp;
    switch (src_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        dst_img.data = (uint8_t *)malloc(src_img->w * src_img->h * 2);
        uint8_t *dstpicx = src_img->data;
        uint16_t *srcpicx = (uint16_t *)dst_img.data;
        for (int i = 0; i < src_img->w * src_img->h; i++)
        {
            *srcpicx = COLOR_BINARY_TO_RGB565(*dstpicx);

            dstpicx++;
            srcpicx++;
        }

        dst_img.bpp = IMAGE_BPP_RGB565;

        if (copy)
        {
            PyObject *bac = back_img(&dst_img);
            free(dst_img.data);
            return bac;
        }
        else
        {
            free(self_image_img.data);
            self_image_img.data = (uint8_t *)malloc(src_img->w * src_img->h * 2);
            memcpy(self_image_img.data, dst_img.data, dst_img.w * dst_img.h * 2);
            self_image_img.bpp = IMAGE_BPP_RGB565;
            free(dst_img.data);
            return Py_BuildValue("i", 0);
        }
    }
    break;
    case IMAGE_BPP_GRAYSCALE:
    {
        dst_img.data = (uint8_t *)malloc(src_img->w * src_img->h * 2);
        uint8_t *dstpicx = src_img->data;
        uint16_t *srcpicx = (uint16_t *)dst_img.data;
        for (int i = 0; i < src_img->w * src_img->h; i++)
        {
            *srcpicx = COLOR_GRAYSCALE_TO_RGB565(*dstpicx);

            dstpicx++;
            srcpicx++;
        }

        dst_img.bpp = IMAGE_BPP_RGB565;

        if (copy)
        {
            PyObject *bac = back_img(&dst_img);
            free(dst_img.data);
            return bac;
        }
        else
        {
            free(self_image_img.data);
            self_image_img.data = (uint8_t *)malloc(src_img->w * src_img->h * 2);
            memcpy(self_image_img.data, dst_img.data, dst_img.w * dst_img.h * 2);
            self_image_img.bpp = IMAGE_BPP_RGB565;
            free(dst_img.data);
            return Py_BuildValue("i", 0);
        }
    }
    break;
    case IMAGE_BPP_RGB565:
    {
        dst_img.data = (uint8_t *)malloc(src_img->w * src_img->h);
        uint8_t *dstpicx = dst_img.data;
        uint16_t *srcpicx = (uint16_t *)src_img->data;
        for (int i = 0; i < src_img->w * src_img->h; i++)
        {
            *dstpicx = COLOR_RGB565_TO_GRAYSCALE(*srcpicx);
            dstpicx++;
            srcpicx++;
        }
        dst_img.bpp = IMAGE_BPP_GRAYSCALE;
        if (copy)
        {
            PyObject *bac = back_img(&dst_img);
            free(dst_img.data);
            return bac;
        }
        else
        {
            free(self_image_img.data);
            memcpy(self_image_img.data, dst_img.data, dst_img.w * dst_img.h);
            self_image_img.bpp = IMAGE_BPP_GRAYSCALE;
            free(dst_img.data);
            return Py_BuildValue("i", 0);
        }
    }
    break;
    case IMAGE_BPP_BAYER:
    {
        return Py_BuildValue("i", -1);
    }
    break;
    case IMAGE_BPP_JPEG:
    {
        return Py_BuildValue("i", -1);
    }
    break;
    default:
        return Py_BuildValue("i", -1);
        break;
    }
}
static PyObject *
py_image_to_rainbow(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *src_img = &self_image_img;
    image_t dst_img;
    bool copy = false;
    static char *kwlist[] = {"copy", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist,
                                     &copy))
        return NULL;
    dst_img.w = src_img->w;
    dst_img.h = src_img->h;
    dst_img.bpp = src_img->bpp;
    switch (src_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        if (copy)
        {
            src_img->bpp = IMAGE_BPP_GRAYSCALE;
            PyObject *bac = back_img(&src_img->data);
            src_img->bpp = dst_img.bpp;
            return bac;
        }
        else
        {
            self_image_img.bpp = IMAGE_BPP_GRAYSCALE;
            return Py_BuildValue("i", 0);
        }
    }
    break;
    case IMAGE_BPP_GRAYSCALE:
    {
        if (copy)
        {
            PyObject *bac = back_img(&src_img->data);
            return bac;
        }
        else
        {
            return Py_BuildValue("i", 0);
        }
    }
    break;
    case IMAGE_BPP_RGB565:
    {
        dst_img.data = (uint8_t *)malloc(src_img->w * src_img->h);
        uint8_t *dstpicx = dst_img.data;
        uint16_t *srcpicx = (uint16_t *)src_img->data;
        for (int i = 0; i < src_img->w * src_img->h; i++)
        {
            *dstpicx = COLOR_RGB565_TO_GRAYSCALE(*srcpicx);
            dstpicx++;
            srcpicx++;
        }
        dst_img.bpp = IMAGE_BPP_GRAYSCALE;
        if (copy)
        {
            PyObject *bac = back_img(&dst_img);
            free(dst_img.data);
            return bac;
        }
        else
        {
            free(self_image_img.data);
            memcpy(self_image_img.data, dst_img.data, dst_img.w * dst_img.h);
            self_image_img.bpp = IMAGE_BPP_GRAYSCALE;
            free(dst_img.data);
            return Py_BuildValue("i", 0);
        }
    }
    break;
    case IMAGE_BPP_BAYER:
    {
        return Py_BuildValue("i", -1);
    }
    break;
    case IMAGE_BPP_JPEG:
    {
        return Py_BuildValue("i", -1);
    }
    break;
    default:
        return Py_BuildValue("i", -1);
        break;
    }
}

*/

// static PyObject *
// py_image_compress(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     if (IM_IS_JPEG((image_t *) py_image_cobj(args[0]))) return args[0];

//     image_t *arg_img = py_helper_arg_to_image_mutable_bayer(args[0]);
//     int arg_q = py_helper_keyword_int(n_args, args, 1, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_quality), 50);
//     PY_ASSERT_TRUE_MSG((1 <= arg_q) && (arg_q <= 100), "Error: 1 <= quality <= 100!");

//     fb_alloc_mark();
//     image_t out = { .w=arg_img->w, .h=arg_img->h, .bpp=0, .data=NULL }; // alloc in jpeg compress
//     PY_ASSERT_FALSE_MSG(jpeg_compress(arg_img, &out, arg_q, false), "Out of Memory!");
//     PY_ASSERT_TRUE_MSG(out.bpp <= image_size(arg_img), "Can't compress in place!");
//     memcpy(arg_img->data, out.data, out.bpp);
//     arg_img->bpp = out.bpp;
//     fb_alloc_free_till_mark();
//     py_helper_update_framebuffer(arg_img);
//     return args[0];
// }

// static PyObject * py_image_compress_for_ide(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     if (IM_IS_JPEG((image_t *) py_image_cobj(args[0]))) return py_image_jpeg_encode_for_ide(args[0]);

//     image_t *arg_img = py_helper_arg_to_image_mutable_bayer(args[0]);
//     int arg_q = py_helper_keyword_int(n_args, args, 1, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_quality), 50);
//     PY_ASSERT_TRUE_MSG((1 <= arg_q) && (arg_q <= 100), "Error: 1 <= quality <= 100!");

//     fb_alloc_mark();
//     image_t out = { .w=arg_img->w, .h=arg_img->h, .bpp=0, .data=NULL }; // alloc in jpeg compress
//     PY_ASSERT_FALSE_MSG(jpeg_compress(arg_img, &out, arg_q, false), "Out of Memory!");
//     int new_size = fb_encode_for_ide_new_size(&out);
//     PY_ASSERT_TRUE_MSG(new_size <= image_size(arg_img), "Can't compress in place!");
//     fb_encode_for_ide(arg_img->data, &out);

//     out.bpp = new_size;
//     arg_img->bpp = out.bpp;
//     fb_alloc_free_till_mark();
//     py_helper_update_framebuffer(arg_img);
//     return args[0];
// }

// static PyObject * py_image_compressed(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     image_t *arg_img = py_image_cobj(args[0]);

//     if (IM_IS_JPEG(arg_img)) {
//         image_t out = { .w=arg_img->w, .h=arg_img->h, .bpp=arg_img->bpp, .data=xalloc(arg_img->bpp) };
//         memcpy(out.data, arg_img->data, arg_img->bpp);
//         return py_image_from_struct(&out);
//     }

//     arg_img = py_helper_arg_to_image_mutable_bayer(args[0]);
//     int arg_q = py_helper_keyword_int(n_args, args, 1, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_quality), 50);
//     PY_ASSERT_TRUE_MSG((1 <= arg_q) && (arg_q <= 100), "Error: 1 <= quality <= 100!");

//     fb_alloc_mark();
//     image_t out = { .w=arg_img->w, .h=arg_img->h, .bpp=0, .data=NULL }; // alloc in jpeg compress
//     PY_ASSERT_FALSE_MSG(jpeg_compress(arg_img, &out, arg_q, false), "Out of Memory!");
//     uint8_t *temp = xalloc(out.bpp);
//     memcpy(temp, out.data, out.bpp);
//     out.data = temp;
//     fb_alloc_free_till_mark();

//     return py_image_from_struct(&out);
// }

// static PyObject * py_image_compressed_for_ide(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     if (IM_IS_JPEG((image_t *) py_image_cobj(args[0]))) return py_image_jpeg_encoded_for_ide(args[0]);

//     image_t *arg_img = py_helper_arg_to_image_mutable_bayer(args[0]);
//     int arg_q = py_helper_keyword_int(n_args, args, 1, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_quality), 50);
//     PY_ASSERT_TRUE_MSG((1 <= arg_q) && (arg_q <= 100), "Error: 1 <= quality <= 100!");

//     fb_alloc_mark();
//     image_t out = { .w=arg_img->w, .h=arg_img->h, .bpp=0, .data=NULL }; // alloc in jpeg compress
//     PY_ASSERT_FALSE_MSG(jpeg_compress(arg_img, &out, arg_q, false), "Out of Memory!");
//     int new_size = fb_encode_for_ide_new_size(&out);
//     uint8_t *temp = xalloc(new_size);
//     fb_encode_for_ide(temp, &out);

//     out.bpp = new_size;
//     out.data = temp;
//     fb_alloc_free_till_mark();

//     return py_image_from_struct(&out);
// }

static PyObject *
py_image_draw_line(PyObject *self, PyObject *args, PyObject *keywds)
{
    int x0;
    int y0;
    int x1;
    int y1;
    int c = 0xffffff;
    int thickness = 1;

    static char *kwlist[] = {"x0", "y0", "x1", "y1", "c", "thickness", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iiii|ii", kwlist,
                                     &x0, &y0, &x1, &y1, &c, &thickness))
        return NULL;
    // printf("thickness:%d\r\n",thickness);
    // printf("--------------x:%d,y:%d,x1:%d,y1:%d\r\n",x0,y0,x1,y1);

    imlib_draw_line(&self_image_img, x0, y0, x1, y1, COLOR_R8_G8_B8_TO_RGB565((c & 0xff0000) >> 16, (c & 0xff00) >> 8, (c & 0xff)), thickness);
    return PyLong_FromLong(0);
}

static PyObject *
py_image_draw_rectangle(PyObject *self, PyObject *args, PyObject *keywds)
{
    PyObject *fil;
    int rx;
    int ry;
    int rw;
    int rh;
    int c = 0xffffff;
    int thickness = 1;
    bool fill = 0;
    static char *kwlist[] = {"rx", "ry", "rw", "rh", "c", "thickness", "fill", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iiii|iiO", kwlist,
                                     &rx, &ry, &rw, &rh, &c, &thickness, &fil))
        return NULL;

    if (fil == Py_True)
        fill = 1;
    else
        fill = 0;

    imlib_draw_rectangle(&self_image_img, rx, ry, rw, rh, COLOR_R8_G8_B8_TO_RGB565((c & 0xff0000) >> 16, (c & 0xff00) >> 8, (c & 0xff)), thickness, fill);

    return PyLong_FromLong(0);
}

static PyObject *
py_image_draw_circle(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    PyObject *fil;
    int arg_cx = 0;
    int arg_cy = 0;
    int arg_cr = 0;
    int arg_c = 0xffffff;
    int arg_thickness = 1;
    bool arg_fill = 0;

    static char *kwlist[] = {"cx", "cy", "cr", "c", "thickness", "fill", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iii|iiO", kwlist,
                                     &arg_cx, &arg_cy, &arg_cr, &arg_c, &arg_thickness, &fil))
        return NULL;

    if (fil == Py_True)
        arg_fill = 1;
    else
        arg_fill = 0;
    imlib_draw_circle(arg_img, arg_cx, arg_cy, arg_cr, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness, arg_fill);
    return PyLong_FromLong(0);
}

static PyObject *
py_image_draw_ellipse(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    PyObject *fil;
    int arg_cx = 0;
    int arg_cy = 0;
    int arg_rx = 0;
    int arg_ry = 0;
    int arg_r = 0;

    int arg_c = 0xffffff;
    int arg_thickness = 1;
    bool arg_fill = 0;

    static char *kwlist[] = {"cx", "cy", "rx", "ry", "r", "c", "thickness", "fill", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iiiii|iiO", kwlist,
                                     &arg_cx, &arg_cy, &arg_rx, &arg_ry, &arg_r, &arg_c, &arg_thickness, &fil))
        return NULL;
    if (fil == Py_True)
        arg_fill = 1;
    else
        arg_fill = 0;
    imlib_draw_ellipse(arg_img, arg_cx, arg_cy, arg_rx, arg_ry, arg_r, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness, arg_fill);
    return PyLong_FromLong(0);
}

// static PyObject * py_image_draw_string(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     image_t *arg_img = &self_image_img;

//     int arg_x_off = 0;
//     int arg_y_off = 0;
//     const char *arg_str = NULL;

//     int arg_c = 0xffffff;
//     float arg_scale = 1.0;
//     int arg_x_spacing = 0;
//     int arg_y_spacing = 0;
//     bool arg_mono_space = 1;
//     int arg_char_rotation = 0;
//     int arg_char_hmirror = 0;
//     int arg_char_vflip = 0;
//     int arg_string_rotation = 0;
//     int arg_string_hmirror = 0;
//     int arg_string_vflip = 0;

//     static char *kwlist[] = {"cx","cy","rx","ry","r","c","thickness","fill", NULL};
//     if (!PyArg_ParseTupleAndKeywords(args, keywds, "iiiii|iiO", kwlist,
//                               &arg_cx,&arg_cy,&arg_rx,&arg_ry,&arg_r,&arg_c,&arg_thickness,&fil)) return NULL;

//     imlib_draw_string(arg_img, arg_x_off, arg_y_off, arg_str,
//                       arg_c, arg_scale, arg_x_spacing, arg_y_spacing, arg_mono_space,
//                       arg_char_rotation, arg_char_hmirror, arg_char_vflip,
//                       arg_string_rotation, arg_string_hmirror, arg_string_vflip);
//     return args[0];
// }

static PyObject *
py_image_draw_cross(PyObject *self, PyObject *args, PyObject *keywds)
{

    image_t *arg_img = &self_image_img;
    int arg_x = 0;
    int arg_y = 0;

    int arg_c = 0xffffff;
    int arg_s = 5;
    int arg_thickness = 1;

    static char *kwlist[] = {"x", "y", "c", "size", "thickness", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ii|iii", kwlist,
                                     &arg_x, &arg_y, &arg_c, &arg_s, &arg_thickness))
        return NULL;

    imlib_draw_line(arg_img, arg_x - arg_s, arg_y, arg_x + arg_s, arg_y, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);
    imlib_draw_line(arg_img, arg_x, arg_y - arg_s, arg_x, arg_y + arg_s, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);
    return PyLong_FromLong(0);
}

static PyObject *
py_image_draw_arrow(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    int arg_x0 = 0;
    int arg_y0 = 0;
    int arg_x1 = 0;
    int arg_y1 = 0;
    int arg_c = 0xffffff;
    int arg_s = 10;
    int arg_thickness = 1;

    static char *kwlist[] = {"x0", "y0", "x1", "y1", "c", "thickness", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iiii|ii", kwlist,
                                     &arg_x0, &arg_y0, &arg_x1, &arg_y1, &arg_c, &arg_thickness))
        return NULL;
    int dx = (arg_x1 - arg_x0);
    int dy = (arg_y1 - arg_y0);
    float length = fast_sqrtf((dx * dx) + (dy * dy));
    float ux = IM_DIV(dx, length);
    float uy = IM_DIV(dy, length);
    float vx = -uy;
    float vy = ux;
    int a0x = fast_roundf(arg_x1 - (arg_s * ux) + (arg_s * vx * 0.5));
    int a0y = fast_roundf(arg_y1 - (arg_s * uy) + (arg_s * vy * 0.5));
    int a1x = fast_roundf(arg_x1 - (arg_s * ux) - (arg_s * vx * 0.5));
    int a1y = fast_roundf(arg_y1 - (arg_s * uy) - (arg_s * vy * 0.5));

    imlib_draw_line(arg_img, arg_x0, arg_y0, arg_x1, arg_y1, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);
    imlib_draw_line(arg_img, arg_x1, arg_y1, a0x, a0y, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);
    imlib_draw_line(arg_img, arg_x1, arg_y1, a1x, a1y, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);
    return PyLong_FromLong(0);
}

static PyObject *
py_image_draw_edges(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    int x0, y0, x1, y1, x2, y2, x3, y3;
    x0 = 0;
    y0 = 0;
    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;
    x3 = 0;
    y3 = 0;

    int arg_c = 0xffffff;
    int arg_s = 0;
    int arg_thickness = 1;
    bool arg_fill = 0;

    static char *kwlist[] = {"x0", "y0", "x1", "y1", "x2", "y2", "x3", "y3", "color", "size", "thickness", "fill", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "[(ii)(ii)(ii)(ii)]|iiii", kwlist,
                                     &x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3, &arg_c, &arg_s, &arg_thickness, &arg_fill))
        return NULL;

    imlib_draw_line(arg_img, x0, y0, x1, y1, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);
    imlib_draw_line(arg_img, x1, y1, x2, y2, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);
    imlib_draw_line(arg_img, x2, y2, x3, y3, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);
    imlib_draw_line(arg_img, x3, y3, x0, y0, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);

    if (arg_s >= 1)
    {
        imlib_draw_circle(arg_img, x0, y0, arg_s, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness, arg_fill);
        imlib_draw_circle(arg_img, x1, y1, arg_s, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness, arg_fill);
        imlib_draw_circle(arg_img, x2, y2, arg_s, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness, arg_fill);
        imlib_draw_circle(arg_img, x3, y3, arg_s, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness, arg_fill);
    }

    return PyLong_FromLong(0);
}

static PyObject *
py_image_draw_image(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    image_t *arg_other = NULL;
    PyObject *py_arg_other = Py_None;
    // py_helper_arg_to_image_mutable(args[1]);

    int arg_cx;
    int arg_cy;

    float arg_x_scale = 1.0f;
    // py_helper_keyword_float(n_args, args, offset + 0, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_scale), 1.0f);
    // PY_ASSERT_TRUE_MSG((0.0f <= arg_x_scale), "Error: 0.0 <= x_scale!");
    float arg_y_scale = 1.0f;
    // py_helper_keyword_float(n_args, args, offset + 1, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_scale), 1.0f);
    // PY_ASSERT_TRUE_MSG((0.0f <= arg_y_scale), "Error: 0.0 <= y_scale!");

    float arg_alpha = 256.0f;
    //     py_helper_keyword_int(n_args, args, offset + 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_alpha), 256) / 256.0f;
    // PY_ASSERT_TRUE_MSG((0 <= arg_alpha) && (arg_alpha <= 1), "Error: 0 <= alpha <= 256!");
    image_t *arg_msk = NULL;
    PyObject *py_arg_msk = Py_None;
    // py_helper_keyword_to_image_mutable_mask(n_args, args, offset + 3, kw_args);

    static char *kwlist[] = {"image", "x", "y", "x_scale", "y_scale", "mask", "alpha", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "Oii|ffOf", kwlist,
                                     &py_arg_other, &arg_cx, &arg_cy, &arg_x_scale, &arg_y_scale, &py_arg_msk, &arg_alpha))
        return NULL;

    r24to_imgr16(py_arg_other, Py_None, Py_None, Py_None, &arg_other);
    r24to_imgr16(py_arg_msk, Py_None, Py_None, Py_None, &arg_msk);

    imlib_draw_image(arg_img, arg_other, arg_cx, arg_cy, arg_x_scale, arg_y_scale, arg_alpha, arg_msk);

    return PyLong_FromLong(0);
}

static PyObject *
py_image_draw_keypoints(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    int arg_c = 0xffffff;
    // py_helper_keyword_color(arg_img, n_args, args, 2, kw_args, -1); // White.
    int arg_s = 10;
    // py_helper_keyword_int(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_size), 10);
    int arg_thickness = 1;
    // py_helper_keyword_int(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_thickness), 1);
    bool arg_fill = false;
    // py_helper_keyword_int(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_fill), false);
    PyObject *py_keypoints = Py_None;

    static char *kwlist[] = {"keypoints", "color", "size", "thickness", "fill", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|iiii", kwlist,
                                     &py_keypoints, &arg_c, &arg_s, &arg_thickness, &arg_fill))
        return NULL;

    if (PyTuple_Check(py_keypoints))
    {
        size_t len = PyTuple_Size(py_keypoints);
        // PyObject *items;

        // mp_obj_get_array(args[1], &len, &items);

        for (size_t i = 0; i < len; i++)
        {
            PyObject *tuple = PyTuple_GetItem(py_keypoints, i);

            // mp_obj_get_array_fixed_n(items[i], 3, &tuple);

            int cx = PyLong_AsLong(PyTuple_GetItem(tuple, 0));
            // mp_obj_get_int(tuple[0]);
            int cy = PyLong_AsLong(PyTuple_GetItem(tuple, 1));
            // mp_obj_get_int(tuple[1]);
            int angle = PyLong_AsLong(PyTuple_GetItem(tuple, 2)) % 360;
            // mp_obj_get_int(tuple[2]) % 360;
            int si = sin_table[angle] * arg_s;
            int co = cos_table[angle] * arg_s;
            imlib_draw_line(arg_img, cx, cy, cx + co, cy + si, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);
            imlib_draw_circle(arg_img, cx, cy, (arg_s - 2) / 2, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness, arg_fill);
        }
    }
    else if (PyList_Check(py_keypoints))
    {
        size_t len = PyList_Size(py_keypoints);
        // PyObject *items;

        // mp_obj_get_array(args[1], &len, &items);

        for (size_t i = 0; i < len; i++)
        {
            PyObject *tuple = PyTuple_GetItem(py_keypoints, i);

            // mp_obj_get_array_fixed_n(items[i], 3, &tuple);

            int cx = PyLong_AsLong(PyList_GetItem(tuple, 0));
            // mp_obj_get_int(tuple[0]);
            int cy = PyLong_AsLong(PyList_GetItem(tuple, 1));
            // mp_obj_get_int(tuple[1]);
            int angle = PyLong_AsLong(PyList_GetItem(tuple, 2)) % 360;
            // mp_obj_get_int(tuple[2]) % 360;
            int si = sin_table[angle] * arg_s;
            int co = cos_table[angle] * arg_s;
            imlib_draw_line(arg_img, cx, cy, cx + co, cy + si, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);
            imlib_draw_circle(arg_img, cx, cy, (arg_s - 2) / 2, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness, arg_fill);
        }
    }
    else
    { //等待确认这是个什么东西后再加入这程序
        // py_kp_obj_t *kpts_obj = py_kpts_obj(args[1]);
        // for (int i = 0, ii = array_length(kpts_obj->kpts); i < ii; i++) {
        //     kp_t *kp = array_at(kpts_obj->kpts, i);
        //     int cx = kp->x;
        //     int cy = kp->y;
        //     int angle = kp->angle % 360;
        //     int si = sin_table[angle] * arg_s;
        //     int co = cos_table[angle] * arg_s;
        //     imlib_draw_line(arg_img, cx, cy, cx + co, cy + si, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness);
        //     imlib_draw_circle(arg_img, cx, cy, (arg_s - 2) / 2, COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_thickness, arg_fill);
    }

    return PyLong_FromLong(0);
}

// static PyObject *
// py_image_draw_image(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     image_t *arg_img = &self_image_img;
//     image_t arg_other;
//     PyObject *py_arg_other = Py_None;

//     int arg_x_off;
//     int arg_y_off;
//     float arg_x_scale = 1.f;
//     float arg_y_scale = 1.f;
//     rectangle_t arg_roi;
//     PyObject *py_roi = Py_None;
//     int arg_rgb_channel = -1;
//     //  mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("-1 <= rgb_channel <= 2!"));
//     int arg_alpha = 256;
//     // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("0 <= alpha <= 256!"));

//     const uint16_t *color_palette = NULL;
//     const uint8_t *alpha_palette = NULL;
//     PyObject *py_color_palette = Py_None;
//     PyObject *py_alpha_palette = Py_None;
//     image_hint_t hint = 0;
//     int arg_x_size;
//     int arg_y_size;

//     static char *kwlist[] = {"image", "x", "y", "x_scale", "y_scale", "roi", "rgb_channel", "alpha", "color_palette", "alpha_palette", "hint", "x_size", "y_size", NULL};
//     if (!PyArg_ParseTupleAndKeywords(args, keywds, "Oii|ffOiiOOiiii", kwlist,
//                                      &py_arg_other, &arg_x_off, &arg_y_off, &arg_x_scale, &arg_y_scale, &py_roi, &arg_rgb_channel, &arg_alpha, &py_color_palette, &py_alpha_palette, &hint, &arg_x_size, &arg_y_size))
//         return NULL;
//     // Py_DECREF(py_color_palette);
//     // Py_DECREF(py_alpha_palette);
//     debug_line;

//     r24to_imgr16(py_arg_other, Py_None, Py_None, Py_None, &arg_other)

//         roi_tan(py_roi, &arg_roi, src_img->w, src_img->h);

//     if ((arg_alpha < 0) || (256 < arg_alpha))
//     {
//         debug_line;
//     }

//     if ((arg_rgb_channel < -1) || (2 < arg_rgb_channel))
//     {
//         debug_line;
//     }

//     bool got_y_scale = &arg_y_scale;
//     bool got_x_scale = &arg_x_scale;

//     bool got_x_size = &arg_x_size;

//     bool got_y_size = &arg_y_size;

//     if (got_x_scale && got_x_size)
//     {
//         debug_line;
//     }

//     //  mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Choose either x_scale or x_size not both!"));
//     if (got_y_scale && got_y_size)
//     {
//         debug_line;
//     }
//     // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Choose either y_scale or y_size not both!"));

//     if (got_x_size)
//         arg_x_scale = arg_x_size / ((float)arg_roi.w);
//     if (got_y_size)
//         arg_y_scale = arg_y_size / ((float)arg_roi.h);

//     if ((!got_x_scale) && (!got_x_size) && got_y_size)
//         arg_x_scale = arg_y_scale;
//     if ((!got_y_scale) && (!got_y_size) && got_x_size)
//         arg_y_scale = arg_x_scale;

//     fb_alloc_mark();
//     imlib_draw_image(arg_img, arg_other, arg_x_off, arg_y_off, arg_x_scale, arg_y_scale, &arg_roi,
//                      arg_rgb_channel, arg_alpha, color_palette, alpha_palette, hint, NULL, NULL);
//     fb_alloc_free_till_mark();
//     return Py_BuildValue("i", 0);
// }

// static PyObject *
// py_image_draw_image(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img = &self_image_img;
//     image_t *arg_other = py_helper_arg_to_image_mutable_bayer(args[1]);

//     const mp_obj_t *arg_vec;
//     uint offset = py_helper_consume_array(n_args, args, 2, 2, &arg_vec);
//     int arg_x_off = mp_obj_get_int(arg_vec[0]);
//     int arg_y_off = mp_obj_get_int(arg_vec[1]);

//     float arg_x_scale = 1.f;
//     bool got_x_scale = py_helper_keyword_float_maybe(n_args, args, offset + 0, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_scale), &arg_x_scale);

//     float arg_y_scale = 1.f;
//     bool got_y_scale = py_helper_keyword_float_maybe(n_args, args, offset + 1, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_scale), &arg_y_scale);

//     rectangle_t arg_roi;
//     py_helper_keyword_rectangle_roi(arg_other, n_args, args, offset + 2, kw_args, &arg_roi);

//     int arg_rgb_channel = py_helper_keyword_int(n_args, args, offset + 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_rgb_channel), -1);
//     if ((arg_rgb_channel < -1) || (2 < arg_rgb_channel)) mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("-1 <= rgb_channel <= 2!"));

//     int arg_alpha = py_helper_keyword_int(n_args, args, offset + 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_alpha), 256);
//     if ((arg_alpha < 0) || (256 < arg_alpha)) mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("0 <= alpha <= 256!"));

//     const uint16_t *color_palette = py_helper_keyword_color_palette(n_args, args, offset + 5, kw_args, NULL);
//     const uint8_t *alpha_palette = py_helper_keyword_alpha_palette(n_args, args, offset + 6, kw_args, NULL);

//     image_hint_t hint = py_helper_keyword_int(n_args, args, offset + 7, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_hint), 0);

//     int arg_x_size;
//     bool got_x_size = py_helper_keyword_int_maybe(n_args, args, offset + 8, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_size), &arg_x_size);

//     int arg_y_size;
//     bool got_y_size = py_helper_keyword_int_maybe(n_args, args, offset + 9, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_size), &arg_y_size);

//     if (got_x_scale && got_x_size) mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Choose either x_scale or x_size not both!"));
//     if (got_y_scale && got_y_size) mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Choose either y_scale or y_size not both!"));

//     if (got_x_size) arg_x_scale = arg_x_size / ((float) arg_roi.w);
//     if (got_y_size) arg_y_scale = arg_y_size / ((float) arg_roi.h);

//     if ((!got_x_scale) && (!got_x_size) && got_y_size) arg_x_scale = arg_y_scale;
//     if ((!got_y_scale) && (!got_y_size) && got_x_size) arg_y_scale = arg_x_scale;

//     fb_alloc_mark();
//     imlib_draw_image(arg_img, arg_other, arg_x_off, arg_y_off, arg_x_scale, arg_y_scale, &arg_roi,
//                      arg_rgb_channel, arg_alpha, color_palette, alpha_palette, hint, NULL, NULL);
//     fb_alloc_free_till_mark();
//     return args[0];
// }

// static PyObject *
// py_image_draw_keypoints(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img = &self_image_img;

//     int arg_c = 0xffffff;
//     int arg_s = 10 ;
//     int arg_thickness =1;
//     bool arg_fill =0;

//     if (MP_OBJ_IS_TYPE(args[1], &mp_type_tuple) || MP_OBJ_IS_TYPE(args[1], &mp_type_list)) {
//         size_t len;
//         mp_obj_t *items;
//         mp_obj_get_array(args[1], &len, &items);
//         for (size_t i = 0; i < len; i++) {
//             mp_obj_t *tuple;
//             mp_obj_get_array_fixed_n(items[i], 3, &tuple);
//             int cx = mp_obj_get_int(tuple[0]);
//             int cy = mp_obj_get_int(tuple[1]);
//             int angle = mp_obj_get_int(tuple[2]) % 360;
//             int si = sin_table[angle] * arg_s;
//             int co = cos_table[angle] * arg_s;
//             imlib_draw_line(arg_img, cx, cy, cx + co, cy + si, arg_c, arg_thickness);
//             imlib_draw_circle(arg_img, cx, cy, (arg_s - 2) / 2, arg_c, arg_thickness, arg_fill);
//         }
//     } else {
// #ifdef IMLIB_ENABLE_FIND_KEYPOINTS
//         py_kp_obj_t *kpts_obj = py_kpts_obj(args[1]);
//         for (int i = 0, ii = array_length(kpts_obj->kpts); i < ii; i++) {
//             kp_t *kp = array_at(kpts_obj->kpts, i);
//             int cx = kp->x;
//             int cy = kp->y;
//             int angle = kp->angle % 360;
//             int si = sin_table[angle] * arg_s;
//             int co = cos_table[angle] * arg_s;
//             imlib_draw_line(arg_img, cx, cy, cx + co, cy + si, arg_c, arg_thickness);
//             imlib_draw_circle(arg_img, cx, cy, (arg_s - 2) / 2, arg_c, arg_thickness, arg_fill);
//         }
// #else

// #endif // IMLIB_ENABLE_FIND_KEYPOINTS
//     }

//     return PyLong_FromLong(0);
// }
// #define IMLIB_ENABLE_FLOOD_FILL 1

#ifdef IMLIB_ENABLE_FLOOD_FILL
static PyObject *
py_image_flood_fill(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    PyObject *py_mask;

    int arg_x_off = 0;
    int arg_y_off = 0;

    float arg_seed_threshold = 0.05;

    float arg_floating_threshold = 0.05;

    int arg_c = 0xffffff;
    bool arg_invert = 0;
    bool clear_background = 0;

    image_t *arg_msk = NULL;

    static char *kwlist[] = {"x", "y", "seed_threshold", "floating_threshold", "color", "invert", "clear_background", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ii|ffiiiO", kwlist,
                                     &arg_x_off, &arg_y_off, &arg_seed_threshold, &arg_floating_threshold, &arg_c, &arg_invert, &clear_background, &py_mask))
        return NULL;

    fb_alloc_mark();
    imlib_flood_fill(arg_img, arg_x_off, arg_y_off,
                     arg_seed_threshold, arg_floating_threshold,
                     COLOR_R8_G8_B8_TO_RGB565((arg_c & 0xff0000) >> 16, (arg_c & 0xff00) >> 8, (arg_c & 0xff)), arg_invert, clear_background, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

#endif // IMLIB_ENABLE_FLOOD_FILL

static PyObject *
py_image_mask_rectangle(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    int arg_rx;
    int arg_ry;
    int arg_rw;
    int arg_rh;
    static char *kwlist[] = {"x", "y", "w", "h", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "[iiii]", kwlist,
                                     &arg_rx, &arg_ry, &arg_rw))
        return NULL;

    fb_alloc_mark();
    image_t temp;
    temp.w = arg_img->w;
    temp.h = arg_img->h;
    temp.bpp = IMAGE_BPP_BINARY;
    temp.data = fb_alloc0(image_size(&temp));

    imlib_draw_rectangle(&temp, arg_rx, arg_ry, arg_rw, arg_rh, -1, 0, true);
    imlib_zero(arg_img, &temp, true);

    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_mask_circle(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    int arg_cx;
    int arg_cy;
    int arg_cr;
    static char *kwlist[] = {"x", "y", "radius", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "[iii]", kwlist,
                                     &arg_cx, &arg_cy, &arg_cr))
        return NULL;

    fb_alloc_mark();
    image_t temp;
    temp.w = arg_img->w;
    temp.h = arg_img->h;
    temp.bpp = IMAGE_BPP_BINARY;
    temp.data = fb_alloc0(image_size(&temp));

    imlib_draw_circle(&temp, arg_cx, arg_cy, arg_cr, -1, 0, true);
    imlib_zero(arg_img, &temp, true);

    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
static PyObject *
py_image_mask_ellipse(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    int arg_cx;
    int arg_cy;
    int arg_rx;
    int arg_ry;
    int arg_r;
    static char *kwlist[] = {"x", "y", "radius_x", "radius_y", "rotation_angle_in_degrees", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iiiii", kwlist,
                                     &arg_cx, &arg_cy, &arg_rx, &arg_ry, &arg_r))
        return NULL;

    fb_alloc_mark();
    image_t temp;
    temp.w = arg_img->w;
    temp.h = arg_img->h;
    temp.bpp = IMAGE_BPP_BINARY;
    temp.data = fb_alloc0(image_size(&temp));

    imlib_draw_ellipse(&temp, arg_cx, arg_cy, arg_rx, arg_ry, arg_r, -1, 0, true);
    imlib_zero(arg_img, &temp, true);

    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}



static PyObject *
py_image_binary(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    list_t arg_thresholds;
    PyObject *py_arg_thresholds=Py_None ;


    bool arg_invert =false;
    bool arg_zero =false;
    image_t *arg_msk =NULL;
    PyObject *py_arg_msk;
    bool arg_to_bitmap =false;
    bool arg_copy =false;



    static char *kwlist[] = {"thresholds", "invert", "zero", "mask", "to_bitmap","copy",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|iiO", kwlist,
                                     &py_arg_thresholds, &arg_invert, &arg_zero, &py_arg_msk,&arg_to_bitmap,&arg_copy))
        return NULL;

    thresholds_tan(py_arg_thresholds, &arg_thresholds);
    r24to_imgr16(py_arg_msk, Py_None, Py_None, Py_None, arg_msk);




    if (arg_to_bitmap && (!arg_copy)) {
        switch(arg_img->bpp) {
            case IMAGE_BPP_GRAYSCALE: {
                // PY_ASSERT_TRUE_MSG((arg_img->w >= (sizeof(uint32_t)/sizeof(uint8_t))),
                //                    "Can't convert to bitmap in place!");
                break;
            }
            case IMAGE_BPP_RGB565: {
                // PY_ASSERT_TRUE_MSG((arg_img->w >= (sizeof(uint32_t)/sizeof(uint16_t))),
                //                    "Can't convert to bitmap in place!");
                break;
            }
            default: {
                break;
            }
        }
    }

    image_t out;
    out.w = arg_img->w;
    out.h = arg_img->h;
    out.bpp = arg_to_bitmap ? IMAGE_BPP_BINARY : arg_img->bpp;
    out.data = arg_copy ? xalloc(image_size(&out)) : arg_img->data;
    out.pix_ai = arg_copy ? xalloc(out.w*out.h*3) : arg_img->pix_ai;

    fb_alloc_mark();
    imlib_binary(&out, arg_img, &arg_thresholds, arg_invert, arg_zero, arg_msk);
    fb_alloc_free_till_mark();

    list_free(&arg_thresholds);

    if (arg_to_bitmap && (!arg_copy) && is_img_data_in_main_fb(out.data)) {
        MAIN_FB()->bpp = out.bpp;
    }

     PyObject *tmp = back_img(&out);
    xfree(out.pixels);
    return tmp;
}

#define INVERT

#ifdef INVERT
static PyObject *
py_image_invert(PyObject *self, PyObject *args)
{
    imlib_invert(&self_image_img);
    return PyLong_FromLong(0);
}
#endif

static PyObject *
py_image_b_and(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    image_t *img = NULL;
    image_t *arg_msk = NULL;
    PyObject *py_img;
    PyObject *py_arg_msk;

    static char *kwlist[] = {"image", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|O", kwlist,
                                     &py_img, &py_arg_msk))
        return NULL;

    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);

    r24to_imgr16(py_arg_msk, Py_None, Py_None, Py_None, arg_msk);

    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_b_and(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
    // }
    // else
    if (img)
    {
        imlib_b_and(arg_img, NULL, img, 0, arg_msk);
    }
    // else
    // {
    //     imlib_b_and(arg_img, NULL, NULL,
    //                 py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //                 arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}



static PyObject *
py_image_b_nand(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    image_t *img = NULL;
    image_t *arg_msk = NULL;
    PyObject *py_img;
    PyObject *py_arg_msk;

    static char *kwlist[] = {"image", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|O", kwlist,
                                     &py_img, &py_arg_msk))
        return NULL;

    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);

    r24to_imgr16(py_arg_msk, Py_None, Py_None, Py_None, arg_msk);



    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_b_nand(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_b_nand(arg_img, NULL, img, 0, arg_msk);
    }
    // else
    // {
    //     imlib_b_nand(arg_img, NULL, NULL,
    //                  py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //                  arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}





static PyObject *
py_image_b_or(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    image_t *img = NULL;
    image_t *arg_msk = NULL;
    PyObject *py_img;
    PyObject *py_arg_msk;

    static char *kwlist[] = {"image", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|O", kwlist,
                                     &py_img, &py_arg_msk))
        return NULL;

    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);

    r24to_imgr16(py_arg_msk, Py_None, Py_None, Py_None, arg_msk);


    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_b_or(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_b_or(arg_img, NULL, img, 0, arg_msk);
    }
    // else
    // {
    //     imlib_b_or(arg_img, NULL, NULL,
    //                py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //                arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}



static PyObject *
py_image_b_nor(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    image_t *img = NULL;
    image_t *arg_msk = NULL;
    PyObject *py_img;
    PyObject *py_arg_msk;

    static char *kwlist[] = {"image", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|O", kwlist,
                                     &py_img, &py_arg_msk))
        return NULL;

    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);

    r24to_imgr16(py_arg_msk, Py_None, Py_None, Py_None, arg_msk);



    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_b_nor(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_b_nor(arg_img, NULL, img, 0, arg_msk);
    }
    // else
    // {
    //     imlib_b_nor(arg_img, NULL, NULL,
    //                 py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //                 arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}

static PyObject *
py_image_b_xor(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    image_t *img = NULL;
    image_t *arg_msk = NULL;
    PyObject *py_img;
    PyObject *py_arg_msk;

    static char *kwlist[] = {"image", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|O", kwlist,
                                     &py_img, &py_arg_msk))
        return NULL;

    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);

    r24to_imgr16(py_arg_msk, Py_None, Py_None, Py_None, arg_msk);



    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_b_xor(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_b_xor(arg_img, NULL, img, 0, arg_msk);
    }
    // else
    // {
    //     imlib_b_xor(arg_img, NULL, NULL,
    //                 py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //                 arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}

static PyObject *
py_image_b_xnor(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    image_t *img = NULL;
    image_t *arg_msk = NULL;
    PyObject *py_img;
    PyObject *py_arg_msk;

    static char *kwlist[] = {"image", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|O", kwlist,
                                     &py_img, &py_arg_msk))
        return NULL;

    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);

    r24to_imgr16(py_arg_msk, Py_None, Py_None, Py_None, arg_msk);



    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_b_xnor(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_b_xnor(arg_img, NULL, img, 0, arg_msk);
    }
    // else
    // {
    //     imlib_b_xnor(arg_img, NULL, NULL,
    //                  py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //                  arg_msk);
    // }

    fb_alloc_free_till_mark();

    return  PyLong_FromLong(0);
}

static PyObject *
py_image_erode(PyObject *self, PyObject *args, PyObject* keywds)
{
    int arg_ksize ;
    int arg_threshold ;
        // py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold),
        //     py_helper_ksize_to_n(arg_ksize) - 1);
    image_t *arg_msk =NULL ;
        PyObject *py_arg_msk;
        // py_helper_keyword_to_image_mutable_mask(n_args, args, 3, kw_args);

    static char *kwlist[] = {"size", "threshold", "mask",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iO", kwlist,
                                     &arg_ksize, &arg_threshold,&py_arg_msk))
        return NULL;

    r24to_imgr16(py_arg_msk, Py_None, Py_None, Py_None, arg_msk);



    fb_alloc_mark();
    imlib_erode(&self_image_img, arg_ksize, arg_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}


static PyObject *
py_image_dilate(PyObject *self, PyObject *args, PyObject *keywds)
{
    int arg_ksize = 0;
    int arg_threshold = 0;
    image_t *arg_msk = NULL;
    PyObject *msk;

    static char *kwlist[] = {"size", "threshold", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iO", kwlist,
                                     &arg_ksize, &arg_threshold, &msk))
        return NULL;
r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);

    fb_alloc_mark();
    imlib_dilate(&self_image_img, arg_ksize, arg_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);

}

static PyObject *
py_image_open(PyObject * self, PyObject * args, PyObject * keywds)
{
    int arg_ksize = 0;
    int arg_threshold = 0;
    image_t *arg_msk = NULL;
    PyObject *msk;

    static char *kwlist[] = {"size", "threshold", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iO", kwlist,
                                     &arg_ksize, &arg_threshold, &msk))
        return NULL;
r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    fb_alloc_mark();
    imlib_open(&self_image_img, arg_ksize, arg_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_close(PyObject * self, PyObject * args, PyObject * keywds)
{
    int arg_ksize = 0;
    int arg_threshold = 0;
    image_t *arg_msk = NULL;
    PyObject *msk;

    static char *kwlist[] = {"size", "threshold", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iO", kwlist,
                                     &arg_ksize, &arg_threshold, &msk))
        return NULL;
r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    fb_alloc_mark();
    imlib_close(&self_image_img, arg_ksize, arg_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_top_hat(PyObject * self, PyObject * args, PyObject * keywds)
{
    int arg_ksize = 0;
    int arg_threshold = 0;
    image_t *arg_msk = NULL;
    PyObject *msk;

    static char *kwlist[] = {"size", "threshold", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iO", kwlist,
                                     &arg_ksize, &arg_threshold, &msk))
        return NULL;
r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    fb_alloc_mark();
    imlib_top_hat(&self_image_img, arg_ksize, arg_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_black_hat(PyObject * self, PyObject * args, PyObject * keywds)
{
    int arg_ksize = 0;
    int arg_threshold = 0;
    image_t *arg_msk = NULL;
    PyObject *msk;

    static char *kwlist[] = {"size", "threshold", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iO", kwlist,
                                     &arg_ksize, &arg_threshold, &msk))
        return NULL;
r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    fb_alloc_mark();
    imlib_black_hat(&self_image_img, arg_ksize, arg_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}







// static PyObject *
//     py_image_gamma_corr(PyObject * self, PyObject * args, PyObject * keywds)
// {
//     image_t *arg_img = &self_image_img;
//     float arg_gamma = 1.0f;
//     float arg_contrast = 1.0f float arg_brightness = 0.0f static char * kwlist[] = {"gamma", "contrast", "brightness", NULL};
//     if (!PyArg_ParseTupleAndKeywords(args, keywds, "|fff", kwlist,
//                                      &arg_gamma, &arg_contrast, &arg_brightness))
//         return NULL;

//     fb_alloc_mark();
//     imlib_gamma_corr(arg_img, arg_gamma, arg_contrast, arg_brightness);
//     fb_alloc_free_till_mark();
//     return PyLong_FromLong(0);
// }

static PyObject *
    py_image_negate(PyObject * self, PyObject * args)
{
    imlib_negate(&self_image_img);
    return PyLong_FromLong(0);
}


static PyObject *
py_image_replace(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    image_t *img = NULL;

    bool arg_hmirror =0;
    bool arg_vflip =0;
    bool arg_transpose =0;
    image_t *arg_msk = NULL;
    PyObject *py_img;
    PyObject *msk;

    static char *kwlist[] = {"image","hmirror","vflip", "mask",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|iiO", kwlist,
                             &py_img, &arg_hmirror,&arg_vflip,&msk)) return NULL;

    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);

    if (arg_transpose) {
        size_t size0 = image_size(arg_img);
        int w = arg_img->w;
        int h = arg_img->h;
        arg_img->w = h;
        arg_img->h = w;
        size_t size1 = image_size(arg_img);
        arg_img->w = w;
        arg_img->h = h;
        // PY_ASSERT_TRUE_MSG(size1 <= size0,
        //                    "Unable to transpose the image because it would grow in size!");
    }

    fb_alloc_mark();

    // mp_obj_t arg_1 = (n_args > 1) ? args[1] : args[0];

    // if (MP_OBJ_IS_STR(arg_1))
    // {
    //     imlib_replace(arg_img, mp_obj_str_get_str(arg_1), NULL, 0,
    //                   arg_hmirror, arg_vflip, arg_transpose, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_replace(arg_img, NULL, img, 0,
                      arg_hmirror, arg_vflip, arg_transpose, arg_msk);
    }
    // else
    // {
    //     imlib_replace(arg_img, NULL, NULL,
    //                   py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //                   arg_hmirror, arg_vflip, arg_transpose, arg_msk);
    // }

    fb_alloc_free_till_mark();

    if (is_img_data_in_main_fb(arg_img->data)) {
        MAIN_FB()->w = arg_img->w;
        MAIN_FB()->h = arg_img->h;
    }

    return PyLong_FromLong(0);
}


static PyObject *
py_image_add(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    image_t *arg_msk =NULL;
        image_t *img = NULL;
    PyObject *py_img;
    PyObject *msk;

    static char *kwlist[] = {"image", "mask",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|O", kwlist,
                             &py_img, &msk)) return NULL;

    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);




    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_add(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_add(arg_img, NULL, img, 0, arg_msk);
    }
    // else
    // {
    //     imlib_add(arg_img, NULL, NULL,
    //               py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //               arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}
static PyObject *
py_image_sub(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    bool arg_reverse = false;
    image_t *arg_msk = NULL;

    image_t *img = NULL;
    PyObject *py_img;
    PyObject *msk;

    static char *kwlist[] = {"image", "reverse", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|iO", kwlist,
                                     &py_img, &arg_reverse, &msk))
        return NULL;

    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);

    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_sub(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_reverse, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_sub(arg_img, NULL, img, 0, arg_reverse, arg_msk);
    }
    // else
    // {
    //     imlib_sub(arg_img, NULL, NULL,
    //               py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //               arg_reverse, arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}
static PyObject *
py_image_mul(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    bool arg_invert =false;
    image_t *arg_msk =NULL;


    image_t *img = NULL;
    PyObject *py_img;
    PyObject *msk;

    static char *kwlist[] = {"image", "invert", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|iO", kwlist,
                                     &py_img, &arg_invert, &msk))
        return NULL;

    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);



    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_mul(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_invert, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_mul(arg_img, NULL, img, 0, arg_invert, arg_msk);
    }
    // else
    // {
    //     imlib_mul(arg_img, NULL, NULL,
    //               py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //               arg_invert, arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}
static PyObject *
py_image_div(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    bool arg_invert =false;
    bool arg_mod = false;
    image_t *arg_msk =NULL;


    image_t *img = NULL;
    PyObject *py_img;
    PyObject *msk;

    static char *kwlist[] = {"image", "invert", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|iO", kwlist,
                                     &py_img, &arg_invert, &msk))
        return NULL;

    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);





    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_div(arg_img, mp_obj_str_get_str(args[1]), NULL, 0,
    //               arg_invert, arg_mod, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_div(arg_img, NULL, img, 0,
                  arg_invert, arg_mod, arg_msk);
    }
    // else
    // {
    //     imlib_div(arg_img, NULL, NULL,
    //               py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //               arg_invert, arg_mod, arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}
static PyObject *
py_image_min(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =
        &self_image_img;
    image_t *arg_msk =NULL;


    image_t *img = NULL;
    PyObject *py_img;
    PyObject *msk;

    static char *kwlist[] = {"image",  "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|O", kwlist,
                                     &py_img, &msk))
        return NULL;

    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);


    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_min(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_min(arg_img, NULL, img, 0, arg_msk);
    }
    // else
    // {
    //     imlib_min(arg_img, NULL, NULL,
    //               py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //               arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}
static PyObject *
py_image_max(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =
        &self_image_img;
    image_t *arg_msk =NULL;

    image_t *img = NULL;
    PyObject *py_img;
    PyObject *msk;

    static char *kwlist[] = {"image",  "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|O", kwlist,
                                     &py_img, &msk))
        return NULL;

    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);

    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_max(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_max(arg_img, NULL, img, 0, arg_msk);
    }
    // else
    // {
    //     imlib_max(arg_img, NULL, NULL,
    //               py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //               arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}
static PyObject *
py_image_difference(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =
        &self_image_img;
    image_t *arg_msk =NULL;


    image_t *img = NULL;
    PyObject *py_img;
    PyObject *msk;

    static char *kwlist[] = {"image",  "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|O", kwlist,
                                     &py_img, &msk))
        return NULL;

    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);

    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_difference(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_difference(arg_img, NULL, img, 0, arg_msk);
    }
    // else
    // {
    //     imlib_difference(arg_img, NULL, NULL,
    //                      py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //                      arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}
static PyObject *
py_image_blend(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =
        &self_image_img;
    float arg_alpha = 128.0f;
    //     py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_alpha), 128) / 256.0f;
    // PY_ASSERT_TRUE_MSG((0 <= arg_alpha) && (arg_alpha <= 1), "Error: 0 <= alpha <= 256!");
    image_t *arg_msk =NULL;
    
    image_t *img = NULL;
    PyObject *py_img;
    PyObject *msk;

    static char *kwlist[] = {"image", "alpha", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|fO", kwlist,
                                     &py_img,&arg_alpha,&msk))
        return NULL;

    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    r24to_imgr16(py_img, Py_None, Py_None, Py_None, img);

    fb_alloc_mark();

    // if (MP_OBJ_IS_STR(args[1]))
    // {
    //     imlib_blend(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_alpha, arg_msk);
    // }
    // else 
    if (img)
    {
        imlib_blend(arg_img, NULL, img, 0, arg_alpha, arg_msk);
    }
    // else
    // {
    //     imlib_blend(arg_img, NULL, NULL,
    //                 py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
    //                 arg_alpha, arg_msk);
    // }

    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}




static PyObject *
py_image_histeq(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    bool arg_adaptive = 0;
    float arg_clip_limit = -1;
    image_t *arg_msk = NULL;
    PyObject *msk;

    static char *kwlist[] = {"adaptive", "clip_limit", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|ifO", kwlist,
                                     &arg_adaptive, &arg_clip_limit, &msk))
        return NULL;
r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    fb_alloc_mark();
    if (arg_adaptive)
        imlib_clahe_histeq(arg_img, arg_clip_limit, arg_msk);
    else
        imlib_histeq(arg_img, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_mean(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
int arg_ksize = 0;
bool arg_threshold = 0;
int arg_offset = 0;
bool arg_invert = 0;
image_t *arg_msk = NULL;

PyObject *msk;

static char *kwlist[] = {"size", "threshold", "offset", "invert", "mask", NULL};
if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iiiO", kwlist,
                                 &arg_ksize, &arg_threshold, &arg_offset, &arg_invert, &msk))
    return NULL;

r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);


fb_alloc_mark();
imlib_mean_filter(arg_img, arg_ksize, arg_threshold, arg_offset, arg_invert, arg_msk);
fb_alloc_free_till_mark();
return PyLong_FromLong(0);
}

static PyObject *
py_image_median(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    int arg_ksize = 0;
    float arg_percentile = 0.5f;
    // PY_ASSERT_TRUE_MSG((0 <= arg_percentile) && (arg_percentile <= 1), "Error: 0 <= percentile <= 1!");
    // if ((0 <= arg_percentile) && (arg_percentile <= 1))
    //     return NULLL;
    bool arg_threshold = 0;
    int arg_offset = 0;
    bool arg_invert = 0;
    image_t *arg_msk = NULL;
    PyObject *msk;

    static char *kwlist[] = {"size", "percentile", "threshold", "offset", "invert", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ifiiiO", kwlist,
                                     &arg_ksize, &arg_percentile, &arg_threshold, &arg_offset, &arg_invert, &msk))
        return NULL;

r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);

    fb_alloc_mark();
    imlib_median_filter(arg_img, arg_ksize, arg_percentile, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_mode(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    int arg_ksize = 0;
    bool arg_threshold = 0;
    int arg_offset = 0;
    bool arg_invert = 0;
    image_t *arg_msk = NULL;
    PyObject *msk;

    static char *kwlist[] = {"size", "threshold", "offset", "invert", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iiiO", kwlist,
                                     &arg_ksize, &arg_threshold, &arg_offset, &arg_invert, &arg_invert, &msk))
        return NULL;
r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    fb_alloc_mark();
    imlib_mode_filter(arg_img, arg_ksize, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_midpoint(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    int arg_ksize = 0;
    float arg_bias = 0.5f;
    // PY_ASSERT_TRUE_MSG((0 <= arg_bias) && (arg_bias <= 1), "Error: 0 <= bias <= 1!");

    bool arg_threshold = 0;
    int arg_offset = 0;
    bool arg_invert = 0;
    image_t *arg_msk = NULL;

    PyObject *msk;

    static char *kwlist[] = {"size", "bias", "threshold", "offset", "invert", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|fiiiO", kwlist,
                                     &arg_ksize, &arg_bias, &arg_threshold, &arg_offset, &arg_invert, &msk))
        return NULL;

r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);

    // if((0 <= arg_bias) && (arg_bias <= 1)) return NULL;
    fb_alloc_mark();
    imlib_midpoint_filter(arg_img, arg_ksize, arg_bias, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
/*
static PyObject *
py_image_morph(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    int arg_ksize =0;
    int n = 0;
    fb_alloc_mark();

    if (arg_m == 0) {
        arg_m = 1;
    }
    float arg_mul =1.0f / arg_m;
    float arg_add =0.0f;
    bool arg_threshold = false;
    int arg_offset =0;
    bool arg_invert = false;
    image_t *arg_msk =NULL;
    int arg_m = 0;
int *arg_krn = fb_alloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arg_krn[i] = mp_obj_get_int(krn[i]);
        arg_m += arg_krn[i];
    }
    imlib_morph(arg_img, arg_ksize, arg_krn, arg_mul, arg_add, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return args[0];
}
*/


static PyObject *
py_image_gaussian(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    int arg_ksize =0;


    float arg_add =0.0f;
    bool arg_threshold =0;
    int arg_offset =0;
    bool arg_invert =0;
    bool unsharp = 0;
    image_t *arg_msk =NULL;

    int arg_m = 0;
    float arg_mul =0;
    PyObject *msk;

    static char *kwlist[] = {"size", "unsharp", "mul","add","threshold", "offset", "invert", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iffiiiO", kwlist,
                                     &arg_ksize, &unsharp, &arg_mul,&arg_add,&arg_threshold, &arg_offset, &arg_invert, &msk))
        return NULL;

    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);


    int k_2 = arg_ksize * 2;
    int n = k_2 + 1;

    fb_alloc_mark();

    int *pascal = fb_alloc(n * sizeof(int));
    pascal[0] = 1;

    for (int i = 0; i < k_2; i++) { // Compute a row of pascal's triangle.
        pascal[i + 1] = (pascal[i] * (k_2 - i)) / (i + 1);
    }

    int *arg_krn = fb_alloc(n * n * sizeof(int));


    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int temp = pascal[i] * pascal[j];
            arg_krn[(i * n) + j] = temp;
            arg_m += temp;
        }
    }
    if (unsharp) {
        arg_krn[((n/2)*n)+(n/2)] -= arg_m * 2;
        arg_m = -arg_m;
    }
     arg_mul = arg_mul ?  arg_mul:1.0f / arg_m;

    imlib_morph(arg_img, arg_ksize, arg_krn, arg_mul, arg_add, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}





static PyObject *
py_image_laplacian(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    int arg_ksize =0;
    int k_2 = arg_ksize * 2;
    int n = k_2 + 1;
    float arg_add =0.0f;
    bool arg_threshold =false;
    int arg_offset =0;
    bool arg_invert =false;
    image_t *arg_msk =NULL;
float arg_mul =0;
bool arg_sharpen = false;


    PyObject *msk;

    static char *kwlist[] = {"size", "sharpen", "mul","add","threshold", "offset", "invert", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iffiiiO", kwlist,
                                     &arg_ksize, &arg_sharpen, &arg_mul,&arg_add,&arg_threshold, &arg_offset, &arg_invert, &msk))
        return NULL;

    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);



    fb_alloc_mark();

    int *pascal = fb_alloc(n * sizeof(int));
    pascal[0] = 1;

    for (int i = 0; i < k_2; i++) { // Compute a row of pascal's triangle.
        pascal[i + 1] = (pascal[i] * (k_2 - i)) / (i + 1);
    }

    int *arg_krn = fb_alloc(n * n * sizeof(int));
    int arg_m = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int temp = pascal[i] * pascal[j];
            arg_krn[(i * n) + j] = -temp;
            arg_m += temp;
        }
    }

    arg_krn[((n/2)*n)+(n/2)] += arg_m;
    arg_m = arg_krn[((n/2)*n)+(n/2)];

    if (arg_sharpen) {
        arg_krn[((n/2)*n)+(n/2)] += arg_m;
    }

    arg_mul = arg_mul ?  arg_mul:1.0f / arg_m;


    imlib_morph(arg_img, arg_ksize, arg_krn, arg_mul, arg_add, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
static PyObject *py_image_bilateral(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    int arg_ksize = 0;
    float arg_color_sigma = 0.1;
    float arg_space_sigma = 1;
    bool arg_threshold = 0;
    int arg_offset = 0;
    bool arg_invert = 0;
    image_t *arg_msk = NULL;

    PyObject *msk;

    static char *kwlist[] = {"size", "color_sigma", "space_sigma", "threshold", "offset", "invert", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|ffiiiO", kwlist,
                                     &arg_ksize, &arg_color_sigma, &arg_space_sigma, &arg_threshold, &arg_offset, &arg_invert, &msk))
        return NULL;
    r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);

    fb_alloc_mark();
    imlib_bilateral_filter(arg_img, arg_ksize, arg_color_sigma, arg_space_sigma, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_cartoon(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    float arg_seed_threshold = 0.05;
    // PY_ASSERT_TRUE_MSG((0.0f <= arg_seed_threshold) && (arg_seed_threshold <= 1.0f),
    //                    "Error: 0.0 <= seed_threshold <= 1.0!");
    float arg_floating_threshold = 0.05;
    // PY_ASSERT_TRUE_MSG((0.0f <= arg_floating_threshold) && (arg_floating_threshold <= 1.0f),
    //                    "Error: 0.0 <= floating_threshold <= 1.0!");
    image_t *arg_msk = NULL;

    PyObject *msk;

    static char *kwlist[] = {"seed_threshold", "floating_threshold", "mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|ffO", kwlist,
                                     &arg_seed_threshold, &arg_floating_threshold, &msk))
        return NULL;
r24to_imgr16(msk, Py_None, Py_None, Py_None, arg_msk);
    fb_alloc_mark();
    imlib_cartoon_filter(arg_img, arg_seed_threshold, arg_floating_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
/*
static PyObject *
py_image_remove_shadows(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    fb_alloc_mark();

    if (n_args < 2) {
        imlib_remove_shadows(arg_img, NULL, NULL, 0, true);
    } else if (MP_OBJ_IS_STR(args[1])) {
        imlib_remove_shadows(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, false);
    } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
        imlib_remove_shadows(arg_img, NULL, py_helper_arg_to_image_color(args[1]), 0, false);
    } else {
        imlib_remove_shadows(arg_img, NULL, NULL,
                             py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
                             false);
    }
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
*/

static PyObject *
py_image_chrominvar(PyObject *self, PyObject *args)
{
    fb_alloc_mark();
    imlib_chrominvar(&self_image_img);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_illuminvar(PyObject *self, PyObject *args)
{
    fb_alloc_mark();
    imlib_illuminvar(&self_image_img);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}









static PyObject *
py_image_linpola(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    // PY_ASSERT_FALSE_MSG(arg_img->w % 2, "Width must be even!");
    // PY_ASSERT_FALSE_MSG(arg_img->h % 2, "Height must be even!");
    bool arg_reverse =0;

    static char *kwlist[] = {"reverse",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist,
                              &arg_reverse)) return NULL;

    fb_alloc_mark();
    imlib_logpolar(arg_img, true, arg_reverse);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_logpolar(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    // PY_ASSERT_FALSE_MSG(arg_img->w % 2, "Width must be even!");
    // PY_ASSERT_FALSE_MSG(arg_img->h % 2, "Height must be even!");
    bool arg_reverse =0;
    static char *kwlist[] = {"reverse",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist,
                              &arg_reverse)) return NULL;

    fb_alloc_mark();
    imlib_logpolar(arg_img, false, arg_reverse);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}


static PyObject * 
py_image_lens_corr(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img =&self_image_img;
     float arg_strength =1.8f;
    // PY_ASSERT_TRUE_MSG(arg_strength > 0.0f, "Strength must be > 0!");
    float arg_zoom = 1.0f;
    // PY_ASSERT_TRUE_MSG(arg_zoom > 0.0f, "Zoom must be > 0!");
    static char *kwlist[] = {"strength","zoom",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|ff", kwlist,
                              &arg_strength,&arg_zoom)) return NULL;

    fb_alloc_mark();
    imlib_lens_corr(arg_img, arg_strength, arg_zoom);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}






static PyObject *
py_image_rotation_corr(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    float arg_x_rotation = 0.0f;
    float arg_y_rotation = 0.0f;
    float arg_z_rotation = 0.0f;
    float arg_x_translation = 0.0f;
    float arg_y_translation = 0.0f;
    float arg_zoom = 1.0f;
    // PY_ASSERT_TRUE_MSG(arg_zoom > 0.0f, "Zoom must be > 0!");
    float arg_fov = 60.0f;
    // PY_ASSERT_TRUE_MSG((0.0f < arg_fov) && (arg_fov < 180.0f), "FOV must be > 0 and < 180!");
    float *arg_corners = 0;

    static char *kwlist[] = {"x_rotation", "y_rotation", "z_rotation", "x_translation", "y_translation", "zoom","fov","corners", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|fffffff", kwlist,
                                     &arg_x_rotation, &arg_y_rotation, &arg_z_rotation, &arg_x_translation, &arg_y_translation, &arg_zoom,&arg_fov,&arg_corners))
        return NULL;

    fb_alloc_mark();
    imlib_rotation_corr(arg_img,
                        arg_x_rotation, arg_y_rotation, arg_z_rotation,
                        arg_x_translation, arg_y_translation,
                        arg_zoom, arg_fov, arg_corners);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

/*
static PyObject *
py_image_get_similarity(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    float avg, std, min, max;

    fb_alloc_mark();

    if (MP_OBJ_IS_STR(other_obj))
    {
        imlib_get_similarity(arg_img, mp_obj_str_get_str(other_obj), NULL, 0, &avg, &std, &min, &max);
    }
    else if (MP_OBJ_IS_TYPE(other_obj, &py_image_type))
    {
        imlib_get_similarity(arg_img, NULL, py_helper_arg_to_image_mutable(other_obj), 0, &avg, &std, &min, &max);
    }
    else
    {
        imlib_get_similarity(arg_img, NULL, NULL,
                             py_helper_keyword_color(arg_img, 1, &other_obj, 0, NULL, 0),
                             &avg, &std, &min, &max);
    }

    fb_alloc_free_till_mark();

    py_similarity_obj_t *o = m_new_obj(py_similarity_obj_t);
    o->base.type = &py_similarity_type;
    o->avg = mp_obj_new_float(avg);
    o->std = mp_obj_new_float(std);
    o->min = mp_obj_new_float(min);
    o->max = mp_obj_new_float(max);
    return o;
}
*/

// static PyObject *
// py_image_get_histogram(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img = &self_image_img;

//     list_t thresholds;
//     PyObject *py_thresholds;
//     // list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
//     bool invert = 0;
//     image_t *other = NULL;
//     rectangle_t roi;
//     PyObject *py_roi = Py_None;
//     histogram_t hist;
//     PyObject *py_bins;
//     PyObject *py_l_bins;
//     PyObject *py_a_bins;
//     PyObject *py_b_bins;

//     switch(arg_img->bpp) {
//         case IMAGE_BPP_BINARY: {
//             // int bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
//             //                                  (COLOR_BINARY_MAX-COLOR_BINARY_MIN+1));
//             // PY_ASSERT_TRUE_MSG(bins >= 2, "bins must be >= 2");
//             // hist.LBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), bins);
//             // PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
//             // hist.ABinCount = 0;
//             // hist.BBinCount = 0;
//             // fb_alloc_mark();
//             // hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
//             // hist.ABins = NULL;
//             // hist.BBins = NULL;
//             // imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert, other);
//             // list_free(&thresholds);
//             break;
//         }
//         case IMAGE_BPP_GRAYSCALE: {
//             // int bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
//             //                                  (COLOR_GRAYSCALE_MAX-COLOR_GRAYSCALE_MIN+1));
//             // PY_ASSERT_TRUE_MSG(bins >= 2, "bins must be >= 2");
//             // hist.LBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), bins);
//             // PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
//             // hist.ABinCount = 0;
//             // hist.BBinCount = 0;
//             // fb_alloc_mark();
//             // hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
//             // hist.ABins = NULL;
//             // hist.BBins = NULL;
//             // imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert, other);
//             // list_free(&thresholds);
//             break;
//         }
//         case IMAGE_BPP_RGB565: {
//             // int l_bins = COLOR_L_MAX-COLOR_L_MIN+1;
//             // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
//             //                                    (COLOR_L_MAX-COLOR_L_MIN+1));
//             // PY_ASSERT_TRUE_MSG(l_bins >= 2, "bins must be >= 2");
//             hist.LBinCount = COLOR_L_MAX-COLOR_L_MIN+1;
//             // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), l_bins);
//             // PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
//             // int a_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
//             //                                    (COLOR_A_MAX-COLOR_A_MIN+1));
//             // PY_ASSERT_TRUE_MSG(a_bins >= 2, "bins must be >= 2");
//             hist.ABinCount = COLOR_A_MAX-COLOR_A_MIN+1;
//             // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_a_bins), a_bins);
//             // PY_ASSERT_TRUE_MSG(hist.ABinCount >= 2, "a_bins must be >= 2");
//             // int b_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
//             //                                    (COLOR_B_MAX-COLOR_B_MIN+1));
//             // PY_ASSERT_TRUE_MSG(b_bins >= 2, "bins must be >= 2");
//             hist.BBinCount = COLOR_B_MAX-COLOR_B_MIN+1;
//             // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_b_bins), b_bins);
//             // PY_ASSERT_TRUE_MSG(hist.BBinCount >= 2, "b_bins must be >= 2");

//             static char *kwlist[] = {"thresholds","invert","roi","bins","l_bins","a_bins","b_bins",NULL};
//             if (!PyArg_ParseTupleAndKeywords(args, keywds, "|OiOOOOO", kwlist,
//                                     &py_thresholds,&invert,&py_roi,&py_bins,&py_l_bins,&py_a_bins,&py_b_bins)) return NULL;

//             fb_alloc_mark();
//             hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
//             hist.ABins = fb_alloc(hist.ABinCount * sizeof(float), FB_ALLOC_NO_HINT);
//             hist.BBins = fb_alloc(hist.BBinCount * sizeof(float), FB_ALLOC_NO_HINT);
//             imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert, other);
//             list_free(&thresholds);
//             break;
//         }
//         default: {
//             return MP_OBJ_NULL;
//         }
//     }

//     py_histogram_obj_t *o = m_new_obj(py_histogram_obj_t);
//     o->base.type = &py_histogram_type;
//     o->bpp = arg_img->bpp;

//     o->LBins = mp_obj_new_list(hist.LBinCount, NULL);
//     o->ABins = mp_obj_new_list(hist.ABinCount, NULL);
//     o->BBins = mp_obj_new_list(hist.BBinCount, NULL);

//     for (int i = 0; i < hist.LBinCount; i++) {
//         ((mp_obj_list_t *) o->LBins)->items[i] = mp_obj_new_float(hist.LBins[i]);
//     }

//     for (int i = 0; i < hist.ABinCount; i++) {
//         ((mp_obj_list_t *) o->ABins)->items[i] = mp_obj_new_float(hist.ABins[i]);
//     }

//     for (int i = 0; i < hist.BBinCount; i++) {
//         ((mp_obj_list_t *) o->BBins)->items[i] = mp_obj_new_float(hist.BBins[i]);
//     }

//     fb_alloc_free_till_mark();

//     return o;

// }


#ifdef IMLIB_ENABLE_GET_STATISTICS
// static PyObject *
// py_histogram_get_statistics(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     histogram_t hist;
//     hist.LBinCount = 0;
//     hist.ABinCount = 0;
//     hist.BBinCount = 0;
//     fb_alloc_mark();
//     hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
//     hist.ABins = fb_alloc(hist.ABinCount * sizeof(float), FB_ALLOC_NO_HINT);
//     hist.BBins = fb_alloc(hist.BBinCount * sizeof(float), FB_ALLOC_NO_HINT);
// PyObject *

//     for (int i = 0; i < hist.LBinCount; i++)
//     {
//         hist.LBins[i] = mp_obj_get_float(((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->LBins)->items[i]);
//     }

//     for (int i = 0; i < hist.ABinCount; i++)
//     {
//         hist.ABins[i] = mp_obj_get_float(((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->ABins)->items[i]);
//     }

//     for (int i = 0; i < hist.BBinCount; i++)
//     {
//         hist.BBins[i] = mp_obj_get_float(((mp_obj_list_t *)((py_histogram_obj_t *)self_in)->BBins)->items[i]);
//     }

//     statistics_t stats;
//     imlib_get_statistics(&stats, ((py_histogram_obj_t *)self_in)->bpp, &hist);
//     fb_alloc_free_till_mark();

//     py_statistics_obj_t *o = m_new_obj(py_statistics_obj_t);
//     o->base.type = &py_statistics_type;
//     o->bpp = ((py_histogram_obj_t *)self_in)->bpp;

//     o->LMean = mp_obj_new_int(stats.LMean);
//     o->LMedian = mp_obj_new_int(stats.LMedian);
//     o->LMode = mp_obj_new_int(stats.LMode);
//     o->LSTDev = mp_obj_new_int(stats.LSTDev);
//     o->LMin = mp_obj_new_int(stats.LMin);
//     o->LMax = mp_obj_new_int(stats.LMax);
//     o->LLQ = mp_obj_new_int(stats.LLQ);
//     o->LUQ = mp_obj_new_int(stats.LUQ);
//     o->AMean = mp_obj_new_int(stats.AMean);
//     o->AMedian = mp_obj_new_int(stats.AMedian);
//     o->AMode = mp_obj_new_int(stats.AMode);
//     o->ASTDev = mp_obj_new_int(stats.ASTDev);
//     o->AMin = mp_obj_new_int(stats.AMin);
//     o->AMax = mp_obj_new_int(stats.AMax);
//     o->ALQ = mp_obj_new_int(stats.ALQ);
//     o->AUQ = mp_obj_new_int(stats.AUQ);
//     o->BMean = mp_obj_new_int(stats.BMean);
//     o->BMedian = mp_obj_new_int(stats.BMedian);
//     o->BMode = mp_obj_new_int(stats.BMode);
//     o->BSTDev = mp_obj_new_int(stats.BSTDev);
//     o->BMin = mp_obj_new_int(stats.BMin);
//     o->BMax = mp_obj_new_int(stats.BMax);
//     o->BLQ = mp_obj_new_int(stats.BLQ);
//     o->BUQ = mp_obj_new_int(stats.BUQ);

//     return o;
// }

static PyObject *
py_image_get_statistics(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    list_t thresholds;
    PyObject *py_thresholds = Py_None;
    bool invert = 0;
    image_t *other = NULL;

    rectangle_t roi;
    PyObject *py_roi = Py_None;
    PyObject *py_bins = Py_None;
    PyObject *py_l_bins = Py_None;
    PyObject *py_a_bins = Py_None;
    PyObject *py_b_bins = Py_None;

    static char *kwlist[] = {"thresholds", "invert", "roi", "bins", "l_bins", "a_bins", "b_bins", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|OiOOOOO", kwlist,
                                     &py_thresholds, &invert, &py_roi, &py_bins, &py_l_bins, &py_a_bins, &py_b_bins))
        return NULL;
    debug_line;
    thresholds_tan(py_thresholds, &thresholds);
    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    histogram_t hist;

    switch (arg_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {
        int bins = PyLong_Check(py_bins) ? PyLong_AsLong(py_bins) : (COLOR_BINARY_MAX - COLOR_BINARY_MIN + 1);
        // PY_ASSERT_TRUE_MSG(bins >= 2, "bins must be >= 2");
        hist.LBinCount = PyLong_Check(py_l_bins) ? PyLong_AsLong(py_l_bins) : bins;
        // PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
        hist.ABinCount = 0;
        hist.BBinCount = 0;
        fb_alloc_mark();
        hist.LBins = fb_alloc(hist.LBinCount * sizeof(float));
        hist.ABins = NULL;
        hist.BBins = NULL;
        imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert);
        list_free(&thresholds);
        break;
    }
    case IMAGE_BPP_GRAYSCALE:
    {
        int bins = PyLong_Check(py_bins) ? PyLong_AsLong(py_bins) : (COLOR_GRAYSCALE_MAX - COLOR_GRAYSCALE_MIN + 1);
        // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
        //                                  (COLOR_GRAYSCALE_MAX - COLOR_GRAYSCALE_MIN + 1));
        // PY_ASSERT_TRUE_MSG(bins >= 2, "bins must be >= 2");
        hist.LBinCount = PyLong_Check(py_l_bins) ? PyLong_AsLong(py_l_bins) : bins;
        // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), bins);
        // PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
        hist.ABinCount = 0;
        hist.BBinCount = 0;
        fb_alloc_mark();
        hist.LBins = fb_alloc(hist.LBinCount * sizeof(float));
        hist.ABins = NULL;
        hist.BBins = NULL;
        imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert);
        list_free(&thresholds);
        break;
    }
    case IMAGE_BPP_RGB565:
    {
        int l_bins = PyLong_Check(py_bins) ? PyLong_AsLong(py_bins) : (COLOR_L_MAX - COLOR_L_MIN + 1);
        // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
        //                                    (COLOR_L_MAX - COLOR_L_MIN + 1));
        // PY_ASSERT_TRUE_MSG(l_bins >= 2, "bins must be >= 2");
        hist.LBinCount = PyLong_Check(py_l_bins) ? PyLong_AsLong(py_l_bins) : l_bins;
        // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), l_bins);
        // PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");

        int a_bins = PyLong_Check(py_bins) ? PyLong_AsLong(py_bins) : (COLOR_A_MAX - COLOR_A_MIN + 1);
        // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
        //                                    (COLOR_A_MAX - COLOR_A_MIN + 1));
        // PY_ASSERT_TRUE_MSG(a_bins >= 2, "bins must be >= 2");
        hist.ABinCount = PyLong_Check(py_a_bins) ? PyLong_AsLong(py_a_bins) : a_bins;
        // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_a_bins), a_bins);
        // PY_ASSERT_TRUE_MSG(hist.ABinCount >= 2, "a_bins must be >= 2");

        int b_bins = PyLong_Check(py_bins) ? PyLong_AsLong(py_bins) : (COLOR_B_MAX - COLOR_B_MIN + 1);
        // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
        //                                    (COLOR_B_MAX - COLOR_B_MIN + 1));
        // PY_ASSERT_TRUE_MSG(b_bins >= 2, "bins must be >= 2");
        hist.BBinCount = PyLong_Check(py_b_bins) ? PyLong_AsLong(py_b_bins) : b_bins;
        // py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_b_bins), b_bins);
        // PY_ASSERT_TRUE_MSG(hist.BBinCount >= 2, "b_bins must be >= 2");
        fb_alloc_mark();
        hist.LBins = fb_alloc(hist.LBinCount * sizeof(float));
        hist.ABins = fb_alloc(hist.ABinCount * sizeof(float));
        hist.BBins = fb_alloc(hist.BBinCount * sizeof(float));
        imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert);
        list_free(&thresholds);
        break;
    }
    default:
    {
        return Py_None;
    }
    }
    statistics_t stats;
    imlib_get_statistics(&stats, arg_img->bpp, &hist);
    fb_alloc_free_till_mark();

    PyObject *o = PyDict_New();
    PyObject *tmp1;
    PyObject *tmp2;

    tmp1 = Py_BuildValue("s", "LMean");
    tmp2 = Py_BuildValue("i", stats.LMean);

    PyDict_SetItem(o, tmp1, tmp2);
    Py_DECREF(tmp1);
    Py_DECREF(tmp2);

    tmp1 = Py_BuildValue("s", "LMedian");
    tmp2 = Py_BuildValue("i", stats.LMedian);

    PyDict_SetItem(o, tmp1, tmp2);
    Py_DECREF(tmp1);
    Py_DECREF(tmp2);
    tmp1 = Py_BuildValue("s", "LMode");
    tmp2 = Py_BuildValue("i", stats.LMode);

    PyDict_SetItem(o, tmp1, tmp2);
    Py_DECREF(tmp1);
    Py_DECREF(tmp2);

    PyDict_SetItem(o, Py_BuildValue("s", "LSTDev"), Py_BuildValue("i", stats.LSTDev));
    PyDict_SetItem(o, Py_BuildValue("s", "LMin"), Py_BuildValue("i", stats.LMin));
    PyDict_SetItem(o, Py_BuildValue("s", "LMax"), Py_BuildValue("i", stats.LMax));
    PyDict_SetItem(o, Py_BuildValue("s", "LLQ"), Py_BuildValue("i", stats.LLQ));
    PyDict_SetItem(o, Py_BuildValue("s", "LUQ"), Py_BuildValue("i", stats.LUQ));
    PyDict_SetItem(o, Py_BuildValue("s", "ASTDev"), Py_BuildValue("i", stats.ASTDev));
    PyDict_SetItem(o, Py_BuildValue("s", "AMin"), Py_BuildValue("i", stats.AMin));
    PyDict_SetItem(o, Py_BuildValue("s", "AMax"), Py_BuildValue("i", stats.AMax));
    PyDict_SetItem(o, Py_BuildValue("s", "ALQ"), Py_BuildValue("i", stats.ALQ));
    PyDict_SetItem(o, Py_BuildValue("s", "BMean"), Py_BuildValue("i", stats.BMean));
    PyDict_SetItem(o, Py_BuildValue("s", "BMedian"), Py_BuildValue("i", stats.BMedian));
    PyDict_SetItem(o, Py_BuildValue("s", "BMode"), Py_BuildValue("i", stats.BMode));
    PyDict_SetItem(o, Py_BuildValue("s", "BSTDev"), Py_BuildValue("i", stats.BSTDev));
    PyDict_SetItem(o, Py_BuildValue("s", "BMin"), Py_BuildValue("i", stats.BMin));
    PyDict_SetItem(o, Py_BuildValue("s", "BMax"), Py_BuildValue("i", stats.BMax));
    PyDict_SetItem(o, Py_BuildValue("s", "BLQ"), Py_BuildValue("i", stats.BLQ));
    PyDict_SetItem(o, Py_BuildValue("s", "BUQ"), Py_BuildValue("i", stats.BUQ));

    return o;
}

#endif //IMLIB_ENABLE_GET_STATISTICS

/*
static PyObject *
py_image_find_template(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    image_t *arg_template = py_helper_arg_to_image_grayscale(args[1]);
    float arg_thresh = mp_obj_get_float(args[2]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 3, kw_args, &roi);

    // Make sure ROI is bigger than or equal to template size
    PY_ASSERT_TRUE_MSG((roi.w >= arg_template->w && roi.h >= arg_template->h),
            "Region of interest is smaller than template!");

    // Make sure ROI is smaller than or equal to image size
    PY_ASSERT_TRUE_MSG(((roi.x + roi.w) <= arg_img->w && (roi.y + roi.h) <= arg_img->h),
            "Region of interest is bigger than image!");

    int step = py_helper_keyword_int(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_step), 2);
    int search = py_helper_keyword_int(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_search), SEARCH_EX);

    // Find template
    rectangle_t r;
    float corr;
    fb_alloc_mark();
    if (search == SEARCH_DS) {
        corr = imlib_template_match_ds(arg_img, arg_template, &r);
    } else {
        corr = imlib_template_match_ex(arg_img, arg_template, &roi, step, &r);
    }
    fb_alloc_free_till_mark();

    if (corr > arg_thresh) {
        mp_obj_t rec_obj[4] = {
            mp_obj_new_int(r.x),
            mp_obj_new_int(r.y),
            mp_obj_new_int(r.w),
            mp_obj_new_int(r.h)
        };
        return mp_obj_new_tuple(4, rec_obj);
    }
    return mp_const_none;
}
*/

/*
static PyObject *
py_image_find_features(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    cascade_t *cascade ;
    PyObject *py_cascade;
    cascade->threshold = 0.5f;
    cascade->scale_factor = 1.5f;

    rectangle_t roi;
    PyObject *py_roi = Py_None;

    static char *kwlist[] = {"cascade", "threshold","scale","roi", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|ffO", kwlist,
									 &py_roi, &threshold,&normalized,&scale_factor,&max_keypoints,&corner_d)) return NULL;

     roi_tan(py_roi,&roi,arg_img->w,int arg_img->h);
    // Make sure ROI is bigger than feature size
    // PY_ASSERT_TRUE_MSG((roi.w > cascade->window.w && roi.h > cascade->window.h),
    //         "Region of interest is smaller than detector window!");

    // Detect objects
    fb_alloc_mark();
    array_t *objects_array = imlib_detect_objects(arg_img, cascade, &roi);
    fb_alloc_free_till_mark();

    // Add detected objects to a new Python list...
    mp_obj_t objects_list = mp_obj_new_list(0, NULL);
    for (int i=0; i<array_length(objects_array); i++) {
        rectangle_t *r = array_at(objects_array, i);
        mp_obj_t rec_obj[4] = {
            mp_obj_new_int(r->x),
            mp_obj_new_int(r->y),
            mp_obj_new_int(r->w),
            mp_obj_new_int(r->h),
        };
        mp_obj_list_append(objects_list, mp_obj_new_tuple(4, rec_obj));
    }
    array_free(objects_array);
    return objects_list;
}
*/
static PyObject *
py_image_find_eye(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;
    PyObject *py_roi = Py_None;
    static char *kwlist[] = {"roi", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &py_roi))
        return NULL;
    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    point_t iris;
    imlib_find_iris(arg_img, &iris, &roi);

    return Py_BuildValue("ii", iris.x, iris.y);
}
/*
static PyObject *
py_image_find_lbp(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;
    PyObject *py_roi

        static char *kwlist[] = {"roi", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &py_roi))
        return NULL;
    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    uint8_t *hist = imlib_lbp_desc(arg_img, &roi);
    return lbp_obj;
}
*/
/*
static PyObject *
py_image_find_keypoints(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;
    PyObject *py_roi = Py_None;

    int threshold = 20;
    bool normalized = 0;
    float scale_factor = 1.5f;
    int max_keypoints = 100;
    corner_detector_t corner_detector = CORNER_AGAST;
    int corner_d = 1;

    static char *kwlist[] = {"roi", "threshold", "normalized", "scale_factor", "max_keypoints", "corner_detector", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|Oiifii", kwlist,
                                     &py_roi, &threshold, &normalized, &scale_factor, &max_keypoints, &corner_d))
        return NULL;

    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);
    if (corner_d == 0)
    {
        corner_detector = CORNER_FAST;
    }
    else
    {
        corner_detector = CORNER_AGAST;
    }

#ifndef IMLIB_ENABLE_FAST
    // Force AGAST when FAST is disabled.
    corner_detector = CORNER_AGAST;
#endif

    // Find keypoints
    fb_alloc_mark();
    array_t *kpts = orb_find_keypoints(arg_img, normalized, threshold, scale_factor, max_keypoints, corner_detector, &roi);
    fb_alloc_free_till_mark();

    if (array_length(kpts))
    {
        py_kp_obj_t *kp_obj = m_new_obj(py_kp_obj_t);

        kp_obj->base.type = &py_kp_type;
        kp_obj->kpts = kpts;
        kp_obj->threshold = threshold;
        kp_obj->normalized = normalized;

        return kp_obj;
    }

    return PyLong_FromLong(0);
}
*/

static PyObject *
py_image_find_edges(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    edge_detector_t edge_type;
    int edge_t;
    rectangle_t roi;
    PyObject *py_thresh;
    static char *kwlist[] = {"edge_type", "threshold", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|O", kwlist,
                                     &edge_t, &py_thresh))
        return NULL;

    int thresh[2] = {100, 200};

    if (PyTuple_Check(py_thresh) && (PyTuple_GET_SIZE(py_thresh) == 2))
    {
        thresh[0] = PyLong_AsLong(PyTuple_GetItem(py_thresh, 0));
        thresh[1] = PyLong_AsLong(PyTuple_GetItem(py_thresh, 1));
    }
    if (edge_t == 0)
    {
        edge_type = EDGE_CANNY;
    }
    else
    {
        edge_type = EDGE_SIMPLE;
    }
    switch (edge_type)
    {
    case EDGE_SIMPLE:
    {
        fb_alloc_mark();
        imlib_edge_simple(arg_img, &roi, thresh[0], thresh[1]);
        fb_alloc_free_till_mark();
        break;
    }
    case EDGE_CANNY:
    {
        fb_alloc_mark();
        imlib_edge_canny(arg_img, &roi, thresh[0], thresh[1]);
        fb_alloc_free_till_mark();
        break;
    }
    }

    return PyLong_FromLong(0);
}

static PyObject *
py_image_find_hog(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;
    rectangle_t roi;
    PyObject *py_roi = Py_None;
    int size = 8;
    static char *kwlist[] = {"roi", "size", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|Oi", kwlist,
                                     &roi, &size))
        return NULL;
    roi_tan(py_roi, &roi, arg_img->w, arg_img->h);

    fb_alloc_mark();
    imlib_find_hog(arg_img, &roi, size);
    fb_alloc_free_till_mark();

    return PyLong_FromLong(0);
}

static PyObject *
keywdarg_parrot(PyObject *self, PyObject *args, PyObject *keywds)
{
    int voltage;
    char *state = "a stiff";
    char *action = "voom";
    char *type = "Norwegian Blue";
    static char *kwlist[] = {"voltage", "state", "action", "type", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|sss", kwlist,
                                     &voltage, &state, &action, &type))
        return NULL;
    printf("-- This parrot wouldn't %s if you put %i Volts through it.\n",
           action, voltage);
    printf("-- Lovely plumage, the %s -- It's %s!\n", type, state);

    Py_RETURN_NONE;
}

static PyMethodDef imageMethods[] = {
    {"Image", (PyCFunction)py_image_img_data_load, METH_VARARGS | METH_KEYWORDS, "python to c module image!"},
    {"img_free", (PyCFunction)py_image_img_data_free, METH_VARARGS, "free img !"},
    {"to_rgb24", (PyCFunction)py_img_torgb24, METH_VARARGS, "back an rgb888 img !"},
    {"image_print", (PyCFunction)py_image_print, METH_VARARGS, "print img !"},
    {"binary_to_grayscale", (PyCFunction)py_image_binary_to_grayscale, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"binary_to_rgb", (PyCFunction)py_image_binary_to_rgb, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"binary_to_lab", (PyCFunction)py_image_binary_to_lab, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"binary_to_yuv", (PyCFunction)py_image_binary_to_yuv, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"grayscale_to_binary", (PyCFunction)py_image_grayscale_to_binary, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"grayscale_to_rgb", (PyCFunction)py_image_grayscale_to_rgb, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"grayscale_to_lab", (PyCFunction)py_image_grayscale_to_lab, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"grayscale_to_yuv", (PyCFunction)py_image_grayscale_to_yuv, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"rgb_to_binary", (PyCFunction)py_image_rgb_to_binary, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"rgb_to_grayscale", (PyCFunction)py_image_rgb_to_grayscale, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"rgb_to_lab", (PyCFunction)py_image_rgb_to_lab, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"rgb_to_yuv", (PyCFunction)py_image_rgb_to_yuv, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"lab_to_binary", (PyCFunction)py_image_lab_to_binary, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"lab_to_grayscale", (PyCFunction)py_image_lab_to_grayscale, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"lab_to_rgb", (PyCFunction)py_image_lab_to_rgb, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"lab_to_yuv", (PyCFunction)py_image_lab_to_yuv, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"yuv_to_binary", (PyCFunction)py_image_yuv_to_binary, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"yuv_to_grayscale", (PyCFunction)py_image_yuv_to_grayscale, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"yuv_to_rgb", (PyCFunction)py_image_yuv_to_rgb, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"yuv_to_lab", (PyCFunction)py_image_yuv_to_lab, METH_VARARGS | METH_KEYWORDS, "img draw line"},
#ifdef IMLIB_ENABLE_GET_HISTOGRAM
    {"get_histogram", (PyCFunction)py_image_get_histogram, METH_VARARGS, "img draw line"},
#endif
#ifdef IMLIB_ENABLE_GET_STATISTICS
    {"get_statistics", (PyCFunction)py_image_get_statistics, METH_VARARGS | METH_KEYWORDS, "find blob !"},
#endif
#ifdef IMLIB_ENABLE_GET_STATISTICS
    {"get_statistics", (PyCFunction)py_image_get_statistics, METH_VARARGS, "img draw line"},
#endif
#ifdef IMLIB_ENABLE_FIND_BLOBS
    {"find_blobs", (PyCFunction)py_image_find_blobs, METH_VARARGS | METH_KEYWORDS, "img draw line"},
#endif
#ifdef IMLIB_ENABLE_FIND_BLOBS
    {"find_lines", (PyCFunction)py_image_find_lines, METH_VARARGS | METH_KEYWORDS, "img draw line"},
#endif
#ifdef IMLIB_ENABLE_FIND_LINE_SEGMENTS
    {"find_line_segments", (PyCFunction)py_image_find_line_segments, METH_VARARGS | METH_KEYWORDS, "img draw line"},
#endif
#ifdef IMLIB_ENABLE_GET_REGRESSION
    {"get_regression", (PyCFunction)py_image_get_regression, METH_VARARGS, "img draw line"},
#endif
#ifdef IMLIB_ENABLE_FIND_CIRCLES
    {"find_circles", (PyCFunction)py_image_find_circles, METH_VARARGS, "img draw line"},
#endif
#ifdef IMLIB_ENABLE_FIND_RECTS
    {"find_rects", (PyCFunction)py_image_find_rects, METH_VARARGS, "img draw line"},
#endif
#ifdef IMLIB_ENABLE_QRCODES
    {"find_qrcodes", (PyCFunction)py_image_find_qrcodes, METH_VARARGS | METH_KEYWORDS, "find blob !"},
#endif
#ifdef IMLIB_ENABLE_APRILTAGS
    {"find_apriltags", (PyCFunction)py_image_find_apriltags, METH_VARARGS, "img draw line"},
#endif
#ifdef IMLIB_ENABLE_DATAMATRICES
    {"find_datamatrices", (PyCFunction)py_image_find_datamatrices, METH_VARARGS, "img draw line"},
#endif
#ifdef IMLIB_ENABLE_BARCODES
    {"find_barcodes", (PyCFunction)py_image_find_barcodes, METH_VARARGS, "img draw line"},
#endif
#ifdef IMLIB_ENABLE_FIND_DISPLACEMENT
    {"find_displacement", (PyCFunction)py_image_find_displacement, METH_VARARGS, "img draw line"},
#endif
    {"width", (PyCFunction)py_image_width, METH_VARARGS, "img draw line"},
    {"height", (PyCFunction)py_image_height, METH_VARARGS, "img draw line"},
    {"format", (PyCFunction)py_image_format, METH_VARARGS, "img draw line"},
    {"size", (PyCFunction)py_image_size, METH_VARARGS, "img draw line"},
    {"get_pixel", (PyCFunction)py_image_get_pixel, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"set_pixel", (PyCFunction)py_image_set_pixel, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"mean_pool", (PyCFunction)py_image_mean_pool, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"mean_pooled", (PyCFunction)py_image_mean_pooled, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"midpoint_pool", (PyCFunction)py_image_midpoint_pool, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"midpoint_pooled", (PyCFunction)py_image_midpoint_pooled, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"to_grayscale", (PyCFunction)py_image_to_grayscale, METH_VARARGS | METH_KEYWORDS, "find blob !"},
    {"to_rgb565", (PyCFunction)py_image_to_rgb565, METH_VARARGS | METH_KEYWORDS, "find blob !"},
    {"to_rainbow", (PyCFunction)py_image_to_rainbow, METH_VARARGS | METH_KEYWORDS, "find blob !"},
    {"copy", (PyCFunction)py_image_copy, METH_VARARGS | METH_KEYWORDS, "find blob !"},
    {"clear", (PyCFunction)py_image_clear, METH_VARARGS | METH_KEYWORDS, "find blob !"},
    {"draw_line", (PyCFunction)py_image_draw_line, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"draw_rectangle", (PyCFunction)py_image_draw_rectangle, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"draw_circle", (PyCFunction)py_image_draw_circle, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    //在图片上写字符串

    {"draw_ellipse", (PyCFunction)py_image_draw_ellipse, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"draw_cross", (PyCFunction)py_image_draw_cross, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"draw_arrow", (PyCFunction)py_image_draw_arrow, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"draw_image", (PyCFunction)py_image_draw_image, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"draw_keypoints", (PyCFunction)py_image_draw_keypoints, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"binary", (PyCFunction)py_image_binary, METH_VARARGS | METH_KEYWORDS, "img draw line"},

    
    {"draw_edges", (PyCFunction)py_image_draw_edges, METH_VARARGS | METH_KEYWORDS, "img draw line"},
#ifdef IMLIB_ENABLE_FLOOD_FILL
    {"flood_fill", (PyCFunction)py_image_flood_fill, METH_VARARGS | METH_KEYWORDS, "img draw line"},
#endif
    {"mask_rectangle", (PyCFunction)py_image_mask_rectangle, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"mask_circle", (PyCFunction)py_image_mask_circle, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"mask_ellipse", (PyCFunction)py_image_mask_ellipse, METH_VARARGS | METH_KEYWORDS, "img draw line"},
#ifdef INVERT
    {"invert", (PyCFunction)py_image_invert, METH_VARARGS | METH_KEYWORDS, "img draw line"},
#endif
    {"b_and", (PyCFunction)py_image_b_and, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"b_nand", (PyCFunction)py_image_b_nand, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"b_or", (PyCFunction)py_image_b_or, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"b_nor", (PyCFunction)py_image_b_nor, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"b_xor", (PyCFunction)py_image_b_xor, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"b_xnor", (PyCFunction)py_image_b_xnor, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"erode", (PyCFunction)py_image_erode, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"dilate", (PyCFunction)py_image_dilate, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"open", (PyCFunction)py_image_open, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"close", (PyCFunction)py_image_close, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"top_hat", (PyCFunction)py_image_top_hat, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"black_hat", (PyCFunction)py_image_black_hat, METH_VARARGS | METH_KEYWORDS, "img draw line"},

    {"negate", (PyCFunction)py_image_negate, METH_VARARGS, "img draw line"},
    {"replace", (PyCFunction)py_image_replace, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"add", (PyCFunction)py_image_add, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"sub", (PyCFunction)py_image_sub, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"mul", (PyCFunction)py_image_mul, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"div", (PyCFunction)py_image_div, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"min", (PyCFunction)py_image_min, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"max", (PyCFunction)py_image_max, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"difference", (PyCFunction)py_image_difference, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"blend", (PyCFunction)py_image_blend, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"mean", (PyCFunction)py_image_mean, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"median", (PyCFunction)py_image_median, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"mode", (PyCFunction)py_image_mode, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"midpoint", (PyCFunction)py_image_midpoint, METH_VARARGS | METH_KEYWORDS, "img draw line"},








    {"histeq", (PyCFunction)py_image_histeq, METH_VARARGS | METH_KEYWORDS, "img draw line"},
    {"find_eye", (PyCFunction)py_image_find_eye, METH_VARARGS | METH_KEYWORDS, "img draw line"},

    // {"find_blobs", (PyCFunction)py_image_find_blobs, METH_VARARGS | METH_KEYWORDS, "find blob !"},

    {"keywdarg_parrot", (PyCFunction)keywdarg_parrot, METH_VARARGS | METH_KEYWORDS, "img draw line"},





    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef spammodule = {
    PyModuleDef_HEAD_INIT,
    "minicv", /* name of module */
    NULL,     /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    imageMethods};

#include "omv.h"

PyMODINIT_FUNC
PyInit_minicv(void)
{
    self_image_img.data = NULL;
    omv_init();
    PyObject *module = PyModule_Create(&spammodule);

    PyModule_AddIntConstant(module, "IMAGE_BPP_BINARY", IMAGE_BPP_BINARY);
    PyModule_AddIntConstant(module, "IMAGE_BPP_GRAYSCALE", IMAGE_BPP_GRAYSCALE);
    PyModule_AddIntConstant(module, "IMAGE_BPP_RGB565", IMAGE_BPP_RGB565);
    PyModule_AddIntConstant(module, "IMAGE_BPP_BAYER", IMAGE_BPP_BAYER);
    PyModule_AddIntConstant(module, "IMAGE_BPP_JPEG", IMAGE_BPP_JPEG);

    // PyModule_AddIntMacro(module, FPUTS_MACRO);

    return module;
}
