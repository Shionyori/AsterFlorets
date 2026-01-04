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

## 组件实现清单

### 通用 (General)
- [x] Button (按钮) - `AsterButton`
- [ ] Icon (图标)
- [ ] Typography (排版)

### 布局 (Layout)
- [x] Divider (分割线) - `AsterDivider`
- [ ] Grid (栅格)
- [ ] Layout (布局)
- [ ] Space (间距)

### 导航 (Navigation)
- [ ] Breadcrumb (面包屑)
- [ ] Dropdown (下拉菜单)
- [ ] Menu (导航菜单)
- [ ] Pagination (分页)
- [ ] Steps (步骤条)

### 数据录入 (Data Entry)
- [x] CheckBox (复选框) - `AsterCheckBox`
- [x] Input (输入框) - `AsterLineEdit`
- [x] Select (选择器) - `AsterSelect`
- [x] Slider (滑动输入条) - `AsterSlider`
- [x] Switch (开关) - `AsterSwitch`
- [x] TextEdit (文本域) - `AsterTextEdit`
- [x] Radio (单选框) - `AsterRadio`
- [ ] DatePicker (日期选择框)
- [ ] Form (表单)
- [ ] InputNumber (数字输入框)
- [ ] Upload (上传)

### 数据展示 (Data Display)
- [x] Card (卡片) - `AsterCard`
- [x] Tag (标签) - `AsterTag`
- [ ] Avatar (头像)
- [ ] Badge (徽标数)
- [ ] Calendar (日历)
- [ ] Carousel (走马灯)
- [ ] Collapse (折叠面板)
- [ ] List (列表)
- [ ] Popover (气泡卡片)
- [ ] Table (表格)
- [ ] Tabs (标签页)
- [ ] Timeline (时间轴)
- [ ] Tooltip (文字提示)
- [ ] Tree (树形控件)

### 反馈 (Feedback)
- [ ] Alert (警告提示)
- [ ] Drawer (抽屉)
- [ ] Message (全局提示)
- [ ] Modal (对话框)
- [ ] Notification (通知提醒框)
- [x] Progress (进度条) - `AsterProgress`
- [ ] Spin (加载中)

### 其他 (Other)
- [x] Theme (主题管理) - `AsterTheme`