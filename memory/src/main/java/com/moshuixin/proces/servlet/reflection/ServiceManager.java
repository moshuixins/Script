package com.moshuixin.proces.servlet.reflection;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

@SuppressLint({"DiscouragedPrivateApi", "PrivateApi", "SoonBlockedPrivateApi"})
public class ServiceManager { // 废弃
    // 每个进程的ServiceManager独享，ServiceManager由系统进程管理
    private static final String TAG = ServiceManager.class.getName();
    private static final ServiceManager manager = new ServiceManager();
    private static boolean isInit = false;
    private static Method MethodGetService;
    private static Method MethodAddServiceV1;
    private static Method MethodAddServiceV2;
    private static Method MethodAddServiceV3;

    private ServiceManager() {
    }

    public static ServiceManager getInstance() {
        return manager;
    }

    public static IBinder getService(String name) {
        getInstance().init();
        try {
            return (IBinder) MethodGetService.invoke(null, name);
        } catch (IllegalAccessException | InvocationTargetException e) {
            Log.e(TAG, "getService: ", e);
        }
        return null;
    }

    public static void addService(String name, IBinder service) {
        getInstance().init();
        try {
            MethodAddServiceV1.invoke(null, name, service);
        } catch (IllegalAccessException | InvocationTargetException e) {
            Log.e(TAG, "getService: ", e);
        }
    }

    public static void addService(String name, IBinder service, boolean allowIsolated) {
        getInstance().init();
        try {
            MethodAddServiceV2.invoke(null, name, service, allowIsolated);
        } catch (IllegalAccessException | InvocationTargetException e) {
            Log.e(TAG, "addService: ", e);
        }
    }

    public static void addService(String name, IBinder service, boolean allowIsolated, int dumpPriority) {
        getInstance().init();
        try {
            MethodAddServiceV3.invoke(null, name, service, allowIsolated, dumpPriority);
        } catch (IllegalAccessException | InvocationTargetException e) {
            Log.e(TAG, "addService: ", e);
        }
    }

    private void init() {
        if (!isInit) {
            try {
                Class<?> clazz = Class.forName("android.os.ServiceManager");

                MethodGetService = clazz.getMethod("getService", String.class);
                MethodAddServiceV1 = clazz.getMethod("addService", String.class, IBinder.class);
                MethodAddServiceV2 = clazz.getMethod("addService", String.class, IBinder.class, java.lang.Boolean.TYPE);
                MethodAddServiceV3 = clazz.getMethod("addService", String.class, IBinder.class, java.lang.Boolean.TYPE, Integer.TYPE);

                isInit = true;
            } catch (ClassNotFoundException | NoSuchMethodException e) {
                Log.e(TAG, "init: ", e);
            }
        }
    }

    private static final Object lock = new Object();

    private static Method mBroadcastIntent;
    private static Integer FLAG_RECEIVER_FROM_SHELL = null;

    public static void sendBroadcast(Intent intent) {
        try {
            // Prevent system from complaining about unprotected broadcast, if the field exists
            intent.setFlags(getInstance().getFlagReceiverFromShell());
            Object oActivityManager = ActivityManager.getActivityManager();
            Method mBroadcastIntent = getInstance().getBroadcastIntent(oActivityManager.getClass());
            if (mBroadcastIntent.getParameterTypes().length == 13) {
                // API 24+
                mBroadcastIntent.invoke(oActivityManager, null, intent, null, null, 0, null, null, null, -1, null, false, false, 0);
                return;
            }
            if (mBroadcastIntent.getParameterTypes().length == 12) {
                // API 21+
                mBroadcastIntent.invoke(oActivityManager, null, intent, null, null, 0, null, null, null, -1, false, false, 0);
                return;
            }
        } catch (Exception e) {
            Log.e(TAG, "sendBroadcast: ", e);
            return;
        }
        throw new RuntimeException("unable to send broadcast");
    }

    private int getFlagReceiverFromShell() {
        synchronized (lock) {
            if (FLAG_RECEIVER_FROM_SHELL != null) {
                return FLAG_RECEIVER_FROM_SHELL;
            }
            try {
                Field fFlagReceiverFromShell = Intent.class.getDeclaredField("FLAG_RECEIVER_FROM_SHELL");
                FLAG_RECEIVER_FROM_SHELL = fFlagReceiverFromShell.getInt(null);
                return FLAG_RECEIVER_FROM_SHELL;
            } catch (NoSuchFieldException | IllegalAccessException e) {
                // not present on all Android versions
                Log.e(TAG, "getFlagReceiverFromShell: ", e);
            }
            FLAG_RECEIVER_FROM_SHELL = 0;
            return FLAG_RECEIVER_FROM_SHELL;
        }
    }

    private Method getBroadcastIntent(Class<?> cActivityManager) {
        synchronized (lock) {
            if (mBroadcastIntent != null) {
                return mBroadcastIntent;
            }
            for (Method m : cActivityManager.getMethods()) {
                if ("broadcastIntent".equals(m.getName()) && m.getParameterTypes().length == 13) {
                    // API 24+
                    mBroadcastIntent = m;
                    return mBroadcastIntent;
                }
                if ("broadcastIntent".equals(m.getName()) && m.getParameterTypes().length == 12) {
                    // API 21+
                    mBroadcastIntent = m;
                    return mBroadcastIntent;
                }
            }
            throw new RuntimeException("unable to retrieve broadcastIntent method");
        }
    }
}
