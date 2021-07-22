#include "tanstation.h"

#include "imdefs.h"
#include "imlib.h"



PyObject *back_img(image_t *img)
{
    PyObject *th_tup;
    th_tup = PyTuple_New(4);
    PyTuple_SetItem(th_tup,0,PyBytes_FromStringAndSize(img->data,img->w * img->h * 3));
    PyTuple_SetItem(th_tup,0,PyLong_FromLong(img->w));
    PyTuple_SetItem(th_tup,0,PyLong_FromLong(img->h));
    PyTuple_SetItem(th_tup,0,PyLong_FromLong(img->bpp));
    return th_tup;
}

int thresholds_tan(PyObject *thr,list_t *pt)
{
    PyObject *thr0;
    int thr_len;
    int thrs;
    color_thresholds_list_lnk_data_t thrm;
    list_init(pt, sizeof(color_thresholds_list_lnk_data_t)); //列表初始化
    thr_len = PyList_Size(thr);
    if (thr_len == 0)
    {
        return 0;
    }
    for (int i = 0; i < thr_len; i++)
    {
        thr0 = PyList_GetItem(thr, i);
        thrs = PyTuple_Size(thr0);
        switch (thrs)
        {
        case 6:
            thrm.LMin = PyLong_AsLong(PyTuple_GetItem(thr0, 0));
            thrm.LMax = PyLong_AsLong(PyTuple_GetItem(thr0, 1));
            thrm.AMin = PyLong_AsLong(PyTuple_GetItem(thr0, 2));
            thrm.AMax = PyLong_AsLong(PyTuple_GetItem(thr0, 3));
            thrm.BMin = PyLong_AsLong(PyTuple_GetItem(thr0, 4));
            thrm.BMax = PyLong_AsLong(PyTuple_GetItem(thr0, 5));
            list_push_back(pt, &thrm);
            break;
        case 2:

            break;
        default:
            break;
        }

    }
    return thr_len;
}

int roi_tan(PyObject *roi,rectangle_t *pt,int w,int h)
{
    int thr_len;
	thr_len = PyList_Size(roi);
	if (thr_len == 0)
	{
		pt->x = 0;
		pt->y = 0;
		pt->w = w;
		pt->h = h;

	}
	else if (thr_len == 4)
	{
		pt->x = PyLong_AsLong(PyList_GetItem(roi, 0));
		pt->y = PyLong_AsLong(PyList_GetItem(roi, 1));
		pt->w = PyLong_AsLong(PyList_GetItem(roi, 2));
		pt->h = PyLong_AsLong(PyList_GetItem(roi, 3));
	}
    return 0;
}