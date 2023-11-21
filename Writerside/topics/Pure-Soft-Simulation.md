# Pure Soft Simulation

## TODO

|    | issue                                                                                                                                                  | status            | desc                                                                  |
|----|--------------------------------------------------------------------------------------------------------------------------------------------------------|-------------------|-----------------------------------------------------------------------|
| 0  | Data-driven `instruments` transform                                                                                                                    | [done](#0)        |                                                                       |
| 1  | Comb software simulation logic                                                                                                                         |                   |                                                                       |
| 2  | Where are the instruments `loaded`                                                                                                                     | [done](#2)        | shift F12 to look every ins.                                          |
| 3  | Search m_posDevice in tube.h                                                                                                                           | [done](#3)        |                                                                       |
| 4  | tool_data add &                                                                                                                                        | [done](#4)        | cause crash                                                           |
| 5  | Add lock for tool_data                                                                                                                                 | [done](#5)        | Race conditions and errors, proper synchronization and thread safety. |
| 6  | change visual for pliers                                                                                                                               | [done](#6)        |                                                                       |
| 7  | Remove SOFA tick in Geomagic                                                                                                                           | [done](#7)        |                                                                       |
| 8  | Analyze the performance footprint                                                                                                                      | [done](#8)        |                                                                       |
| 9  | In tube.h::Initialize(),remove shared memory                                                                                                           |                   |                                                                       |
| 10 | In InstrumentSwitch.h::Initialize,remove the sofa                                                                                                      |                   |                                                                       |
| 11 | sofa_scene->Tick() and BeforeTick() does what?                                                                                                         |                   |                                                                       |
| 12 | lock once ,when write 40 float to array                                                                                                                | [done](#12)       |                                                                       |
| 13 | take a look about ablationCatheter and Rongeur                                                                                                         |                   |                                                                       |
| 14 | complete instrument driven                                                                                                                             |                   |                                                                       |
| 15 | Ensure that the transform of the instrument (sleeve, etc.) is completely driven by the sensor, and does not undergo coupling calculation such as SOFA. |                   |                                                                       |
| 16 | AblationCatheter 's transform from where? Is it from SOFA? Pass to Unity?                                                                              |                   |                                                                       |
| 17 | Take a look of ablationCatheter,in sofa,how its transform has been computed?                                                                           |                   |                                                                       |
| 18 | A careful look at the pliers script, which involves software tearing, is very important                                                                |                   |                                                                       |
| 19 | How to control rongeur trans and motion                                                                                                                |                   |                                                                       |
| 20 | Do a test for delay                                                                                                                                    | [processing](#20) |                                                                       |

## Issues

### 2

Where are the instruments loaded
: SpineEndoscope::Initialize()
: InstrumentSwitch::Initialize()
: m_tube->Initialize();

### 0

`m_actualPos` and `m_actualRot` are values controlled motion of instruments.

```C++
  virtual void Update() {
    if (!m_posDevice && (!tool_data || tool_data->empty())) return;
    m_tubePart->UpdateState(m_actualPos, m_actualRot);
    UpdateShapes();
  }
protected:
  Vec3 m_actualPos;
  Quat m_actualRot;
```

Updated these two `values` in `GetToolData`

`m_posDevice` comes from SOFA

```C++
  virtual void GetToolData() {
    if (m_posDevice) {
      m_actualPos = Vec3(m_posDevice->GetTranslation());
      m_actualRot = Matrix33(m_posDevice->GetAxis(0), m_posDevice->GetAxis(1), m_posDevice->GetAxis(2));
      return;
    }

    if (!tool_data || tool_data->empty()) return;

    array<float,40> &toolData = *tool_data;
    //左手坐标转右手坐标
    m_actualPos = Vec3(-toolData[6], toolData[7], toolData[8]) * m_scale;
    //欧拉角转四元数
    m_actualRot = QuatFromAxisAngle(Vec3(0, 1, 0), DegToRad(-toolData[10])) * QuatFromAxisAngle(Vec3(1, 0, 0), DegToRad(toolData[9])) * QuatFromAxisAngle(Vec3(0, 0, 1), DegToRad(-toolData[11]));
  }
```

### 4

All assignments are done through shared memory addresses, with no additional memory copy.

![](tool_data.png)

### 3

![](sofaposdevice.png)

In `Instrument.h`,defines the `m_posDevice`

```C++
Matrix44 *m_posDevice = nullptr;
```

### 7

Remove the sofa tick

```C++
  void Loop() {
    while (true) {
      if (!m_isLooping) break;

      if (g_pause) continue;

      /*if (sofa_scene) {
        if (sofa_scene->m_initialized) {
          sofa_scene->BeforeTick();
          Tick();
          sofa_scene->Tick();
        }
      } else*/
        Tick();
    }
  }
```

### 6

```C++
geo_magic->d_frameVisu = false;
```

### 5

`commit SHA`

47b6e540176bc667292196910dfd944b74b01a14

### 8

The main performance footprint is caused by eCAL

![1](ana1.png)

![2](ana2.png)

### 12

`frame time`:24.3ms

9f19378482a8482b3e11a597caa631d96389a083

### 20

先把oht的性能优化的代码合并上来,然后看静息占用,开着publisher的占用,是否还是24

岳博的意思是怀疑器械动起来以后,刚体碰撞计算才开始,导致的性能上升,所以我把器械驱动的值修改成随机的,然后验证下.

