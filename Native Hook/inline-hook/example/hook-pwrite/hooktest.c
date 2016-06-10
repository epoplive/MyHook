
#include <stdio.h>  
#include <unistd.h>
#include <android/log.h>  

#include "inlineHook.h"


#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)   


ssize_t (*old_pwrite)(int fd, const void *buf, size_t count, off_t offset) = NULL;

size_t new_pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    LOGD("[*] inlineHook success ~");
    LOGD("[*] pwrite buf (0 exit): %s", buf);
}

int hook()
{
    LOGD("[*] start inline Hook ~ \r\n");

    if (registerInlineHook((uint32_t) pwrite, (uint32_t) new_pwrite, (uint32_t **) &old_pwrite) != ELE7EN_OK)
        return -1;

    if (inlineHook((uint32_t) pread) != ELE7EN_OK)
        return -1;

    LOGD("[*] Old pwrite = %p\n", pwrite); 
    LOGD("[*] New pwrite = %p\n", new_pwrite); 

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) pwrite) != ELE7EN_OK)
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