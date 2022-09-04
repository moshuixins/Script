# MemoryTools



MemoryTools模块提供了一些安卓应用内存的读写的函数，可通过以下方式引入：

```java
import com.moshuixin.memory.tools.*;
```



## MemoryTools.getInstance()

* 返回 {MemoryTools}

返回MemoryTools类对象，用于使用MemoryTools类的其他函数。



## MemoryTools.getInstance().init(context, isRoot)

* `context` {android.content.Context} 窗口环境
* `isRoot` {boolean} 是否root

* 返回 {boolean} 

初始化MemoryTools，在root环境下连接ipc线程，在非root环境下直接载入libmemory.so，并返回是否初始化成功。



## MemoryTools.getInstance().getProcessPid(packageName)

* `packageName` {String} 包名
* 返回 {int} pid

如果包名对应的应用正在运行，则返回应用pid，否则返回0。



## MemoryTools.getInstance().selectAppByPid(pid)

* `pid` {int} 

通过应用pid选择要调试的应用。



## MemoryTools.getInstance().selectAppByPackage(packageName)

* `packageName` {String} 包名

通过应用包名选择要调试的应用。



## MemoryTools.getInstance().getSelectApp()

* 返回 {int} pid

返回已选择应用的pid。



## MemoryTools.getInstance().setRange(range)

* `range` {int} 内存范围

设置搜索的内存范围。



## MemoryTools.getInstance().getRange()

* 返回 {int} 内存范围

返回设置的内存范围。



## MemoryTools.getInstance().getModuleBase(name, index)

* `name` {String} 模块名
* `index` {int} 内存页索引
* 返回 {long} 地址

返回模块的头地址。



## MemoryTools.getInstance().searchMemory(value, type)

* `value` {String} 值
* `type` {int} 数据类型

在内存中搜索指定类型的值，并将搜索到的地址存储到搜索列表。



## MemoryTools.getInstance().refineMemory(value, type, offset)

* `value` {String} 值
* `type` {int} 数据类型
* `offset` {long} 偏移

改善搜索列表的地址，并重新存储。



## MemoryTools.getInstance().getResultsCount()

* 返回 {int} 长度

返回搜索列表的长度。



## MemoryTools.getInstance().getResults(start, end)

* `start` {int} 头索引
* `end` {int} 尾索引
* 返回 {List<Result>} 搜索列表

返回指定头尾的搜索列表。



## MemoryTools.getInstance().clearResults()

清空搜索列表。



## MemoryTools.getInstance().getMemory(address, type)

* `address` {long} 地址
* `type` {int} 数据类型

* 返回 {Object} 值

返回指定地址的指定类型的值。



## MemoryTools.getInstance().setMemory(value, address, type)

* `value` {String} 值
* `address` {long} 地址
* `type` {int} 数据类型

修改指定地址的指定类型的值。



## MemoryTools.getInstance().startFreeze()

开始冻结。



## MemoryTools.getInstance().stopFreeze()

停止冻结。



## MemoryTools.getInstance().setFreezeDelay(delay)

* `delay` {long} 间隔

设置冻结间隔。



## MemoryTools.getInstance().addFreezeItem(value, address, type)

* `value` {String} 值
* `address` {long} 地址
* `type` {int} 数据类型

将指定地址的指定类型的值加入冻结列表。



## MemoryTools.getInstance().clearFreezeItem(address)

* `address` {long} 地址

将指定地址从冻结列表中移除。



## MemoryTools.getInstance().clearAllFreezeItem()

清空冻结列表。



## MemoryTools.getInstance().killGG()

杀死GG进程。



## MemoryTools.getInstance().killXs()

杀死XScript进程。



## MemoryTools.getInstance().byPassGameSafe()

通过部分游戏检测。



