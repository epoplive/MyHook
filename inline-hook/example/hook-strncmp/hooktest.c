
#include <stdio.h>  
#include <string.h>
#include <android/log.h>  

#include "inlineHook.h"


#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)   


int (*old_strncmp)(const char *s1, const char *s2, size_t n) = NULL;

int new_strncmp(const char *s1, const char *s2, size_t n)
{
    LOGD("[*] inlineHook success ~");

    char* result = old_strncmp(s1, s2, n);
    if(result == NULL)
        return result;

    LOGD("[*] strncmp s1 : %s\r\n", s1);
    LOGD("[*] strncmp s2 : %s\r\n", s2);
}

int hook()
{
    LOGD("[*] start inline Hook ~ \r\n");

    if (registerInlineHook((uint32_t) strncmp, (uint32_t) new_strncmp, (uint32_t **) &old_strncmp) != ELE7EN_OK)
        return -1;

    if (inlineHook((uint32_t) strncmp) != ELE7EN_OK)
        return -1;

    LOGD("[*] Old strncpy = %p\n", strncmp); 
    LOGD("[*] New strncpy = %p\n", new_strncmp); 

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) strcmp) != ELE7EN_OK)
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