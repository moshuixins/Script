package com.moshuixin.util;

import android.content.Context;
import android.content.SharedPreferences;


public class storages {

    private static final String NAME_PREFIX = "moshuixin.storage.";
    private SharedPreferences mSharedPreferences;

    public storages(Context context, String name) {
        mSharedPreferences = context.getSharedPreferences(NAME_PREFIX + name, Context.MODE_PRIVATE);
    }

    public storages put(String key, String value) {
        mSharedPreferences.edit()
                .putString(key, value)
                .apply();
        return this;
    }

    public storages put(String key, long value) {
        mSharedPreferences.edit()
                .putLong(key, value)
                .apply();
        return this;
    }

    public storages put(String key, boolean value) {
        mSharedPreferences.edit()
                .putBoolean(key, value)
                .apply();
        return this;
    }

    public long getNumber(String key, long defaultValue) {
        return mSharedPreferences.getLong(key, defaultValue);
    }

    public boolean getBoolean(String key, boolean defaultValue) {
        return mSharedPreferences.getBoolean(key, defaultValue);
    }

    public String getString(String key, String defaultValue) {
        return mSharedPreferences.getString(key, defaultValue);
    }

    public long getNumber(String key) {
        return getNumber(key, 0);
    }

    public boolean getBoolean(String key) {
        return getBoolean(key, false);
    }

    public String getString(String key) {
        return getString(key, null);
    }

    public void remove(String key) {
        mSharedPreferences.edit().remove(key).apply();
    }

    public boolean contains(String key) {
        return mSharedPreferences.contains(key);
    }

    public void clear() {
        mSharedPreferences.edit().clear().apply();
    }
}

