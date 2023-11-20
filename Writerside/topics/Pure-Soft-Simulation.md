# Pure Soft Simulation

## TODO

|   | issue                              | desc                                                                  | status     |
|---|------------------------------------|-----------------------------------------------------------------------|------------|
| 0 | Data-driven `instruments` transform |                                                                       | [done](#0) |
| 1 | Comb software simulation logic     |                                                                       |            |
| 2 | Where are the instruments `loaded` | shift F12 to look every ins.                                          | [done](#2) |
| 3 | Search m_posDevice in tube.h       |                                                                       | [done](#3) |
| 4 | tool_data add &                    | cause crash                                                           | [done](#4) |
| 5 | Add lock for tool_data             | Race conditions and errors, proper synchronization and thread safety. |            |
| 6 | change visual for plier            |                                                                       |            |

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