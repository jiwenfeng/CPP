#ifndef __AMTSVR_ERROR_H__
#define __AMTSVR_ERROR_H__

#include <string>

#define amtsvr_log printf("%s:%d %s, ", __FILE__, __LINE__, __FUNCTION__),printf

#endif // __AMTSVR_ERROR_H__