# 环境
机箱：华清远见嵌入式ARM实验箱Ⅲ。
芯片：ARM Cortex-M4微处理器。
环境：Keil uVision5 on Windows 10。
项目：基于3_DC_Motor项目文件。

# 功能

## 基本功能
主功能为根据存储的乐谱播放音乐，其它功能有：按键选择曲目、按键打断播放、数码管显示当前状态。

功能实现基于状态机，共四个状态：
1.	等待（用户键盘输入选择曲目）。
2.	播放曲目1。
3.	播放曲目2。
4.	播放曲目3。


## 安全功能
- 数据备份
- 数据校验

- 热启动
- 看门狗

- 输入滤波
- 输出滤波

- 控制冗余（检查执行顺序）
- 执行冗余
  - 独立代码随机顺序执行
  - 等价实现随机选择执行
  - 随机Delay

- 睡眠降低功耗、躲避干扰
- 代码、数据优化

# 其它

更详细的描述和实现可以参考`./report.docx`。