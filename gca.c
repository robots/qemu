
// TODO: error checking needs to be done correctly

#include "config.h"
#include "qemu-common.h"

#include <err.h>
#include "cpu.h"
#include "gdbstub.h"

#include <Python.h>
#include "gca.h"

#define GCA_ERROR(s) \
	do { \
		printf("%s: %s \n", __func__, s); \
		gca_state = 0; \
	} while(0);

#ifndef TARGET_CPU_MEMORY_RW_DEBUG
static inline int target_memory_rw_debug(CPUState *env, target_ulong addr,
                                         uint8_t *buf, int len, int is_write)
{
    return cpu_memory_rw_debug(env, addr, buf, len, is_write);
}
#else
/* target_memory_rw_debug() defined in cpu.h */
#endif

int gca_state = 0;
PyObject *gca_module;
PyObject *True;


unsigned int gca_threadlist_create(CPUState *cpu)
{
	PyObject *fnc;
	PyObject *args;
	PyObject *val;
	long int ret = 0;

	fnc = PyObject_GetAttrString(gca_module, "threadlist_create");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(1);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (PyErr_Occurred()) {
			PyErr_Print();
			gca_state = 0;
		}

		if (val == NULL) {
			if (PyErr_Occurred())
				PyErr_Print();
			printf("threadlist_create failed\n");
			return -1;
		}

		ret = PyLong_AsLong(val);
		Py_DECREF(val);
		Py_DECREF(fnc);
	} else {
		if (PyErr_Occurred())
			PyErr_Print();
		printf("no such function\n");
	}

	return ret;
}

unsigned int gca_threadlist_count(CPUState *cpu)
{
	PyObject *fnc;
	PyObject *args;
	PyObject *val;
	long int ret = 0;

	fnc = PyObject_GetAttrString(gca_module, "threadlist_count");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(1);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (PyErr_Occurred()) {
			PyErr_Print();
			gca_state = 0;
		}

		if (val == NULL) {
			if (PyErr_Occurred())
				PyErr_Print();
			printf("threadlist_count failed\n");
			return 0;
		}

		ret = PyLong_AsLong(val);
		Py_DECREF(val);
	}
	Py_DECREF(fnc);

	return ret;
}

unsigned int gca_threadlist_getnext(CPUState *cpu)
{
	PyObject *fnc;
	PyObject *args;
	PyObject *val;
	long int ret = 0;

	fnc = PyObject_GetAttrString(gca_module, "threadlist_getnext");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(1);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (PyErr_Occurred()) {
			PyErr_Print();
			gca_state = 0;
		}

		if (val == NULL) {
			if (PyErr_Occurred())
				PyErr_Print();
			printf("threadlist_getnext failed\n");
			return 0;
		}

		ret = PyLong_AsLong(val);
		Py_DECREF(val);
	}
	Py_DECREF(fnc);

	return ret;
}

unsigned int gca_thread_getinfo(CPUState *cpu, target_ulong id, char *buf, target_ulong *len)
{
	PyObject *fnc;
	PyObject *args;
	PyObject *val;

	fnc = PyObject_GetAttrString(gca_module, "thread_getinfo");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(2);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));
		PyTuple_SetItem(args, 1, PyLong_FromLong(id));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (PyErr_Occurred()) {
			PyErr_Print();
			gca_state = 0;
		}

		if (val == NULL) {
			printf("thread_getinfo failed\n");
			return 1;
		} else if (!PyString_Check(val)) {
			return 1;
		}
		
		*len = PyString_Size(val);
		strncpy(buf, PyString_AsString(val), *len);

		Py_DECREF(val);
	}
	Py_DECREF(fnc);

	return 0;
}

int gca_thread_isalive(CPUState *cpu, target_ulong id)
{
	PyObject *fnc;
	PyObject *args;
	PyObject *val;
	int ret = 0;

	fnc = PyObject_GetAttrString(gca_module, "thread_isalive");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(2);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));
		PyTuple_SetItem(args, 1, PyLong_FromLong(id));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (PyErr_Occurred()) {
			PyErr_Print();
			gca_state = 0;
		}

		if ((val == NULL) || !PyBool_Check(val)) {
			gca_state = 0;
			return 0;
		}

		if (val == True) {
			ret = 1;
		}
		Py_DECREF(val);
		
	}
	Py_DECREF(fnc);

	return ret;
}

target_ulong gca_thread_getcurrent(CPUState *cpu)
{
	PyObject *fnc;
	PyObject *args;
	PyObject *val;
	target_ulong ret = 0;

	fnc = PyObject_GetAttrString(gca_module, "thread_getcurrent");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(1);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (PyErr_Occurred()) {
			PyErr_Print();
			gca_state = 0;
		}

		if (val == NULL) {
			if (PyErr_Occurred())
				PyErr_Print();
			printf("thread_getcurrent failed\n");
			return 0;
		}

		ret = PyLong_AsLong(val);
		Py_DECREF(val);
	}
	Py_DECREF(fnc);

	return ret;
}
/*
target_ulong gca_thread_getregister(CPUState *cpu, unsigned int id, uint8_t *data)
{
	PyObject *fnc;
	PyObject *args;
	PyObject *val;
	target_ulong ret = 0;

	fnc = PyObject_GetAttrString(gca_module, "thread_getregister");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(1);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (PyErr_Occurred()) {
			PyErr_Print();
			gca_state = 0;
		}

		if (val == NULL) {
			if (PyErr_Occurred())
				PyErr_Print();
			printf("thread_getcurrent failed\n");
			return 0;
		}

		ret = PyLong_AsLong(val);
		Py_DECREF(val);
	}
	Py_DECREF(fnc);

	return ret;
}

target_ulong gca_thread_setregister(CPUState *cpu, unsigned int id, uint8_t *data);
*/

int gca_thread_regs_read(CPUState *cpu, target_ulong id, uint8_t *regs, target_ulong *len)
{
	PyObject *fnc = NULL;
	PyObject *args = NULL;
	PyObject *val = NULL;
	Py_ssize_t str_len;
	int ret = 0;

	fnc = PyObject_GetAttrString(gca_module, "thread_regs_read");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(2);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));
		PyTuple_SetItem(args, 1, PyLong_FromLong(id));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (val != NULL) {
			if (PyString_Check(val)) {
				str_len = PyString_Size(val);
				memcpy(regs, PyString_AsString(val), str_len);

				// FIXME: shouldn't this check against expected value?
				ret = (str_len > 0) ? 1 : 0;
				*len = str_len;
			} else {
				GCA_ERROR("unexpected type");
			}
		}
	}

	if (val)
		Py_DECREF(val);

	if (fnc)
		Py_DECREF(fnc);

	if (PyErr_Occurred()) {
		GCA_ERROR("interpreter error - disabling");
		PyErr_Print();
	}

	return ret;
}

int gca_thread_regs_write(CPUState *cpu, target_ulong id, uint8_t *regs, target_ulong len)
{
	PyObject *fnc = NULL;
	PyObject *args = NULL;
	PyObject *val = NULL;
	int ret = 0;

	fnc = PyObject_GetAttrString(gca_module, "thread_regs_write");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(3);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));
		PyTuple_SetItem(args, 1, PyLong_FromLong(id));
		PyTuple_SetItem(args, 2, PyString_FromStringAndSize((const char *)regs, len));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (val != NULL) {
			if (PyBool_Check(val)) {
				if (val == True){
					ret = 1;
				}

			} else {
				GCA_ERROR("unexpected type");
			}
		}
	}

	if (val)
		Py_DECREF(val);

	if (fnc)
		Py_DECREF(fnc);

	if (PyErr_Occurred()) {
		GCA_ERROR("interpreter error - disabling");
		PyErr_Print();
	}

	return ret;
}

int gca_thread_mem_read(CPUState *cpu, target_ulong id, target_ulong addr, uint8_t *mem, target_ulong len)
{
	PyObject *fnc = NULL;
	PyObject *args = NULL;
	PyObject *val = NULL;
	Py_ssize_t str_len;
	int ret = 0;

	fnc = PyObject_GetAttrString(gca_module, "thread_mem_read");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(4);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));
		PyTuple_SetItem(args, 1, PyLong_FromLong(id));
		PyTuple_SetItem(args, 2, PyLong_FromLong(addr));
		PyTuple_SetItem(args, 3, PyLong_FromLong(len));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (val != NULL) {
			if (PyString_Check(val)) {
				str_len = PyString_Size(val);
				memcpy(mem, PyString_AsString(val), str_len);

				ret = (str_len == len) ? 1 : 0;
			} else {
				GCA_ERROR("unexpected type");
			}
		}
	}

	if (val)
		Py_DECREF(val);

	if (fnc)
		Py_DECREF(fnc);

	if (PyErr_Occurred()) {
		GCA_ERROR("interpreter error - disabling");
		PyErr_Print();
	}

	return ret;
}

int gca_thread_mem_write(CPUState *cpu, target_ulong id, target_ulong addr, uint8_t *mem, target_ulong len)
{
	PyObject *fnc = NULL;
	PyObject *args = NULL;
	PyObject *val = NULL;
	int ret = 0;

	fnc = PyObject_GetAttrString(gca_module, "thread_mem_write");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(4);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));
		PyTuple_SetItem(args, 1, PyLong_FromLong(id));
		PyTuple_SetItem(args, 2, PyLong_FromLong(addr));
		PyTuple_SetItem(args, 3, PyString_FromStringAndSize((const char *)mem, len));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (val != NULL) {
			if (PyBool_Check(val)) {
				if (val == True) {
					ret = 1;
				}
			} else {
				GCA_ERROR("unexpected type");
			}
		}
	}

	if (val)
		Py_DECREF(val);

	if (fnc)
		Py_DECREF(fnc);

	if (PyErr_Occurred()) {
		GCA_ERROR("interpreter error - disabling");
		PyErr_Print();
	}

	return ret;
}

unsigned int gca_symbol_getunknown(char *name, target_ulong *len)
{
	PyObject *fnc;
	PyObject *val;

	fnc = PyObject_GetAttrString(gca_module, "symbol_getunknown");
			if (PyErr_Occurred())
				PyErr_Print();

	if (fnc && PyCallable_Check(fnc)) {
		val = PyObject_CallObject(fnc, NULL);

		if (PyErr_Occurred()) {
			PyErr_Print();
			gca_state = 0;
		}


		if (val == NULL) {
			printf("symbol_getunknown failed\n");
			return 0;
		} else if (!PyString_Check(val)) {
			if (PyErr_Occurred())
				PyErr_Print();
			return 0;
		}
		
		*len = PyString_Size(val);
		strncpy(name, PyString_AsString(val), *len);

		Py_DECREF(val);
	}

	if (fnc)
		Py_DECREF(fnc);

	return *len;
}

unsigned int gca_symbol_add(const char *name, target_ulong addr)
{
	PyObject *fnc;
	PyObject *val;
	PyObject *args;

	fnc = PyObject_GetAttrString(gca_module, "symbol_add");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(2);
		PyTuple_SetItem(args, 0, PyString_FromString(name));
		PyTuple_SetItem(args, 1, PyLong_FromLong(addr));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (PyErr_Occurred()) {
			PyErr_Print();
			gca_state = 0;
		}

		if (val != NULL) {
			Py_DECREF(val);
		}
	}
	Py_DECREF(fnc);

	return 0;
}

char *gca_monitor_command(const char *command, const char *arg)
{
	PyObject *fnc = NULL;
	PyObject *val = NULL;
	PyObject *args = NULL;
	char *out = NULL;

	fnc = PyObject_GetAttrString(gca_module, "monitor_command");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(2);
		PyTuple_SetItem(args, 0, PyString_FromString(command));
		if (arg) {
			PyTuple_SetItem(args, 1, PyString_FromString(arg));
		} else {
			PyTuple_SetItem(args, 1, PyString_FromString(""));
		}

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (val != NULL) {
			if (PyString_Check(val)) {
				out = strdup(PyString_AsString(val));
				if (!out) {
					GCA_ERROR("out of memory");
				}
			} else {
				GCA_ERROR("unexpected type");
			}
		}
	}

	if (val)
		Py_DECREF(val);

	if (fnc)
		Py_DECREF(fnc);

	if (PyErr_Occurred()) {
		GCA_ERROR("interpreter error - disabling");
		PyErr_Print();
	}

	return out;
}

int gca_hook_breakpoint(CPUState *cpu)
{
	PyObject *fnc = NULL;
	PyObject *args = NULL;
	PyObject *val = NULL;
	int ret = 0;

	fnc = PyObject_GetAttrString(gca_module, "hook_breakpoint");

	if (fnc && PyCallable_Check(fnc)) {
		args = PyTuple_New(1);
		PyTuple_SetItem(args, 0, PyLong_FromLong((long int)cpu));

		val = PyObject_CallObject(fnc, args);
		Py_DECREF(args);

		if (val != NULL) {
			if (PyBool_Check(val)) {
				if (val == True) {
					ret = 1;
				}
			} else {
				GCA_ERROR("unexpected type");
			}
		}
	}

	if (val)
		Py_DECREF(val);

	if (fnc)
		Py_DECREF(fnc);

	if (PyErr_Occurred()) {
		GCA_ERROR("interpreter error - disabling");
		PyErr_Print();
	}

	return ret;
}

// target_memory_write(cpu, addr, string)
static PyObject* gca_target_memory_write(PyObject *self, PyObject *args)
{
	CPUState *cpu;
	target_ulong addr;
	uint8_t *data;
	int len;
	int ret;

	if(!PyArg_ParseTuple(args, "lls#:target_memory_write", (long int *)&cpu, (long int *)&addr, (char *)&data, &len))
		return NULL;
	
//	printf("write %lx %lx %lx %d\n", cpu, addr, data, len);
	ret = target_memory_rw_debug(cpu, addr, data, len, 1);

	return Py_BuildValue("i", ret);
}

// target_memory_read(cpu, addr, len)
static PyObject* gca_target_memory_read(PyObject *self, PyObject *args)
{
	CPUState *cpu;
	target_ulong addr;
	uint8_t data[4096];
	int len;

	if(!PyArg_ParseTuple(args, "lli:target_memory_read", (long int *)&cpu, (long int *)&addr, &len))
		return NULL;
	
//	printf("read %lx %lx %d\n", cpu, addr, len);
	if (len > 4096) {
		len = 0;
	}

	if (len > 0) {
		target_memory_rw_debug(cpu, addr, data, len, 0);
	}

	return Py_BuildValue("s#", data, len);
}

// target_regs_write(cpu, string)
static PyObject* gca_target_regs_write(PyObject *self, PyObject *args)
{
	CPUState *cpu;
	uint8_t *data;
	int len;
	int ret = 1;

	if(!PyArg_ParseTuple(args, "ls#:target_regs_write", (long int *)&cpu, (char *)&data, &len))
		return NULL;
	
	gdb_write_registers(cpu, data, len);

	return Py_BuildValue("i", ret);
}

// target_regs_read(cpu)
static PyObject* gca_target_regs_read(PyObject *self, PyObject *args)
{
	CPUState *cpu;
	uint8_t data[4096];
	target_ulong len;

	if(!PyArg_ParseTuple(args, "l:target_regs_read", (long int *)&cpu))
		return NULL;
	

	gdb_read_registers(cpu, data, &len);

	return Py_BuildValue("s#", data, len);
}

static PyObject* gca_breakpoint_insert(PyObject *self, PyObject *args)
{
	target_ulong addr;
	target_ulong len;
	int type;
	int ret;

	if(!PyArg_ParseTuple(args, "lli:breakpoint_insert", (long int *)&addr, (long int *)&len, &type))
		return NULL;

	ret = gdb_breakpoint_insert(addr, len, type);

	if (ret >= 0)
		return Py_True;

	return Py_False;
}

static PyObject* gca_breakpoint_remove(PyObject *self, PyObject *args)
{
	target_ulong addr;
	target_ulong len;
	int type;
	int ret;

	if(!PyArg_ParseTuple(args, "lli:breakpoint_remove", (long int *)&addr, (long int *)&len, &type))
		return NULL;

	ret = gdb_breakpoint_remove(addr, len, type);

	if (ret >= 0)
		return Py_True;

	return Py_False;
}

static PyObject* gca_breakpoint_remove_all(PyObject *self, PyObject *args)
{
	if(!PyArg_ParseTuple(args, ":breakpoint_remove_all"))
		return NULL;

	gdb_breakpoint_remove_all();

	return Py_True;
}

static PyObject* gca_monitor_output(PyObject *self, PyObject *args)
{
	uint8_t *data;
	int len;
	int ret = 1;

	if(!PyArg_ParseTuple(args, "s#:gdb_monitor_output", (char *)&data, &len))
		return NULL;

	printf("%s\n", data);
	ret = gdb_monitor_write(NULL, data, len);
	gdb_monitor_write(NULL, (const uint8_t *)"\n", 1);

	return Py_BuildValue("i", ret);
}

static PyMethodDef GcaMethods[] = {
	{"target_memory_write", gca_target_memory_write, METH_VARARGS, "Write target memory"},
	{"target_memory_read", gca_target_memory_read, METH_VARARGS, "Read target memory"},
	{"target_regs_write", gca_target_regs_write, METH_VARARGS, "Write target registers"},
	{"target_regs_read", gca_target_regs_read, METH_VARARGS, "Read target registers"},
	{"breakpoint_insert", gca_breakpoint_insert, METH_VARARGS, "Insert breakpoint"},
	{"breakpoint_remove", gca_breakpoint_remove, METH_VARARGS, "Remove breakpoint"},
	{"breakpoint_remove_all", gca_breakpoint_remove_all, METH_VARARGS, "Remove all breakpoints"},
	{"monitor_output", gca_monitor_output, METH_VARARGS, "Write text to remote gdb"},

	{NULL, NULL, 0, NULL}
};

int gca_active(void)
{
	return gca_state == 1;
}

int gca_init(const char *file)
{
	PyObject *py_file;

	printf("GCA: loading \"%s\"\n", file);
	Py_Initialize();
	True = Py_True;

	Py_InitModule("gca", GcaMethods);

	py_file = PyString_FromString(file);
	if (py_file == NULL) {
		err(1, "name");
	}

	gca_module = PyImport_Import(py_file);
	if (gca_module == NULL) {
		if (PyErr_Occurred())
			PyErr_Print();
		printf("Unable to load module \"%s\"\n", file);
		return 1;
	}

	Py_DECREF(py_file);

	gca_state = 1;
	return 0;
}

void gca_destroy(void)
{
	Py_DECREF(gca_module);
	Py_DECREF(True);
	Py_Finalize();
}

