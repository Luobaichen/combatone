# CombatCore

UE 5.8.3 C++ 近战战斗 Demo（个人项目）。

## 当前进度

- [x] 工程骨架：UE 5.8 C++ 工程，运行时模块 `CombatCore`
- [x] 测试地图 `Content/Maps/L_CombatCore`（地板、参照方块、PlayerStart、太阳光/天光/大气）
- [x] 模板资产：Mannequin 网格/动画、Enhanced Input 资产
- [x] 角色、相机、Enhanced Input 移动/视角
- [x] 属性组件、受击接口、假人
- [x] 武器、近战扫掠判定
- [x] 动画通知状态驱动的判定窗口、轻/重击
- [x] 受击/死亡、翻滚、UMG HUD（Day 5，PIE 实测通过）
- [ ] 自动化测试、README 完整化

## 文档

- [`Docs/开发计划.md`](Docs/开发计划.md)：7 天计划、类清单、每日验收标准
- [`Docs/Day1_角色与输入代码讲解.md`](Docs/Day1_角色与输入代码讲解.md)：Day 1 代码逐段讲解
- [`Docs/Day2_属性组件_手把手.md`](Docs/Day2_属性组件_手把手.md)：Day 2 闯关式操作教程（逐段代码 + 验证点）
- [`Docs/Day3_武器与扫掠_手把手.md`](Docs/Day3_武器与扫掠_手把手.md)：Day 3 武器、扫掠判定、调试绘制教程
- [`Docs/Day4_动画通知与轻重击_手把手.md`](Docs/Day4_动画通知与轻重击_手把手.md)：Day 4 动画通知状态、轻/重击、体力门槛教程
- [`Docs/Day5_受击死亡翻滚与HUD_手把手.md`](Docs/Day5_受击死亡翻滚与HUD_手把手.md)：Day 5 受击、死亡、翻滚、UMG HUD 教程

## 构建

1. 用 UE 5.8 打开 `CombatCore.uproject`（首次会提示编译模块，选“是”）。
2. 或命令行：`Engine\Build\BatchFiles\Build.bat CombatCoreEditor Win64 Development -project="<路径>\CombatCore.uproject"`。
3. 默认地图 `L_CombatCore`，Play 即可。

## 第三方资产

- `Content/Characters/Mannequins/**`、`Content/Input/**` 来自 Unreal Engine 5.8 Third Person 模板（Epic Games，遵循 UE EULA）。
