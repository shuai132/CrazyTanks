# CrazyTanks

坦克小游戏  
Powered by [Cocos2d-x](https://github.com/cocos2d/cocos2d-x)

声明：禁止用于商业用途。部分素材来源于网络，如有侵权请联系我删除。

## Build

为了节省空间，采用外部引用cocos，请将cocos2d-x引擎下载到工程同级目录。

1. PC端
使用CMake:
```bash
mkdir build && cd build && cmake .. && make
```
2. Android/iOS见各自工程

## Issues

1. CLion构建问题  
`error: Objective-C was disabled in PCH file but is currently enabled`  
请把构建工具更改为使用本地cmake(Build --> Toolchains --> CMake)。
