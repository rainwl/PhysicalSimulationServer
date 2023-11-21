# Code analysis

## Instruments

### Tube

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


## Rongeur.h

## m_posDevice (Instrument.h)

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

## tool_data

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

## tool_data call

SingletonDataReceiver::getInstance().getData()

```C++
const int instrument_index = static_cast<int>(SingletonDataReceiver::getInstance().getData(14));

```


