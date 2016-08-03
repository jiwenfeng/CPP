#ifndef AMTSVR_ENV_H
#define AMTSVR_ENV_H

const char * amtsvr_getenv(const char *key);
void amtsvr_setenv(const char *key, const char *value);

void amtsvr_env_init();
void amtsvr_env_release();

#endif
