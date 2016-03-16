
#include <stdio.h>  
#include <string.h>
#include <android/log.h>  

#include "inlineHook.h"


#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)   


char* (*old_strcpy)(char *dest, const char *src) = NULL;

char* new_strcpy(char *dest, const char *src)
{
    LOGD("[*] inlineHook success ~");

    char* result = old_strcpy(dest, src);
    if(result == NULL)
        return result;

    LOGD("[*] strcpy dest : %s\r\n", dest);
    LOGD("[*] strcpy src : %s\r\n", src);
}

int hook()
{
    LOGD("[*] start inline Hook ~ \r\n");

    if (registerInlineHook((uint32_t) strcpy, (uint32_t) new_strcpy, (uint32_t **) &old_strcpy) != ELE7EN_OK)
        return -1;

    if (inlineHook((uint32_t) strlen) != ELE7EN_OK)
        return -1;

    LOGD("[*] Old strlen = %p\n", strcpy); 
    LOGD("[*] New strlen = %p\n", new_strcpy); 

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) strcpy) != ELE7EN_OK)
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