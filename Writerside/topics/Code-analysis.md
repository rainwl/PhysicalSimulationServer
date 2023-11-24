# Code analysis

## Instruments

### Tube {collapsible="true"}

```C++
  void Update() override {
    tube_part_->UpdateState(m_actualPos, m_actualRot);
    UpdateShapes();
  }
```
````C++
void UpdateState(const Vec3 &posBase, const Quat &orientBase) {
      m_pos = posBase;
      m_rot = orientBase;

      if (!m_sdfFile.empty()) {
        m_pos += Rotate(m_rot, m_sdfRelativePos);

        //AddSDF(m_sdfColliModel, m_pos, m_rot, m_s);
        g_buffers->shapePositions[m_shapeOffset] = Vec4(m_pos, 0.0);
        g_buffers->shapeRotations[m_shapeOffset] = m_rot;
        g_buffers->shapePrevPositions[m_shapeOffset] = Vec4(m_prevPos, 0.0f);
        g_buffers->shapePrevRotations[m_shapeOffset] = m_prevRot;
      }

      m_prevPos = m_pos;
      m_prevRot = m_rot;
    }
````
在Update函数中,通过调用part的updateState,将位置和旋转传递给g_buffers

初始化的时候,新建了一个共享内存,我不知道这个干嘛用的.

![](copyfrominst.png)


### Rongeur {collapsible="true"}
```C++
  Vec3 m_roiLocalCenter0;
  Vec3 m_roiLocalCenter1;
```

#### I.What does this function do ?
```C++
void Move(float d) override {
  if (!m_upperPart) return;
  SetPosture(m_upperPart->m_rx + d * m_scale);
}
```
{collapsible="true"}

#### II.Work backwards from the results
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
  rxs.push_back(m_upperPart->m_rx);
  m_sharedMemory->CopyToSharedMemory(rxs, offset);
}
```
{collapsible="true"}

The `m_actualPos` and `m_actualRot` is updated in this function.

```C++
void Update() override {
    m_actualRot = Matrix33(m_posDevice->GetAxis(0), m_posDevice->GetAxis(1), m_posDevice->GetAxis(2)) * m_localRot;
    m_actualPos = Vec3(m_posDevice->GetTranslation()) + Rotate(m_actualRot, m_localPos);

    m_lowerPart->UpdateState(m_actualPos, m_actualRot);
    m_upperPart->UpdateState(m_actualPos, m_actualRot);
    UpdateShapes();
  }
```
{collapsible="true"}

and the `m_localRot` is a const variable assigned in  `m_cur_instrument->m_localRot = QuatFromAxisAngle(Vec3(0, 0, 1), DegToRad(0));`

So,we only need to know where assign the `m_posDevice`

#### III.m_posDevice

In **InstrumentSwitch.h**

```C++
m_cur_instrument->m_posDevice = m_pos_device;
```
And in **LoadScenes.h**

```C++
instrument_switch->m_pos_device = &sofa_scene->m_posDevice;
```
And in **SofaScene.h**

```C++
Rigid3Types::Coord pose = m_instrumentState0->x.getValue()[0];
sofa::defaulttype::Quatd orient = pose.getOrientation();
sofa::defaulttype::Vec3d center = pose.getCenter();
Vec3 pos(center[0], center[1], center[2]);
Quat q(orient[0], orient[1], orient[2], orient[3]);
m_posDevice = TranslationMatrix(Point3(pos)) * RotationMatrix(q);
```



### m_posDevice (Instrument.h) {collapsible="true"}

This value is interesting.

```C++
Matrix44 *m_posDevice = nullptr;
```

In `InstrumentSwitch.h`,the value has been assigned.

```C++
m_endoscope->m_posDevice = m_pos_endoscope;
m_tube->m_posDevice = m_pos_tube;
m_cur_instrument->m_posDevice = m_pos_device;
```

Take a look at `m_pos_endoscope`

```C++
instrument_switch->m_pos_endoscope = &sofa_scene->m_posEndoscope;
```

It is from SOFA!

### tool_data {collapsible="true"}

```C++
geo_magic->tool_data = &fusion_data->tool_data;
instrument_switch->m_tool_data = &fusion_data->tool_data;
```

In `InstrumentSwitch.h`

```C++
std::array<float,40> *m_tool_data = nullptr;
```

```C++
m_endoscope->tool_data = m_tool_data;
m_tube->tool_data = m_tool_data;
const std::array<float,40> &tool_data = *m_tool_data;
m_cur_instrument->tool_data = m_tool_data;
if (m_tool_data && m_tool_data->size() > 38) {
        soft_body->m_processable = static_cast<bool>(m_tool_data->at(soft_body_type_start + soft_body->m_type));
```

### tool_data call {collapsible="true"}

SingletonDataReceiver::getInstance().getData()

```C++
const int instrument_index = static_cast<int>(SingletonDataReceiver::getInstance().getData(14));

```


