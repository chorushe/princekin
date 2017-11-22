## 概述
#### 小王子是针对Android手机研发的一套自动化测试工具，可对用户的点击操作进行录制并回放，同时，还可对手机进行基于Monkey原理的压力测试，针对某一款app进行控件遍历测试，埋点统计，接口测试等，此外，还可以对手机的CPU、内存、电池温度、CPU温度、流量使用情况进行检测，返回包括日志和截图的详细测试报告，有效提高了测试效率，缩减测试周期。

## 环境安装及环境变量配置
### 相关环境安装： 
- ##3# JAVA（务必1.7版本，否则会出错）
- #### ANT
- #### Android-sdk
- #### Python及mitmproxy（Python要求3以上版本，尽量使用3.6版本，其他版本在后续安装mitmproxy时会很麻烦）
- #### plink

### 环境变量配置：
- #### JAVA: JAVA_HOME=D:\program\oracle\JAVA7\JDK ； JRE_HOME=D:\program\oracle\JAVA7\JRE
- #### ANT: ANT_HOME=D:\program\ant
- #### ANDROID：ANDROID_HOME=D:\program\android
- #### Python: PYTHON_HOME= D:\Program Files\Python3.6
- #### WinRAR:WINRAR_HOME=C:\Program Files (x86)\WinRAR
- #### Path：
    #### %ANDROID_HOME%\platform-tools;
    #### %ANDROID_HOME%\tools;
    #### %ANDROID_HOME%\build-tools\23.0.2;
    #### %ANDROID_HOME%\platforms\android-23;
    #### %ANT_HOME%\bin;
    #### %JAVA_HOME%\bin;
    #### %JAVA_HOME%\jre\bin;
    #### %JRE_HOME%\bin;
    #### %PYTHON_HOME%;
    #### %WINRAR_HOME%;

### mitmprox安装

#### Mitmproxy是用于抓包的、基于python安装第三方工具，即可抓取http协议数据，也可抓取https协议数据。

#### 在配置好python的环境变量的前提下，打开cmd窗口，输入命令：
#### python –m pip install mitmproxy

## 模块
- #### 脚本录制
- #### 脚本执行
- #### 性能测试
- #### 稳定性测试
- #### 埋点统计
- #### 遍历测试
- #### 接口测试
- #### 工具箱

#### *详细使用文档可在安装后的主界面中“帮助”中查看*

## 源代码打开方式
- #### Windows系统
- #### QT Creater

## 界面效果图
![image](https://github.com/chorushe/princekin/blob/master/image-folder/princekin.png)
