#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "collections.h"
#include "imdefs.h"
#include "imlib.h"

image_t self_image_img; //获得图片指针

// image.img_data_load(data,w,h,bpp)

static PyObject *
py_image_img_data_load(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *o_data;
	PyObject *o_w;
	PyObject *o_h;
	PyObject *o_bpp;

	static char *kwlist[] = {"img_data", "w", "h", "bpp", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, keywds, "OO|OO", kwlist,
									 &o_data, &o_w, &o_h, &o_bpp))
		return NULL;

	if (PyTuple_Check(o_w))
	{
		self_image_img.w = PyLong_AsLong(PyTuple_GetItem(o_w, 0));
		self_image_img.h = PyLong_AsLong(PyTuple_GetItem(o_w, 1));
	}
	else
	{
		self_image_img.w = PyLong_AsLong(o_w);
		self_image_img.h = PyLong_AsLong(o_h);
	}
	if (PyLong_Check(o_bpp))
	{
		self_image_img.bpp = PyLong_AsLong(o_bpp);
	}
	else
	{
		self_image_img.bpp = IMAGE_BPP_RGB565;
	}
	if (!PyBytes_Check(o_data))
		return NULL;
	uint8_t *r24_pixel;
	r24_pixel = PyBytes_AsString(o_data);

	uint16_t *r16_pixel;
	switch (self_image_img.bpp)
	{
	case IMAGE_BPP_RGB565:
		self_image_img.data = (uint8_t *)malloc(w * h * 2);
		r16_pixel = (uint16_t *)self_image_img.data for (int i = 0; i < w * h * 3; i += 3)
		{
			*r16_pixel = COLOR_R8_G8_B8_TO_RGB565(r24_pixel[i], r24_pixel[i + 1], r24_pixel[i + 2]);
			r16_pixel++;
		}
		break;

	default:
		return NULL;
		break;
	}
	return Py_BuildValue("i", 0);
}

static PyObject *
py_image_img_data_free(PyObject *self, PyObject *args)
{
	free(self_image_img.data);
}

static PyObject *
py_img_torgb24(PyObject *self, PyObject *args)
{
	PyObject *data1;
	uint8_t *_888_data;
	_888_data = (uint8_t *)malloc(self_image_img.w * self_image_img.h * 3);
	if (self_image_img.bpp != IMAGE_BPP_RGB565)
		return NULL;
	uint16_t *r16_pixel;
	r16_pixel = (uint16_t *)self_image_img.data;
	for (int i = 0; i < self_image_img.w * self_image_img.h * 3; i += 3)
	{
		_888_data[i] = COLOR_RGB565_TO_R8(*r16_pixel);

		_888_data[i + 1] = COLOR_RGB565_TO_G8(*r16_pixel);

		_888_data[i + 2] = COLOR_RGB565_TO_B8(*r16_pixel);

		r16_pixel++;
	}
#if _BIG
	uint8_t cha;
	unsigned short *nihao;
	nihao = (unsigned short *)arg_img.data;
	for (int i = 0; i < arg_img.w * arg_img.h; i++)
	{
		cha = nihao[i];
		nihao[i] = nihao[i] >> 8;
		nihao[i] |= cha << 8;
	}
#endif
	data1 = PyBytes_FromStringAndSize(_888_data, self_image_img.w * self_image_img.h * 3);
	free(_888_data);
	// Py_INCREF(data1);
	return data1;
}

static PyObject *
py_image_print(PyObject *self, PyObject *args)
{
    switch(self_image_img.bpp) {
        case IMAGE_BPP_BINARY: {
            printf("{\"w\":%d, \"h\":%d, \"type\"=\"binary\", \"size\":%d}",
                      self_image_img.w, self_image_img.h,
                      ((self_image_img.w + UINT32_T_MASK) >> UINT32_T_SHIFT) * self_image_img.h);
            break;
        }
        case IMAGE_BPP_GRAYSCALE: {
            printf("{\"w\":%d, \"h\":%d, \"type\"=\"grayscale\", \"size\":%d}",
                      self_image_img.w, self_image_img.h,
                      (self_image_img.w * self_image_img.h) * sizeof(uint8_t));
            break;
        }
        case IMAGE_BPP_RGB565: {
            printf("{\"w\":%d, \"h\":%d, \"type\"=\"rgb565\", \"size\":%d}",
                      self_image_img.w, self_image_img.h,
                      (self_image_img.w * self_image_img.h) * sizeof(uint16_t));
            break;
        }
        case IMAGE_BPP_BAYER: {
            printf("{\"w\":%d, \"h\":%d, \"type\"=\"bayer\", \"size\":%d}",
                      self_image_img.w, self_image_img.h,
                      (self_image_img.w * self_image_img.h) * sizeof(uint8_t));
            break;
        }
        default: {
            if((self_image_img.data[0] == 0xFE) && (self_image_img.data[self_image_img.bpp-1] == 0xFE)) { // for ide
                print->print_strn(print->data, (const char *) self_image_img.data, self_image_img.bpp);
            } else { // not for ide
                printf("{\"w\":%d, \"h\":%d, \"type\"=\"jpeg\", \"size\":%d}",
                          self_image_img.w, self_image_img.h,
                          self_image_img.bpp);
            }
            break;
        }
    }
}

static PyObject *
py_image_binary_to_grayscale(PyObject *self, PyObject *args, PyObject *keywds)
{
    int8_t b;
	static char *kwlist[] = {"binary_image_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &b))return NULL;
    return Py_BuildValue("i", COLOR_BINARY_TO_GRAYSCALE(b));
}


static PyObject *
py_image_binary_to_rgb(PyObject *self, PyObject *args, PyObject *keywds)
{
    int8_t b = 0;
	static char *kwlist[] = {"binary_image_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &b))return NULL;

    uint16_t rgb565 = COLOR_BINARY_TO_RGB565(b);
    
	
	return Py_BuildValue("iii", COLOR_RGB565_TO_R8(rgb565), COLOR_RGB565_TO_G8(rgb565), COLOR_RGB565_TO_B8(rgb565));
}

static PyObject *
py_image_binary_to_lab(PyObject *self, PyObject *args, PyObject *keywds)
{
    int8_t b = 0;
	static char *kwlist[] = {"binary_image_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &b))return NULL;
    uint16_t rgb565 = COLOR_BINARY_TO_RGB565(b);
    return Py_BuildValue("iii",COLOR_RGB565_TO_L(rgb565),COLOR_RGB565_TO_A(rgb565),COLOR_RGB565_TO_B(rgb565));
}

static PyObject *
py_image_binary_to_yuv(PyObject *self, PyObject *args, PyObject *keywds)
{
    int8_t b = 0;
	static char *kwlist[] = {"binary_image_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &b))return NULL;
    uint16_t rgb565 = COLOR_BINARY_TO_RGB565(b);
    return Py_BuildValue("iii",COLOR_RGB565_TO_Y(rgb565),COLOR_RGB565_TO_U(rgb565),COLOR_RGB565_TO_V(rgb565));
}
static PyObject *
py_image_grayscale_to_binary(PyObject *self, PyObject *args, PyObject *keywds)
{
    int8_t g = 0;
	static char *kwlist[] = {"grayscale_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &g))return NULL;
    return Py_BuildValue("i" ,COLOR_GRAYSCALE_TO_BINARY(g));
}

static PyObject *
py_image_grayscale_to_rgb(PyObject *self, PyObject *args, PyObject *keywds)
{
    int8_t g = 0;
	static char *kwlist[] = {"grayscale_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &g))return NULL;

    uint16_t rgb565 = COLOR_GRAYSCALE_TO_RGB565(g);
    return Py_BuildValue("iii",COLOR_RGB565_TO_R8(rgb565),COLOR_RGB565_TO_G8(rgb565),COLOR_RGB565_TO_B8(rgb565));

}

static PyObject *
py_image_grayscale_to_lab(PyObject *self, PyObject *args, PyObject *keywds)
{
    int8_t g = 0;
	static char *kwlist[] = {"grayscale_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &g))return NULL;	
    uint16_t rgb565 = COLOR_GRAYSCALE_TO_RGB565(g);
    return Py_BuildValue("iii",COLOR_RGB565_TO_L(rgb565),COLOR_RGB565_TO_A(rgb565),COLOR_RGB565_TO_B(rgb565));
}

static PyObject *
py_image_grayscale_to_yuv(PyObject *self, PyObject *args, PyObject *keywds)
{
    int8_t g = 0;
	static char *kwlist[] = {"grayscale_value", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i", kwlist,
                                     &g))return NULL;	
    uint16_t rgb565 = COLOR_GRAYSCALE_TO_RGB565(g);
    return Py_BuildValue("iii",COLOR_RGB565_TO_Y(rgb565),COLOR_RGB565_TO_U(rgb565),COLOR_RGB565_TO_V(rgb565));
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
                                     &rgb_tuple))return NULL;
	r = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,0));
	g = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,1));
	b = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,2));								 

    uint16_t rgb565 = COLOR_R8_G8_B8_TO_RGB565(r, g, b);

    return Py_BuildValue("i",COLOR_RGB565_TO_BINARY(rgb565));
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
                                     &rgb_tuple))return NULL;
	r = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,0));
	g = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,1));
	b = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,2));
    uint16_t rgb565 = COLOR_R8_G8_B8_TO_RGB565(r, g, b);
    return Py_BuildValue("i",COLOR_RGB565_TO_GRAYSCALE(rgb565));
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
                                     &rgb_tuple))return NULL;
	r = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,0));
	g = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,1));
	b = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,2));
    uint16_t rgb565 = COLOR_R8_G8_B8_TO_RGB565(r, g, b);
    return Py_BuildValue("iii",COLOR_RGB565_TO_L(rgb565),COLOR_RGB565_TO_A(rgb565),COLOR_RGB565_TO_B(rgb565));
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
                                     &rgb_tuple))return NULL;
	r = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,0));
	g = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,1));
	b = PyLong_AsLong(PyTuple_GetItem(rgb_tuple,2));
    uint16_t rgb565 = COLOR_R8_G8_B8_TO_RGB565(r, g, b);
    return Py_BuildValue("iii",COLOR_RGB565_TO_Y(rgb565),COLOR_RGB565_TO_U(rgb565),COLOR_RGB565_TO_V(rgb565));

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
                                     &lab_tuple))return NULL;
	l = PyLong_AsLong(PyTuple_GetItem(lab_tuple,0));
	a = PyLong_AsLong(PyTuple_GetItem(lab_tuple,1));
	b = PyLong_AsLong(PyTuple_GetItem(lab_tuple,2));

    uint16_t rgb565 = COLOR_LAB_TO_RGB565(l, a, b);
    return Py_BuildValue("i",COLOR_RGB565_TO_BINARY(rgb565));
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
                                     &lab_tuple))return NULL;
	l = PyLong_AsLong(PyTuple_GetItem(lab_tuple,0));
	a = PyLong_AsLong(PyTuple_GetItem(lab_tuple,1));
	b = PyLong_AsLong(PyTuple_GetItem(lab_tuple,2));
    uint16_t rgb565 = COLOR_LAB_TO_RGB565(l, a, b);
    return Py_BuildValue("i",COLOR_RGB565_TO_GRAYSCALE(rgb565));
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
                                     &lab_tuple))return NULL;
	l = PyLong_AsLong(PyTuple_GetItem(lab_tuple,0));
	a = PyLong_AsLong(PyTuple_GetItem(lab_tuple,1));
	b = PyLong_AsLong(PyTuple_GetItem(lab_tuple,2));
    uint16_t rgb565 = COLOR_LAB_TO_RGB565(l, a, b);
    return Py_BuildValue("iii",COLOR_RGB565_TO_R8(rgb565),COLOR_RGB565_TO_G8(rgb565),COLOR_RGB565_TO_B8(rgb565));

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
                                     &lab_tuple))return NULL;
	l = PyLong_AsLong(PyTuple_GetItem(lab_tuple,0));
	a = PyLong_AsLong(PyTuple_GetItem(lab_tuple,1));
	b = PyLong_AsLong(PyTuple_GetItem(lab_tuple,2));
    uint16_t rgb565 = COLOR_LAB_TO_RGB565(l, a, b);
    return Py_BuildValue("iii",COLOR_RGB565_TO_Y(rgb565),COLOR_RGB565_TO_U(rgb565),COLOR_RGB565_TO_V(rgb565));

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
                                     &yuv_tuple))return NULL;
	y = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,0));
	u = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,1));
	v = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,2));

    uint16_t rgb565 = COLOR_YUV_TO_RGB565(y, u, v);
    return Py_BuildValue("i",COLOR_RGB565_TO_BINARY(rgb565));
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
                                     &yuv_tuple))return NULL;
	y = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,0));
	u = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,1));
	v = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,2));
    uint16_t rgb565 = COLOR_YUV_TO_RGB565(y, u, v);
    return Py_BuildValue("i",COLOR_RGB565_TO_GRAYSCALE(rgb565));
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
                                     &yuv_tuple))return NULL;
	y = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,0));
	u = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,1));
	v = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,2));

    uint16_t rgb565 = COLOR_YUV_TO_RGB565(y, u, v);
    return Py_BuildValue("iii",COLOR_RGB565_TO_R8(rgb565),COLOR_RGB565_TO_G8(rgb565),COLOR_RGB565_TO_B8(rgb565));

}

static PyObject *
py_image_yuv_to_lab(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *yuv_tuple;
    uint8_t y = 0;
    uint8_t u = 0;
    uint8_t v = 0;
	static char *kwlist[] = {"yuv_tuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O", kwlist,
                                     &yuv_tuple))return NULL;
	y = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,0));
	u = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,1));
	v = PyLong_AsLong(PyTuple_GetItem(yuv_tuple,2));


    uint16_t rgb565 = COLOR_YUV_TO_RGB565(y, u, v);
    return Py_BuildValue("iii",COLOR_RGB565_TO_L(rgb565),COLOR_RGB565_TO_A(rgb565),COLOR_RGB565_TO_B(rgb565));

}


//////////////
// Get Methods
//////////////

#if IMLIB_ENABLE_GET_SIMILARITY
// Similarity Object //
#define py_similarity_obj_size 4
typedef struct py_similarity_obj {
    mp_obj_base_t base;
    mp_obj_t avg, std, min, max;
} py_similarity_obj_t;

static void py_similarity_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind)
{
    py_similarity_obj_t *self = self_in;
    mp_printf(print,
              "{\"mean\":%f, \"stdev\":%f, \"min\":%f, \"max\":%f}",
              (double) mp_obj_get_float(self->avg),
              (double) mp_obj_get_float(self->std),
              (double) mp_obj_get_float(self->min),
              (double) mp_obj_get_float(self->max));
}

static mp_obj_t py_similarity_subscr(mp_obj_t self_in, mp_obj_t index, mp_obj_t value)
{
    if (value == MP_OBJ_SENTINEL) { // load
        py_similarity_obj_t *self = self_in;
        if (MP_OBJ_IS_TYPE(index, &mp_type_slice)) {
            mp_bound_slice_t slice;
            if (!mp_seq_get_fast_slice_indexes(py_similarity_obj_size, index, &slice)) {
                mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("only slices with step=1 (aka None) are supported"));
            }
            mp_obj_tuple_t *result = mp_obj_new_tuple(slice.stop - slice.start, NULL);
            mp_seq_copy(result->items, &(self->avg) + slice.start, result->len, mp_obj_t);
            return result;
        }
        switch (mp_get_index(self->base.type, py_similarity_obj_size, index, false)) {
            case 0: return self->avg;
            case 1: return self->std;
            case 2: return self->min;
            case 3: return self->max;
        }
    }
    return MP_OBJ_NULL; // op not supported
}

mp_obj_t py_similarity_mean(mp_obj_t self_in) { return ((py_similarity_obj_t *) self_in)->avg; }
mp_obj_t py_similarity_stdev(mp_obj_t self_in) { return ((py_similarity_obj_t *) self_in)->std; }
mp_obj_t py_similarity_min(mp_obj_t self_in) { return ((py_similarity_obj_t *) self_in)->min; }
mp_obj_t py_similarity_max(mp_obj_t self_in) { return ((py_similarity_obj_t *) self_in)->max; }


static mp_obj_t py_image_get_similarity(mp_obj_t img_obj, mp_obj_t other_obj)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(img_obj);
    float avg, std, min, max;

    fb_alloc_mark();

    if (MP_OBJ_IS_STR(other_obj)) {
        imlib_get_similarity(arg_img, mp_obj_str_get_str(other_obj), NULL, 0, &avg, &std, &min, &max);
    } else if (MP_OBJ_IS_TYPE(other_obj, &py_image_type)) {
        imlib_get_similarity(arg_img, NULL, py_helper_arg_to_image_mutable(other_obj), 0, &avg, &std, &min, &max);
    } else {
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
STATIC MP_DEFINE_CONST_FUN_OBJ_2(py_image_get_similarity_obj, py_image_get_similarity);
#endif // IMLIB_ENABLE_GET_SIMILARITY


#if IMLIB_ENABLE_GET_HISTOGRAM

static PyObject *
py_image_get_histogram(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    list_t thresholds;
    list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
    py_helper_keyword_thresholds(n_args, args, 1, kw_args, &thresholds);
    bool invert = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_invert), false);
    image_t *other = py_helper_keyword_to_image_mutable(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_difference), NULL);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 3, kw_args, &roi);

    histogram_t hist;
    switch(arg_img->bpp) {
        case IMAGE_BPP_BINARY: {
            int bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                             (COLOR_BINARY_MAX-COLOR_BINARY_MIN+1));
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
        case IMAGE_BPP_GRAYSCALE: {
            int bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                             (COLOR_GRAYSCALE_MAX-COLOR_GRAYSCALE_MIN+1));
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
        case IMAGE_BPP_RGB565: {
            int l_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                               (COLOR_L_MAX-COLOR_L_MIN+1));
            PY_ASSERT_TRUE_MSG(l_bins >= 2, "bins must be >= 2");
            hist.LBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), l_bins);
            PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
            int a_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                               (COLOR_A_MAX-COLOR_A_MIN+1));
            PY_ASSERT_TRUE_MSG(a_bins >= 2, "bins must be >= 2");
            hist.ABinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_a_bins), a_bins);
            PY_ASSERT_TRUE_MSG(hist.ABinCount >= 2, "a_bins must be >= 2");
            int b_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                               (COLOR_B_MAX-COLOR_B_MIN+1));
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
        default: {
            return MP_OBJ_NULL;
        }
    }

    py_histogram_obj_t *o = m_new_obj(py_histogram_obj_t);
    o->base.type = &py_histogram_type;
    o->bpp = arg_img->bpp;

    o->LBins = mp_obj_new_list(hist.LBinCount, NULL);
    o->ABins = mp_obj_new_list(hist.ABinCount, NULL);
    o->BBins = mp_obj_new_list(hist.BBinCount, NULL);

    for (int i = 0; i < hist.LBinCount; i++) {
        ((mp_obj_list_t *) o->LBins)->items[i] = mp_obj_new_float(hist.LBins[i]);
    }

    for (int i = 0; i < hist.ABinCount; i++) {
        ((mp_obj_list_t *) o->ABins)->items[i] = mp_obj_new_float(hist.ABins[i]);
    }

    for (int i = 0; i < hist.BBinCount; i++) {
        ((mp_obj_list_t *) o->BBins)->items[i] = mp_obj_new_float(hist.BBins[i]);
    }

    fb_alloc_free_till_mark();

    return o;
}
#endif// IMLIB_ENABLE_GET_HISTOGRAM

#if IMLIB_ENABLE_GET_PERCENTILE

static PyObject *
py_histogram_get_percentile(PyObject *self, PyObject *args, PyObject *keywds)
{
    histogram_t hist;
    hist.LBinCount = ((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->LBins)->len;
    hist.ABinCount = ((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->ABins)->len;
    hist.BBinCount = ((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->BBins)->len;
    fb_alloc_mark();
    hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
    hist.ABins = fb_alloc(hist.ABinCount * sizeof(float), FB_ALLOC_NO_HINT);
    hist.BBins = fb_alloc(hist.BBinCount * sizeof(float), FB_ALLOC_NO_HINT);

    for (int i = 0; i < hist.LBinCount; i++) {
        hist.LBins[i] = mp_obj_get_float(((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->LBins)->items[i]);
    }

    for (int i = 0; i < hist.ABinCount; i++) {
        hist.ABins[i] = mp_obj_get_float(((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->ABins)->items[i]);
    }

    for (int i = 0; i < hist.BBinCount; i++) {
        hist.BBins[i] = mp_obj_get_float(((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->BBins)->items[i]);
    }

    percentile_t p;
    imlib_get_percentile(&p, ((py_histogram_obj_t *) self_in)->bpp, &hist, mp_obj_get_float(percentile));
    fb_alloc_free_till_mark();

    py_percentile_obj_t *o = m_new_obj(py_percentile_obj_t);
    o->base.type = &py_percentile_type;
    o->bpp = ((py_histogram_obj_t *) self_in)->bpp;

    o->LValue = mp_obj_new_int(p.LValue);
    o->AValue = mp_obj_new_int(p.AValue);
    o->BValue = mp_obj_new_int(p.BValue);

    return o;
}

#endif // IMLIB_ENABLE_GET_PERCENTILE

#if IMLIB_ENABLE_GET_THRESHOLD
static PyObject *
py_histogram_get_threshold(PyObject *self, PyObject *args, PyObject *keywds)
{
    histogram_t hist;
    hist.LBinCount = ((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->LBins)->len;
    hist.ABinCount = ((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->ABins)->len;
    hist.BBinCount = ((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->BBins)->len;
    fb_alloc_mark();
    hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
    hist.ABins = fb_alloc(hist.ABinCount * sizeof(float), FB_ALLOC_NO_HINT);
    hist.BBins = fb_alloc(hist.BBinCount * sizeof(float), FB_ALLOC_NO_HINT);

    for (int i = 0; i < hist.LBinCount; i++) {
        hist.LBins[i] = mp_obj_get_float(((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->LBins)->items[i]);
    }

    for (int i = 0; i < hist.ABinCount; i++) {
        hist.ABins[i] = mp_obj_get_float(((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->ABins)->items[i]);
    }

    for (int i = 0; i < hist.BBinCount; i++) {
        hist.BBins[i] = mp_obj_get_float(((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->BBins)->items[i]);
    }

    threshold_t t;
    imlib_get_threshold(&t, ((py_histogram_obj_t *) self_in)->bpp, &hist);
    fb_alloc_free_till_mark();

    py_threshold_obj_t *o = m_new_obj(py_threshold_obj_t);
    o->base.type = &py_threshold_type;
    o->bpp = ((py_threshold_obj_t *) self_in)->bpp;

    o->LValue = mp_obj_new_int(t.LValue);
    o->AValue = mp_obj_new_int(t.AValue);
    o->BValue = mp_obj_new_int(t.BValue);

    return o;
}

#endif //IMLIB_ENABLE_GET_THRESHOLD

#if IMLIB_ENABLE_GET_STATISTICS
static PyObject *
py_histogram_get_statistics(PyObject *self, PyObject *args, PyObject *keywds)
{
    histogram_t hist;
    hist.LBinCount = ((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->LBins)->len;
    hist.ABinCount = ((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->ABins)->len;
    hist.BBinCount = ((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->BBins)->len;
    fb_alloc_mark();
    hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
    hist.ABins = fb_alloc(hist.ABinCount * sizeof(float), FB_ALLOC_NO_HINT);
    hist.BBins = fb_alloc(hist.BBinCount * sizeof(float), FB_ALLOC_NO_HINT);

    for (int i = 0; i < hist.LBinCount; i++) {
        hist.LBins[i] = mp_obj_get_float(((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->LBins)->items[i]);
    }

    for (int i = 0; i < hist.ABinCount; i++) {
        hist.ABins[i] = mp_obj_get_float(((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->ABins)->items[i]);
    }

    for (int i = 0; i < hist.BBinCount; i++) {
        hist.BBins[i] = mp_obj_get_float(((mp_obj_list_t *) ((py_histogram_obj_t *) self_in)->BBins)->items[i]);
    }

    statistics_t stats;
    imlib_get_statistics(&stats, ((py_histogram_obj_t *) self_in)->bpp, &hist);
    fb_alloc_free_till_mark();

    py_statistics_obj_t *o = m_new_obj(py_statistics_obj_t);
    o->base.type = &py_statistics_type;
    o->bpp = ((py_histogram_obj_t *) self_in)->bpp;

    o->LMean = mp_obj_new_int(stats.LMean);
    o->LMedian = mp_obj_new_int(stats.LMedian);
    o->LMode= mp_obj_new_int(stats.LMode);
    o->LSTDev = mp_obj_new_int(stats.LSTDev);
    o->LMin = mp_obj_new_int(stats.LMin);
    o->LMax = mp_obj_new_int(stats.LMax);
    o->LLQ = mp_obj_new_int(stats.LLQ);
    o->LUQ = mp_obj_new_int(stats.LUQ);
    o->AMean = mp_obj_new_int(stats.AMean);
    o->AMedian = mp_obj_new_int(stats.AMedian);
    o->AMode= mp_obj_new_int(stats.AMode);
    o->ASTDev = mp_obj_new_int(stats.ASTDev);
    o->AMin = mp_obj_new_int(stats.AMin);
    o->AMax = mp_obj_new_int(stats.AMax);
    o->ALQ = mp_obj_new_int(stats.ALQ);
    o->AUQ = mp_obj_new_int(stats.AUQ);
    o->BMean = mp_obj_new_int(stats.BMean);
    o->BMedian = mp_obj_new_int(stats.BMedian);
    o->BMode= mp_obj_new_int(stats.BMode);
    o->BSTDev = mp_obj_new_int(stats.BSTDev);
    o->BMin = mp_obj_new_int(stats.BMin);
    o->BMax = mp_obj_new_int(stats.BMax);
    o->BLQ = mp_obj_new_int(stats.BLQ);
    o->BUQ = mp_obj_new_int(stats.BUQ);

    return o;
}

static PyObject *
py_image_get_statistics(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    list_t thresholds;
    list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
    py_helper_keyword_thresholds(n_args, args, 1, kw_args, &thresholds);
    bool invert = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_invert), false);
    image_t *other = py_helper_keyword_to_image_mutable(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_difference), NULL);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 3, kw_args, &roi);

    histogram_t hist;
    switch(arg_img->bpp) {
        case IMAGE_BPP_BINARY: {
            int bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                             (COLOR_BINARY_MAX-COLOR_BINARY_MIN+1));
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
        case IMAGE_BPP_GRAYSCALE: {
            int bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                             (COLOR_GRAYSCALE_MAX-COLOR_GRAYSCALE_MIN+1));
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
        case IMAGE_BPP_RGB565: {
            int l_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                               (COLOR_L_MAX-COLOR_L_MIN+1));
            PY_ASSERT_TRUE_MSG(l_bins >= 2, "bins must be >= 2");
            hist.LBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), l_bins);
            PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
            int a_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                               (COLOR_A_MAX-COLOR_A_MIN+1));
            PY_ASSERT_TRUE_MSG(a_bins >= 2, "bins must be >= 2");
            hist.ABinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_a_bins), a_bins);
            PY_ASSERT_TRUE_MSG(hist.ABinCount >= 2, "a_bins must be >= 2");
            int b_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
                                               (COLOR_B_MAX-COLOR_B_MIN+1));
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
        default: {
            return MP_OBJ_NULL;
        }
    }

    statistics_t stats;
    imlib_get_statistics(&stats, arg_img->bpp, &hist);
    fb_alloc_free_till_mark();

    py_statistics_obj_t *o = m_new_obj(py_statistics_obj_t);
    o->base.type = &py_statistics_type;
    o->bpp = arg_img->bpp;

    o->LMean = mp_obj_new_int(stats.LMean);
    o->LMedian = mp_obj_new_int(stats.LMedian);
    o->LMode= mp_obj_new_int(stats.LMode);
    o->LSTDev = mp_obj_new_int(stats.LSTDev);
    o->LMin = mp_obj_new_int(stats.LMin);
    o->LMax = mp_obj_new_int(stats.LMax);
    o->LLQ = mp_obj_new_int(stats.LLQ);
    o->LUQ = mp_obj_new_int(stats.LUQ);
    o->AMean = mp_obj_new_int(stats.AMean);
    o->AMedian = mp_obj_new_int(stats.AMedian);
    o->AMode= mp_obj_new_int(stats.AMode);
    o->ASTDev = mp_obj_new_int(stats.ASTDev);
    o->AMin = mp_obj_new_int(stats.AMin);
    o->AMax = mp_obj_new_int(stats.AMax);
    o->ALQ = mp_obj_new_int(stats.ALQ);
    o->AUQ = mp_obj_new_int(stats.AUQ);
    o->BMean = mp_obj_new_int(stats.BMean);
    o->BMedian = mp_obj_new_int(stats.BMedian);
    o->BMode= mp_obj_new_int(stats.BMode);
    o->BSTDev = mp_obj_new_int(stats.BSTDev);
    o->BMin = mp_obj_new_int(stats.BMin);
    o->BMax = mp_obj_new_int(stats.BMax);
    o->BLQ = mp_obj_new_int(stats.BLQ);
    o->BUQ = mp_obj_new_int(stats.BUQ);

    return o;
}




#endif //IMLIB_ENABLE_GET_STATISTICS

#if IMLIB_ENABLE_FIND_BLOBS

static PyObject *
py_image_find_blobs(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    list_t thresholds;
    list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
    py_helper_arg_to_thresholds(args[1], &thresholds);
    if (!list_size(&thresholds)) return mp_obj_new_list(0, NULL);
    bool invert = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_invert), false);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 3, kw_args, &roi);

    unsigned int x_stride =
        py_helper_keyword_int(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_stride), 2);
    PY_ASSERT_TRUE_MSG(x_stride > 0, "x_stride must not be zero.");
    unsigned int y_stride =
        py_helper_keyword_int(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_stride), 1);
    PY_ASSERT_TRUE_MSG(y_stride > 0, "y_stride must not be zero.");
    unsigned int area_threshold =
        py_helper_keyword_int(n_args, args, 6, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_area_threshold), 10);
    unsigned int pixels_threshold =
        py_helper_keyword_int(n_args, args, 7, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_pixels_threshold), 10);
    bool merge =
        py_helper_keyword_int(n_args, args, 8, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_merge), false);
    int margin =
        py_helper_keyword_int(n_args, args, 9, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_margin), 0);
    mp_obj_t threshold_cb =
        py_helper_keyword_object(n_args, args, 10, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold_cb), NULL);
    mp_obj_t merge_cb =
        py_helper_keyword_object(n_args, args, 11, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_merge_cb), NULL);
    unsigned int x_hist_bins_max =
        py_helper_keyword_int(n_args, args, 12, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_hist_bins_max), 0);
    unsigned int y_hist_bins_max =
        py_helper_keyword_int(n_args, args, 13, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_hist_bins_max), 0);

    list_t out;
    fb_alloc_mark();
    imlib_find_blobs(&out, arg_img, &roi, x_stride, y_stride, &thresholds, invert,
            area_threshold, pixels_threshold, merge, margin,
            py_image_find_blobs_threshold_cb, threshold_cb, py_image_find_blobs_merge_cb, merge_cb, x_hist_bins_max, y_hist_bins_max);
    fb_alloc_free_till_mark();
    list_free(&thresholds);

    mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
    for (size_t i = 0; list_size(&out); i++) {
        find_blobs_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        py_blob_obj_t *o = m_new_obj(py_blob_obj_t);
        o->base.type = &py_blob_type;
        o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
            {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*0)/4].x),
                                                mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*0)/4].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*1)/4].x),
                                                mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*1)/4].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*2)/4].x),
                                                mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*2)/4].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*3)/4].x),
                                                mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*3)/4].y)})});
        point_t min_corners[4];
        point_min_area_rectangle(lnk_data.corners, min_corners, FIND_BLOBS_CORNERS_RESOLUTION);
        o->min_corners = mp_obj_new_tuple(4, (mp_obj_t [])
            {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(min_corners[0].x), mp_obj_new_int(min_corners[0].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(min_corners[1].x), mp_obj_new_int(min_corners[1].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(min_corners[2].x), mp_obj_new_int(min_corners[2].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(min_corners[3].x), mp_obj_new_int(min_corners[3].y)})});
        o->x = mp_obj_new_int(lnk_data.rect.x);
        o->y = mp_obj_new_int(lnk_data.rect.y);
        o->w = mp_obj_new_int(lnk_data.rect.w);
        o->h = mp_obj_new_int(lnk_data.rect.h);
        o->pixels = mp_obj_new_int(lnk_data.pixels);
        o->cx = mp_obj_new_float(lnk_data.centroid_x);
        o->cy = mp_obj_new_float(lnk_data.centroid_y);
        o->rotation = mp_obj_new_float(lnk_data.rotation);
        o->code = mp_obj_new_int(lnk_data.code);
        o->count = mp_obj_new_int(lnk_data.count);
        o->perimeter = mp_obj_new_int(lnk_data.perimeter);
        o->roundness = mp_obj_new_float(lnk_data.roundness);
        o->x_hist_bins = mp_obj_new_list(lnk_data.x_hist_bins_count, NULL);
        o->y_hist_bins = mp_obj_new_list(lnk_data.y_hist_bins_count, NULL);

        for (int i = 0; i < lnk_data.x_hist_bins_count; i++) {
            ((mp_obj_list_t *) o->x_hist_bins)->items[i] = mp_obj_new_int(lnk_data.x_hist_bins[i]);
        }

        for (int i = 0; i < lnk_data.y_hist_bins_count; i++) {
            ((mp_obj_list_t *) o->y_hist_bins)->items[i] = mp_obj_new_int(lnk_data.y_hist_bins[i]);
        }

        objects_list->items[i] = o;
        if (lnk_data.x_hist_bins) xfree(lnk_data.x_hist_bins);
        if (lnk_data.y_hist_bins) xfree(lnk_data.y_hist_bins);
    }

    return objects_list;
}

#endif //IMLIB_ENABLE_FIND_BLOBS

#ifdef IMLIB_ENABLE_FIND_LINES
static mp_obj_t py_image_find_lines(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    unsigned int x_stride = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_stride), 2);
    PY_ASSERT_TRUE_MSG(x_stride > 0, "x_stride must not be zero.");
    unsigned int y_stride = py_helper_keyword_int(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_stride), 1);
    PY_ASSERT_TRUE_MSG(y_stride > 0, "y_stride must not be zero.");
    uint32_t threshold = py_helper_keyword_int(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold), 1000);
    unsigned int theta_margin = py_helper_keyword_int(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_theta_margin), 25);
    unsigned int rho_margin = py_helper_keyword_int(n_args, args, 6, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_rho_margin), 25);

    list_t out;
    fb_alloc_mark();
    imlib_find_lines(&out, arg_img, &roi, x_stride, y_stride, threshold, theta_margin, rho_margin);
    fb_alloc_free_till_mark();

    mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
    for (size_t i = 0; list_size(&out); i++) {
        find_lines_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        py_line_obj_t *o = m_new_obj(py_line_obj_t);
        o->base.type = &py_line_type;
        o->x1 = mp_obj_new_int(lnk_data.line.x1);
        o->y1 = mp_obj_new_int(lnk_data.line.y1);
        o->x2 = mp_obj_new_int(lnk_data.line.x2);
        o->y2 = mp_obj_new_int(lnk_data.line.y2);
        int x_diff = lnk_data.line.x2 - lnk_data.line.x1;
        int y_diff = lnk_data.line.y2 - lnk_data.line.y1;
        o->length = mp_obj_new_int(fast_roundf(fast_sqrtf((x_diff * x_diff) + (y_diff * y_diff))));
        o->magnitude = mp_obj_new_int(lnk_data.magnitude);
        o->theta = mp_obj_new_int(lnk_data.theta);
        o->rho = mp_obj_new_int(lnk_data.rho);

        objects_list->items[i] = o;
    }

    return objects_list;
}
#endif // IMLIB_ENABLE_FIND_LINES

#ifdef IMLIB_ENABLE_FIND_LINE_SEGMENTS
static PyObject *
py_image_find_line_segments(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    unsigned int merge_distance = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_merge_distance), 0);
    unsigned int max_theta_diff = py_helper_keyword_int(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_max_theta_diff), 15);

    list_t out;
    fb_alloc_mark();
    imlib_lsd_find_line_segments(&out, arg_img, &roi, merge_distance, max_theta_diff);
    fb_alloc_free_till_mark();

    mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
    for (size_t i = 0; list_size(&out); i++) {
        find_lines_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        py_line_obj_t *o = m_new_obj(py_line_obj_t);
        o->base.type = &py_line_type;
        o->x1 = mp_obj_new_int(lnk_data.line.x1);
        o->y1 = mp_obj_new_int(lnk_data.line.y1);
        o->x2 = mp_obj_new_int(lnk_data.line.x2);
        o->y2 = mp_obj_new_int(lnk_data.line.y2);
        int x_diff = lnk_data.line.x2 - lnk_data.line.x1;
        int y_diff = lnk_data.line.y2 - lnk_data.line.y1;
        o->length = mp_obj_new_int(fast_roundf(fast_sqrtf((x_diff * x_diff) + (y_diff * y_diff))));
        o->magnitude = mp_obj_new_int(lnk_data.magnitude);
        o->theta = mp_obj_new_int(lnk_data.theta);
        o->rho = mp_obj_new_int(lnk_data.rho);

        objects_list->items[i] = o;
    }

    return objects_list;
}
#endif // IMLIB_ENABLE_FIND_LINE_SEGMENTS

#ifdef IMLIB_ENABLE_GET_REGRESSION
static mp_obj_t py_image_get_regression(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    list_t thresholds;
    list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
    py_helper_arg_to_thresholds(args[1], &thresholds);
    if (!list_size(&thresholds)) return mp_const_none;
    bool invert = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_invert), false);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 3, kw_args, &roi);

    unsigned int x_stride = py_helper_keyword_int(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_stride), 2);
    PY_ASSERT_TRUE_MSG(x_stride > 0, "x_stride must not be zero.");
    unsigned int y_stride = py_helper_keyword_int(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_stride), 1);
    PY_ASSERT_TRUE_MSG(y_stride > 0, "y_stride must not be zero.");
    unsigned int area_threshold = py_helper_keyword_int(n_args, args, 6, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_area_threshold), 10);
    unsigned int pixels_threshold = py_helper_keyword_int(n_args, args, 7, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_pixels_threshold), 10);
    bool robust = py_helper_keyword_int(n_args, args, 8, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_robust), false);

    find_lines_list_lnk_data_t out;
    fb_alloc_mark();
    bool result = imlib_get_regression(&out, arg_img, &roi, x_stride, y_stride, &thresholds, invert, area_threshold, pixels_threshold, robust);
    fb_alloc_free_till_mark();
    list_free(&thresholds);
    if (!result) {
        return mp_const_none;
    }

    py_line_obj_t *o = m_new_obj(py_line_obj_t);
    o->base.type = &py_line_type;
    o->x1 = mp_obj_new_int(out.line.x1);
    o->y1 = mp_obj_new_int(out.line.y1);
    o->x2 = mp_obj_new_int(out.line.x2);
    o->y2 = mp_obj_new_int(out.line.y2);
    int x_diff = out.line.x2 - out.line.x1;
    int y_diff = out.line.y2 - out.line.y1;
    o->length = mp_obj_new_int(fast_roundf(fast_sqrtf((x_diff * x_diff) + (y_diff * y_diff))));
    o->magnitude = mp_obj_new_int(out.magnitude);
    o->theta = mp_obj_new_int(out.theta);
    o->rho = mp_obj_new_int(out.rho);

    return o;
}

#endif // IMLIB_ENABLE_GET_REGRESSION

#if IMLIB_ENABLE_FIND_CIRCLES
static mp_obj_t py_image_find_circles(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    unsigned int x_stride = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_stride), 2);
    PY_ASSERT_TRUE_MSG(x_stride > 0, "x_stride must not be zero.");
    unsigned int y_stride = py_helper_keyword_int(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_stride), 1);
    PY_ASSERT_TRUE_MSG(y_stride > 0, "y_stride must not be zero.");
    uint32_t threshold = py_helper_keyword_int(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold), 2000);
    unsigned int x_margin = py_helper_keyword_int(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_margin), 10);
    unsigned int y_margin = py_helper_keyword_int(n_args, args, 6, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_margin), 10);
    unsigned int r_margin = py_helper_keyword_int(n_args, args, 7, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_r_margin), 10);
    unsigned int r_min = IM_MAX(py_helper_keyword_int(n_args, args, 8, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_r_min),
            2), 2);
    unsigned int r_max = IM_MIN(py_helper_keyword_int(n_args, args, 9, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_r_max),
            IM_MIN((roi.w / 2), (roi.h / 2))), IM_MIN((roi.w / 2), (roi.h / 2)));
    unsigned int r_step = py_helper_keyword_int(n_args, args, 10, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_r_step), 2);

    list_t out;
    fb_alloc_mark();
    imlib_find_circles(&out, arg_img, &roi, x_stride, y_stride, threshold, x_margin, y_margin, r_margin,
                       r_min, r_max, r_step);
    fb_alloc_free_till_mark();

    mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
    for (size_t i = 0; list_size(&out); i++) {
        find_circles_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        py_circle_obj_t *o = m_new_obj(py_circle_obj_t);
        o->base.type = &py_circle_type;
        o->x = mp_obj_new_int(lnk_data.p.x);
        o->y = mp_obj_new_int(lnk_data.p.y);
        o->r = mp_obj_new_int(lnk_data.r);
        o->magnitude = mp_obj_new_int(lnk_data.magnitude);

        objects_list->items[i] = o;
    }

    return objects_list;
}
#endif // IMLIB_ENABLE_FIND_CIRCLES

#if IMLIB_ENABLE_FIND_RECTS
static mp_obj_t py_image_find_rects(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    uint32_t threshold = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold), 1000);

    list_t out;
    fb_alloc_mark();
    imlib_find_rects(&out, arg_img, &roi, threshold);
    fb_alloc_free_till_mark();

    mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
    for (size_t i = 0; list_size(&out); i++) {
        find_rects_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        py_rect_obj_t *o = m_new_obj(py_rect_obj_t);
        o->base.type = &py_rect_type;
        o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
            {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[0].x), mp_obj_new_int(lnk_data.corners[0].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[1].x), mp_obj_new_int(lnk_data.corners[1].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[2].x), mp_obj_new_int(lnk_data.corners[2].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[3].x), mp_obj_new_int(lnk_data.corners[3].y)})});
        o->x = mp_obj_new_int(lnk_data.rect.x);
        o->y = mp_obj_new_int(lnk_data.rect.y);
        o->w = mp_obj_new_int(lnk_data.rect.w);
        o->h = mp_obj_new_int(lnk_data.rect.h);
        o->magnitude = mp_obj_new_int(lnk_data.magnitude);

        objects_list->items[i] = o;
    }

    return objects_list;
}

#endif // IMLIB_ENABLE_FIND_RECTS

#if IMLIB_ENABLE_QRCODES
static mp_obj_t py_image_find_qrcodes(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    list_t out;
    fb_alloc_mark();
    imlib_find_qrcodes(&out, arg_img, &roi);
    fb_alloc_free_till_mark();

    mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
    for (size_t i = 0; list_size(&out); i++) {
        find_qrcodes_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        py_qrcode_obj_t *o = m_new_obj(py_qrcode_obj_t);
        o->base.type = &py_qrcode_type;
        o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
            {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[0].x), mp_obj_new_int(lnk_data.corners[0].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[1].x), mp_obj_new_int(lnk_data.corners[1].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[2].x), mp_obj_new_int(lnk_data.corners[2].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[3].x), mp_obj_new_int(lnk_data.corners[3].y)})});
        o->x = mp_obj_new_int(lnk_data.rect.x);
        o->y = mp_obj_new_int(lnk_data.rect.y);
        o->w = mp_obj_new_int(lnk_data.rect.w);
        o->h = mp_obj_new_int(lnk_data.rect.h);
        o->payload = mp_obj_new_str(lnk_data.payload, lnk_data.payload_len);
        o->version = mp_obj_new_int(lnk_data.version);
        o->ecc_level = mp_obj_new_int(lnk_data.ecc_level);
        o->mask = mp_obj_new_int(lnk_data.mask);
        o->data_type = mp_obj_new_int(lnk_data.data_type);
        o->eci = mp_obj_new_int(lnk_data.eci);

        objects_list->items[i] = o;
        xfree(lnk_data.payload);
    }

    return objects_list;
}

#endif // IMLIB_ENABLE_QRCODES

#if IMLIB_ENABLE_APRILTAGS
static mp_obj_t py_image_find_apriltags(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_mutable_bayer_jpeg(args[0]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);
#ifndef IMLIB_ENABLE_HIGH_RES_APRILTAGS
    PY_ASSERT_TRUE_MSG((roi.w * roi.h) < 65536, "The maximum supported resolution for find_apriltags() is < 64K pixels.");
#endif
    if ((roi.w < 4) || (roi.h < 4)) {
        return mp_obj_new_list(0, NULL);
    }

    apriltag_families_t families = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_families), TAG36H11);
    // 2.8mm Focal Length w/ OV7725 sensor for reference.
    float fx = py_helper_keyword_float(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_fx), (2.8 / 3.984) * arg_img->w);
    // 2.8mm Focal Length w/ OV7725 sensor for reference.
    float fy = py_helper_keyword_float(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_fy), (2.8 / 2.952) * arg_img->h);
    // Use the image versus the roi here since the image should be projected from the camera center.
    float cx = py_helper_keyword_float(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_cx), arg_img->w * 0.5);
    // Use the image versus the roi here since the image should be projected from the camera center.
    float cy = py_helper_keyword_float(n_args, args, 6, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_cy), arg_img->h * 0.5);

    list_t out;
    fb_alloc_mark();
    imlib_find_apriltags(&out, arg_img, &roi, families, fx, fy, cx, cy);
    fb_alloc_free_till_mark();

    mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
    for (size_t i = 0; list_size(&out); i++) {
        find_apriltags_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        py_apriltag_obj_t *o = m_new_obj(py_apriltag_obj_t);
        o->base.type = &py_apriltag_type;
        o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
            {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[0].x), mp_obj_new_int(lnk_data.corners[0].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[1].x), mp_obj_new_int(lnk_data.corners[1].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[2].x), mp_obj_new_int(lnk_data.corners[2].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[3].x), mp_obj_new_int(lnk_data.corners[3].y)})});
        o->x = mp_obj_new_int(lnk_data.rect.x);
        o->y = mp_obj_new_int(lnk_data.rect.y);
        o->w = mp_obj_new_int(lnk_data.rect.w);
        o->h = mp_obj_new_int(lnk_data.rect.h);
        o->id = mp_obj_new_int(lnk_data.id);
        o->family = mp_obj_new_int(lnk_data.family);
        o->cx = mp_obj_new_int(lnk_data.centroid.x);
        o->cy = mp_obj_new_int(lnk_data.centroid.y);
        o->rotation = mp_obj_new_float(lnk_data.z_rotation);
        o->decision_margin = mp_obj_new_float(lnk_data.decision_margin);
        o->hamming = mp_obj_new_int(lnk_data.hamming);
        o->goodness = mp_obj_new_float(lnk_data.goodness);
        o->x_translation = mp_obj_new_float(lnk_data.x_translation);
        o->y_translation = mp_obj_new_float(lnk_data.y_translation);
        o->z_translation = mp_obj_new_float(lnk_data.z_translation);
        o->x_rotation = mp_obj_new_float(lnk_data.x_rotation);
        o->y_rotation = mp_obj_new_float(lnk_data.y_rotation);
        o->z_rotation = mp_obj_new_float(lnk_data.z_rotation);

        objects_list->items[i] = o;
    }

    return objects_list;
}

#endif // IMLIB_ENABLE_APRILTAGS

#if IMLIB_ENABLE_DATAMATRICES
static mp_obj_t py_image_find_datamatrices(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    int effort = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_effort), 200);

    list_t out;
    fb_alloc_mark();
    imlib_find_datamatrices(&out, arg_img, &roi, effort);
    fb_alloc_free_till_mark();

    mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
    for (size_t i = 0; list_size(&out); i++) {
        find_datamatrices_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        py_datamatrix_obj_t *o = m_new_obj(py_datamatrix_obj_t);
        o->base.type = &py_datamatrix_type;
        o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
            {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[0].x), mp_obj_new_int(lnk_data.corners[0].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[1].x), mp_obj_new_int(lnk_data.corners[1].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[2].x), mp_obj_new_int(lnk_data.corners[2].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[3].x), mp_obj_new_int(lnk_data.corners[3].y)})});
        o->x = mp_obj_new_int(lnk_data.rect.x);
        o->y = mp_obj_new_int(lnk_data.rect.y);
        o->w = mp_obj_new_int(lnk_data.rect.w);
        o->h = mp_obj_new_int(lnk_data.rect.h);
        o->payload = mp_obj_new_str(lnk_data.payload, lnk_data.payload_len);
        o->rotation = mp_obj_new_float(IM_DEG2RAD(lnk_data.rotation));
        o->rows = mp_obj_new_int(lnk_data.rows);
        o->columns = mp_obj_new_int(lnk_data.columns);
        o->capacity = mp_obj_new_int(lnk_data.capacity);
        o->padding = mp_obj_new_int(lnk_data.padding);

        objects_list->items[i] = o;
        xfree(lnk_data.payload);
    }

    return objects_list;
}
#endif // IMLIB_ENABLE_DATAMATRICES

#if IMLIB_ENABLE_BARCODES
static mp_obj_t py_image_find_barcodes(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    list_t out;
    fb_alloc_mark();
    imlib_find_barcodes(&out, arg_img, &roi);
    fb_alloc_free_till_mark();

    mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
    for (size_t i = 0; list_size(&out); i++) {
        find_barcodes_list_lnk_data_t lnk_data;
        list_pop_front(&out, &lnk_data);

        py_barcode_obj_t *o = m_new_obj(py_barcode_obj_t);
        o->base.type = &py_barcode_type;
        o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
            {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[0].x), mp_obj_new_int(lnk_data.corners[0].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[1].x), mp_obj_new_int(lnk_data.corners[1].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[2].x), mp_obj_new_int(lnk_data.corners[2].y)}),
             mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[3].x), mp_obj_new_int(lnk_data.corners[3].y)})});
        o->x = mp_obj_new_int(lnk_data.rect.x);
        o->y = mp_obj_new_int(lnk_data.rect.y);
        o->w = mp_obj_new_int(lnk_data.rect.w);
        o->h = mp_obj_new_int(lnk_data.rect.h);
        o->payload = mp_obj_new_str(lnk_data.payload, lnk_data.payload_len);
        o->type = mp_obj_new_int(lnk_data.type);
        o->rotation = mp_obj_new_float(IM_DEG2RAD(lnk_data.rotation));
        o->quality = mp_obj_new_int(lnk_data.quality);

        objects_list->items[i] = o;
        xfree(lnk_data.payload);
    }

    return objects_list;
}
#endif // IMLIB_ENABLE_BARCODES

#if IMLIB_ENABLE_FIND_DISPLACEMENT
static mp_obj_t py_image_find_displacement(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_mutable(args[0]);
    image_t *arg_template_img = py_helper_arg_to_image_mutable(args[1]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 2, kw_args, &roi);

    rectangle_t template_roi;
    py_helper_keyword_rectangle(arg_template_img, n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_template_roi), &template_roi);

    PY_ASSERT_FALSE_MSG((roi.w != template_roi.w) || (roi.h != template_roi.h), "ROI(w,h) != TEMPLATE_ROI(w,h)");

    bool logpolar = py_helper_keyword_int(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_logpolar), false);
    bool fix_rotation_scale = py_helper_keyword_int(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_fix_rotation_scale), false);

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


#if IMLIB_ENABLE_DESCRIPTOR
static mp_obj_t py_image_match_descriptor(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    mp_obj_t match_obj = mp_const_none;
    const mp_obj_type_t *desc1_type = mp_obj_get_type(args[0]);
    const mp_obj_type_t *desc2_type = mp_obj_get_type(args[1]);
    PY_ASSERT_TRUE_MSG((desc1_type == desc2_type), "Descriptors have different types!");

    if (0) {
    #if defined(IMLIB_ENABLE_FIND_LBP)
    } else if (desc1_type ==  &py_lbp_type) {
        py_lbp_obj_t *lbp1 = ((py_lbp_obj_t*)args[0]);
        py_lbp_obj_t *lbp2 = ((py_lbp_obj_t*)args[1]);

        // Sanity checks
        PY_ASSERT_TYPE(lbp1, &py_lbp_type);
        PY_ASSERT_TYPE(lbp2, &py_lbp_type);

        // Match descriptors
        match_obj = mp_obj_new_int(imlib_lbp_desc_distance(lbp1->hist, lbp2->hist));
    #endif //IMLIB_ENABLE_FIND_LBP
    #if defined(IMLIB_ENABLE_FIND_KEYPOINTS)
    } else if (desc1_type == &py_kp_type) {
        py_kp_obj_t *kpts1 = ((py_kp_obj_t*)args[0]);
        py_kp_obj_t *kpts2 = ((py_kp_obj_t*)args[1]);
        int threshold = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold), 85);
        int filter_outliers = py_helper_keyword_int(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_filter_outliers), false);

        // Sanity checks
        PY_ASSERT_TYPE(kpts1, &py_kp_type);
        PY_ASSERT_TYPE(kpts2, &py_kp_type);
        PY_ASSERT_TRUE_MSG((threshold >=0 && threshold <= 100), "Expected threshold between 0 and 100");

        int theta = 0;          // Estimated angle of rotation
        int count = 0;          // Number of matches
        point_t c = {0};        // Centroid
        rectangle_t r = {0};    // Bounding rectangle
        // List of matching keypoints indices
        mp_obj_t match_list = mp_obj_new_list(0, NULL);

        if (array_length(kpts1->kpts) && array_length(kpts1->kpts)) {
            fb_alloc_mark();
            int *match = fb_alloc(array_length(kpts1->kpts) * sizeof(int) * 2, FB_ALLOC_NO_HINT);

            // Match the two keypoint sets
            count = orb_match_keypoints(kpts1->kpts, kpts2->kpts, match, threshold, &r, &c, &theta);

            // Add matching keypoints to Python list.
            for (int i=0; i<count*2; i+=2) {
                mp_obj_t index_obj[2] = {
                    mp_obj_new_int(match[i+0]),
                    mp_obj_new_int(match[i+1]),
                };
                mp_obj_list_append(match_list, mp_obj_new_tuple(2, index_obj));
            }

            // Free match list
            fb_alloc_free_till_mark();

            if (filter_outliers == true) {
                count = orb_filter_keypoints(kpts2->kpts, &r, &c);
            }
        }

        py_kptmatch_obj_t *o = m_new_obj(py_kptmatch_obj_t);
        o->base.type = &py_kptmatch_type;
        o->cx    = mp_obj_new_int(c.x);
        o->cy    = mp_obj_new_int(c.y);
        o->x     = mp_obj_new_int(r.x);
        o->y     = mp_obj_new_int(r.y);
        o->w     = mp_obj_new_int(r.w);
        o->h     = mp_obj_new_int(r.h);
        o->count = mp_obj_new_int(count);
        o->theta = mp_obj_new_int(theta);
        o->match = match_list;
        match_obj = o;
    #endif //IMLIB_ENABLE_FIND_KEYPOINTS
    } else {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("Descriptor type is not supported"));
    }

    return match_obj;
}

#endif //IMLIB_ENABLE_DESCRIPTOR


////////////////
// Basic Methods
////////////////


static PyObject *
py_image_width(PyObject *self, PyObject *args, PyObject *keywds)
{
    return Py_BuildValue("i",self_image_img.w);
}

static PyObject *
py_image_height(mp_obj_t img_obj)
{
    return Py_BuildValue("i",self_image_img.h);
}




// static PyObject *
// keywdarg_parrot(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     int voltage;
//     const char *state = "a stiff";
//     const char *action = "voom";
//     const char *type = "Norwegian Blue";

//     static char *kwlist[] = {"voltage", "state", "action", "type", NULL};

//     if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|sss", kwlist,
//                                      &voltage, &state, &action, &type))
//         return NULL;

//     printf("-- This parrot wouldn't %s if you put %i Volts through it.\n",
//            action, voltage);
//     printf("-- Lovely plumage, the %s -- It's %s!\n", type, state);

    // Py_RETURN_NONE;
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
	printf("thickness:%d\r\n", thickness);
	imlib_draw_line(&arg_img, x0, y0, x1, y1, c, thickness);
	return PyLong_FromLong(0);
}

static PyObject *
py_image_find_blobs(PyObject *self, PyObject *args, PyObject *keywds)
{
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

	list_t out;														  //定义输出列表
	list_t thresholds;												  //定义一个LAB阈值列表
	list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t)); //列表初始化
	color_thresholds_list_lnk_data_t thrm;
	rectangle_t roi; //定义感兴趣区域
	bool invert;	 //反相标志
	bool merge;		 //合并标志

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

	fb_alloc_mark(); //未知函数,暂时先调用
	int thr_len;
	int thrs;
	thr_len = PyList_Size(rece_sta.thresholds);
	if (thr_len == 0)
		return PyList_New(0); //检查传递阈值
	for (int i = 0; i < thr_len; i++)
	{
		thr = PyList_GetItem(rece_sta.thresholds, i);
		if (!PyTuple_Check(thr))
			return NULL;
		thrs = PyTuple_Size(thr);
		if (thrs != 6)
			return NULL;
		thr2 = PyTuple_GetItem(thr, 0);
		thrm.LMin = PyLong_AsLong(thr2);
		thr2 = PyTuple_GetItem(thr, 1);
		thrm.LMax = PyLong_AsLong(thr2);
		thr2 = PyTuple_GetItem(thr, 2);
		thrm.AMin = PyLong_AsLong(thr2);
		thr2 = PyTuple_GetItem(thr, 3);
		thrm.AMax = PyLong_AsLong(thr2);
		thr2 = PyTuple_GetItem(thr, 4);
		thrm.BMin = PyLong_AsLong(thr2);
		thr2 = PyTuple_GetItem(thr, 5);
		thrm.BMax = PyLong_AsLong(thr2);
		list_push_back(&thresholds, &thrm);
	}
	thr_len = PyList_Size(rece_sta.roi);
	if (thr_len == 0)
	{
		roi.x = 0;
		roi.y = 0;
		roi.w = arg_img.w;
		roi.h = arg_img.h;
	}
	else if (thr_len == 4)
	{
		thr = PyList_GetItem(rece_sta.roi, 0);
		roi.x = PyLong_AsLong(thr);
		thr = PyList_GetItem(rece_sta.roi, 1);
		roi.y = PyLong_AsLong(thr);
		thr = PyList_GetItem(rece_sta.roi, 2);
		roi.w = PyLong_AsLong(thr);
		thr = PyList_GetItem(rece_sta.roi, 3);
		roi.h = PyLong_AsLong(thr);
	}
	else
	{
		return NULL;
	}

	if (rece_sta.invert == Py_True)
		invert = 1;
	else
		invert = 0;

	if (rece_sta.merge == Py_True)
		merge = 1;
	else
		merge = 0;

	imlib_find_blobs(&out, &arg_img, &roi, rece_sta.x_stride, rece_sta.y_stride, &thresholds, invert,
					 rece_sta.area_threshold, rece_sta.pixels_threshold, merge, rece_sta.margin,NULL,NULL,NULL,NULL,0,0);

	thr = PyList_New(0);

	while (out.size > 0)
	{
		list_pop_front(&out, &lnk_blob);

		thr2 = PyDict_New();
		Py_INCREF(thr2);
		//目前并不清楚这里需不需要加上引用,后期可以选择在这里优化一下
		thr3 = PyLong_FromLong(lnk_blob.rect.x);
		PyDict_SetItemString(thr2, "x", thr3);
		thr3 = PyLong_FromLong(lnk_blob.rect.y);
		PyDict_SetItemString(thr2, "y", thr3);
		thr3 = PyLong_FromLong(lnk_blob.rect.w);
		PyDict_SetItemString(thr2, "w", thr3);
		thr3 = PyLong_FromLong(lnk_blob.rect.h);
		PyDict_SetItemString(thr2, "h", thr3);
		thr3 = PyLong_FromLong(lnk_blob.pixels);
		PyDict_SetItemString(thr2, "pixels", thr3);
		thr3 = PyLong_FromLong(lnk_blob.centroid.x);
		PyDict_SetItemString(thr2, "cx", thr3);
		thr3 = PyLong_FromLong(lnk_blob.centroid.y);
		PyDict_SetItemString(thr2, "cy", thr3);
		thr3 = PyFloat_FromDouble(lnk_blob.rotation);
		PyDict_SetItemString(thr2, "rotation", thr3);
		thr3 = PyLong_FromLong(lnk_blob.code);
		PyDict_SetItemString(thr2, "code", thr3);
		thr3 = PyLong_FromLong(lnk_blob.count);
		PyDict_SetItemString(thr2, "count", thr3);
		PyList_Append(thr, thr2);
	}
	return thr;
}

static PyMethodDef imageMethods[] = {
	{"send_to_image", python_send_to_c_image, METH_VARARGS, "python to c module image!"},

	{"find_blobs", (PyCFunction)py_find_blobs, METH_VARARGS | METH_KEYWORDS, "find blob !"},
	{"img_back", (PyCFunction)py_img_back, METH_VARARGS, "back an rgb888 img !"},
	{"draw_line", (PyCFunction)py_draw_line, METH_VARARGS | METH_KEYWORDS, "img draw line"},

	{NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef spammodule = {
	PyModuleDef_HEAD_INIT,
	"minicv", /* name of module */
	NULL,	  /* module documentation, may be NULL */
	-1,		  /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
	imageMethods};

PyMODINIT_FUNC
PyInit_minicv(void)
{
	return PyModule_Create(&spammodule);
}
