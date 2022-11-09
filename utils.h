#ifndef __UTILS_H__
#define __UTILS_H__

#define ERROR(msg) error_handler(__FILE__, __LINE__, msg)
void error_handler(char* file, int line, char* msg);

#endif