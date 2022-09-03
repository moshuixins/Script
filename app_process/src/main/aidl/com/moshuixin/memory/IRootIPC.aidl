// IRootIPC.aidl
package com.moshuixin.memory;

interface IRootIPC {
    void hello(IBinder self);

    IBinder getUserIPC();

    void bye(IBinder self);
}