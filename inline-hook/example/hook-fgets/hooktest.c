#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>  

#include "inlineHook.h"

#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)   


char* (*old_fgets)(char* dest, int size, FILE* stream) = NULL;

char* new_fgets(char* dest, int size, FILE* stream)
{
    LOGD("[*] inlineHook success ~");
    LOGD("[*] fgets dest = %s\r\n", dest);
}

int hook()
{
    LOGD("[*] start inline Hook ~ \r\n");
    
    if (registerInlineHook((uint32_t) fgets, (uint32_t) new_fgets, (uint32_t **) &old_fgets) != ELE7EN_OK)
        return -1;

    if (inlineHook((uint32_t) fgets) != ELE7EN_OK)
        return -1;

    LOGD("[*] Old fgets = %p\n", fgets); 
    LOGD("[*] New fgets = %p\n", new_fgets);

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) fgets) != ELE7EN_OK)
        return -1;

    LOGD("[*] exit inline Hook~ \r\n");
    return 0;
}



int hook_entry(char * a)
{
    hook();
    //unHook();

    return 0;  
}