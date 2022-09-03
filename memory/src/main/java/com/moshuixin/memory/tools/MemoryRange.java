package com.moshuixin.memory.tools;

public enum MemoryRange {
    ALL(0),  //所有内存
    B_BAD(1),  //B内存
    C_ALLOC(2),  //Ca内存
    C_BSS(3),  //Cb内存
    CODE_APP(4),  //XA内存
    C_DATA(5),  //CD内存
    C_HEAP(6),  //Ch内存
    JAVA_HEAP(7),  //Jh内存
    A_ANONMYOUS(8),  //A内存
    CODE_SYSTEM(9),  //Xs内存
    STACK(10),  //S内存
    ASHMEM(11); //As内存

    private final int i;

    MemoryRange(int i) {
        this.i = i;
    }

    public int getI() {
        return i;
    }
}
