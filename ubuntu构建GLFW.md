# 1.下载glfw
[官网](https://www.glfw.org/)下载源码，通过源码编译，然后解压

# 2.安装cmake

```ubuntu
➜  ~ sudo apt-get install cmake
➜  ~ cmake -version
cmake version 3.10.2
➜  ~ sudo apt-get install libx11-dev libxrandr-dev libxi-dev libsdl2-dev
```
安装cmake，以及相关依赖库

# 3.编译glfw
```
➜  ~ mkdir glfw-build
➜  ~ cd glfw-build
➜  glfw-build cmake ../glfw-3.3
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Using X11 for window creation
-- Looking for IceConnectionNumber in ICE
-- Looking for IceConnectionNumber in ICE - found
-- Configuring done
-- Generating done
-- Build files have been written to: /home/test/glfw-build
➜  glfw-build make
................................
➜  glfw-build sudo make install
................................
Install the project...
-- Install configuration: ""
-- Installing: /usr/local/include/GLFW
-- Installing: /usr/local/include/GLFW/glfw3.h
-- Installing: /usr/local/include/GLFW/glfw3native.h
-- Installing: /usr/local/lib/cmake/glfw3/glfw3Config.cmake
-- Installing: /usr/local/lib/cmake/glfw3/glfw3ConfigVersion.cmake
-- Installing: /usr/local/lib/cmake/glfw3/glfw3Targets.cmake
-- Installing: /usr/local/lib/cmake/glfw3/glfw3Targets-noconfig.cmake
-- Installing: /usr/local/lib/pkgconfig/glfw3.pc
-- Installing: /usr/local/lib/libglfw3.a

```
1. 在glfw-3.3同级建立文件夹glfw-build，并进入。
2. 执行`cmake ../glfw-3.3`（如果有有报错缺少库，请看2中是否有安装相关库）。
3. make && sudo make install。
