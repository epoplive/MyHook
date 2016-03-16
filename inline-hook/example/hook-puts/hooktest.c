
#include <stdio.h>  
#include <android/log.h>  

#include "inlineHook.h"


#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)   

int (*old_puts)(const char *) = NULL;

int new_puts(const char *string)
{
    LOGD("[*] ````````````````````````\n");
    LOGD("[*] puts : %s\r\n", string);
    LOGD("[*] ````````````````````````\n");
}

int hook()
{
    LOGD("[*] Old puts = %p\n", puts); 

    if (registerInlineHook((uint32_t) puts, (uint32_t) new_puts, (uint32_t **) &old_puts) != ELE7EN_OK)
        return -1;

    LOGD("[*] New puts = %p\n", new_puts); 

    if (inlineHook((uint32_t) puts) != ELE7EN_OK)
        return -1;

    LOGD("[*] start inline Hook ~ \r\n");

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) puts) != ELE7EN_OK)
        return -1;

    LOGD("[*] exit inline Hook~ \r\n");
    return 0;
}



int hook_entry(char * a)
{
    puts("hello, world!");
    
    hook();
    puts("hello, world!");
    puts("hello, world!");
    
    unHook();
    puts("hello, world!");

    return 0;  
}  