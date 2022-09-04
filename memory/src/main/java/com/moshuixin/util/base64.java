package com.moshuixin.util;

import android.util.Base64;

public class base64 {

    public static String encode(String str) {
        return Base64.encodeToString(str.getBytes(), Base64.DEFAULT);
    }

    public static String decode(String str) {
        return new String(Base64.decode(str, Base64.DEFAULT));
    }

    public static String encode(byte[] bytes) {
        return Base64.encodeToString(bytes, Base64.DEFAULT);
    }

    public static String decode(byte[] bytes) {
        return new String(Base64.decode(bytes, Base64.DEFAULT));
    }

}
