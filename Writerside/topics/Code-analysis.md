# Code analysis

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

