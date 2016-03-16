
#include <stdio.h>  
#include <unistd.h>
#include <android/log.h>  

#include "inlineHook.h"


#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)   

ssize_t (*old_read)(int fd, void *buf, size_t count) = NULL;


ssize_t new_read(int fd, void *buf, size_t count)
{
    LOGD("[*] inlineHook success ~");
    LOGD("[*] read buf : %s", buf);
}

int hook()
{
    LOGD("[*] start inline Hook ~ \r\n");

    if (registerInlineHook((uint32_t) read, (uint32_t) new_read, (uint32_t **) &old_read) != ELE7EN_OK)
        return -1;

    if (inlineHook((uint32_t) read) != ELE7EN_OK)
        return -1;

    LOGD("[*] Old read = %p\n", read); 
    LOGD("[*] New read = %p\n", new_read); 

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) read) != ELE7EN_OK)
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