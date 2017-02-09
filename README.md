# Curvefitting
首次上传
                                LEVMAR
                              version 2.5
                          By Manolis Lourakis

                     Institute of Computer Science
            Foundation for Research and Technology - Hellas
                       Heraklion, Crete, Greece
使用以上开源算法进行拟合，非常好用

点击http://www.netlib.org/clapack/CLAPACK-3.1.1-VisualStudio.zip 下载clapack

点击http://www.cmake.org/files/v2.8/cmake-2.8.0-win32-x86.exe   下载cmake

点击http://www.ics.forth.gr/~lourakis/levmar/levmar-2.5.tgz     	 下载LEVMAR

打开CLAPACK-3.1.1-VisualStudio文件夹，先把\LIB\Win32的lib都删了,以免混淆

双击clapack.vcproj打开工程项目文件，下面的各编译步骤都编译成debug模式

依次编译F2CLIBS,tmglib,blas,CLAPACK,结果是在\LIB\Win32下依次生成了libf2cd.lib,tmglibd.lib,BLASd.lib,clapackd.lib

使用cmake作用于levmar-2.5文件夹，得到工程项目文件LEVMAR.vcproj，打开它

工具->选项->项目和解决方案->vc++目录->包含文件处添加     \INCLUDE目录

工具->选项->项目和解决方案->vc++目录->库文件处添加        \LIB\Win32目录

欲在LEVMAR中调用CLAPACK，还需要打开LEVMAR的misc_core.c文件，然后在文件起始处写上#include "blaswrap.h"

然后重新生成levmar.lib


在自己的项目#include "levmar.h" 将levmar.lib放在目录下

项目属性->连接器->输入->附加依赖项levmar.lib libf2cd.lib BLASd.lib clapackd.lib tmglibd.lib

项目属性->连接器->输入->忽略特定库Libcmtd.lib


参考

http://www.cnblogs.com/random_walk/archive/2009/11/13/1602682.html

http://www.wilmott.com/messageview.cfm?catid=34&threadid=51193


欢迎大家与我讨论，有问题烦请帮忙指正，谢谢！

转帖请注明出处，尊重他人劳动成果


本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/lkbwx/archive/2010/02/19/5311802.aspx
