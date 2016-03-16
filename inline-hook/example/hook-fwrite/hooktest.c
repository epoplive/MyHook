
#include <stdio.h>  
#include <unistd.h>
#include <android/log.h>  

#include "inlineHook.h"


#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)   

size_t (*old_fwrite)(const void *ptr, size_t size, size_t nmemb, FILE *stream) = NULL;

size_t new_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    LOGD("[*] inlineHook success ~");
    LOGD("[*] read buf (0 exit): %s", ptr);
}

int hook()
{
    LOGD("[*] start inline Hook ~ \r\n");

    if (registerInlineHook((uint32_t) fwrite, (uint32_t) new_fwrite, (uint32_t **) &old_fwrite) != ELE7EN_OK)
        return -1;

    if (inlineHook((uint32_t) fwrite) != ELE7EN_OK)
        return -1;

    LOGD("[*] Old read = %p\n", fwrite); 
    LOGD("[*] New read = %p\n", new_fwrite); 

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) fwrite) != ELE7EN_OK)
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