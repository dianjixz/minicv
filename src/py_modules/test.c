#define PY_SSIZE_T_CLEAN
#include <Python.h>

// #include "collections.h"
// #include "imdefs.h"
#include "imlib.h"
#include "tanstation.h"

image_t self_image_img ;


static PyObject *
py_image_img_data_load(PyObject *self,PyObject *args,PyObject *keywds)
{
    return PyLong_FromLong(0);
}

static PyMethodDef imageMethods[] = {
	{"img_data_load", py_image_img_data_load, METH_VARARGS, "python to c module image!"},
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