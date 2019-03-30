# Change-To-Icon
基于QT C++的跨平台GUI程序，旨在免去复杂的图标制作过程，快速创建大小统一、背景透明的图标文件

## 开发环境：
开发语言：C++  
开发环境：QT 5.11.1  

## 适用平台：
适用平台：`Windows、Linux、Mac Os X`（相应的编译环境编译得到二进制程序即可）  
注：对windows xp的支持需要`QT版本小于QT 5.6.3`

## 功能说明： 
* 支持多输入多输出

* 自定义输出大小
  * 16*16
  * 32*32
  * 48*48
  * 64*64
  * 72*72
  * 96*961
  * 28*128

* 多种输入格式
  * png
  * jpg
  * jpeg
  * bmp
  * ico
  * 注：可随意混合后同时输入

* 特殊处理
  * 自动背景透明化 （适用于纯色背景图片)
  * 自动剪裁，确保图像内容在正方形正中央（上下左右边距均为2px）

* 智能存储路径

## 二进制构建步骤：
#### 1、下载源代码文件夹：
```Bash
wget https://codeload.github.com/oneflyingfish/Change-To-Icon/zip/master #可通过浏览器下载
unzip Change-To-Icon-master.zip                                          #可用其它方式解压
cd ...\Change-To-Icon-master
```

#### 2、构建Makefile：
``` Bash
qmake       #需要QT环境支持
```

#### 3、编译release版本：
```Bash
make        #Windows环境make可用nmake替代
```

#### 4、编译debug版本：
```Bash
make debug  #Windows环境make可用nmake替代
```

#### 5、编译debug+release版本：
```Bash
make all    #Windows环境make可用nmake替代
```
#### 6、添加资源文件
将Change-To-Icon-master\文件夹下的"Icon文件夹"拷贝到二进制文件所在目录即可

`注：`程序提供了releases版本下载，可前往[下载链接](https://github.com/oneflyingfish/Change-To-Icon/releases)
