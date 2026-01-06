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

### 0. Core Systems (核心架构) [Infrastructure]
库的底层支撑，决定整体的性能、交互手感和开发体验。
- [x] **Theme** (主题系统) - `AsterTheme` (需升级为 Token 化设计，支持动态切换)
- [ ] **Motion** (动画引擎) - `AsterMotion` (统一管理 Hover/Press/Slide/Fade 动画，避免割裂感)
- [ ] **WindowManger** (窗口管理) - `AsterWindowManager` (处理无边框、阴影、拖拽、Aero Snap、DPI适配)
- [ ] **ModelAdapter** (数据适配器) - `AsterModelAdapter` (简化 Table/Tree 的 Model 开发，桥接普通数据结构)

### 1. Application (应用框架) [Desktop Special]
桌面应用的基石，提供统一的窗口外观和交互。
- [ ] **Application** (应用辅助) - `AsterApp` (全局初始化、字体管理、事件过滤器)
- [ ] **Window** (主窗口) - `AsterWindow` (基于 WindowManager 实现，支持 QWidget/QWindow 双模式)
- [ ] **TitleBar** (标题栏) - `AsterTitleBar` (集成系统按钮，支持自定义内容)

### 2. General (通用)
最基础的 UI 元素。
- [x] **Button** (按钮) - `AsterButton`
- [x] **Icon** (图标) - `AsterIcon` (封装 SVG/FontAwesome)
- [x] **Typography** (排版) - `AsterText` / `AsterTitle`
- [x] **ScrollArea** (滚动区域) - `AsterScrollArea` (重点优化：平滑滚动、边缘光晕、自动隐藏、触控板惯性)

### 3. Layout (布局)
组织页面结构。
- [x] **Divider** (分割线) - `AsterDivider`
- [x] **Space** (间距) - `AsterSpace` (用于自动处理控件间距)
- [x] **Grid** (栅格系统) - `AsterGrid` (Row/Col，支持 Gutter 和响应式布局，统一间距规范)
- [x] **FlowLayout** (流式布局) - `AsterFlowLayout` (自动换行，用于 Tag 组或卡片列表)

### 4. Navigation (导航)
页面跳转与层级指引。
- [ ] **Menu** (菜单) - `AsterMenu` (重绘 QMenu，用于下拉和右键菜单)
- [ ] **Tabs** (标签页) - `AsterTabs` (高性能重绘，支持动画开关)
- [ ] **Breadcrumb** (面包屑) - `AsterBreadcrumb` (桌面端深层级导航必备)
- [ ] **Pagination** (分页) - `AsterPagination` (表格/列表数据分页)
- [ ] **SideBar** (侧边导航) - `AsterSideBar`
- [ ] **Drawer** (抽屉) - `AsterDrawer` (侧边滑出面板)

### 5. Data Entry (数据录入)
用户输入数据的控件。
- [x] **Input** (输入框) - `AsterTextInput` (原 `AsterLineEdit`)
- [x] **TextArea** (多行文本) - `AsterTextArea` (原 `AsterTextEdit`)
- [x] **Select** (选择器) - `AsterSelect`
- [x] **CheckBox** (复选框) - `AsterCheckBox`
- [x] **Radio** (单选框) - `AsterRadio`
- [x] **Switch** (开关) - `AsterSwitch`
- [x] **Slider** (滑动条) - `AsterSlider`
- [ ] **NumberInput** (数字输入) - `AsterNumberInput` (原 `AsterSpinBox`)
- [ ] **DatePicker** (日期选择) - `AsterDatePicker`
- [ ] **TimePicker** (时间选择) - `AsterTimePicker`
- [ ] **Upload** (文件上传) - `AsterUpload` (文件拖拽区域 / 增强型文件选择器)
- [ ] **ColorPicker** (颜色选择器) - `AsterColorPicker` (桌面端常用高级组件)
- [ ] **AutoComplete** (自动补全) - `AsterAutoComplete` (带样式的自动补全)

### 6. Data Display (数据展示)
展示结构化或非结构化数据。
- [x] **Avatar** (头像) - `AsterAvatar`
- [x] **Card** (卡片) - `AsterCard`
- [x] **Tag** (标签) - `AsterTag`
- [ ] **List** (列表) - `AsterList` 
- [ ] **Table** (表格) - `AsterTable` (基于 ModelAdapter)
- [ ] **Tree** (树形控件) - `AsterTree`
- [ ] **Tooltip** (文字提示) - `AsterTooltip`
- [ ] **Popover** (气泡卡片) - `AsterPopover` (可交互的复杂浮层)
- [ ] **Badge** (徽标数) - `AsterBadge`
- [ ] **Collapse** (折叠面板) - `AsterCollapse` (设置页/侧边栏常用)
- [ ] **Descriptions** (描述列表) - `AsterDescriptions` (结构化信息展示)
- [ ] **Image** (图片) - `AsterImage`
- [ ] **Statistic** (统计数值) - `AsterStatistic`

### 7. Feedback (反馈)
用户操作后的反馈交互。
- [x] **Progress** (进度条) - `AsterProgress`
- [ ] **Message** (全局提示) - `AsterMessage` (Toast 风格，自动消失)
- [ ] **Dialog** (对话框) - `AsterDialog` (模态确认框)
- [ ] **Notification** (通知提醒框) - `AsterNotification` (右下角弹出)
- [ ] **Alert** (警告提示) - `AsterAlert` (非模态的静态提示条)
- [ ] **Spin** (加载中) - `AsterSpin`
- [ ] **Skeleton** (骨架屏) - `AsterSkeleton`