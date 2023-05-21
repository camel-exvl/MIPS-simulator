# MIPS-simulator

# 文件架构

- main.h: 包含全局类和函数的声明

- main.cpp: 程序入口

## back

后端功能实现

- assembler.h: MIPS汇编器相关功能声明

- assembler.cpp: MIPS汇编器相关功能实现

- disassembler.h: MIPS反汇编器相关功能声明

- disassembler.cpp: MIPS反汇编器相关功能实现

- systemsimulation.h: 系统模拟相关功能声明

- systemsimulation.cpp: 系统模拟相关功能实现

## front

前端功能实现

- Main.qml: 主界面

- MessagePopup.qml: 消息弹窗

## middle

中间件，连接前后端

- FileProcess.h: 文件处理相关功能声明

- FileProcess.cpp: 文件处理相关功能实现

- codetablemodel.h: 代码显示表格模型相关功能声明

- codetablemodel.cpp: 代码显示表格模型相关功能实现

- memorytablemodel.h: 内存显示表格模型相关功能声明

- memorytablemodel.cpp: 内存显示表格模型相关功能实现

- registertablemodel.h: 寄存器显示表格模型相关功能声明

- registertablemodel.cpp: 寄存器显示表格模型相关功能实现

- Debug.h: 调试相关功能声明

- Debug.cpp: 调试相关功能实现

- tools.h: 小工具相关功能声明

- tools.cpp: 小工具相关功能实现


# 打包方式	

切记使用Qt 6.5.0 (MinGW 11.2.0 64-bit)的shell打开

```
windeployqt --qmldir "QML路径" "文件路径" --no-translations
```

