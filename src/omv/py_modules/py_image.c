#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "collections.h"
#include "imdefs.h"
#include "imlib.h"
#include "tanstation.h"

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
    image_t *arg_img = &self_image_img;

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
    image_t *arg_img = &self_image_img;

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
    image_t *arg_img = &self_image_img;

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
    image_t *arg_img = &self_image_img;

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
    image_t *arg_img = &self_image_img;

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
    image_t *arg_img = &self_image_img;

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
    image_t *arg_img = &self_image_img;

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
    image_t *arg_img = &self_image_img;

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
    image_t *arg_img = &self_image_img;

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
    image_t *arg_img = &self_image_img;

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
    image_t *arg_img = &self_image_img;

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
    image_t *arg_img = &self_image_img;
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
py_image_height(PyObject *self, PyObject *args, PyObject *keywds)
{
    return Py_BuildValue("i",self_image_img.h);
}

static PyObject *
py_image_format(PyObject *self, PyObject *args)
{
    switch (self_image_img.bpp) {
        case IMAGE_BPP_BINARY: return  Py_BuildValue("i",PIXFORMAT_BINARY);
        case IMAGE_BPP_GRAYSCALE: return  Py_BuildValue("i",PIXFORMAT_GRAYSCALE);
        case IMAGE_BPP_RGB565: return  Py_BuildValue("i",PIXFORMAT_RGB565);
        case IMAGE_BPP_BAYER: return  Py_BuildValue("i",PIXFORMAT_BAYER);
        default: return  Py_BuildValue("i",PIXFORMAT_JPEG);
    }
}

static PyObject *
py_image_size(PyObject *self, PyObject *args)
{
    return Py_BuildValue("ii",self_image_img.w,self_image_img.h));
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

    static char *kwlist[] = {"x","y","rgbtuple", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|OO", kwlist,
                                     &py_arg_x,&py_arg_y,&py_rgbtuple))return NULL;
    if(PyTuple_Check(py_arg_x))
    {
        arg_x =  PyLong_AsLong(PyTuple_GetItem(py_arg_x,0));
        arg_y =  PyLong_AsLong(PyTuple_GetItem(py_arg_x,1));
        
    }
    else
    {
        arg_x = PyLong_AsLong(py_arg_x);
        arg_y = PyLong_AsLong(py_arg_y);
    }
    if (PyBool_Check(py_rgbtuple))
    {
        if (py_rgbtuple == Py_True)
            arg_rgbtuple = 1;
        else
            arg_rgbtuple = 0;
    }
    else
    {
        if (PyLong_AsLong(py_rgbtuple) == 1)
            arg_rgbtuple = 1;
        else
            arg_rgbtuple = 0;
    }
    if ((!IM_X_INSIDE(arg_img, arg_x)) || (!IM_Y_INSIDE(arg_img, arg_y))) {
        return Py_None;
    }

    switch (arg_img->bpp) {
        case IMAGE_BPP_BINARY: {
            if (arg_rgbtuple) {
                int pixel = IMAGE_GET_BINARY_PIXEL(arg_img, arg_x, arg_y);
                return Py_BuildValue("iii",COLOR_RGB565_TO_R8(COLOR_BINARY_TO_RGB565(pixel)),COLOR_RGB565_TO_G8(COLOR_BINARY_TO_RGB565(pixel)),COLOR_RGB565_TO_B8(COLOR_BINARY_TO_RGB565(pixel)));

            } else {
                return Py_BuildValue("i",IMAGE_GET_BINARY_PIXEL(arg_img, arg_x, arg_y));
            }
        }
        case IMAGE_BPP_GRAYSCALE: {
            if (arg_rgbtuple) {
                int pixel = IMAGE_GET_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y);


                return Py_BuildValue("iii",COLOR_RGB565_TO_R8(COLOR_GRAYSCALE_TO_RGB565(pixel)),COLOR_RGB565_TO_G8(COLOR_GRAYSCALE_TO_RGB565(pixel)),COLOR_RGB565_TO_B8(COLOR_GRAYSCALE_TO_RGB565(pixel)));
            } else {
                return Py_BuildValue("i",IMAGE_GET_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y));
            }
        }
        case IMAGE_BPP_RGB565: {
            if (arg_rgbtuple) {
                int pixel = IMAGE_GET_RGB565_PIXEL(arg_img, arg_x, arg_y);
                return Py_BuildValue("iii",COLOR_RGB565_TO_R8(pixel),COLOR_RGB565_TO_G8(pixel),COLOR_RGB565_TO_B8(pixel));
            } else {
                return Py_BuildValue("i",IMAGE_GET_RGB565_PIXEL(arg_img, arg_x, arg_y));
            }
        }
        case IMAGE_BPP_BAYER:
            if (arg_rgbtuple) {
                uint16_t pixel; imlib_debayer_line_to_rgb565(arg_x, arg_x + 1, arg_y, &pixel, arg_img);
                return Py_BuildValue("iii",COLOR_RGB565_TO_R8(pixel),COLOR_RGB565_TO_G8(pixel),COLOR_RGB565_TO_B8(pixel));
            } else {
                return Py_BuildValue("i",IMAGE_GET_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y)); // Correct!
            }
        default: return Py_None;
    }
}

static PyObject *
py_image_set_pixel(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    int arg_x = 0;
    int arg_y = 0;
    int arg_c =0xffffff;

    static char *kwlist[] = {"x","y","c", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iii", kwlist,
                                     &arg_x,&arg_y,&arg_c))return NULL;



    if ((!IM_X_INSIDE(arg_img, arg_x)) || (!IM_Y_INSIDE(arg_img, arg_y))) {
        return Py_None;
    }

    switch (arg_img->bpp) {
        case IMAGE_BPP_BINARY: {
            IMAGE_PUT_BINARY_PIXEL(arg_img, arg_x, arg_y, arg_c);
            return Py_None;
        }
        case IMAGE_BPP_GRAYSCALE: {
            IMAGE_PUT_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y, arg_c);
            return Py_None;
        }
        case IMAGE_BPP_RGB565: {
            IMAGE_PUT_RGB565_PIXEL(arg_img, arg_x, arg_y, arg_c);
            return Py_None;
        }
        case IMAGE_BPP_BAYER: {
            IMAGE_PUT_GRAYSCALE_PIXEL(arg_img, arg_x, arg_y, arg_c); // Correct!
            return Py_None;
        }
        default: return Py_None;
    }
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


// static PyObject *
// py_image_to(PyObject *self, PyObject *args, PyObject *keywds)
// {
//     image_t *src_img = &self_image_img;
//     PyObject *img_tuple;
//     PyObject *img_data;
//     PyObject *img_w;
//     PyObject *img_h;

//     float arg_x_scale = 1.f;
//     bool got_x_scale = 0;

//     float arg_y_scale = 1.f;
//     bool got_y_scale = 0;

//     rectangle_t arg_roi;


//     int arg_rgb_channel = 0;
//     if ((arg_rgb_channel < -1) || (2 < arg_rgb_channel)) {
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("-1 <= rgb_channel <= 2!"));
//         return NULL;
//     }

//     int arg_alpha = 0;
//     if ((arg_alpha < 0) || (256 < arg_alpha)) {
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("0 <= alpha <= 256!"));
//         return NULL;
//     }

//     const uint16_t *color_palette = NULL;
//     const uint8_t *alpha_palette = NULL;

//     image_hint_t hint ;

//     int arg_x_size;
//     bool got_x_size =0;

//     int arg_y_size;
//     bool got_y_size = 0;

//     if (got_x_scale && got_x_size) {
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Choose either x_scale or x_size not both!"));
//         return NULL;
//     }

//     if (got_y_scale && got_y_size) {
//         // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Choose either y_scale or y_size not both!"));
//         return NULL;
//     }

//     if (got_x_size) {
//         arg_x_scale = arg_x_size / ((float) arg_roi.w);
//     }

//     if (got_y_size) {
//         arg_y_scale = arg_y_size / ((float) arg_roi.h);
//     }

//     if ((!got_x_scale) && (!got_x_size) && got_y_size) {
//         arg_x_scale = arg_y_scale;
//     }

//     if ((!got_y_scale) && (!got_y_size) && got_x_size) {
//         arg_y_scale = arg_x_scale;
//     }

//     // mp_obj_t copy_obj ;
//     bool copy = false;
//     image_t *arg_other = copy_to_fb ? NULL : src_img;

//     // if (copy_obj) {
//     //     if (mp_obj_is_integer(copy_obj)) {
//     //         copy = mp_obj_get_int(copy_obj);
//     //     } else {
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

//     if (dst_img.bpp == IMAGE_BPP_BAYER) {
//         if (((arg_x_scale != 1) && (arg_x_scale != -1)) ||
//             ((arg_y_scale != 1) && (arg_y_scale != -1)) ||
//             (arg_rgb_channel != -1) ||
//             (arg_alpha != 256) ||
//             (color_palette != NULL) ||
//             (alpha_palette != NULL)) {
//             // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Only bayer copying/cropping is supported!"));
//             return NULL
//         } else {
//             hint &= ~(IMAGE_HINT_AREA |
//                       IMAGE_HINT_BICUBIC |
//                       IMAGE_HINT_BILINEAR |
//                       IMAGE_HINT_EXTRACT_RGB_CHANNEL_FIRST |
//                       IMAGE_HINT_APPLY_COLOR_PALETTE_FIRST);
//         }
//     } else if (dst_img.bpp >= IMAGE_BPP_JPEG) {
//         if ((arg_x_scale != 1) ||
//             (arg_y_scale != 1) ||
//             (arg_roi.x != 0) ||
//             (arg_roi.y != 0) ||
//             (arg_roi.w != src_img->w) ||
//             (arg_roi.h != src_img->h) ||
//             (arg_rgb_channel != -1) ||
//             (arg_alpha != 256) ||
//             (color_palette != NULL) ||
//             (alpha_palette != NULL)) {
//             // mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Only jpeg copying is supported!"));
//             return NULL;
//         }
//     }

//     if (copy) {
//         if (copy_to_fb) {
//             // py_helper_set_to_framebuffer(&dst_img);
//         } else {
//             // dst_img.data = xalloc(image_size(&dst_img));
//         }
//     } else if (arg_other) {
//         // bool fb = py_helper_is_equal_to_framebuffer(arg_other);
//         // size_t size = fb ? framebuffer_get_buffer_size() : image_size(arg_other);
//         // PY_ASSERT_TRUE_MSG((image_size(&dst_img) <= size),
//         //         "The new image won't fit in the target frame buffer!");
//         return NULL;
//         // DO NOT MODIFY arg_other YET (as it could point to src_img)!
//         dst_img.data = arg_other->data;
//         // py_helper_update_framebuffer(&dst_img);
//     } else {
//         // dst_img.data = xalloc(image_size(&dst_img));
//     }

//     if (dst_img.bpp >= IMAGE_BPP_JPEG) {
//         if (dst_img.data != src_img->data) {
//             memcpy(dst_img.data, src_img->data, dst_img.bpp);
//         }
//     } else {
//         fb_alloc_mark();
//         imlib_draw_image(&dst_img, src_img, 0, 0, arg_x_scale, arg_y_scale, &arg_roi,
//                         arg_rgb_channel, arg_alpha, color_palette, alpha_palette,
//                         (hint & (~IMAGE_HINT_CENTER)) | IMAGE_HINT_BLACK_BACKGROUND, NULL, NULL);
//         fb_alloc_free_till_mark();
//     }

//     if (arg_other) {
//         arg_other->w = dst_img.w;
//         arg_other->h = dst_img.h;
//         arg_other->bpp = dst_img.bpp;
//     }
//     return img_tuple;
//     // return py_image_from_struct(&dst_img);
// }

// static PyObject * py_image_to_bitmap(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
// {
//     return py_image_to(IMAGE_BPP_BINARY, NULL, false, n_args, args, kw_args);
// }

// static PyObject * py_image_to_grayscale(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
// {
//     return py_image_to(IMAGE_BPP_GRAYSCALE, NULL, false, n_args, args, kw_args);
// }

// static PyObject * py_image_to_rgb565(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
// {
//     return py_image_to(IMAGE_BPP_RGB565, NULL, false, n_args, args, kw_args);
// }

// static PyObject * py_image_to_rainbow(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
// {
//     return py_image_to(IMAGE_BPP_RGB565, rainbow_table, false, n_args, args, kw_args);
// }

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

// static PyObject * py_image_compressed(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
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

// static PyObject * py_image_compressed_for_ide(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
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
py_image_draw_line(PyObject *self, PyObject *args, PyObject* keywds)
{
	int x0;
	int y0;
	int x1;
	int y1;
	int c = 0xffffff;
	int thickness = 1;
	
	static char *kwlist[] = {"x0","y0","x1","y1","c","thickness", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iiii|ii", kwlist,
                              &x0,&y0,&x1,&y1,&c,&thickness)) return NULL;
	// printf("thickness:%d\r\n",thickness);
	imlib_draw_line(&arg_img,x0,y0,x1,y1,c,thickness);
	return PyLong_FromLong(0);
}


static PyObject *
py_draw_rectangle(PyObject *self, PyObject *args, PyObject* keywds)
{
	PyObject *fil;
	int rx;
	int ry;
	int rw ;
	int rh;
	int c = 0xffffff;
	int thickness = 1;
	bool fill = 0;
	static char *kwlist[] = {"rx","ry","rw","rh","c","thickness","fill", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iiii|iiO", kwlist,
                              &rx,&ry,&rw,&rh,&c,&thickness,&fil)) return NULL;

	if(fil == Py_True) fill = 1;else fill = 0;


	imlib_draw_rectangle(&arg_img, rx, ry, rw, rh, c, thickness, fill);

	return PyLong_FromLong(0);
}

static PyObject *
py_image_draw_circle(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    PyObject *fil;
    int arg_cx = 0;
    int arg_cy = 0;
    int arg_cr = 0;
    int arg_c = 0xffffff ;
    int arg_thickness = 1;
    bool arg_fill = 0;

    static char *kwlist[] = {"cx","cy","cr","c","thickness","fill", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iii|iiO", kwlist,
                              &arg_cx,&arg_cy,&arg_cr,&arg_c,&arg_thickness,&fil)) return NULL;

    if(fil == Py_True) arg_fill = 1;else arg_fill = 0;
    imlib_draw_circle(arg_img, arg_cx, arg_cy, arg_cr, arg_c, arg_thickness, arg_fill);
    return PyLong_FromLong(0);
}




static PyObject *
py_image_draw_ellipse(PyObject *self, PyObject *args, PyObject* keywds)
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

    static char *kwlist[] = {"cx","cy","rx","ry","r","c","thickness","fill", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iiiii|iiO", kwlist,
                              &arg_cx,&arg_cy,&arg_rx,&arg_ry,&arg_r,&arg_c,&arg_thickness,&fil)) return NULL;
    if(fil == Py_True) arg_fill = 1;else arg_fill = 0;
    imlib_draw_ellipse(arg_img, arg_cx, arg_cy, arg_rx, arg_ry, arg_r, arg_c, arg_thickness, arg_fill);
    return PyLong_FromLong(0);
}



// static PyObject * py_image_draw_string(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
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
py_image_draw_cross(PyObject *self, PyObject *args, PyObject* keywds)
{
{
    image_t *arg_img = &self_image_img;
    int arg_x = 0;
    int arg_y = 0;

    int arg_c = 0xffffff;
    int arg_s = 5;
    int arg_thickness = 1;

    static char *kwlist[] = {"x","y","c","size","thickness", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ii|iii", kwlist,
                              &arg_x,&arg_y,&arg_c,&arg_s,&arg_thickness)) return NULL;


    imlib_draw_line(arg_img, arg_x - arg_s, arg_y        , arg_x + arg_s, arg_y        , arg_c, arg_thickness);
    imlib_draw_line(arg_img, arg_x        , arg_y - arg_s, arg_x        , arg_y + arg_s, arg_c, arg_thickness);
    return PyLong_FromLong(0);
}

static PyObject *
py_image_draw_arrow(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;

 
    int arg_x0 = 0;
    int arg_y0 = 0;
    int arg_x1 = 0;
    int arg_y1 = 0;

    int arg_c = 0xfffffff;
    int arg_thickness = 1;

	
	static char *kwlist[] = {"x0","y0","x1","y1","c","thickness", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "iiii|ii", kwlist,
                              &arg_x0,&arg_y0,&arg_x1,&arg_y1,&arg_c,&arg_thickness)) return NULL;
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

    imlib_draw_line(arg_img, arg_x0, arg_y0, arg_x1, arg_y1, arg_c, arg_thickness);
    imlib_draw_line(arg_img, arg_x1, arg_y1, a0x, a0y, arg_c, arg_thickness);
    imlib_draw_line(arg_img, arg_x1, arg_y1, a1x, a1y, arg_c, arg_thickness);
    return PyLong_FromLong(0);
}

static PyObject *
py_image_draw_edges(PyObject *self, PyObject *args, PyObject* keywds)
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

	static char *kwlist[] = {"x0","y0","x1","y1","x2","y2","x3","y3","color","size","thickness","fill", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "[(ii)(ii)(ii)(ii)]|iiii", kwlist,
                              &x0,&y0,&x1,&y1,&x2,&y2,&x3,&y3,&arg_c,&arg_s,&arg_thickness,&arg_fill)) return NULL;

    imlib_draw_line(arg_img, x0, y0, x1, y1, arg_c, arg_thickness);
    imlib_draw_line(arg_img, x1, y1, x2, y2, arg_c, arg_thickness);
    imlib_draw_line(arg_img, x2, y2, x3, y3, arg_c, arg_thickness);
    imlib_draw_line(arg_img, x3, y3, x0, y0, arg_c, arg_thickness);

    if (arg_s >= 1) {
        imlib_draw_circle(arg_img, x0, y0, arg_s, arg_c, arg_thickness, arg_fill);
        imlib_draw_circle(arg_img, x1, y1, arg_s, arg_c, arg_thickness, arg_fill);
        imlib_draw_circle(arg_img, x2, y2, arg_s, arg_c, arg_thickness, arg_fill);
        imlib_draw_circle(arg_img, x3, y3, arg_s, arg_c, arg_thickness, arg_fill);
    }

    return PyLong_FromLong(0);
}

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

#if IMLIB_ENABLE_FLOOD_FILL
static PyObject *
py_image_flood_fill(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    PyObject *py_mask;

    int arg_x_off = 0;
    int arg_y_off = 0;

    float arg_seed_threshold =0.05;

    float arg_floating_threshold =0.05;

    int arg_c =0xffffff;
    bool arg_invert =0;
    bool clear_background =0;

    image_t *arg_msk  = NULL;

	static char *kwlist[] = {"x","y","seed_threshold","floating_threshold","color","invert","clear_background","mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ii|ffiiiO", kwlist,
                              &arg_x_off,&arg_y_off,&arg_seed_threshold,&arg_floating_threshold,&arg_c,&arg_invert,&clear_background,&py_mask)) return NULL;

    fb_alloc_mark();
    imlib_flood_fill(arg_img, arg_x_off, arg_y_off,
                     arg_seed_threshold, arg_floating_threshold,
                     arg_c, arg_invert, clear_background, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

#endif // IMLIB_ENABLE_FLOOD_FILL

static PyObject *
py_image_mask_rectangle(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    int arg_rx;
    int arg_ry;
    int arg_rw;
    int arg_rh;
	static char *kwlist[] = {"x","y","w","h", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "[iiii]", kwlist,
                              &arg_rx,&arg_ry,&arg_rw)) return NULL;


    fb_alloc_mark();
    image_t temp;
    temp.w = arg_img->w;
    temp.h = arg_img->h;
    temp.bpp = IMAGE_BPP_BINARY;
    temp.data = fb_alloc0(image_size(&temp), FB_ALLOC_NO_HINT);

    imlib_draw_rectangle(&temp, arg_rx, arg_ry, arg_rw, arg_rh, -1, 0, true);
    imlib_zero(arg_img, &temp, true);

    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_mask_circle(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    int arg_cx;
    int arg_cy;
    int arg_cr;
	static char *kwlist[] = {"x","y","radius", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "[iii]", kwlist,
                              &arg_cx,&arg_cy,&arg_cr)) return NULL;


    fb_alloc_mark();
    image_t temp;
    temp.w = arg_img->w;
    temp.h = arg_img->h;
    temp.bpp = IMAGE_BPP_BINARY;
    temp.data = fb_alloc0(image_size(&temp), FB_ALLOC_NO_HINT);

    imlib_draw_circle(&temp, arg_cx, arg_cy, arg_cr, -1, 0, true);
    imlib_zero(arg_img, &temp, true);

    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
static PyObject *
py_image_mask_ellipse(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    int arg_cx;
    int arg_cy;
    int arg_rx;
    int arg_ry;
    int arg_r;
	static char *kwlist[] = {"x","y","radius_x","radius_y","rotation_angle_in_degrees", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "[iiiii]", kwlist,
                              &arg_cx,&arg_cy,&arg_rx,&arg_ry,&arg_r)) return NULL;


    fb_alloc_mark();
    image_t temp;
    temp.w = arg_img->w;
    temp.h = arg_img->h;
    temp.bpp = IMAGE_BPP_BINARY;
    temp.data = fb_alloc0(image_size(&temp), FB_ALLOC_NO_HINT);

    imlib_draw_ellipse(&temp, arg_cx, arg_cy, arg_rx, arg_ry, arg_r, -1, 0, true);
    imlib_zero(arg_img, &temp, true);

    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

// static PyObject *
// py_image_binary(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img = &self_image_img;
//     PyObject *thresholds;
//     PyObject *msk;

//     list_t arg_thresholds;
//     list_init(&arg_thresholds, sizeof(color_thresholds_list_lnk_data_t));


//     bool arg_invert =0;
//     bool arg_zero =0;
//     image_t *arg_msk =NULL;
//     bool arg_to_bitmap =0;
//     bool arg_copy =0;

//     static char *kwlist[] = {"thresholds","invert","zero","mask","to_bitmap", "copy",NULL};
//     if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|iiOii", kwlist,
//                               &thresholds,&arg_invert,&arg_zero,&msk,&arg_to_bitmap,&arg_copy)) return NULL;



//     if (arg_to_bitmap && (!arg_copy)) {
//         switch(arg_img->bpp) {
//             case IMAGE_BPP_GRAYSCALE: {
//                 // PY_ASSERT_TRUE_MSG((arg_img->w >= (sizeof(uint32_t)/sizeof(uint8_t))),
//                 //                    "Can't convert to bitmap in place!");
//                 return NULL;
//                 break;
//             }
//             case IMAGE_BPP_RGB565: {
//                 // PY_ASSERT_TRUE_MSG((arg_img->w >= (sizeof(uint32_t)/sizeof(uint16_t))),
//                 //                    "Can't convert to bitmap in place!");
//                 return NULL;
//                 break;
//             }
//             default: {
//                 break;
//             }
//         }
//     }

//     image_t out;
//     out.w = arg_img->w;
//     out.h = arg_img->h;
//     out.bpp = arg_to_bitmap ? IMAGE_BPP_BINARY : arg_img->bpp;
//     out.data = arg_copy ? xalloc(image_size(&out)) : arg_img->data;

//     fb_alloc_mark();
//     imlib_binary(&out, arg_img, &arg_thresholds, arg_invert, arg_zero, arg_msk);
//     fb_alloc_free_till_mark();

//     list_free(&arg_thresholds);

//     if (arg_to_bitmap && (!arg_copy)) {
//         arg_img->bpp = IMAGE_BPP_BINARY;
//         py_helper_update_framebuffer(&out);
//     }


//     return py_image_from_struct(&out);
// }
static PyObject *
py_image_invert(PyObject *self, PyObject *args)
{
    imlib_invert(&self_image_img);
    return img_obj;
}

// static PyObject *
// py_image_b_and(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =&self_image_img;
//     PyObject *msk;
//     image_t *arg_msk =NULL;

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1]))
//     {
//         imlib_b_and(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
//     }
//     else if (MP_OBJ_IS_TYPE(args[1], &py_image_type))
//     {
//         imlib_b_and(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_msk);
//     }
//     else
//     {
//         imlib_b_and(arg_img, NULL, NULL,
//                     py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                     arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
// static PyObject *
// py_image_b_nand(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =&self_image_img;
//     image_t *arg_msk =NULL;

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_b_nand(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_b_nand(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_msk);
//     } else {
//         imlib_b_nand(arg_img, NULL, NULL,
//                      py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                      arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
// static PyObject *
// py_image_b_or(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =
//         &self_image_img;
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 2, kw_args);

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_b_or(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_b_or(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_msk);
//     } else {
//         imlib_b_or(arg_img, NULL, NULL,
//                    py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                    arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }

// static PyObject *
// py_image_b_nor(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =
//         &self_image_img;
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 2, kw_args);

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_b_nor(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_b_nor(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_msk);
//     } else {
//         imlib_b_nor(arg_img, NULL, NULL,
//                     py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                     arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
// static PyObject *
// py_image_b_xor(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =
//         &self_image_img;
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 2, kw_args);

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_b_xor(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_b_xor(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_msk);
//     } else {
//         imlib_b_xor(arg_img, NULL, NULL,
//                     py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                     arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
// static PyObject *
// py_image_erode(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     int arg_ksize =
//         py_helper_arg_to_ksize(args[1]);
//     int arg_threshold =
//         py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold),
//             py_helper_ksize_to_n(arg_ksize) - 1);
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 3, kw_args);

//     fb_alloc_mark();
//     imlib_erode(&self_image_img, arg_ksize, arg_threshold, arg_msk);
//     fb_alloc_free_till_mark();
//     return args[0];
// }

static PyObject *
py_image_dilate(PyObject *self, PyObject *args, PyObject* keywds)
{
    int arg_ksize =0;
    int arg_threshold =0;
    image_t *arg_msk =NULL;
    PyObject *msk;

	static char *kwlist[] = {"size","threshold","mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iO", kwlist,
                              &arg_ksize,&arg_threshold,&msk)) return NULL;


    fb_alloc_mark();
    imlib_dilate(&self_image_img, arg_ksize, arg_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);

static PyObject *
py_image_open(PyObject *self, PyObject *args, PyObject* keywds)
{
    int arg_ksize =0;
    int arg_threshold =0;
    image_t *arg_msk =NULL;
    PyObject *msk;

	static char *kwlist[] = {"size","threshold","mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iO", kwlist,
                              &arg_ksize,&arg_threshold,&msk)) return NULL;

    fb_alloc_mark();
    imlib_open(&self_image_img, arg_ksize, arg_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_close(PyObject *self, PyObject *args, PyObject* keywds)
{
    int arg_ksize =0;
    int arg_threshold =0;
    image_t *arg_msk =NULL;
    PyObject *msk;

	static char *kwlist[] = {"size","threshold","mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iO", kwlist,
                              &arg_ksize,&arg_threshold,&msk)) return NULL;



    fb_alloc_mark();
    imlib_close(&self_image_img, arg_ksize, arg_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
static PyObject *
py_image_top_hat(PyObject *self, PyObject *args, PyObject* keywds)
{
    int arg_ksize =0;
    int arg_threshold =0;
    image_t *arg_msk =NULL;
    PyObject *msk;

	static char *kwlist[] = {"size","threshold","mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iO", kwlist,
                              &arg_ksize,&arg_threshold,&msk)) return NULL;


    fb_alloc_mark();
    imlib_top_hat(&self_image_img, arg_ksize, arg_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
static PyObject *
py_image_black_hat(PyObject *self, PyObject *args, PyObject* keywds)
{
    int arg_ksize =0;
    int arg_threshold =0;
    image_t *arg_msk =NULL;
    PyObject *msk;

	static char *kwlist[] = {"size","threshold","mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iO", kwlist,
                              &arg_ksize,&arg_threshold,&msk)) return NULL;


    fb_alloc_mark();
    imlib_black_hat(&self_image_img, arg_ksize, arg_threshold, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_gamma_corr(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;
    float arg_gamma =1.0f;
    float arg_contrast =1.0f
    float arg_brightness =0.0f
    static char *kwlist[] = {"gamma","contrast","brightness", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|fff", kwlist,
                              &arg_gamma,&arg_contrast,&arg_brightness)) return NULL;

    fb_alloc_mark();
    imlib_gamma_corr(arg_img, arg_gamma, arg_contrast, arg_brightness);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}

static PyObject *
py_image_negate(PyObject *self, PyObject *args)
{
    imlib_negate(&self_image_img);
    return PyLong_FromLong(0);
}
// static PyObject *
// py_image_replace(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =&self_image_img;
//     bool arg_hmirror =0;
//     bool arg_vflip =0;
//     bool arg_transpose =0;
//     image_t *arg_msk = NULL;
//     PyObject *msk;

//     static char *kwlist[] = {"image","hmirror","vflip", "mask",NULL};
//     if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|iiO", kwlist,
//                               &arg_hmirror,&arg_contrast,&arg_brightness)) return NULL;




//     if (arg_transpose) {
//         size_t size0 = image_size(arg_img);
//         int w = arg_img->w;
//         int h = arg_img->h;
//         arg_img->w = h;
//         arg_img->h = w;
//         size_t size1 = image_size(arg_img);
//         arg_img->w = w;
//         arg_img->h = h;
//         // PY_ASSERT_TRUE_MSG(size1 <= size0,
//         //                    "Unable to transpose the image because it would grow in size!");
//         if(size1 <= size0) return NULL;
//     }

//     fb_alloc_mark();

//     mp_obj_t arg_1 = (n_args > 1) ? args[1] : args[0];

//     if (MP_OBJ_IS_STR(arg_1)) {
//         imlib_replace(arg_img, mp_obj_str_get_str(arg_1), NULL, 0,
//                       arg_hmirror, arg_vflip, arg_transpose, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(arg_1, &py_image_type)) {
//         imlib_replace(arg_img, NULL, py_helper_arg_to_image_mutable(arg_1), 0,
//                       arg_hmirror, arg_vflip, arg_transpose, arg_msk);
//     } else {
//         imlib_replace(arg_img, NULL, NULL,
//                       py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                       arg_hmirror, arg_vflip, arg_transpose, arg_msk);
//     }

//     fb_alloc_free_till_mark();
//     py_helper_update_framebuffer(arg_img);
//     return args[0];
// }
// static PyObject *
// py_image_add(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =&self_image_img;
//     image_t *arg_msk =NULL;

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_add(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_add(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_msk);
//     } else {
//         imlib_add(arg_img, NULL, NULL,
//                   py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                   arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
// static PyObject *
// py_image_sub(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =
//         &self_image_img;
//     bool arg_reverse =
//         py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_reverse), false);
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 3, kw_args);

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_sub(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_reverse, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_sub(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_reverse, arg_msk);
//     } else {
//         imlib_sub(arg_img, NULL, NULL,
//                   py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                   arg_reverse, arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
// static PyObject *
// py_image_mul(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =
//         &self_image_img;
//     bool arg_invert =
//         py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_invert), false);
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 3, kw_args);

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_mul(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_invert, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_mul(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_invert, arg_msk);
//     } else {
//         imlib_mul(arg_img, NULL, NULL,
//                   py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                   arg_invert, arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
// static PyObject *
// py_image_div(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =
//         &self_image_img;
//     bool arg_invert =
//         py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_invert), false);
//     bool arg_mod =
//         py_helper_keyword_int(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_mod), false);
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 4, kw_args);

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_div(arg_img, mp_obj_str_get_str(args[1]), NULL, 0,
//                   arg_invert, arg_mod, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_div(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0,
//                   arg_invert, arg_mod, arg_msk);
//     } else {
//         imlib_div(arg_img, NULL, NULL,
//                   py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                   arg_invert, arg_mod, arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
// static PyObject *
// py_image_min(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =
//         &self_image_img;
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 2, kw_args);

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_min(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_min(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_msk);
//     } else {
//         imlib_min(arg_img, NULL, NULL,
//                   py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                   arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
// static PyObject *
// py_image_max(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =
//         &self_image_img;
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 2, kw_args);

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_max(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_max(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_msk);
//     } else {
//         imlib_max(arg_img, NULL, NULL,
//                   py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                   arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
// static PyObject *
// py_image_difference(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =
//         &self_image_img;
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 2, kw_args);

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_difference(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_difference(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_msk);
//     } else {
//         imlib_difference(arg_img, NULL, NULL,
//                          py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                          arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
// static PyObject *
// py_image_blend(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =
//         &self_image_img;
//     float arg_alpha =
//         py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_alpha), 128) / 256.0f;
//     PY_ASSERT_TRUE_MSG((0 <= arg_alpha) && (arg_alpha <= 1), "Error: 0 <= alpha <= 256!");
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 3, kw_args);

//     fb_alloc_mark();

//     if (MP_OBJ_IS_STR(args[1])) {
//         imlib_blend(arg_img, mp_obj_str_get_str(args[1]), NULL, 0, arg_alpha, arg_msk);
//     } else if (MP_OBJ_IS_TYPE(args[1], &py_image_type)) {
//         imlib_blend(arg_img, NULL, py_helper_arg_to_image_mutable(args[1]), 0, arg_alpha, arg_msk);
//     } else {
//         imlib_blend(arg_img, NULL, NULL,
//                     py_helper_keyword_color(arg_img, n_args, args, 1, NULL, 0),
//                     arg_alpha, arg_msk);
//     }

//     fb_alloc_free_till_mark();

//     return args[0];
// }
static PyObject *
py_image_histeq(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    bool arg_adaptive =0;
    float arg_clip_limit =-1;
    image_t *arg_msk =NULL;
    PyObject *msk;

    static char *kwlist[] = {"adaptive","clip_limit","mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|ifO", kwlist,
                              &arg_adaptive,&arg_clip_limit,&msk)) return NULL;



    fb_alloc_mark();
    if (arg_adaptive) imlib_clahe_histeq(arg_img, arg_clip_limit, arg_msk); else imlib_histeq(arg_img, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
static PyObject *
py_image_mean(PyObject *self, PyObject *args, PyObject* keywds)
    image_t *arg_img =&self_image_img;
    int arg_ksize =0;
    bool arg_threshold =0;
    int arg_offset =0;
    bool arg_invert =0;
    image_t *arg_msk =NULL;

    PyObject *msk;

    static char *kwlist[] = {"size","threshold","offset","invert","mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iiiO", kwlist,
                              &arg_ksize,&arg_threshold,&arg_offset,&arg_invert,&msk)) return NULL;



    fb_alloc_mark();
    imlib_mean_filter(arg_img, arg_ksize, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
static PyObject *
py_image_median(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    int arg_ksize =0;
    float arg_percentile =0.5f;
    // PY_ASSERT_TRUE_MSG((0 <= arg_percentile) && (arg_percentile <= 1), "Error: 0 <= percentile <= 1!");
    if((0 <= arg_percentile) && (arg_percentile <= 1)) return NULLL;
    bool arg_threshold =0;
    int arg_offset =0;
    bool arg_invert =0;
    image_t *arg_msk =NULL;
    PyObject *msk;

    static char *kwlist[] = {"size","percentile","threshold","offset","invert","mask", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ifiiiO", kwlist,
                              &arg_ksize,&arg_percentile,&arg_threshold,&arg_offset,&arg_invert,&msk)) return NULL;
    fb_alloc_mark();
    imlib_median_filter(arg_img, arg_ksize, arg_percentile, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
static PyObject *
py_image_mode(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    int arg_ksize =0;
    bool arg_threshold =0;
    int arg_offset =0;
    bool arg_invert =0;
    image_t *arg_msk =NULL;
    PyObject *msk;

    static char *kwlist[] = {"size","threshold","offset","invert","mask",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|iiiO", kwlist,
                              &arg_ksize,&arg_threshold,&arg_offset,&arg_invert,&arg_invert,&msk)) return NULL;




    fb_alloc_mark();
    imlib_mode_filter(arg_img, arg_ksize, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
static PyObject *
py_image_midpoint(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    int arg_ksize =0;
    float arg_bias =0.5f;
    // PY_ASSERT_TRUE_MSG((0 <= arg_bias) && (arg_bias <= 1), "Error: 0 <= bias <= 1!");
    
    bool arg_threshold =0;
    int arg_offset =0;
    bool arg_invert =0;
    image_t *arg_msk =NULL;

    PyObject *msk;

    static char *kwlist[] = {"size","bias","threshold","offset","invert","mask",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|fiiiO", kwlist,
                              &arg_ksize,&arg_bias,&arg_threshold,&arg_offset,&arg_invert,&msk)) return NULL;


    // if((0 <= arg_bias) && (arg_bias <= 1)) return NULL;
    fb_alloc_mark();
    imlib_midpoint_filter(arg_img, arg_ksize, arg_bias, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
// static PyObject *
// py_image_morph(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =&self_image_img;
//     int arg_ksize =0;

//     int n = 0;


//     fb_alloc_mark();

//     int *arg_krn = fb_alloc(n * sizeof(int), FB_ALLOC_NO_HINT);
//     int arg_m = 0;

//     for (int i = 0; i < n; i++) {
//         arg_krn[i] = mp_obj_get_int(krn[i]);
//         arg_m += arg_krn[i];
//     }

//     if (arg_m == 0) {
//         arg_m = 1;
//     }

//     float arg_mul =
//         py_helper_keyword_float(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_mul), 1.0f / arg_m);
//     float arg_add =
//         py_helper_keyword_float(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_add), 0.0f);
//     bool arg_threshold =
//         py_helper_keyword_int(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold), false);
//     int arg_offset =
//         py_helper_keyword_int(n_args, args, 6, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_offset), 0);
//     bool arg_invert =
//         py_helper_keyword_int(n_args, args, 7, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_invert), false);
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 8, kw_args);

//     imlib_morph(arg_img, arg_ksize, arg_krn, arg_mul, arg_add, arg_threshold, arg_offset, arg_invert, arg_msk);
//     fb_alloc_free_till_mark();
//     return args[0];
// }
// static PyObject *
// py_image_gaussian(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =&self_image_img;
//     int arg_ksize =0;

//     int k_2 = arg_ksize * 2;
//     int n = k_2 + 1;

//     fb_alloc_mark();

//     int *pascal = fb_alloc(n * sizeof(int), FB_ALLOC_NO_HINT);
//     pascal[0] = 1;

//     for (int i = 0; i < k_2; i++) { // Compute a row of pascal's triangle.
//         pascal[i + 1] = (pascal[i] * (k_2 - i)) / (i + 1);
//     }

//     int *arg_krn = fb_alloc(n * n * sizeof(int), FB_ALLOC_NO_HINT);
//     int arg_m = 0;

//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             int temp = pascal[i] * pascal[j];
//             arg_krn[(i * n) + j] = temp;
//             arg_m += temp;
//         }
//     }
//     bool unsharp = 0;


//     if (unsharp) {
//         arg_krn[((n/2)*n)+(n/2)] -= arg_m * 2;
//         arg_m = -arg_m;
//     }

//     float arg_mul =1.0f / arg_m;

//     float arg_add =0.0f;
//     bool arg_threshold =0;
//     int arg_offset =0;
//     bool arg_invert =0;
//     image_t *arg_msk =NULL;




//     imlib_morph(arg_img, arg_ksize, arg_krn, arg_mul, arg_add, arg_threshold, arg_offset, arg_invert, arg_msk);
//     fb_alloc_free_till_mark();
//     return PyLong_FromLong(0);
// }
// static PyObject *
// py_image_laplacian(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img =&self_image_img;
//     int arg_ksize =0;

//     int k_2 = arg_ksize * 2;
//     int n = k_2 + 1;

//     fb_alloc_mark();

//     int *pascal = fb_alloc(n * sizeof(int), FB_ALLOC_NO_HINT);
//     pascal[0] = 1;

//     for (int i = 0; i < k_2; i++) { // Compute a row of pascal's triangle.
//         pascal[i + 1] = (pascal[i] * (k_2 - i)) / (i + 1);
//     }

//     int *arg_krn = fb_alloc(n * n * sizeof(int), FB_ALLOC_NO_HINT);
//     int arg_m = 0;

//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             int temp = pascal[i] * pascal[j];
//             arg_krn[(i * n) + j] = -temp;
//             arg_m += temp;
//         }
//     }

//     arg_krn[((n/2)*n)+(n/2)] += arg_m;
//     arg_m = arg_krn[((n/2)*n)+(n/2)];

//     if (py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_sharpen), false)) {
//         arg_krn[((n/2)*n)+(n/2)] += arg_m;
//     }

//     float arg_mul =
//         py_helper_keyword_float(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_mul), 1.0f / arg_m);
//     float arg_add =
//         py_helper_keyword_float(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_add), 0.0f);
//     bool arg_threshold =
//         py_helper_keyword_int(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold), false);
//     int arg_offset =
//         py_helper_keyword_int(n_args, args, 6, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_offset), 0);
//     bool arg_invert =
//         py_helper_keyword_int(n_args, args, 7, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_invert), false);
//     image_t *arg_msk =
//         py_helper_keyword_to_image_mutable_mask(n_args, args, 8, kw_args);

//     imlib_morph(arg_img, arg_ksize, arg_krn, arg_mul, arg_add, arg_threshold, arg_offset, arg_invert, arg_msk);
//     fb_alloc_free_till_mark();
//     return args[0];
// }
static PyObject * py_image_bilateral(PyObject *self, PyObject *args, PyObject* keywds)
    image_t *arg_img =&self_image_img;
    int arg_ksize =0;
    float arg_color_sigma =0.1;
    float arg_space_sigma =1;
    bool arg_threshold =0;
    int arg_offset =0;
    bool arg_invert =0;
    image_t *arg_msk =NULL;

    PyObject *msk;

    static char *kwlist[] = {"size","color_sigma","space_sigma","threshold","offset","invert","mask",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|ffiiiO", kwlist,
                              &arg_ksize,&arg_color_sigma,&arg_space_sigma,&arg_threshold,&arg_offset,&arg_invert,&msk)) return NULL;



    fb_alloc_mark();
    imlib_bilateral_filter(arg_img, arg_ksize, arg_color_sigma, arg_space_sigma, arg_threshold, arg_offset, arg_invert, arg_msk);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
static PyObject *
py_image_cartoon(PyObject *self, PyObject *args, PyObject* keywds)
    image_t *arg_img =&self_image_img;
    float arg_seed_threshold =0.05;
    // PY_ASSERT_TRUE_MSG((0.0f <= arg_seed_threshold) && (arg_seed_threshold <= 1.0f),
    //                    "Error: 0.0 <= seed_threshold <= 1.0!");
    float arg_floating_threshold =0.05;
    // PY_ASSERT_TRUE_MSG((0.0f <= arg_floating_threshold) && (arg_floating_threshold <= 1.0f),
    //                    "Error: 0.0 <= floating_threshold <= 1.0!");
    image_t *arg_msk =NULL;

    PyObject *msk;

    static char *kwlist[] = {"seed_threshold","floating_threshold","mask",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|ffO", kwlist,
                              &arg_seed_threshold,&arg_floating_threshold,&msk)) return NULL;



    fb_alloc_mark();
    imlib_cartoon_filter(arg_img, arg_seed_threshold, arg_floating_threshold, arg_msk);
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
// static PyObject * py_image_lens_corr(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
// {
//     image_t *arg_img =
//         &self_image_img;
//     PY_ASSERT_FALSE_MSG(arg_img->w % 2, "Width must be even!");
//     PY_ASSERT_FALSE_MSG(arg_img->h % 2, "Height must be even!");
//     float arg_strength =
//         py_helper_keyword_float(n_args, args, 1, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_strength), 1.8f);
//     PY_ASSERT_TRUE_MSG(arg_strength > 0.0f, "Strength must be > 0!");
//     float arg_zoom =
//         py_helper_keyword_float(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_zoom), 1.0f);
//     PY_ASSERT_TRUE_MSG(arg_zoom > 0.0f, "Zoom must be > 0!");

//     float arg_x_corr =
//         py_helper_keyword_float(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_x_corr), 0.0f);
//     float arg_y_corr =
//         py_helper_keyword_float(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_y_corr), 0.0f);

//     fb_alloc_mark();
//     imlib_lens_corr(arg_img, arg_strength, arg_zoom, arg_x_corr, arg_y_corr);
//     fb_alloc_free_till_mark();
//     return args[0];
// }
static PyObject *
py_image_rotation_corr(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
    float arg_x_rotation =0.0f;
    float arg_y_rotation =0.0f;
    float arg_z_rotation =0.0f;
    float arg_x_translation =0.0f;
    float arg_y_translation =0.0f;
    float arg_zoom =1.0f;
    // PY_ASSERT_TRUE_MSG(arg_zoom > 0.0f, "Zoom must be > 0!");
    float arg_fov =60.0f;
    // PY_ASSERT_TRUE_MSG((0.0f < arg_fov) && (arg_fov < 180.0f), "FOV must be > 0 and < 180!");
    float *arg_corners = 0;



    static char *kwlist[] = {"x_rotation","y_rotation","z_rotation","x_translation","y_translation","zoom",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|ffffff", kwlist,
                              &arg_x_rotation,&arg_y_rotation,&arg_z_rotation,&arg_x_translation,&arg_y_translation,&arg_zoom)) return NULL;



    fb_alloc_mark();
    imlib_rotation_corr(arg_img,
                        arg_x_rotation, arg_y_rotation, arg_z_rotation,
                        arg_x_translation, arg_y_translation,
                        arg_zoom, arg_fov, arg_corners);
    fb_alloc_free_till_mark();
    return PyLong_FromLong(0);
}
// static PyObject *
// py_image_get_similarity(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img = &self_image_img;
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
//     PyObject *py_roi;
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





/*
该算法没有使用感兴趣区，等参数
*/
static PyObject *
py_image_get_statistics(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img =&self_image_img;
	//mp init
    char *heap = malloc(heap_size);
    gc_init(heap, heap + heap_size);
    mp_init();
    mp_obj_t tmp = mp_obj_new_int(1);
    int res = mp_obj_get_int(tmp);


	//python args
    struct RECE_STA
	{
		PyObject *thresholds;
        PyObject *invert;
		PyObject *roi;

        PyObject *bins;
        PyObject *l_bins;
        PyObject *a_bins;
        PyObject *b_bins;
		
	} rece_sta;
	PyObject *thr;
	PyObject *thr2;
	PyObject *thr3;
	//imlib args
	list_t thresholds;
	bool invert;
	rectangle_t roi;
	color_thresholds_list_lnk_data_t thrm;
	histogram_t hist;
	image_t *other = NULL;
	

	//imlib args init
	hist.ABinCount = 101;
    hist.BBinCount = 256;
    hist.LBinCount = 256;
	list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
	

    static char *kwlist[] = {"thresholds", "invert", "roi", "bins","l_bins","a_bins","b_bins", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|OOOOOOO", kwlist,
                              &rece_sta.thresholds,&rece_sta.invert,&rece_sta.roi,&rece_sta.bins,&rece_sta.l_bins,&rece_sta.a_bins,&rece_sta.b_bins)) return NULL;

	
	//py args conversion imlib
	
	fb_alloc_mark();						//未知函数,暂时先调用


	if(rece_sta.invert == Py_True) invert = 1;else invert = 0;

	int thr_len,thrs;
	thr_len = PyList_Size(rece_sta.thresholds);
	if(thr_len == 0) return PyList_New(0);		//检查传递阈值
	for(int i=0;i<thr_len;i++)
	{
		thr = PyList_GetItem(rece_sta.thresholds,i);
		if(!PyTuple_Check(thr)) return NULL;
		thrs = PyTuple_Size(thr);



		if(thrs == 6)
		{
		thr2 = PyTuple_GetItem(thr,0);thrm.LMin = PyLong_AsLong(thr2);
		thr2 = PyTuple_GetItem(thr,1);thrm.LMax = PyLong_AsLong(thr2);
		thr2 = PyTuple_GetItem(thr,2);thrm.AMin = PyLong_AsLong(thr2);
		thr2 = PyTuple_GetItem(thr,3);thrm.AMax = PyLong_AsLong(thr2);
		thr2 = PyTuple_GetItem(thr,4);thrm.BMin = PyLong_AsLong(thr2);
		thr2 = PyTuple_GetItem(thr,5);thrm.BMax = PyLong_AsLong(thr2);
        list_push_back(&thresholds, &thrm);
		}
		else if(thrs == 2)
		{

		}
		else return NULL;
	}



	hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
	hist.ABins = fb_alloc(hist.ABinCount * sizeof(float), FB_ALLOC_NO_HINT);
	hist.BBins = fb_alloc(hist.BBinCount * sizeof(float), FB_ALLOC_NO_HINT);

	imlib_get_histogram(&hist, &arg_img, &roi, &thresholds, invert, other);
	// imlib_get_histogram(&hist, &arg_img, &roi, &thresholds, invert);



	statistics_t stats;
    imlib_get_statistics(&stats, arg_img.bpp, &hist);



    if (hist.BBinCount) fb_free();
    if (hist.ABinCount) fb_free();
    if (hist.LBinCount) fb_free();
    fb_alloc_free_till_mark();

    PyObject *o = PyDict_New();
    PyDict_SetItem(o,Py_BuildValue("s", "LMean"),Py_BuildValue("i", stats.LMean));
    PyDict_SetItem(o,Py_BuildValue("s", "LMedian"),Py_BuildValue("i", stats.LMedian));
    PyDict_SetItem(o,Py_BuildValue("s", "LMode"),Py_BuildValue("i", stats.LMode));
    PyDict_SetItem(o,Py_BuildValue("s", "LSTDev"),Py_BuildValue("i", stats.LSTDev));
    PyDict_SetItem(o,Py_BuildValue("s", "LMin"),Py_BuildValue("i", stats.LMin));
    PyDict_SetItem(o,Py_BuildValue("s", "LMax"),Py_BuildValue("i", stats.LMax));
    PyDict_SetItem(o,Py_BuildValue("s", "LLQ"),Py_BuildValue("i", stats.LLQ));
    PyDict_SetItem(o,Py_BuildValue("s", "LUQ"),Py_BuildValue("i", stats.LUQ));
    PyDict_SetItem(o,Py_BuildValue("s", "ASTDev"),Py_BuildValue("i", stats.ASTDev));
    PyDict_SetItem(o,Py_BuildValue("s", "AMin"),Py_BuildValue("i", stats.AMin));
    PyDict_SetItem(o,Py_BuildValue("s", "AMax"),Py_BuildValue("i", stats.AMax));
    PyDict_SetItem(o,Py_BuildValue("s", "ALQ"),Py_BuildValue("i", stats.ALQ));
    PyDict_SetItem(o,Py_BuildValue("s", "BMean"),Py_BuildValue("i", stats.BMean));
    PyDict_SetItem(o,Py_BuildValue("s", "BMedian"),Py_BuildValue("i", stats.BMedian));
    PyDict_SetItem(o,Py_BuildValue("s", "BMode"),Py_BuildValue("i", stats.BMode));
    PyDict_SetItem(o,Py_BuildValue("s", "BSTDev"),Py_BuildValue("i", stats.BSTDev));
    PyDict_SetItem(o,Py_BuildValue("s", "BMin"),Py_BuildValue("i", stats.BMin));
    PyDict_SetItem(o,Py_BuildValue("s", "BMax"),Py_BuildValue("i", stats.BMax));
    PyDict_SetItem(o,Py_BuildValue("s", "BLQ"),Py_BuildValue("i", stats.BLQ));
    PyDict_SetItem(o,Py_BuildValue("s", "BUQ"),Py_BuildValue("i", stats.BUQ));



	//mp free
	 free(heap);
    return o;

}
















// static PyObject * py_image_get_statistics(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
// {
//     image_t *arg_img = &self_image_img;

//     list_t thresholds;
//     list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
//     py_helper_keyword_thresholds(n_args, args, 1, kw_args, &thresholds);
//     bool invert = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_invert), false);
//     image_t *other = py_helper_keyword_to_image_mutable(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_difference), NULL);

//     rectangle_t roi;
//     py_helper_keyword_rectangle_roi(arg_img, n_args, args, 3, kw_args, &roi);

//     histogram_t hist;
//     switch(arg_img->bpp) {
//         case IMAGE_BPP_BINARY: {
//             int bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
//                                              (COLOR_BINARY_MAX-COLOR_BINARY_MIN+1));
//             PY_ASSERT_TRUE_MSG(bins >= 2, "bins must be >= 2");
//             hist.LBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), bins);
//             PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
//             hist.ABinCount = 0;
//             hist.BBinCount = 0;
//             fb_alloc_mark();
//             hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
//             hist.ABins = NULL;
//             hist.BBins = NULL;
//             imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert, other);
//             list_free(&thresholds);
//             break;
//         }
//         case IMAGE_BPP_GRAYSCALE: {
//             int bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
//                                              (COLOR_GRAYSCALE_MAX-COLOR_GRAYSCALE_MIN+1));
//             PY_ASSERT_TRUE_MSG(bins >= 2, "bins must be >= 2");
//             hist.LBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), bins);
//             PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
//             hist.ABinCount = 0;
//             hist.BBinCount = 0;
//             fb_alloc_mark();
//             hist.LBins = fb_alloc(hist.LBinCount * sizeof(float), FB_ALLOC_NO_HINT);
//             hist.ABins = NULL;
//             hist.BBins = NULL;
//             imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert, other);
//             list_free(&thresholds);
//             break;
//         }
//         case IMAGE_BPP_RGB565: {
//             int l_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
//                                                (COLOR_L_MAX-COLOR_L_MIN+1));
//             PY_ASSERT_TRUE_MSG(l_bins >= 2, "bins must be >= 2");
//             hist.LBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_l_bins), l_bins);
//             PY_ASSERT_TRUE_MSG(hist.LBinCount >= 2, "l_bins must be >= 2");
//             int a_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
//                                                (COLOR_A_MAX-COLOR_A_MIN+1));
//             PY_ASSERT_TRUE_MSG(a_bins >= 2, "bins must be >= 2");
//             hist.ABinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_a_bins), a_bins);
//             PY_ASSERT_TRUE_MSG(hist.ABinCount >= 2, "a_bins must be >= 2");
//             int b_bins = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_bins),
//                                                (COLOR_B_MAX-COLOR_B_MIN+1));
//             PY_ASSERT_TRUE_MSG(b_bins >= 2, "bins must be >= 2");
//             hist.BBinCount = py_helper_keyword_int(n_args, args, n_args, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_b_bins), b_bins);
//             PY_ASSERT_TRUE_MSG(hist.BBinCount >= 2, "b_bins must be >= 2");
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

//     statistics_t stats;
//     imlib_get_statistics(&stats, arg_img->bpp, &hist);
//     fb_alloc_free_till_mark();

//     py_statistics_obj_t *o = m_new_obj(py_statistics_obj_t);
//     o->base.type = &py_statistics_type;
//     o->bpp = arg_img->bpp;

//     o->LMean = mp_obj_new_int(stats.LMean);
//     o->LMedian = mp_obj_new_int(stats.LMedian);
//     o->LMode= mp_obj_new_int(stats.LMode);
//     o->LSTDev = mp_obj_new_int(stats.LSTDev);
//     o->LMin = mp_obj_new_int(stats.LMin);
//     o->LMax = mp_obj_new_int(stats.LMax);
//     o->LLQ = mp_obj_new_int(stats.LLQ);
//     o->LUQ = mp_obj_new_int(stats.LUQ);
//     o->AMean = mp_obj_new_int(stats.AMean);
//     o->AMedian = mp_obj_new_int(stats.AMedian);
//     o->AMode= mp_obj_new_int(stats.AMode);
//     o->ASTDev = mp_obj_new_int(stats.ASTDev);
//     o->AMin = mp_obj_new_int(stats.AMin);
//     o->AMax = mp_obj_new_int(stats.AMax);
//     o->ALQ = mp_obj_new_int(stats.ALQ);
//     o->AUQ = mp_obj_new_int(stats.AUQ);
//     o->BMean = mp_obj_new_int(stats.BMean);
//     o->BMedian = mp_obj_new_int(stats.BMedian);
//     o->BMode= mp_obj_new_int(stats.BMode);
//     o->BSTDev = mp_obj_new_int(stats.BSTDev);
//     o->BMin = mp_obj_new_int(stats.BMin);
//     o->BMax = mp_obj_new_int(stats.BMax);
//     o->BLQ = mp_obj_new_int(stats.BLQ);
//     o->BUQ = mp_obj_new_int(stats.BUQ);

//     return o;
// }



static PyObject *
py_image_get_regression(PyObject *self, PyObject *args, PyObject* keywds)
{
    image_t *arg_img = &self_image_img;

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



static PyObject *
py_image_find_blobs(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img =&self_image_img;
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
	// list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t)); //列表初始化
	// color_thresholds_list_lnk_data_t thrm;
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
    if(thresholds_tan(rece_sta.thresholds,&thresholds) == 0)
    {
        return PyList_New(0);
    }

    thresholds_tan(rece_sta.roi,&roi,arg_img.w,arg_img.h);


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
// static PyObject *
// py_image_find_blobs(PyObject *self, PyObject *args, PyObject* keywds)
// {
//     image_t *arg_img = &self_image_img;

//     list_t thresholds;
//     list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
//     py_helper_arg_to_thresholds(args[1], &thresholds);
//     if (!list_size(&thresholds)) return mp_obj_new_list(0, NULL);
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
//             area_threshold, pixels_threshold, merge, margin,
//             py_image_find_blobs_threshold_cb, threshold_cb, py_image_find_blobs_merge_cb, merge_cb, x_hist_bins_max, y_hist_bins_max);
//     fb_alloc_free_till_mark();
//     list_free(&thresholds);

//     mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
//     for (size_t i = 0; list_size(&out); i++) {
//         find_blobs_list_lnk_data_t lnk_data;
//         list_pop_front(&out, &lnk_data);

//         py_blob_obj_t *o = m_new_obj(py_blob_obj_t);
//         o->base.type = &py_blob_type;
//         o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
//             {mp_obj_new_tuple(2, (mp_obj_t (uint n_args, const mp_obj_t *args, mp_map_t *kw_args)[]) {mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*0)/4].x),
//                                                 mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*0)/4].y)}),
//              mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*1)/4].x),
//                                                 mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*1)/4].y)}),
//              mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*2)/4].x),
//                                                 mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*2)/4].y)}),
//              mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*3)/4].x),
//                                                 mp_obj_new_int(lnk_data.corners[(FIND_BLOBS_CORNERS_RESOLUTION*3)/4].y)})});
//         point_t min_corners[4];
//         point_min_area_rectangle(lnk_data.corners, min_corners, FIND_BLOBS_CORNERS_RESOLUTION);
//         o->min_corners = mp_obj_new_tuple(4, (mp_obj_t [])
//             {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(min_corners[0].x), mp_obj_new_int(min_corners[0].y)}),
//              mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(min_corners[1].x), mp_obj_new_int(min_corners[1].y)}),
//              mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(min_corners[2].x), mp_obj_new_int(min_corners[2].y)}),
//              mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(min_corners[3].x), mp_obj_new_int(min_corners[3].y)})});
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

//         for (int i = 0; i < lnk_data.x_hist_bins_count; i++) {
//             ((mp_obj_list_t *) o->x_hist_bins)->items[i] = mp_obj_new_int(lnk_data.x_hist_bins[i]);
//         }

//         for (int i = 0; i < lnk_data.y_hist_bins_count; i++) {
//             ((mp_obj_list_t *) o->y_hist_bins)->items[i] = mp_obj_new_int(lnk_data.y_hist_bins[i]);
//         }

//         objects_list->items[i] = o;
//         if (lnk_data.x_hist_bins) xfree(lnk_data.x_hist_bins);
//         if (lnk_data.y_hist_bins) xfree(lnk_data.y_hist_bins);
//     }

//     return objects_list;
// }



















































static PyObject *
py_image_find_lines(PyObject *self, PyObject *args, PyObject *keywds)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;
    PyObject *py_roi;
    unsigned int x_stride = 2;
    // PY_ASSERT_TRUE_MSG(x_stride > 0, "x_stride must not be zero.");
    unsigned int y_stride = 1;
    // PY_ASSERT_TRUE_MSG(y_stride > 0, "y_stride must not be zero.");
    uint32_t threshold = 1000;
    unsigned int theta_margin = 25;
    unsigned int rho_margin = 25;


	static char *kwlist[] = {"roi", "x_stride", "y_stride", "threshold", "theta_margin", "rho_margin", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, keywds, "|Oiiiii", kwlist,
									 &py_roi, &x_stride, &y_stride, &threshold, &theta_margin, &rho_margin))



    list_t out;
    fb_alloc_mark();
    imlib_find_lines(&out, arg_img, &roi, x_stride, y_stride, threshold, theta_margin, rho_margin);
    fb_alloc_free_till_mark();

    mp_obj_list_t *objects_list = mp_obj_new_list(list_size(&out), NULL);
    PyObject *objects_list = PyList_New(0);
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




static PyObject * py_image_find_line_segments(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = &self_image_img;

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
static PyObject * py_image_find_circles(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = &self_image_img;

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
static PyObject * py_image_find_rects(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = &self_image_img;

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

static PyObject * py_image_find_qrcodes(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = &self_image_img;

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
static PyObject * py_image_find_apriltags(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
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

static PyObject * py_image_find_datamatrices(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = &self_image_img;

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

static PyObject * py_image_find_barcodes(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = &self_image_img;

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
static PyObject * py_image_find_displacement(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = &self_image_img;
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
static PyObject * py_image_find_template(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_grayscale(args[0]);
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
static PyObject * py_image_find_features(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = &self_image_img;
    cascade_t *cascade = py_cascade_cobj(args[1]);
    cascade->threshold = py_helper_keyword_float(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold), 0.5f);
    cascade->scale_factor = py_helper_keyword_float(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_scale_factor), 1.5f);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 4, kw_args, &roi);

    // Make sure ROI is bigger than feature size
    PY_ASSERT_TRUE_MSG((roi.w > cascade->window.w && roi.h > cascade->window.h),
            "Region of interest is smaller than detector window!");

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
static PyObject * py_image_find_eye(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_grayscale(args[0]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    point_t iris;
    imlib_find_iris(arg_img, &iris, &roi);

    mp_obj_t eye_obj[2] = {
        mp_obj_new_int(iris.x),
        mp_obj_new_int(iris.y),
    };

    return mp_obj_new_tuple(2, eye_obj);
}
static PyObject * py_image_find_lbp(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_grayscale(args[0]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    py_lbp_obj_t *lbp_obj = m_new_obj(py_lbp_obj_t);
    lbp_obj->base.type = &py_lbp_type;
    lbp_obj->hist = imlib_lbp_desc(arg_img, &roi);
    return lbp_obj;
}
static PyObject * py_image_find_keypoints(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = &self_image_img;

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    int threshold =
        py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold), 20);
    bool normalized =
        py_helper_keyword_int(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_normalized), false);
    float scale_factor =
        py_helper_keyword_float(n_args, args, 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_scale_factor), 1.5f);
    int max_keypoints =
        py_helper_keyword_int(n_args, args, 5, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_max_keypoints), 100);
    corner_detector_t corner_detector =
        py_helper_keyword_int(n_args, args, 6, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_corner_detector), CORNER_AGAST);

    #ifndef IMLIB_ENABLE_FAST
    // Force AGAST when FAST is disabled.
    corner_detector = CORNER_AGAST;
    #endif

    // Find keypoints
    fb_alloc_mark();
    array_t *kpts = orb_find_keypoints(arg_img, normalized, threshold, scale_factor, max_keypoints, corner_detector, &roi);
    fb_alloc_free_till_mark();

    if (array_length(kpts)) {
        py_kp_obj_t *kp_obj = m_new_obj(py_kp_obj_t);
        kp_obj->base.type = &py_kp_type;
        kp_obj->kpts = kpts;
        kp_obj->threshold = threshold;
        kp_obj->normalized = normalized;
        return kp_obj;
    }
    return mp_const_none;
}
static PyObject * py_image_find_edges(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_grayscale(args[0]);
    edge_detector_t edge_type = mp_obj_get_int(args[1]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 2, kw_args, &roi);

    int thresh[2] = {100, 200};
    mp_obj_t thresh_obj = py_helper_keyword_object(n_args, args, 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_threshold), NULL);

    if (thresh_obj) {
        mp_obj_t *thresh_array;
        mp_obj_get_array_fixed_n(thresh_obj, 2, &thresh_array);
        thresh[0] = mp_obj_get_int(thresh_array[0]);
        thresh[1] = mp_obj_get_int(thresh_array[1]);
    }

    switch (edge_type) {
        case EDGE_SIMPLE: {
            fb_alloc_mark();
            imlib_edge_simple(arg_img, &roi, thresh[0], thresh[1]);
            fb_alloc_free_till_mark();
            break;
        }
        case EDGE_CANNY: {
            fb_alloc_mark();
            imlib_edge_canny(arg_img, &roi, thresh[0], thresh[1]);
            fb_alloc_free_till_mark();
            break;
        }

    }

    return args[0];
}
static PyObject * py_image_find_hog(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_helper_arg_to_image_grayscale(args[0]);

    rectangle_t roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, 1, kw_args, &roi);

    int size = py_helper_keyword_int(n_args, args, 2, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_size), 8);

    fb_alloc_mark();
    imlib_find_hog(arg_img, &roi, size);
    fb_alloc_free_till_mark();

    return args[0];
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
