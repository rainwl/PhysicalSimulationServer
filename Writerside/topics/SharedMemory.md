# SharedMemory

## SharedMemory

shared memory item(old)
: `GeomagicCalibration` unity->flex
: `HapticDeviceStatus`
: `Evaluation` flex->unity
: `SerialPort`
: `Endoscope`        
: `Tube`             
: `InsturmentSwitch`
: `nerve`            
: `FlocculeSoftBody`
: `dfspace`          
: `fat`              
: `vessel1`          
: `vessel2`          
: `posterior`        
: `annulus`          
: `nucleus`          
: `dura`


## Tube.h
I think ths is not useful,so remove it.

```C++
  void Initialize() override {
    m_sharedMemory = std::make_shared<SharedMemory>("Tube", 44);
    }
```
在这里,经过了SOFA运算后的Tube的姿态,再次通过共享内存发送给了Unity.

这里是不需要的,所以我直接去掉这个就完了,然后Unity那个需要把这个去掉,只由传感器数据进行驱动.
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
## Endoscope.h
ditto with Tube.h

