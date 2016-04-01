#include "Python.h"
#include "calutil.h"
#include <stdbool.h>

/************************
caltoolpylink.c
Linkage code and wrapper functions that allow communication between xcal GUI
program and caltool command line program

Author: Brandon Chester : 0877477
Contact: bchester@mail.uoguelph.ca
Created: Mar 10, 2016
Last modified: Mar 10, 2016
*************************/

static PyObject *Cal_readFile(PyObject *self, PyObject *args) {
    PyObject *result;
    const char *filename;
    PyArg_ParseTuple(args, "sO", &filename, &result);

    CalComp *comp = NULL;
    CalStatus readStatus;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        char returnString[400];
        sprintf(returnString, "Error opening file %s: %s", filename,
                strerror(errno));
        PyObject *ret = Py_BuildValue("s", returnString);
        return ret;
    }
    readStatus = readCalFile(file, &comp);
    if (readStatus.code != OK) {
        char returnString[400];
        sprintf(returnString,
                "ReadCalFile returned error code %d on line %d in file %s",
                readStatus.code, readStatus.lineto, filename);
        PyObject *ret = Py_BuildValue("s", returnString);
        readStatus.code = OK;
        readStatus.lineto = 0;
        readStatus.linefrom = 0;
        return ret;
    }

    PyObject *compObj = Py_BuildValue("k", ((unsigned long)comp));
    PyList_Append(result, compObj);
    PyObject *compList = PyList_New(comp->ncomps);

    for (int i = 0; i < comp->ncomps; i++) {
        CalComp *c = comp->comp[i];
        char *summary = NULL;
        CalProp *traverseProps = c->prop;
        for (int j = 0; j < c->nprops; j++) {
            if (strcmp(traverseProps->name, "SUMMARY") == 0) {
                summary = traverseProps->value;
                break;
            }
            traverseProps = traverseProps->next;
        }

        if (summary == NULL) {
            PyObject *temp =
                Py_BuildValue("siis", c->name, c->nprops, c->ncomps, "");
            PyList_SetItem(compList, i, temp);
        } else {
            PyObject *temp =
                Py_BuildValue("siis", c->name, c->nprops, c->ncomps, summary);
            PyList_SetItem(compList, i, temp);
        }
    }
    PyList_Append(result, compList);
    PyObject *ret = Py_BuildValue("s", "OK");
    return ret;
}

static PyObject *Cal_writeFile(PyObject *self, PyObject *args) {
    char *filename;
    PyObject *pcal;
    PyObject *complist;
    PyArg_ParseTuple(args, "skO", &filename, (unsigned long *)&pcal, &complist);

    CalComp *pcomp = (CalComp *)pcal;
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        char returnString[400];
        sprintf(returnString, "Error writing to file %s: %s", filename,
                strerror(errno));
        PyObject *ret = Py_BuildValue("s", returnString);
        return ret;
    }
    if (PyList_Check(complist) == 1) {
        int compListLength = PyList_Size(complist);

        if (compListLength < pcomp->ncomps) {
            char *tempStr = malloc(3);
            assert(tempStr != NULL); 
            strcpy(tempStr, "");

            CalComp *tempComps[pcomp->ncomps];
            CalComp *reducedComps[compListLength];

            int reducedCompsSize = 0;
            int tempCompsSize = pcomp->ncomps;

            for (int i = 0; i < pcomp->ncomps; i++) {
                tempComps[i] = pcomp->comp[i];
            }
            //  puts("test1");
            for (int i = 0; i < compListLength; i++) {
                PyObject *listItem = PyList_GetItem(complist, i);
                char *name;
                char *summary;
                int ncomps;
                int nprops;

                PyArg_ParseTuple(listItem, "siis", &name, &nprops, &ncomps,
                                 &summary);

                bool isFound = false;
                char *propSummary = NULL;
                int j;
                for (j = 0; j < pcomp->ncomps; j++) {
                    CalProp *traverseProps = pcomp->comp[j]->prop;

                    for (int k = 0; k < pcomp->comp[j]->nprops; k++) {
                        if (strcmp(traverseProps->name, "SUMMARY") == 0) {
                            propSummary = traverseProps->value;
                            break;
                        }
                        traverseProps = traverseProps->next;
                    }
                    if (propSummary == NULL) {
                        propSummary = tempStr;
                    }

                    if (pcomp->comp[j]->ncomps == ncomps &&
                        pcomp->comp[j]->nprops == nprops &&
                        strcmp(summary, propSummary) == 0 &&
                        strcmp(name, pcomp->comp[j]->name) == 0) {
                        isFound = true;
                        break;
                    }
                }
                if (isFound) {
                    reducedComps[reducedCompsSize] = pcomp->comp[j];
                    reducedCompsSize++;
                    isFound = false;
                }
            }

            pcomp->ncomps = reducedCompsSize;
            for (int i = 0; i < reducedCompsSize; i++) {
                pcomp->comp[i] = reducedComps[i];
            }
            writeCalComp(file, pcomp);

            pcomp->ncomps = tempCompsSize;
            for (int i = 0; i < tempCompsSize; i++) {
                pcomp->comp[i] = tempComps[i];
            }

            free(tempStr);
        } else {
            writeCalComp(file, pcomp);
        }
    } else if (PyLong_Check(complist) == 1) {
        long int pos = PyLong_AsLong(complist);
        writeCalComp(file, pcomp->comp[pos]);
    }
    fclose(file);

    PyObject *ret = Py_BuildValue("s", "OK");
    return ret;
}

static PyObject *Cal_freeFile(PyObject *self, PyObject *args) {
    CalComp *pcal;
    PyArg_ParseTuple(args, "k", (unsigned long *)&pcal);
    freeCalComp(pcal);
    PyObject *ret = Py_BuildValue("s", "OK");
    return ret;
}

static PyMethodDef CalMethods[] = {{"readFile", Cal_readFile, METH_VARARGS},
                                   {"writeFile", Cal_writeFile, METH_VARARGS},
                                   {"freeFile", Cal_freeFile, METH_VARARGS},
                                   {NULL, NULL}};

static struct PyModuleDef calModuleDef = {
    PyModuleDef_HEAD_INIT,
    "Cal",     // enable "import Cal"
    NULL,      // omit module documentation
    -1,        // don't reinitialize the module
    CalMethods // link module name "Cal" to methods table
};

PyMODINIT_FUNC PyInit_Cal(void) { return PyModule_Create(&calModuleDef); }
