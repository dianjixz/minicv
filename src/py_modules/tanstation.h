#ifndef __TANSTATION_H__
#define __TANSTATION_H__

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "imlib.h"

PyObject *back_img(image_t *img);
int thresholds_tan(PyObject *thr,list_t *pt);


int roi_tan(PyObject *roi,rectangle_t *pt,int w,int h);





#endif