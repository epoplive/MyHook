
#include <stdio.h>  
#include <android/log.h>  

#include "inlineHook.h"


#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)   


FILE* (*old_fopen)(const char *path, const char *mode) = NULL;

FILE* new_fopen(const char *path, const char *mode)
{
    LOGD("[*] inlineHook success ~");
    LOGD("[*] fopen path : %s", path);
}

int hook()
{
    LOGD("[*] start inline Hook ~ \r\n");

    if (registerInlineHook((uint32_t) fopen, (uint32_t) new_fopen, (uint32_t **) &old_fopen) != ELE7EN_OK)
        return -1;

    if (inlineHook((uint32_t) fopen) != ELE7EN_OK)
        return -1;

    LOGD("[*] Old fopen = %p\n", fopen); 
    LOGD("[*] New fopen = %p\n", new_fopen); 

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) fopen) != ELE7EN_OK)
        return -1;

    LOGD("[*] exit inline Hook~ \r\n");
    return 0;
}



int hook_entry(char * a)
{
    hook();
    // unHook();

    return 0;  
}  