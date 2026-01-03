# AsterUI

AsterUI 是一个基于 C++ Qt 的现代化 UI 组件库，设计风格参考 Ant Design，并融入了东方 Project 角色依神紫苑（Shion Yorigami）的配色元素（蓝紫色/白色）。

## 目标

- **现代化风格**：扁平化、简洁、高效的 Ant Design 风格。
- **高度自定义**：减少第三方依赖，尽可能自实现核心控件，提供高度可控性。
- **易用性**：提供清晰的 API 和示例，便于集成到其他 Qt 项目中。

## 构建要求

- CMake 3.16+
- Qt 6.0+ (推荐) 或 Qt 5.15+
- C++17 编译器

## 构建步骤

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## 目录结构

- `include/AsterUI`: 公共头文件
- `src`: 源代码
- `examples`: 示例程序
- `tests`: 单元测试
