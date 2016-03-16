
#include <stdio.h>  
#include <string.h>
#include <android/log.h>  

#include "inlineHook.h"


#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)   

size_t (*old_strlen)(const char *s) = NULL;

size_t new_strlen(const char *s)
{
    // LOGD("[*] inlineHook success ~");
    // LOGD("[*] strlen string : %s", s);
    return 0;
}

int hook()
{
    // LOGD("[*] start inline Hook ~ \r\n");

    if (registerInlineHook((uint32_t) strlen, (uint32_t) new_strlen, (uint32_t **) &old_strlen) != ELE7EN_OK)
        return -1;

    if (inlineHook((uint32_t) strlen) != ELE7EN_OK)
        return -1;

    // LOGD("[*] Old strlen = %p\n", strlen); 
    // LOGD("[*] New strlen = %p\n", new_strlen); 

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) strlen) != ELE7EN_OK)
        return -1;

    // LOGD("[*] exit inline Hook~ \r\n");
    return 0;
}



int hook_entry(char * a)
{
    hook();
    // unHook();

    return 0;  
}  