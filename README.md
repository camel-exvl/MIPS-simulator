# MIPS-simulator

# 文件架构

- main.h: 包含全局类和函数的声明

- main.cpp: 主程序

- assembler.h: MIPS汇编器相关功能声明

- assembler.cpp: MIPS汇编器相关功能实现

- disassembler.h: MIPS反汇编器相关功能声明

- disassembler.cpp: MIPS反汇编器相关功能实现

打包方式	切记使用Qt 6.5.0 (MinGW 11.2.0 64-bit)的shell打开

```
windeployqt --qmldir "QML路径" "文件路径" --no-translations
```

