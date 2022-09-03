package com.moshuixin.memory.tools;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.os.Build;

import java.io.File;
import java.util.Locale;

import dalvik.system.BaseDexClassLoader;

public class MemoryExt {
    @SuppressLint("SdCardPath")
    public static final String getLibraryPath(Context context, String libName) {
        String name = libName;
        if (Build.VERSION.SDK_INT >= 23) {
            if ((context.getApplicationInfo().flags & ApplicationInfo.FLAG_EXTRACT_NATIVE_LIBS) == 0) {
                throw new RuntimeException("incompatible with extractNativeLibs=\"false\" in your manifest");
            }
        }
        if (name.toLowerCase(Locale.getDefault()).startsWith("lib")) {
            name = name.substring(3);
        }
        if (name.toLowerCase(Locale.getDefault()).endsWith(".so")) {
            name = name.substring(0, name.length() - 3);
        }
        String packageName = context.getPackageName();

        // try nativeLibraryDir
        ApplicationInfo appInfo = context.getApplicationInfo();
        for (String candidate : new String[]{
                appInfo.nativeLibraryDir + File.separator.toString() + "lib" + name + ".so",
                appInfo.nativeLibraryDir + File.separator.toString() + name + ".so"}) {
            if (new File(candidate).exists()) {
                return candidate;
            }
        }

        // try BaseDexClassLoader
        if (context.getClassLoader() instanceof BaseDexClassLoader) {
            try {
                BaseDexClassLoader bdcl = (BaseDexClassLoader) context.getClassLoader();
                return bdcl.findLibrary(name);
            } catch (Throwable ignored) {
                // not a standard call: catch Errors and Violations too aside from Exceptions
            }
        }

        // try (old) default location
        for (String candidate : new String[]{
                java.lang.String.format(Locale.ENGLISH, "/data/data/%s/lib/lib%s.so", packageName, name),
                java.lang.String.format(Locale.ENGLISH, "/data/data/%s/lib/%s.so", packageName, name)}) {
            if (new File(candidate).exists()) {
                return candidate;
            }
        }
        return "";
    }

    public static final class NotFoundPidException extends RuntimeException {
        public NotFoundPidException() {
            super("no corresponding application found");
        }
    } // 应用不在运行状态或者未安装

    public static final class NoSearchResults extends RuntimeException {
        public NoSearchResults() {
            super("unknown error");
        }
    }
    public static final class UnknownException extends RuntimeException {
        public UnknownException() {
            super("unknown error");
        }
    }

}
