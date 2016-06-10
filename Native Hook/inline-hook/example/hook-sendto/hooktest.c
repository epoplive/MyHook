
#include <stdio.h>  
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <android/log.h>  

#include "inlineHook.h"


#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)   


ssize_t (*old_sendto)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) = NULL;


ssize_t new_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{
    int i = 0;

    //char* tempbuff = (char*)malloc(sizof(char)*len);

    LOGD("[*] inlineHook success ~");

    // for(i = 0; i < len; i++)
    // {
    //   sprintf(tempbuff,"%02x ",buff[i]);
    //   strcat(dbgout, tempbuff);
    // }

    // LOGD("sendto buff:%s\r\n",dbgout);
    LOGD("[*] sendto buff : %s\r\n", buf);
    LOGD("````````````````````````````````\r\n");

    // free(tempbuff);
}

int hook()
{
    LOGD("[*] start inline Hook ~ \r\n");

    if (registerInlineHook((uint32_t) sendto, (uint32_t) new_sendto, (uint32_t **) &old_sendto) != ELE7EN_OK)
        return -1;

    if (inlineHook((uint32_t) sendto) != ELE7EN_OK)
        return -1;

    LOGD("[*] Old sendto = %p\n", sendto); 
    LOGD("[*] New sendto = %p\n", new_sendto); 

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) sendto) != ELE7EN_OK)
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