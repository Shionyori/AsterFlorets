# AsterUI

AsterUI 是一个基于 C++ Qt 的现代化 UI 组件库，设计风格参考 Ant Design，并融入了东方 Project 角色依神紫苑（Shion Yorigami）的配色元素（蓝紫色/白色）。

## 目标

- **现代化风格**：扁平化、简洁、高效的 Ant Design 风格。
- **高度自定义**：减少第三方依赖，尽可能自实现核心控件，提供高度可控性。
- **易用性**：提供清晰的 API 和示例，便于集成到其他 Qt 项目中。

## 构建要求

- CMake 3.16+
- Qt 6.0+
- C++17+
- MSVC/GCC

## 构建步骤

1. 首先在根目录下创建新的CMakeUserPresets.json文件，并设置 Qt 路径，例如：

```json
{
  "version": 6,
  "configurePresets": [
    {
      "name": "windows-msvc-local",
      "inherits": "windows-msvc",
      "cacheVariables": {
        "CMAKE_PREFIX_PATH": "D:/Qt/6.8.3/msvc2022_64"
      }
    }
  ]
}
```
2. 然后使用以下命令构建项目：

```bash
cmake --preset windows-msvc-local
cmake --build --preset windows-msvc-local
```

3. 运行示例程序：

```bash
cd ./build/windows-msvc/bin/Debug
./demo.exe
```

## 目录结构
- `include/AsterUI`: 公共头文件
- `src`: 源代码
- `examples`: 示例程序
- `tests`: 测试用例（暂未创建）
