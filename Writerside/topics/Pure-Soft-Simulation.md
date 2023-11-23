# Soft Simulation

## TODO

|    | **issue**                                                                                     | **status**  | desc |
|----|-----------------------------------------------------------------------------------------------|-------------|------|
| 0  | ~~Data-driven `instruments` transform~~                                                       | [done](#0)  |      |
| 1  | Organize software simulation logic                                                            | processing  |      |
| 2  | ~~Instruments `loaded`~~                                                                      | [done](#2)  |      |
| 3  | ~~Search m_posDevice in tube.h~~                                                              | [done](#3)  |      |
| 4  | ~~tool_data add &~~                                                                           | [done](#4)  |      |
| 5  | ~~Add lock for tool_data~~                                                                    | [done](#5)  |      |
| 6  | ~~change visual for pliers~~                                                                  | [done](#6)  |      |
| 7  | ~~Remove SOFA tick in Geomagic~~                                                              | [done](#7)  |      |
| 8  | ~~Analyze the performance footprint~~                                                         | [done](#8)  |      |
| 9  | ~~In tube.h::Initialize(),remove shared memory~~                                              | done        |      |
| 10 | In InstrumentSwitch.h::Initialize,remove the sofa                                             |             |      |
| 11 | sofa_scene->Tick() and BeforeTick() does what?                                                |             |      |
| 12 | ~~lock once ,when write 40 float to array~~                                                   | [done](#12) |      |
| 13 | take a look about ablationCatheter and Rongeur                                                |             |      |
| 14 | complete instrument driven                                                                    |             |      |
| 15 | Ensure that the transform of the instrument (sleeve, etc.) is completely driven by the sensor |             |      |
| 16 | AblationCatheter 's transform from where? Is it from SOFA? Pass to Unity?                     |             |      |
| 17 | Take a look of ablationCatheter,in sofa,how its transform has been computed?                  |             |      |
| 18 | A careful look at the pliers script, which involves software tearing, is very important       |             |      |
| 19 | How to control rongeur trans and motion                                                       |             |      |
| 20 | ~~Do a test for delay~~                                                                       | [done](#20) |      |
| 21 | ~~eCAL loop in update(rec)~~                                                                  | [done](#21) |      |
| 22 | remove eCAL::OK,pub sub put in main thread.                                                   |             |      |
| 23 | The sequence of open Simulate Server and Flex has different perform                           |             |      |
| 24 | Tidy all shared memory between FleX and Unity                                                 |             |      |
| 25 | The tube direction is random every time.(bin) Does it have anything to do with not locking it |             |      |

## Issues

### 0 {collapsible="true"}

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

### 2 {collapsible="true"}

Where are the instruments loaded
: SpineEndoscope::Initialize()
: InstrumentSwitch::Initialize()
: m_tube->Initialize();

### 3 {collapsible="true"}

![](sofaposdevice.png)

In `Instrument.h`,defines the `m_posDevice`

```C++
Matrix44 *m_posDevice = nullptr;
```

### 4 {collapsible="true"}

cause crash

All assignments are done through shared memory addresses, with no additional memory copy.

![](tool_data.png)

### 5 {collapsible="true"}

Race conditions and errors, proper synchronization and thread safety.

`commit SHA`

47b6e540176bc667292196910dfd944b74b01a14

### 6 {collapsible="true"}

```C++
geo_magic->d_frameVisu = false;
```

### 7 {collapsible="true"}

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

### 8 {collapsible="true"}

The main performance footprint is caused by eCAL

![1](ana1.png)

![2](ana2.png)

### 12 {collapsible="true"}

`frame time`:24.3ms

9f19378482a8482b3e11a597caa631d96389a083

### 20 {collapsible="true"}

[analyze](eCAL-performance-measure.md)

### 21 {collapsible="true"}

I have been tested eCAL receive in Update(),but the performance is as same as in thread with callback,so I use
callback currently.