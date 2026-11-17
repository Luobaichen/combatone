# CombatCore

UE 5.8.3 C++ 近战战斗 Demo。核心逻辑全部由 C++ 实现（UI 蓝图只负责外观），
目标是跑通一条完整的近战链路：输入 → 动画通知判定 → 扫掠命中 → 伤害结算 → 受击/死亡 → HUD。

![CombatCore demo](media/day6_combat_demo.gif)

## 功能

- Enhanced Input 第三人称移动、视角、跳跃
- 轻击 / 重击：Montage + AnimNotifyState 控制判定窗口，扫掠检测，同一次挥砍对同一目标只结算一次
- 属性组件：生命 / 体力、伤害下限、治疗上限、死亡事件
- 受击硬直、死亡布娃娃、翻滚（消耗体力）
- UMG HUD：血条 / 体力条，委托驱动实时更新
- 5 个自动化测试覆盖核心数值规则

## 架构

```text
Enhanced Input (IMC_Default)
  ├─ IA_Move / IA_Look / IA_Jump ──► ACombatCoreCharacter
  ├─ IA_Attack / IA_HeavyAttack ──► TryPlayAttack
  │                                   ├─ 死亡 / 正在播蒙太奇 → 拒绝
  │                                   ├─ TryConsumeStamina → 不够就拒绝
  │                                   └─ PlayAnimMontage(AM_*)
  │                                         └─ UCombatAnimNotifyState_Trace
  │                                               ├─ StartTrace（打开判定窗口）
  │                                               └─ StopTrace（关闭）
  │                                                     └─ UMeleeTraceComponent 每帧扫掠
  │                                                           └─ ICombatDamageable::Execute_ApplyHit
  │                                                                 ├─ ACombatCoreDummy
  │                                                                 └─ ACombatCoreCharacter
  │                                                                       └─ ApplyDamage
  │                                                                             ├─ OnHealthChanged → HUD
  │                                                                             └─ OnDeath → HandleDeath
  └─ IA_Roll ──► Roll ──► TryConsumeStamina → PlayAnimMontage(AM_Roll)
```

### 类职责

| 类                              | 职责                           |
| ------------------------------ | ---------------------------- |
| `ACombatCoreCharacter`         | 玩家 Pawn：输入、相机、攻击编排、受击、死亡、HUD |
| `ACombatCoreGameMode`          | 指定默认 Pawn                    |
| `UCombatAttributeComponent`    | 生命 / 体力规则与事件，不依赖角色           |
| `ICombatDamageable`            | 受击接口：攻击方不需要知道目标类型            |
| `ACombatCoreDummy`             | 训练假人，实现同一个受击接口               |
| `ACombatWeapon`                | 不可见的判定锚点：挂在手部 Socket 上，空手时两个扫掠点都在拳头上 |
| `UMeleeTraceComponent`         | 扫掠、碰撞查询、同一次挥砍去重              |
| `UCombatAnimNotifyState_Trace` | 在动画通知窗口内开关扫掠                 |
| `UCombatHUDWidget`             | HUD 的 C++ 基类，订阅属性事件          |

### 设计要点

- **接口解耦**：攻击方只调用 `ICombatDamageable`，玩家和假人共用同一条伤害链。
- **动画驱动判定**：判定窗口由 `AnimNotifyState` 开关，窗口外组件不 Tick。
- **数据与表现分离**：`UCombatAttributeComponent` 只算规则，HUD 只读事件，互不依赖。
- **事件而非轮询**：HUD 用委托更新，不需要每帧读生命值。
- **规则有测试**：伤害下限、治疗上限、体力拒绝、死亡只广播一次，共 5 个自动化测试。

## 操作

| 输入              | 动作           |
| --------------- | ------------ |
| `W / A / S / D` | 移动           |
| 鼠标移动            | 视角           |
| `Space`         | 跳跃           |
| 鼠标左键            | 轻击（消耗 10 体力） |
| 鼠标右键            | 重击（消耗 25 体力） |
| `Left Shift`    | 翻滚（消耗 15 体力） |

PIE 调试命令（先按 `~` 打开控制台）：

| 命令                 | 作用                 |
| ------------------ | ------------------ |
| `DebugHurtSelf 20` | 对自己造成 20 点伤害       |
| `DebugHitDummy 25` | 对场景中最近的假人造成 25 点伤害 |

## 构建与运行

环境要求：

- Windows 10/11
- Unreal Engine 5.8.3
- Visual Studio 2022（勾选 “Game development with C++” 工作负载）

步骤：

1. 右键 `CombatCore.uproject` → **Generate Visual Studio project files**；
2. 打开 `CombatCore.sln`，配置 `Development Editor | Win64`，构建；
3. 双击 `CombatCore.uproject` 打开编辑器，直接 Play（默认地图 `L_CombatCore`）。

第一次打开时，编辑器也可能提示“模块缺失，是否重新编译”，选“是”即可。

## 自动化测试

编辑器 → `Tools → Session Frontend → Automation`，搜索 `CombatCore`，
勾选 `Engine` 档位，运行 `CombatCore.Attribute.*` 共 5 个测试。

覆盖内容：伤害扣减、生命下限、治疗上限、体力不足拒绝、`OnDeath` 只广播一次。

命令行无头跑法（结果看 `Saved/Logs/CombatCore.log`）：

```bat
UnrealEditor-Cmd.exe "D:\Myproject\CombatCore\CombatCore.uproject" ^
  -ExecCmds="Automation RunTests CombatCore.Attribute" ^
  -unattended -nopause -testexit="Automation Test Queue Empty" -NullRHI -NoSplash -log
```

## 验收记录（2026-11-14）

- 构建：`CombatCoreEditor Win64 Development` — `Result: Succeeded`，0 error
- 自动化测试：`CombatCore.Attribute.*` — 5/5 `Success`，`Automation Test Queue Empty`
- PIE（`L_CombatCore`，真实键鼠输入）9 项验收全部通过：
  - 轻击：假人 100 → 75，体力 100 → 90，日志 `Actual=25.0, Heavy=no`
  - 重击：假人 75 → 25，体力 90 → 65，日志 `Actual=50.0, Heavy=yes`
  - 第三击致死：假人显示 `DEAD`、切换布娃娃；死亡后不再受击
  - 翻滚：体力 −15，动作结束后移动/攻击恢复正常
  - 体力不足：不播放、不扣体力，日志 `Not enough stamina`
  - `DebugHurtSelf 20` × 5：玩家 100 → 0，死亡布娃娃；死亡后攻击/翻滚无效
  - 去重：每次判定窗口只出现一条 `[Trace] Hit`
  - 移动中攻击：不穿透，判定球跟随武器
- HUD：血条红色、体力条绿色；实测宽度约屏幕的 35%（`RenderTransform.ScaleX=0.35`）；
  血量降到 0 时 Percent=0，无负数 / NaN
- 演示：`media/day6_combat_demo.gif`（12 秒）

已知限制：本项目是单机 Demo，不含联网、GAS、敌人 AI；第三方翻滚动画
`A_RollUE5` 仅用于本地学习，公开分发前需替换。HUD 只有血条/体力条，
WBP 中没有放置数值文本控件（`HealthText` / `StaminaText` 为可选的 BindWidget），
因此无文字读数，数值以日志和条长变化为准。

## 第三方资产

- `Content/Characters/Mannequins/**`、`Content/Input/**`：Unreal Engine 5.8 Third Person 模板（Epic Games，遵循 UE EULA）。
- `Content/Animations/A_RollUE5*`：从第三方动画包 `Combatframkeworkassets` 导入（原始文件 `CommonAnimations/A_RollUE5.FBX`）。该素材包未附带许可证文件，本项目仅作学习演示使用；如需公开发布或再分发，请替换为引擎自带或自行录制的动画。
- 代码：除模板生成的部分外，为本项目自行实现。


