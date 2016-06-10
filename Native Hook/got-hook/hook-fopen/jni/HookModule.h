#include <jni.h>

#ifndef _Included_com_sirc_jni_HelloWorld
#define _Included_com_sirc_jni_HelloWorld

#ifdef __cplusplus

extern "C" 
{
#endif

JNIEXPORT jstring JNICALL Java_com_sirc_jni_HelloWorld_printJNI(JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif










#include <jni.h>


/* Native interface, it will be call in java code */
JNIEXPORT jstring JNICALL Java_com_sirc_jni_HelloWorld_printJNI(JNIEnv *env, jobject obj, jstring inputStr) 
{
    LOGI("dufresne Hello World From libhelloworld.so!");

    // 从 instring 字符串取得指向字符串 UTF 编码的指针
    const char *str = (const char *) (*env)->GetStringUTFChars(env, inputStr, JNI_FALSE);
    LOGI("dufresne--->%s", (const char *)str);

    // 通知虚拟机本地代码不再需要通过 str 访问 Java 字符串。
    (*env)->ReleaseStringUTFChars(env, inputStr, (const char *) str);

    return (*env)->NewStringUTF(env, "Hello World! I am Native interface");
}

/* This function will be call when the library first be load.
 * You can do some init in the libray. return which version jni it support.
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved) 
{
    void *venv;
    LOGI("dufresne----->JNI_OnLoad!");
    
    if ((*vm)->GetEnv(vm, (void**) &venv, JNI_VERSION_1_4) != JNI_OK) 
    {
        LOGE("dufresne--->ERROR: GetEnv failed");
        return -1;
    }
    return JNI_VERSION_1_4;
}