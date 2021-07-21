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
