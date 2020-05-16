# CrazyTanks

![icon](./proj.android/app/res/mipmap-xxxhdpi/ic_launcher.png)

坦克小游戏  
Powered by [Cocos2d-x](https://github.com/cocos2d/cocos2d-x)

声明：禁止用于商业用途。部分素材来源于网络，如有侵权请联系我删除。

### Clone

因包含submodule，克隆仓库请添加`--recursive`参数，或clone后执行：
```bash
git submodule update --init --recursive
```

## Build

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
