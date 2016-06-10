/*
    HOOK模块
    1、遍历HOOK模块中函数地址
    2、替换为自己函数地址
*/

#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <android/log.h>  
#include <elf.h>  
#include <fcntl.h>  
#include <sys/mman.h>  
  
#define LOG_TAG "DEBUG"  
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)    

#define LIBSF_PATH  "/data/app-lib/ffx0.joy.crack1-1/libffx0.so"

char* (*old_fgets)(char* dest, int size, FILE* stream) = -1;

char* new_fgets(char* dest, int size, FILE* stream)
{
    char* result = old_fgets(dest, size, stream);
    if(result == NULL)
        return result;

    LOGD("[*] fgets dest = %s\r\n" dest);
}

void* get_module_base(pid_t pid, const char* module_name)  
{  
    FILE *fp;  
    long addr = 0;  
    char *pch;  
    char filename[32];  
    char line[1024];  
  
    if (pid < 0) 
    {
        /* self process */  
        snprintf(filename, sizeof(filename), "/proc/self/maps", pid);  
    }
    else
    {  
        snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);  
    }  
  
    fp = fopen(filename, "r");  
  
    if (fp != NULL) 
    {
        while (fgets(line, sizeof(line), fp)) 
        {
            if (strstr(line, module_name))          // 查找字符串第一次出现的位置
            {
                pch = strtok( line, "-" );          // 分割字符串
                addr = strtoul( pch, NULL, 16 );    // 将字符串转换成无符号长整型数
  
                if (addr == 0x8000)  
                    addr = 0;  
  
                break;  
            }  
        }  
  
        fclose(fp) ;  
    }  
  
    return (void *)addr;  
}  

int hook_fgets()    
{
    // 保存待hook函数地址
    old_fgets = fgets;
    LOGD("[*] Old fgets = %p\n", old_fgets);    

    // 获取待hook模块基地址
    void * base_addr = get_module_base(getpid(), LIBSF_PATH);    

    LOGD("[*] libc.so module base address = %p\n", base_addr);    
  
    int fd = 0;
    fd = open(LIBSF_PATH, O_RDONLY);    
    if (-1 == fd) 
    {
        LOGD("[*] fopen error\n");    
        return -1;    
    }    
  
    // 读取elf头
    Elf32_Ehdr ehdr;    
    read(fd, &ehdr, sizeof(Elf32_Ehdr));

    // 得到节表信息
    unsigned long shdr_addr = ehdr.e_shoff;     // 节表偏移    
    int shnum = ehdr.e_shnum;                   // 节表个数
    int shent_size = ehdr.e_shentsize;          // shstrtab 节大小
    unsigned long stridx = ehdr.e_shstrndx;     // shstrtab 节索引

    // 读取shstrtab节
    Elf32_Shdr shdr;    
    lseek(fd, shdr_addr + stridx * shent_size, SEEK_SET);      // 文件指针指向shstrtab节表偏移位置
    read(fd, &shdr, shent_size);      

    // 读取shstrtab节区
    char * string_table = (char *)malloc(shdr.sh_size);      
    lseek(fd, shdr.sh_offset, SEEK_SET);      
    read(fd, string_table, shdr.sh_size);    
    lseek(fd, shdr_addr, SEEK_SET);      
  
    int i;      
    uint32_t out_addr = 0;    
    uint32_t out_size = 0;    
    uint32_t got_item = 0;  
    int32_t got_found = 0;    
  
    // 遍历节表
    for (i = 0; i < shnum; i++) 
    {
        read(fd, &shdr, shent_size);      
        if (shdr.sh_type == SHT_PROGBITS) 
        {
            int name_idx = shdr.sh_name;

            // 查找.GOT.PLT表      
            if (strcmp(&(string_table[name_idx]), ".got.plt") == 0 || strcmp(&(string_table[name_idx]), ".got") == 0) 
            {
                out_addr = base_addr + shdr.sh_addr;    // 基地地址 + GOT表虚拟地址
                out_size = shdr.sh_size;

                LOGD("[*] shdr.sh_addr = 0x%08X\n", shdr.sh_addr);
                LOGD("[*] out_addr = 0x%08X, out_size = %d\n", out_addr, out_size);
  
                LOGD("[*] start traverse got table ~");
                for (i = 0; i < out_size; i += 4) 
                {
                    got_item = *(uint32_t *)(out_addr + i);
                    LOGD("[*] got_item = 0x%08X\n", got_item);
                    
                    if (got_item  == old_fgets) 
                    {
                        LOGD("[*] Found fgets in got\n");    
                        got_found = 1;  
  
                        uint32_t page_size = getpagesize();  
                        uint32_t entry_page_start = (out_addr + i) & (~(page_size - 1));  
                        mprotect((uint32_t *)entry_page_start, page_size, PROT_READ | PROT_WRITE);  

                        // 替换为 new 函数地址
                        *(uint32_t *)(out_addr + i) = new_fgets;  
                        LOGD("[*] Hook success\n");    
                        break;      
                    }
                    else if (got_item == new_fgets) 
                    {
                        LOGD("[*] Already hooked\n");    
                        break;      
                    }
                }

                if (got_found)   
                    break;  
                else
                    LOGD("[*] Not found fgets addres\n");
            }     
        }      
    }      
  
    free(string_table);      
    close(fd);    
}

int hook_entry(char * a)
{
    LOGD("[*] Start hooking by xk~ \n");  
    hook_fgets(); 

    return 0;  
}  