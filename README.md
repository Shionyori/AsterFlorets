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

1. 首先在根目录下创建新的 CMakeUserPresets.json 文件，并设置 Qt 路径，例如：

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

- **组件库演示**:
  ```bash
  cd ./build/windows-msvc/bin/Debug
  ./demo.exe
  ```
- **布局演示**:
  ```bash
  cd ./build/windows-msvc/bin/Debug
  ./layout_demo.exe
  ```

## 目录结构
- `include/AsterUI`: 公共头文件
- `src`: 源代码
- `examples`: 示例程序
- `tests`: 测试用例（暂未创建）

## 组件实现清单

### 1. Layout Abstract (布局抽象)
常用布局模式的封装，与 Qt 原生布局管理器配合使用。
这部分并不是为了取代 Qt 布局系统，而是提供更高层次的布局抽象，简化常见布局场景的实现。
- [x] **Space** (间距容器) - `AsterSpace` (自动处理子控件间距，支持横向/纵向堆叠)
- [x] **Row** (行容器) - `AsterRow` (栅格系统行，管理 Gutter 和列的排列)
- [x] **Column** (列容器) - `AsterColumn` (栅格系统列，支持 span 跨度设置，作为 Row 的子容器)
- [x] **EqualGrid** (等分网格容器) - `AsterGrid` (等分网格，统一管理子元素间距和排列)
- [x] **Flow** (流式容器) - `AsterFlow` (自动换行，适用于标签组或不定宽卡片列表)
- [x] **ScrollArea** (滚动容器) - `AsterScrollArea` (支持平滑滚动的容器封装)
- [x] **Divider** (分割线) - `AsterDivider`
- [x] **Layout** (布局容器) - `AsterLayout` (页面级布局容器，包含 Header, Sider, Content, Footer)

### 2. General (通用)
最基础的 UI 元素。
- [x] **Button** (按钮) - `AsterButton` (支持 Primary, Dashed, Text, Link 等多种类型)
- [x] **Icon** (图标) - `AsterIcon` (图标封装，支持颜色和大小调整)
- [x] **Typography** (排版) - `AsterTitle` / `AsterText` (预设样式的文本组件)

### 3. Data Entry (数据录入)
用户输入数据的控件。
- [x] **TextInput** (输入框) - `AsterTextInput` (支持清除按钮、前后缀插槽)
- [x] **TextArea** (多行文本) - `AsterTextArea`
- [x] **Select** (选择器) - `AsterSelect`
- [x] **CheckBox** (复选框) - `AsterCheckBox`
- [x] **Radio** (单选框) - `AsterRadio`
- [x] **Switch** (开关) - `AsterSwitch`
- [x] **Slider** (滑动条) - `AsterSlider`
- [ ] **NumberInput** (数字输入) - `AsterNumberInput`
- [ ] **DatePicker** (日期选择) - `AsterDatePicker`
- [ ] **TimePicker** (时间选择) - `AsterTimePicker`
- [ ] **Upload** (文件上传) - `AsterUpload`
- [ ] **Form** (表单) - `AsterForm` 
- [ ] **Rate** (评分) - `AsterRate` 
- [ ] **ColorPicker** (颜色选择器) - `AsterColorPicker`
- [ ] **Cascader** (级联选择) - `AsterCascader`
- [ ] **TreeSelect** (树形选择) - `AsterTreeSelect`
- [ ] **Transfer** (穿梭框) - `AsterTransfer`

### 4. Data Display (数据展示)
用于展示数据的可视化组件。
- [x] **Avatar** (头像) - `AsterAvatar` (支持图片、字符、图标，圆形/方形)
- [ ] **Badge** (徽标数) - `AsterBadge`
- [x] **Card** (卡片) - `AsterCard` (包含标题栏和内容区，支持阴影和边框优化)
- [ ] **Collapse** (折叠面板) - `AsterCollapse`
- [ ] **Descriptions** (描述列表) - `AsterDescriptions`
- [x] **Tag** (标签) - `AsterTag`
- [x] **Progress** (进度条) - `AsterProgress`
- [ ] **List** (列表) - `AsterList` 
- [ ] **Table** (表格) - `AsterTable`
- [ ] **Tree** (树形控件) - `AsterTree`
- [ ] **Tooltip** (文字提示) - `AsterTooltip`
- [ ] **Popover** (气泡卡片) - `AsterPopover`
- [ ] **Image** (图片) - `AsterImage`
- [ ] **Statistic** (统计数值) - `AsterStatistic`
- [ ] **Calendar** (日历) - `AsterCalendar`
- [ ] **Carousel** (走马灯) - `AsterCarousel`
- [ ] **Timeline** (时间轴) - `AsterTimeline`

### 5. Navigation (导航)
页面跳转与层级指引。
- [ ] **Anchor** (锚点) - `AsterAnchor`
- [ ] **Breadcrumb** (面包屑) - `AsterBreadcrumb`
- [ ] **Dropdown** (下拉菜单) - `AsterDropdown`
- [ ] **Menu** (菜单) - `AsterMenu`
- [ ] **Pagination** (分页) - `AsterPagination`
- [ ] **Steps** (步骤条) - `AsterSteps`
- [ ] **Tabs** (标签页) - `AsterTabs`

### 6. Feedback (反馈)
用户操作后的反馈交互。
- [ ] **Alert** (警告提示) - `AsterAlert` (信息提示条)
- [ ] **Drawer** (抽屉) - `AsterDrawer` (侧边滑出层)
- [x] **Message** (全局提示) - `AsterMessage` (Toast 风格，自动消失)
- [x] **Modal** (模态对话框) - `AsterModal` (中心弹出，可自定义内容和按钮) 原 `AsterDialog`
- [x] **Notification** (通知提醒框) - `AsterNotification` (右下角弹出)
- [ ] **popconfirm** (气泡确认框) - `AsterPopconfirm` (带确认操作的气泡提示)
- [ ] **Result** (结果页) - `AsterResult`
- [ ] **Skeleton** (骨架屏) - `AsterSkeleton`
- [ ] **Spin** (加载中) - `AsterSpin`

### 7. Other (其他)
- [x] **Theme** (主题管理) - `AsterTheme` (集中管理全局样式变量，如颜色、字体、间距等)