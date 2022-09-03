# 快速开始

将Script库引入你的项目可以通过以下两种方式：

### 一、使用jitpack引入

Step 1. 添加JitPack仓库到根目录build.gradle文件

```java
allprojects {
	repositories {
		...
		maven { url 'https://jitpack.io' }
	}
}
```

Step 2.
在module添加依赖，注意version版本[![](https://jitpack.io/v/moshuixins/Script.svg)](https://jitpack.io/#moshuixins/Script)

```java
dependencies {
        implementation 'com.github.moshuixins:Script:2.18'
}
```

### 二、**直接使用aar包引入**

Step 1. [点击这里下载aar包](https://cdn.jsdelivr.net/gh/moshuixins/Script@master/aar/memory-release.aar)

Step 2. 将aar放入app-libs目录下，若无自己新建

![](https://img-blog.csdnimg.cn/20210516171606889.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3ODMzNDEz,size_16,color_FFFFFF,t_70)

Step 3. 配置build.gradle文件，添加如下：

```java
repositories {
    flatDir {
         dirs'libs'
  	}
}

```

![](https://img-blog.csdnimg.cn/20210516171724457.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3ODMzNDEz,size_16,color_FFFFFF,t_70)

Step 4. 引入aar包
`implementation (name: ‘memory-release’, ext: ‘aar’)`

![](https://img-blog.csdnimg.cn/20210516172231735.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3ODMzNDEz,size_16,color_FFFFFF,t_70)

