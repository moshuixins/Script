package com.moshuixin.proces.servlet.reflection;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Looper;

import java.lang.reflect.Method;

@SuppressLint("PrivateApi")
public class ActivityManager {
    private static final Object lock = new Object();

    private static Object oActivityManager;
    @SuppressLint("StaticFieldLeak")
    private static Context systemContext;

    public static Context getSystemContext() {
        synchronized(lock) {
            try {
                if (ActivityManager.systemContext != null) {
                    return systemContext;
                }

                // a prepared Looper is required for the calls below to succeed
                if (Looper.getMainLooper() == null) {
                    try {
                        Looper.prepareMainLooper();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
                Class<?> cActivityThread =
                        Class.forName("android.app.ActivityThread");
                Method mSystemMain = cActivityThread.getMethod("systemMain");
                Method mGetSystemContext =
                        cActivityThread.getMethod("getSystemContext");
                Object oActivityThread = mSystemMain.invoke(null);
                systemContext = (Context) mGetSystemContext.invoke(oActivityThread);
                return systemContext;
            } catch (Exception e) {
                e.printStackTrace();
                throw new RuntimeException("unexpected exception in getSystemContext()");
            }
        }
    }

    public static Object getActivityManager() {
        // Return object is AIDL interface IActivityManager, not an ActivityManager or
        // ActivityManagerService
        synchronized(lock) {
            if (oActivityManager != null) {
                return oActivityManager;
            }
            try { // marked deprecated in Android source
                Class<?> cActivityManagerNative = Class.forName("android.app.ActivityManagerNative");
                Method mGetDefault = cActivityManagerNative.getMethod("getDefault");
                oActivityManager = mGetDefault.invoke(null);
                return oActivityManager;
            } catch (Exception e ) {
                // possibly removed
            }
            try {
                // alternative
                Class<?> cActivityManager =
                        Class.forName("android.app.ActivityManager");
                Method mGetService  = cActivityManager.getMethod("getService");
                oActivityManager = mGetService.invoke(null);
                return oActivityManager;
            } catch (Exception e) {
                e.printStackTrace();
            }
            throw new RuntimeException("unable to retrieve ActivityManager");
        }
    }
}
