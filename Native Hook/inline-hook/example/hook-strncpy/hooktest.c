
#include <stdio.h>  
#include <string.h>
#include <android/log.h>  

#include "inlineHook.h"


#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)   

char* (*old_strncpy)(char *dest, const char *src, size_t n) = NULL;

char* new_strncpy(char *dest, const char *src, size_t n)
{
    LOGD("[*] inlineHook success ~");

    char* result = old_strncpy(dest, src, n);
    if(result == NULL)
        return result;

    LOGD("[*] strcpy dest : %s\r\n", dest);
    LOGD("[*] strcpy src : %s\r\n", src);
}

int hook()
{
    LOGD("[*] start inline Hook ~ \r\n");

    if (registerInlineHook((uint32_t) strncpy, (uint32_t) new_strncpy, (uint32_t **) &old_strncpy) != ELE7EN_OK)
        return -1;

    if (inlineHook((uint32_t) strncpy) != ELE7EN_OK)
        return -1;

    LOGD("[*] Old strncpy = %p\n", strncpy); 
    LOGD("[*] New strncpy = %p\n", new_strncpy); 

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) strncpy) != ELE7EN_OK)
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