# CombatCore

UE 5.8.3 C++ 近战战斗 Demo（个人项目）。

## 当前进度

- [x] 工程骨架：UE 5.8 C++ 工程，运行时模块 `CombatCore`
- [x] 测试地图 `Content/Maps/L_CombatCore`（地板、参照方块、PlayerStart、太阳光/天光/大气）
- [x] 模板资产：Mannequin 网格/动画、Enhanced Input 资产
- [ ] 角色、相机、Enhanced Input 移动/视角
- [ ] 属性组件、受击接口、假人
- [ ] 武器、近战扫掠判定
- [ ] 动画通知状态驱动的判定窗口、轻/重击
- [ ] 受击/死亡、翻滚、UMG HUD
- [ ] 自动化测试、README 完整化

## 构建

1. 用 UE 5.8 打开 `CombatCore.uproject`（首次会提示编译模块，选“是”）。
2. 或命令行：`Engine\Build\BatchFiles\Build.bat CombatCoreEditor Win64 Development -project="<路径>\CombatCore.uproject"`。
3. 默认地图 `L_CombatCore`，Play 即可。

## 第三方资产

- `Content/Characters/Mannequins/**`、`Content/Input/**` 来自 Unreal Engine 5.8 Third Person 模板（Epic Games，遵循 UE EULA）。
