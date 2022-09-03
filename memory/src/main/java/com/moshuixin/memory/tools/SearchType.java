package com.moshuixin.memory.tools;

public enum SearchType {
    DWORD(0),
    FLOAT(1),
    DOUBLE(2),
    WORD(3),
    BYTE(4),
    QWORD(5);



    private final int i;

    SearchType(int i) {
        this.i = i;
    }

    public int getI() {
        return i;
    }
}
