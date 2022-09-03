package com.moshuixin.memory.tools;

/*
https://man7.org/linux/man-pages/man5/proc.5.html
 */
public enum ProcessState {
    DORMANT, // D 无法中断的休眠状态（通常 IO 的进程）；
    RUNNING, // R 正在运行，在可中断队列中；
    SLEEP, // S 处于休眠状态，静止状态；
    TRACK, // T 停止或被追踪，暂停执行；
    WAIT, // W 进入内存交换（从内核2.6开始无效）；
    DEAD, // X 死掉的进程；
    ZOMBIE, // Z 僵尸进程不存在但暂时无法消除；
    WAKING, // W: 活跃的
    NOBLE,  // <: 高优先级进程
    LOW, // N: 低优先序进程
    LOCK, // L: 有记忆体分页分配并锁在记忆体内 (即时系统或捱A I/O)，即,有些页被锁进内存
    LEADER, // s 进程的领导者（在它之下有子进程）；
    MULTI, // l 多进程的（使用 CLONE_THREAD, 类似 NPTL pthreads）；
    BACKSTAGE // + 位于后台的进程组；
}