#include "tanstation.h"

// #include "imdefs.h"
// #include "imlib.h"

// #define debug_line printf("[%s %s] %s:%d: %s\n", __DATE__, __TIME__, __FILE__, __LINE__, __func__)

#define debug_line
PyObject *back_img(image_t *img)
{
    PyObject *th_tup = Py_None;
    uint8_t *_888_data;
    if (img->data == NULL)
    {
        return Py_None;
    }
    th_tup = PyTuple_New(4);
    switch (img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {

        break;
    }
    case IMAGE_BPP_GRAYSCALE:
    {

        break;
    }
    case IMAGE_BPP_RGB565:
    {
        _888_data = (uint8_t *)malloc(img->w * img->h * 3);
        uint16_t *r16_pixel;
        r16_pixel = (uint16_t *)img->data;

        for (int i = 0; i < img->w * img->h * 3; i += 3)
        {
            _888_data[i] = COLOR_RGB565_TO_R8(*r16_pixel);

            _888_data[i + 1] = COLOR_RGB565_TO_G8(*r16_pixel);

            _888_data[i + 2] = COLOR_RGB565_TO_B8(*r16_pixel);

            r16_pixel++;
        }
        PyTuple_SetItem(th_tup, 0, PyBytes_FromStringAndSize(_888_data, img->w * img->h * 3));
        free(_888_data);
    }
    case IMAGE_BPP_BAYER:
    {

        break;
    }
    default:
    {
        return Py_None;
        break;
    }
    }

    PyTuple_SetItem(th_tup, 1, PyLong_FromLong(img->w));
    PyTuple_SetItem(th_tup, 2, PyLong_FromLong(img->h));
    PyTuple_SetItem(th_tup, 3, PyLong_FromLong(img->bpp));
    return th_tup;
}

int thresholds_tan(PyObject *thr, list_t *pt)
{
    PyObject *thr0 = Py_None;
    int thr_len;
    int thrs;
    color_thresholds_list_lnk_data_t thrm;
    list_init(pt, sizeof(color_thresholds_list_lnk_data_t)); //列表初始化
    if (PyList_Check(thr))
    {
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
    }
    return thr_len;
}

int r24to_imgr16(PyObject *img_or_data, PyObject *w, PyObject *h, PyObject *bpp, image_t *pt)
{
    image_t *arg_img = pt;
    uint8_t *r24_pixel;
    uint16_t *r16_pixel;
    // arg_img = malloc(sizeof(image_t));
    if (arg_img->data != NULL)
    {
        free(arg_img->data);
        arg_img->data = NULL;
    }
    debug_line;
    if (PyTuple_Check(img_or_data))
    {
        debug_line;
        r24_pixel = PyBytes_AsString(PyTuple_GetItem(img_or_data, 0));
        arg_img->w = PyLong_AsLong(PyTuple_GetItem(img_or_data, 1));
        arg_img->h = PyLong_AsLong(PyTuple_GetItem(img_or_data, 2));
        arg_img->bpp = PyLong_AsLong(PyTuple_GetItem(img_or_data, 3));
    }
    else
    {
        debug_line;
        r24_pixel = PyBytes_AsString(img_or_data);
        arg_img->w = PyLong_AsLong(w);
        arg_img->h = PyLong_AsLong(h);
        arg_img->bpp = PyLong_AsLong(bpp);
    }

    debug_line;
    switch (arg_img->bpp)
    {
    case IMAGE_BPP_BINARY:
    {

        break;
    }
    case IMAGE_BPP_GRAYSCALE:
    {

        break;
    }
    case IMAGE_BPP_RGB565:
    {
        if (arg_img->data == NULL)
        {
            arg_img->data = (uint8_t *)malloc(arg_img->w * arg_img->h * 2);
        }
        r16_pixel = (uint16_t *)arg_img->data;
        for (int i = 0; i < arg_img->w * arg_img->h * 3; i += 3)
        {
            *r16_pixel = COLOR_R8_G8_B8_TO_RGB565(r24_pixel[i], r24_pixel[i + 1], r24_pixel[i + 2]);
            r16_pixel++;
        }
        break;
    }
    case IMAGE_BPP_BAYER:
    {

        break;
    }
    default:
    {
        return -1;
        break;
    }
    }
    return 1;
}

int roi_tan(PyObject *roi, rectangle_t *pt, int w, int h)
{
    int thr_len;
    debug_line;
    if (PyTuple_Check(roi))
    {
        debug_line;

        thr_len = PyTuple_Size(roi);
        if (thr_len == 4)
        {
            pt->x = PyLong_AsLong(PyTuple_GetItem(roi, 0));
            pt->y = PyLong_AsLong(PyTuple_GetItem(roi, 1));
            pt->w = PyLong_AsLong(PyTuple_GetItem(roi, 2));
            pt->h = PyLong_AsLong(PyTuple_GetItem(roi, 3));
        }
        else
        {
            pt->x = 0;
            pt->y = 0;
            pt->w = w;
            pt->h = h;
        }
    }
    else
    {
        debug_line;
        pt->x = 0;
        pt->y = 0;
        pt->w = w;
        pt->h = h;
    }
    return 0;
}