# Soft Contact Force


```C++
// Geomagic.h 
inline HDCallbackCode HDCALLBACK StateCallback(void *user_data) {
  Vec3 currentForce;
  if (driver->m_instrumentSwitch) {
    currentForce = driver->m_instrumentSwitch->m_actual_force;
    if (driver->sofa_scene) { currentForce = RotateInv(driver->sofa_scene->m_rotAdd, currentForce); }
}
```
currentForce是从`driver->m_instrumentSwitch->m_actual_force`这里得到的.
等下注意仔细看.

`InstrumentSwitch.h`中,`void collision_detect`中
```C++
if (m_cur_instrument->m_computeSoftBodyForce) actual_force = force;
else actual_force = Vec3();
```

## Needed to global control

|                 |   |   |
|-----------------|---|---|
| spring_constant |   |   |
|                 |   |   |
|                 |   |   |
