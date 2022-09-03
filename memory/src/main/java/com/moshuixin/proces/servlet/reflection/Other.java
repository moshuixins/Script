package com.moshuixin.proces.servlet.reflection;

import android.annotation.SuppressLint;
import android.os.Process;

import java.lang.reflect.Method;

public class Other {
    @SuppressLint("SoonBlockedPrivateApi")
    public static boolean isVmDebugEnabled() {
        if (Process.myUid() >= 10000) {
            return false;
        } else {
            try {
                Class<?> cVMDebug = Class.forName("dalvik.system.VMDebug");
                Method mIsDebuggingEnabled = cVMDebug.getDeclaredMethod("isDebuggingEnabled");
                Object inv = mIsDebuggingEnabled.invoke(null);
                if (inv != null) {
                    return (boolean) inv;
                }
                return false;
            } catch (Exception e) {
                e.printStackTrace();
                return false;
            }
        }
    }
}
