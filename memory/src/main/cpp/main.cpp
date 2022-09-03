#include <jni.h>
#include <string>

#include "MemoryTools.h"
#include "JniHelper.h"

extern "C"
{ // 信息类root操作
JNIEXPORT jint JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_getPid(JNIEnv *env, jclass clazz, jstring package_name) {
    int pid = getPID(jstringToString(env, package_name));
    env->DeleteLocalRef(package_name);
    return (jint) pid;
}

JNIEXPORT jchar JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_getProcessState(JNIEnv *env, jclass clazz,
                                                          jstring package_name) {
    char state = GetProcessState(jstringToString(env, package_name));
    env->DeleteLocalRef(package_name);
    return state;
}
}

extern "C"
{ // 辅助操作性功能
JNIEXPORT void JNICALL Java_com_moshuixin_memory_ess_MemoryToolsNative_killGG(JNIEnv *env, jclass clazz) {
    // killGG();
    //在/data/data/[GG修改器包名]/files/里面有一个文件夹名字是GG-****
    //如果有这个文件夹，就获取上面所说的包名，杀掉GG修改器
    DIR *dir;
    DIR *dirGG = nullptr;
    struct dirent *ptr;
    struct dirent *ptrGG;
    char filepath[256];
    dir = opendir("/data/data"); // 打开路径
    if (dir != nullptr) {
        while ((ptr = readdir(dir)) != nullptr) // 循环读取路径下的每一个文件/文件夹
        {
            // 如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (ptr->d_type != DT_DIR)
                continue;
            sprintf(filepath, "/data/data/%s/files", ptr->d_name);
            dirGG = opendir(filepath); // 打开文件
            if (dirGG != nullptr) {
                while ((ptrGG = readdir(dirGG)) != nullptr) {
                    if ((strcmp(ptrGG->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                        continue;
                    if (ptrGG->d_type != DT_DIR)
                        continue;
                    if (strstr(ptrGG->d_name, "GG")) {
                        int pid;                   // pid
                        pid = getPID(ptr->d_name); //获取GG包名
                        // ptr->d_name存储文件名字(也就是软件包名)
                        if (pid == 0) //如果pid是0，代表GG没有运行
                            continue;
                        else //如果成功获取pid
                            killProcessByOs(ptr->d_name);
                    }
                }
            }
        }
    }
    closedir(dir);
    closedir(dirGG);
}

JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_withPassGameSafe(JNIEnv *env, jclass clazz) {
    system("echo 0 > /proc/sys/fs/inotify/max_user_watches");
}

JNIEXPORT void JNICALL Java_com_moshuixin_memory_ess_MemoryToolsNative_killXs(JNIEnv *env, jclass clazz) {
    killXs();
}
}

extern "C" JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_searchMemory(JNIEnv *env, jclass clazz,
                                                       jint pid, jstring value, jint type) {
    int gs;
    char *v = jstringToString(env, value);
    env->DeleteLocalRef(value);
    MemorySearch(pid, v, &gs, type);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_refineMemory(JNIEnv *env, jclass clazz, jint pid,
                                                       jstring value, jint type, jlong offset) {
    int gs;
    char *v = jstringToString(env, value);
    env->DeleteLocalRef(value);
    MemoryOffset(pid, v, (long) offset, &gs, type);
}
extern "C" JNIEXPORT jint JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_getResultsCount(JNIEnv *env, jclass clazz) {
    return ResCount;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_getResults(JNIEnv *env, jclass clazz, jint start,
                                                     jint end) {
    PMAPS temp = Res;
    jclass dataClz = env->FindClass("com/moshuixin/memory/data/Result");
    jmethodID dataInit = env->GetMethodID(dataClz, "<init>", "(J)V");
    jclass arraylistClz = env->FindClass("java/util/ArrayList");
    jmethodID listInit = env->GetMethodID(arraylistClz, "<init>", "()V");
    jobject list = env->NewObject(arraylistClz, listInit);
    jmethodID listAdd = env->GetMethodID(arraylistClz, "add", "(Ljava/lang/Object;)Z");
    if (ResCount != 0 && temp != nullptr) {
        for (int i = 0; i < ResCount; ++i) {
            if (i < start)
                continue;
            if (i >= end)
                break;
            jobject data = env->NewObject(dataClz, dataInit, (long long) temp->addr);
            env->CallBooleanMethod(list, listAdd, data);
            temp = temp->next;
        }
    }
    env->DeleteLocalRef(dataClz);
    env->DeleteLocalRef(arraylistClz);
    return list;
}
extern "C" JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_clearResults(JNIEnv *env, jclass clazz) {
    ClearResults();
}
extern "C" JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_setRange(JNIEnv *env, jclass clazz, jint range) {
    SetSearchRange(range);
}
extern "C" JNIEXPORT jint JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_getRange(JNIEnv *env, jclass clazz) {
    return MemorySearchRange;
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_getModuleBase(JNIEnv *env, jclass clazz, jint pid,
                                                        jstring name, jint index) {
    char *Name = jstringToString(env, name);
    env->DeleteLocalRef(name);
    return (long long) GetModuleBase(pid, Name, index);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_getMemory(JNIEnv *env, jclass clazz, jint pid,
                                                    jlong address, jint type) {
    char *Memory = MemoryRead(pid, (long) address, type);
    jstring MemoryString = stringToJstring(env, Memory);
    return MemoryString;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_setMemory(JNIEnv *env, jclass clazz, jint pid,
                                                    jstring value, jlong address, jint type) {
    char *Value = jstringToString(env, value);
    env->DeleteLocalRef(value);
    MemoryWrite(pid, Value, (long) address, type);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_addFreezeItem(JNIEnv *env, jclass clazz, jint pid,
                                                        jstring value, jlong address, jint type) {
    char *Value = jstringToString(env, value);
    env->DeleteLocalRef(value);
    AddFreezeItem(pid, (long) address, Value, type);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_clearFreezeItem(JNIEnv *env, jclass clazz,
                                                          jlong address) {
    RemoveFreezeItem((long) address);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_clearAllFreezeItem(JNIEnv *env, jclass clazz) {
    RemoveFreezeItem_All();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_startFreeze(JNIEnv *env, jclass clazz, jint pid) {
    StartFreeze(pid);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_stopFreeze(JNIEnv *env, jclass clazz) {
    StopFreeze();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_setFreezeDelay(JNIEnv *env, jclass clazz, jlong delay) {
    SetFreezeDelay((long) delay);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_searchRangeMemory(JNIEnv *env, jclass clazz, jint pid,
                                                            jstring from_value, jstring to_value,
                                                            jint type) {
    int gs;
    char *fv = jstringToString(env, from_value);
    char *tv = jstringToString(env, to_value);
    env->DeleteLocalRef(from_value);
    env->DeleteLocalRef(to_value);
    RangeMemorySearch(pid, fv, tv, &gs, type);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_moshuixin_memory_ess_MemoryToolsNative_refineRangeMemory(JNIEnv *env, jclass clazz, jint pid,
                                                            jstring from_value, jstring to_value,
                                                            jlong offset, jint type) {
    int gs;
    char *fv = jstringToString(env, from_value);
    char *tv = jstringToString(env, to_value);
    env->DeleteLocalRef(from_value);
    env->DeleteLocalRef(to_value);
    RangeMemoryOffset(pid, fv, tv, (long) offset, &gs, type);
}