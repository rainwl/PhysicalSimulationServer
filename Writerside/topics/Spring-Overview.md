# Spring Overview

## Spring Test Case {collapsible="true"}

|   | TODO                                          | status     |
|---|-----------------------------------------------|------------|
| 0 | update spring rebuild time                    | [done](#0) |
| 1 | open all soft tissues                         | done       |
| 2 | performance under different number of springs | done       |

### Issues

#### 0

目前的弹簧逻辑是

闭合或者没闭合的时候保持弹簧不变

当夹取粒子的时候,弹簧依然没有变化,`但是此时应该断开
也就是一旦有了夹的粒子,就应该断开弹簧,在Update里
tornparticles位置判断一下,执行ClearSprings`

当放开的时候,清空了弹簧,然后将粒子移开了,
然后重建了弹簧.

**应该修改成的逻辑**

只要是抓了粒子,那就断开弹簧,当松开的时候,重建

## Performance {collapsible="true"}

in `release without debugging` mode

| linkRadius | minimum radius | spring count | frame | stiffness | penetration      |
|------------|----------------|--------------|-------|-----------|------------------|
| 2          | 0.1            |              |       |           |                  |
|            | 0.2            |              |       |           |                  |
|            | 0.5            | 2851352      | 33ms  | 不错        | 不错               |
|            | 1.0            | 2455777      | 31ms  | 不错        | 会穿一点点吧           |
|            | 1.5            | 1541173      | 26ms  | 还行吧       | 有一点点穿            |
|            | 1.75           | 832745       | 22ms  | 凑合,比1.5略差 | 比1.5穿的多一些,但是效果还在 |
| 3          | 1.75           | 6476138      | 53ms  | 不太好       | 很穿               |
|            | 2.0            | 4931021      | 45ms  | 不太好       | 很穿               |
| 4          |                |              |       |           |                  |
|            |                |              |       |           |                  |


| linkRadius | minimum radius | spring count | frame | stiffness      | penetration |
|------------|----------------|--------------|-------|----------------|-------------|
| 2.0        | 0.5            | 2851352      | 33ms  | good           | good        |
|            | 1.0            | 2455777      | 31ms  | good           | a little    |
|            | 1.5            | 1541173      | 26ms  | not too bad    | a little    |
|            | 1.75           | 832745       | 22ms  | worse than 1.5 | not too bad |
| 3.0        | 1.75           | 6476138      | 53ms  | not good       | very        |
|            | 2.0            | 4931021      | 45ms  | not good       | very        |

## Parameter influence {collapsible="true"}

| properties          | value  |
|---------------------|--------|
| mRadius             | 0.1f   |
| g_params.damping    | 200    |
| **link_stiffness**  | 1.0f   |
| **link_radius**     | 2.0f   |
| **volume_sampling** | 8.0f   |
| **cluster_spacing** | 40.0f  |
| **cluster_radius**  | 120.0f |
