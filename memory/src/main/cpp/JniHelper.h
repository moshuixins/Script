#ifndef MEMORY_JNIHELPER_H
#define MEMORY_JNIHELPER_H

char *jstringToString(JNIEnv *env, jstring jstr);

jstring stringToJstring(JNIEnv *env, const char *pat);

#endif // MEMORY_JNIHELPER_H

//void log(JNIEnv *env,char* msg){
//    jstring c= stringToJstring(env,msg);
//    jclass Dclass=env->FindClass("com/moshuixin/myapplication/MainActivity");
//    jmethodID Did = env->GetStaticMethodID(Dclass,"log", "(Ljava/lang/String;)V");
//    env->CallStaticVoidMethod(Dclass,Did,c);
//}

char *jstringToString(JNIEnv *env, jstring jstr)
{
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
        rtn = (char *)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    env->DeleteLocalRef(clsstring);
    env->DeleteLocalRef(strencode);
    return rtn;
}

jstring stringToJstring(JNIEnv *env, const char *pat)
{
    jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *)pat);
    jstring encoding = env->NewStringUTF("utf-8");
    jstring result = (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
    env->DeleteLocalRef(bytes);
    env->DeleteLocalRef(encoding);
    env->DeleteLocalRef(strClass);
    return result;
}
