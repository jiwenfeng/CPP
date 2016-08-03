#ifndef AMTSVR_MODULE_H
#define AMTSVR_MODULE_H

struct amtsvr_context;

typedef void * (*amtsvr_dl_create)(void);
typedef int (*amtsvr_dl_init)(void * inst, struct amtsvr_context *, const char * parm);
typedef void (*amtsvr_dl_release)(void * inst);

struct amtsvr_module {
	const char * name;
	//void * module;
	amtsvr_dl_create create;
	amtsvr_dl_init init;
	amtsvr_dl_release release;
};

int amtsvr_module_create(const char* name, amtsvr_dl_create create, amtsvr_dl_init init, amtsvr_dl_release release);
void amtsvr_module_insert(struct amtsvr_module *mod);
struct amtsvr_module * amtsvr_module_query(const char * name);
void * amtsvr_module_instance_create(struct amtsvr_module *);
int amtsvr_module_instance_init(struct amtsvr_module *, void * inst, struct amtsvr_context *ctx, const char * parm);
void amtsvr_module_instance_release(struct amtsvr_module *, void *inst);

void amtsvr_module_init(const char *path);
void amtsvr_module_release();
#endif
