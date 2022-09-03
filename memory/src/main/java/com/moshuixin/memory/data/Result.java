package com.moshuixin.memory.data;

import android.os.Parcel;
import android.os.Parcelable;

public class Result implements Parcelable {
    public long addr;

    protected Result(Parcel in) {
        addr = in.readLong();
    }

    protected Result(long addr) {
        this.addr = addr;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeLong(addr);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<Result> CREATOR = new Creator<Result>() {
        @Override
        public Result createFromParcel(Parcel in) {
            return new Result(in);
        }

        @Override
        public Result[] newArray(int size) {
            return new Result[size];
        }
    };
}
