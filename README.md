## Overview
Implementation of some basic Computer Graphics algorithm by OpenGL.

[Reflective Shadow map](http://www.klayge.org/material/3_12/GI/rsm.pdf):
<img src="https://github.com/ArthurHsu142857/ComputerGraphicsAlgo/assets/43297470/1206033c-f277-4447-95fc-e77e65588875" alt="GI" width="400"/>
<img src="https://github.com/ArthurHsu142857/ComputerGraphicsAlgo/assets/43297470/0ab8c9e3-0ca9-4944-8d37-b2cb96c5fa36" alt="FinalColor" width="400"/>

## Build
Use [vcpkg](https://github.com/microsoft/vcpkg) to get we need.

``` sh
> vcpkg install glfw:x64-windows
> vcpkg install glad:x64-windows
> vcpkg install assimp:x64-windows
> vcpkg install glm:x64-windows
> vcpkg install stb
```
## Note
Replace your vcpkg path in CMakeLists.txt

Remember to add vcpkg ./bin to your environment variable
