#include "stdafx.h"
#include "amtsvr_module.h"

#include <assert.h>
#include <string.h>
#ifndef _WIN32
#include <dlfcn.h>
#endif // _WIN32
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_MODULE_TYPE 32

struct modules {
	int count;
	int lock;
	const char * path;
	struct amtsvr_module m[MAX_MODULE_TYPE];
};

static struct modules * M = NULL;

/*
static void *
_try_open(struct modules *m, const char * name) {
	const char * path = m->path;
	size_t path_size = strlen(path);
	size_t name_size = strlen(name);

	int sz = path_size + name_size;
	char tmp[sz];
	int i;
	for (i=0;path[i]!='?' && path[i]!='\0';i++) {
		tmp[i] = path[i];
	}
	memcpy(tmp+i,name,name_size);
	if (path[i] == '?') {
		strcpy(tmp+i+name_size,path+i+1);
	} else {
		fprintf(stderr,"Invalid C service path\n");
		exit(1);
	}

	void * dl = dlopen(tmp, RTLD_NOW | RTLD_GLOBAL);

	if (dl == NULL) {
		fprintf(stderr, "try open %s failed : %s\n",tmp,dlerror());
	}

	return dl;
}
*/

static struct amtsvr_module * _query(const char * name) {
	for (int i = 0; i < M->count; ++i) {
		if (strcmp(M->m[i].name, name) == 0) {
			return &M->m[i];
		}
	}

	return NULL;
}

/*

static int
_open_sym(struct amtsvr_module *mod) {
	size_t name_size = strlen(mod->name);
	char tmp[name_size + 9]; // create/init/release , longest name is release (7)
	memcpy(tmp, mod->name, name_size);
	strcpy(tmp+name_size, "_create");
	mod->create = (amtsvr_dl_create)dlsym(mod->module, tmp);
	strcpy(tmp+name_size, "_init");
	mod->init = (amtsvr_dl_init)dlsym(mod->module, tmp);
	strcpy(tmp+name_size, "_release");
	mod->release = (amtsvr_dl_release)dlsym(mod->module, tmp);

	return mod->init == NULL;
}*/

struct amtsvr_module * 
amtsvr_module_query(const char * name) {
	struct amtsvr_module * result = _query(name);
	/*if (result)
		return result;

	while(__sync_lock_test_and_set(&M->lock,1)) {}

	result = _query(name); // double check

	if (result == NULL && M->count < MAX_MODULE_TYPE) {
		int index = M->count;
		void * dl = _try_open(M,name);
		if (dl) {
			M->m[index].name = name;
			M->m[index].module = dl;

			if (_open_sym(&M->m[index]) == 0) {
				M->m[index].name = strdup(name);
				M->count ++;
				result = &M->m[index];
			}
		}
	}

	__sync_lock_release(&M->lock);*/

	return result;
}

#ifdef _WIN32
#define strdup _strdup
#endif // _WIN32

int 
amtsvr_module_create(const char* name, amtsvr_dl_create create, amtsvr_dl_init init, amtsvr_dl_release release)
{
    struct amtsvr_module *mod = (struct amtsvr_module*)malloc(sizeof(struct amtsvr_module));
    mod->name = strdup(name);
    //mod->module = NULL;
    mod->create = create;
    mod->init = init;
    mod->release = release;
    amtsvr_module_insert(mod);
    return 0;
}

void 
amtsvr_module_insert(struct amtsvr_module *mod) {
	//while(__sync_lock_test_and_set(&M->lock,1)) {}

	struct amtsvr_module * m = _query(mod->name);
	assert(m == NULL && M->count < MAX_MODULE_TYPE);
	int index = M->count;
	M->m[index] = *mod;
    free(mod);
	++M->count;
	//__sync_lock_release(&M->lock);
}

void * 
amtsvr_module_instance_create(struct amtsvr_module *m) {
	if (m->create) {
		return m->create();
	} else {
		return (void *)(intptr_t)(~0);
	}
}

int
amtsvr_module_instance_init(struct amtsvr_module *m, void * inst, struct amtsvr_context *ctx, const char * parm) {
	return m->init(inst, ctx, parm);
}

void 
amtsvr_module_instance_release(struct amtsvr_module *m, void *inst) {
	if (m->release) {
        //free((void*)m->name);
		m->release(inst);
	}
}

void 
amtsvr_module_init(const char *path) {
	struct modules *m = (struct modules *)malloc(sizeof(*m));
	m->count = 0;
	m->path = strdup(path);
	m->lock = 0;

	M = m;
}

void amtsvr_module_release()
{
    free((void*)M->path);
    for (int i = 0; i < M->count; ++i) {
        free((void*)M->m[i].name);
    }
    free((void*)M);
}
