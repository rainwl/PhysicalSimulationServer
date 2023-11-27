# SharedMemory

|        | **shared memory name** | **description**             |
|--------|------------------------|-----------------------------|
| **0**  | `GeomagicCalibration`  |                             |
| **1**  | `HapticDeviceStatus`   |                             |
| **2**  | `Evaluation`           |                             |
| **3**  | `SerialPort`           | [fusion data](#fusion-data) |
| **4**  | `Endoscope`            | remove                      |
| **5**  | `Tube`                 | [remove](#tube)             |
| **6**  | `InsturmentSwitch`     |                             |
| **7**  | `nerve`                |                             |
| **8**  | `FlocculeSoftBody`     |                             |
| **9**  | `dfspace`              |                             |
| **10** | `fat`                  |                             |
| **11** | `vessel1`              |                             |
| **12** | `vessel2`              |                             |
| **13** | `posterior`            |                             |
| **14** | `annulus`              |                             |
| **15** | `nucleus`              |                             |
| **16** | `dura`                 |                             |

## fusion data {collapsible="true"}

`SerialPort` has been replaced with `FusionData`,and the protocol item is in [protocol fusion data](Protocol.md).

## Tube {collapsible="true"}

I think ths is not useful,so remove it.

```C++
  void Initialize() override {
    m_sharedMemory = std::make_shared<SharedMemory>("Tube", 44);
    }
```

Here, the Tube transform, after SOFA calculation,
is sent to Unity again via shared memory.

I don't need it here, so I'll just remove this one,
and then Unity needs to remove this one and just drive the sensor data.

```C++
  void TransmitData() override {
    int offset = 0;
    vector<Vec3> poses;
    poses.push_back(m_actualPos);
    m_sharedMemory->CopyToSharedMemory(poses, offset);

    vector<Quat> rots;
    rots.push_back(m_actualRot);
    m_sharedMemory->CopyToSharedMemory(rots, offset);
  }
```

## Endoscope.h {collapsible="true"}

ditto with Tube.h

## Rongeur.h {collapsible="true"}

```C++
  void TransmitData() override {
    Matrix33 frame = m_actualRot;
    Vec3 pos = m_actualPos;

    int offset = 0;
    vector<Vec3> poses;
    poses.push_back(pos);
    m_sharedMemory->CopyToSharedMemory(poses, offset);

    vector<Quat> rots;
    rots.push_back(frame);
    m_sharedMemory->CopyToSharedMemory(rots, offset);

    vector<float> rxs;
    rxs.push_back(upper_part_->m_rx);
    m_sharedMemory->CopyToSharedMemory(rxs, offset);
  }
```

{collapsible="true"}