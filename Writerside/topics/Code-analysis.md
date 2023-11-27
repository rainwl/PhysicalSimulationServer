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

#### IV.void ProcessContacts

```C++
  void ProcessContacts(shared_ptr<Scene> &contacting_soft_body, Vec3 &force) override {
    shared_ptr<Scene> soft_body = contacting_soft_body;
#pragma region unfinished soft tissue
    // If there is any unfinished soft tissue, we should finish it first.
    if (m_processingSoftBody) {
      if (soft_body.get() != m_processingSoftBody.get()) {
        cout << "Handle previously unfinished soft tissue" << endl;
        soft_body = m_processingSoftBody;
        contacting_soft_body = soft_body;
      }
    }
#pragma endregion

    if (soft_body && soft_body->m_type == 8) { std::cout << "whether the dura can be torn: " << soft_body->m_processable << "\n"; }
    if (!soft_body || !soft_body->m_processable || !soft_body->m_tearable) {
      if (just_closed_) just_closed_ = false;
      return;
    }
    if (!upper_part_ && !lower_part_) return;

    // The two spherical centers of the capsule
    const Vec3 roi_global_center0 = m_actualPos + Rotate(m_actualRot, roi_local_center0);
    const Vec3 roi_global_center1 = m_actualPos + Rotate(m_actualRot, roi_local_center1);

    // The distance from the center of the instrument's area of interest to the reference point, such as the center of the sphere
    const float distance_from_reference = Dot(Rotate(m_actualRot, Vec3(0, 0, 1)), roi_global_center0 - *m_referencePos);

    // When there are particles to be torn in the soft tissue
    // First set the soft body collision force to zero
    // Then in the next frame, let the Sofa go into a tearing state
    // and creating a tearing force
    // In order to avoid jitter, the soft body collision force
    // and tearing force cannot exist at the same time

    if (m_isTearing && !soft_body->GetTearParticles()->empty()) { if (!*m_isTearing) { *m_isTearing = true; } }

    // when rongeur closed just now 
    if (is_closed_ && just_closed_) {
      // Detect the particle count in the ROI range
      particles_in_roi_.resize(0);
      for (int i = soft_body->m_particleOffset; i < soft_body->m_particleOffset + soft_body->GetNumParticles(); ++i) {
        auto p = Vec3(g_buffers->positions[i]);

        // The original region of interest was a sphere
        // if (LengthSq(p - roi_global_center0) < roi_radius_ * roi_radius_)
        //     particles_in_roi_.push_back(i);

        // Now switch to capsules
        Vec3 c0c1 = roi_global_center1 - roi_global_center0;
        Vec3 l = Normalize(c0c1);
        Vec3 c0p = p - roi_global_center0;
        Vec3 c0q = l * Dot(l, c0p);
        Vec3 q = -c0p + c0q;

        if (Dot(l, c0q) < 0.0f) q = roi_global_center0;
        if (Dot(l, c0q) > Dot(l, c0c1)) q = roi_global_center1;

        if (LengthSq(p - q) < roi_radius_ * roi_radius_) particles_in_roi_.push_back(i);
      }
      // std::cout << "The number of particles in the ROI range: " << m_particlesInROI.size() << "\n";

      if (!particles_in_roi_.empty()) force = Vec3();

      //调用完SetTearParticles()之后，softBody的tearParticles会发生变化，长度会等于numValidTearParticles
      soft_body->SetTearParticles(particles_in_roi_);
      const vector<int> &tearParticles = *soft_body->GetTearParticles();
      int numValidTearParticles = tearParticles.size();
      //cout << "能用于撕裂的夹中粒子数：" << numValidTearParticles << endl;

      //让粒子固定在钳子上
      if (numValidTearParticles) {
        //由于在撕裂的瞬间，粒子可能突然脱离与钳子的接触，从而导致该函数执行不下去
        //为避免此种情形发生，钳子张开之前所处理的软体必须被夹闭时的软体占用
        m_processingSoftBody = soft_body;

        //刚夹闭粒子，让粒子固定在钳子上
        for (int i = 0; i < tearParticles.size(); ++i) {
          Vec3 p = g_buffers->positions[tearParticles[i]];
          Vec3 localP = RotateInv(m_actualRot, p - m_actualPos);
          local_positions_.push_back(localP);

          g_buffers->positions[tearParticles[i]].w = 0.0;
          g_buffers->velocities[tearParticles[i]] = 0.0;
        }
      } else { soft_body->ClearTearParticles(); }

      just_closed_ = false;
    }

    // If the instrument just opened,
    // And caught the particles before opening,
    // These particles are un anchored
    if (!is_closed_) {
      m_processingSoftBody = nullptr;

      const vector<int> &torn_particles = *soft_body->GetTornParticles();
      const vector<int> &tear_particles = *soft_body->GetTearParticles();

      // If the device holds only the particles that have already been torn apart
      // if (tornParticles.empty() && tearParticles.empty())
      // {
      // 	//m_particlesInROI用来判断夹闭的时候有没夹到粒子，并且只在刚张开的时候调用一次
      // 	if (m_particlesInROI.size())
      // 	{
      // 		if (distanceFromReference>100)
      // 		{
      // 			//std::cout << "m_zInstrument-m_zEndoscope: " << *m_zInstrument-*m_zEndoscope << std::endl;
      // 			m_particlesInROI.resize(0);
      // 			for (int i = softBody->m_particleOffset; i < softBody->m_particleOffset + softBody->GetNumParticles(); ++i)
      // 			{
      // 				Vec3 p = Vec3(g_buffers->positions[i]);
      // 				if (LengthSq(p - roiGlobalCenter) < m_roiRadius * m_roiRadius)
      // 					m_particlesInROI.push_back(i);
      // 			}
      // 			cout << "ROI范围内的粒子数：" << m_particlesInROI.size() << endl;
      // 			if (m_particlesInROI.size())
      // 			{
      // 				softBody->RemoveParticles(m_particlesInROI, Vec3(10.0, 10.0, 10.0));
      // 			}
      // 		}
      // 		m_particlesInROI.resize(0);
      // 	}
      // }

      // Loosen the particles that have been torn
      if (!torn_particles.empty()) {
        for (const int torn_particle : torn_particles) { g_buffers->positions[torn_particle].w = 1.0; }

        soft_body->ClearTornParticles();
        local_positions_.resize(0);

        // If the pliers retreat to a very backward position
        // Move the torn particles out of sight
        // and fixed them
        // It's like the particle disappeared
        cout << "The grip particles release instantaneously 'distance_from_reference':" << distance_from_reference << endl;
        if (distance_from_reference > -15.0f) {
          //std::cout << "m_zInstrument-m_zEndoscope: " << *m_zInstrument-*m_zEndoscope << std::endl;
          vector<int> rigidParticles;
          // Pass an empty array
          // to represent removes the last rigid-body implicated particle
          soft_body->RemoveParticles(rigidParticles, Vec3(10.0, 10.0, 10.0));
          cout << "Loose and disappear" << endl;
        }
        m_tornAndHeld = false;

        // the number of soft body bleeding increases
        // if (m_bloodingPoints)
        // {
        //     for (int i = 0; i < m_bloodingPoints->size(); ++i)
        //     {
        //         if (m_bloodingPoints->at(i).first == softBody)
        //         {
        //             softBody->m_bloodingTimes++;
        //             break;
        //         }
        //     }
        // }
      }

      // Loosen un torn particles
      if (!tear_particles.empty()) {
        for (const int tear_particle : tear_particles) { g_buffers->positions[tear_particle].w = 1.0; }

        soft_body->ClearTearParticles();
        local_positions_.resize(0);

        if (m_isTearing) {
          *m_isTearing = false;
          if (m_tearForce) *m_tearForce = 0.0;
        }

        // the number of soft body bleeding increases
        // if (m_bloodingPoints)
        // {
        //     for (int i = 0; i < m_bloodingPoints->size(); ++i)
        //     {
        //         if (m_bloodingPoints->at(i).first == softBody)
        //         {
        //             softBody->m_bloodingTimes++;
        //             break;
        //         }
        //     }
        // }
      }

      //是抓住未撕裂的时候提示呢?还是撕裂后才提示?
      // Is it a cue to grab it when it's not torn? Or is it after the tear?
      //if (m_tipsPoints && m_tipsPoints->size())
      //	m_tipsPoints->resize(0);
    }

    // 如果tearParticles有值，说明已经夹闭了一些粒子，并且未发生撕裂
    const vector<int> &tear_particles = *soft_body->GetTearParticles();
    if (!tear_particles.empty()) {
      force = Vec3();
      if (m_tearForce) {
        //cout << "tearForce:" << *m_tearForce << endl;
        soft_body->SetTearForce(*m_tearForce);
      }

      for (int i = 0; i < tear_particles.size(); ++i) {
        Vec3 p = Rotate(m_actualRot, local_positions_[i]) + m_actualPos;
        g_buffers->positions[tear_particles[i]] = Vec4(p, 0);
        g_buffers->velocities[tear_particles[i]] = 0.0;
      }

      //是抓住未撕裂的时候提示呢？还是撕裂后才提示？
      //if (m_tipsPoints)
      //	m_tipsPoints->push_back(std::pair<shared_ptr<Scene>, uint32_t>(softBody, softBody->m_tipsPointIndex));
    } else {
      //如果是刚撕裂，取消撕扯力
      if (m_isTearing && *m_isTearing) {
        force = Vec3();
        if (*m_isTearing) {
          *m_isTearing = false;
          if (m_tearForce) *m_tearForce = 0.0;

          m_tornAndHeld = true;

          cout << "Just torn, torn soft tissue name:" << soft_body->GetName() << " Number of soft tissue tears:" << soft_body->GetTornParticles()->size() << endl;
        }
      }

      //处理出血点
      if (m_bloodingPoints && soft_body->m_isBlooding) {
        bool bleedingExists = false;
        //如果该软组织早已出血，那说明出血点被抓走了，此时要换一个出血点
        for (int i = 0; i < m_bloodingPoints->size(); ++i) {
          if (m_bloodingPoints->at(i).first == soft_body) {
            m_bloodingPoints->at(i).second = soft_body->m_bloodingPointIndex;
            bleedingExists = true;
            break;
          }
        }

        if (!bleedingExists) { m_bloodingPoints->push_back(std::pair<shared_ptr<Scene>,uint32_t>(soft_body, soft_body->m_bloodingPointIndex)); }
        soft_body->m_isBlooding = false;

        //softBody->m_processable = false;

        //auto temp = Vec3(g_buffers->positions[softBody->m_particleOffset + softBody->m_bloodingPointIndex]);
        //std::cout << "blooding points: " << temp[0] << "," << temp[1] << "," << temp[2] << std::endl;
      }

      const vector<int> &tornParticles = *soft_body->GetTornParticles();
      //当发生撕裂后，m_tearParticles无值，m_tornParticles有值
      if (tornParticles.size() && local_positions_.size()) {
        force = Vec3();
        for (int i = 0; i < tornParticles.size(); ++i) {
          Vec3 p = Rotate(m_actualRot, local_positions_[i]) + m_actualPos;
          g_buffers->positions[tornParticles[i]] = Vec4(p, 0);
          g_buffers->velocities[tornParticles[i]] = 0.0;
        }

        //发生撕裂后，钳子在夹闭状态下退到较远的位置消失，排除血管和神经根
        //cout << "发生撕裂后夹住粒子的distanceFromReference:" << distanceFromReference << endl;
        //if (distanceFromReference > -7.0f && softBody->m_type !=3 && softBody->m_type != 9)
        //{
        //	m_processingSoftBody = nullptr;

        //	for (int i = 0; i < tornParticles.size(); ++i)
        //	{
        //		g_buffers->positions[tornParticles[i]].w = 1.0;
        //	}

        //	softBody->ClearTornParticles();
        //	m_localPositions.resize(0);
        //	vector<int> rigidParticles;
        //	//传一个空数组，表示移除最后一个刚体牵连的粒子
        //	softBody->RemoveParticles(rigidParticles, Vec3(10.0, 10.0, 10.0));
        //	cout << "不松开消失" << endl;
        //	m_tornAndHeld = true;
        //}

      }
    }

  }

```
{collapsible="true"}

- 这段 C++ 代码主要用于处理与一个物理场景中的软体物体发生接触的过程。在这个过程中可能会进行软体物体的破裂处理。
- 该方法首先对参数 contacting_soft_body 进行检查看看是否与上次处理的软体相同，如果不同则会对上次的软体完成一些额外操作。
- 然后該方法会檢查軟體物體是否可以進行處理和破裂。如果無法處理或破裂，那麼該方法會立即返回，不進行後續操作。
- 该方法在处理过程中会使用几何计算与向量计算来实现碰撞检测、粒子产生等功能。
- 方法也会对粒子的状态进行更改。例如，在执行某些条件满足时，会对粒子的位置、速度进行变更，以实现类似撕裂的效果。
- 该方法还处理了软体物体是否在撕裂的检测，包括当仅夹取撕裂粒子，松开瞬间处理，以及撕裂后处理等。
- 這段代碼還會夾取並固定在鉗子上的粒子，並在撕裂後或撕裂前執行一些額外處理。
- 最后，如果撕裂发生，会对粒子的状态进行相应更改，包括位置、速度等，并可能会更改撕裂力等数据。
- 总的来说，这段代码是物理碰撞处理的一部分，是实现软体物理仿真的关键部分之一，主要用于处理软体接触和可能的撕裂过程。

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

We know that the driver is `m_instrumentState0`,and in `Main.scn`,
```xml
<MechanicalObject name="Articulations" template="Vec1d" 
                  size="8" tags="R" 
                  position="@innerInstrumentSwitch.posture" 
                  rest_position="@innerInstrumentSwitch.posture"/>
```
We can see clearly the position of the driver is `innerInstrumentSwitch.posture`,

Corresponds to FleX,is **InnerInstrumentSwitch.h**
```C++
Data<Vec1dTypes::VecCoord> d_posture;
```
They have a relational relationship.(Answers received from @oht)

So, in **Geomagic.h**,
```C++
void updatePosition() {
  if (sofa_scene.get() && sofa_scene->sofa_instrument_switch.get()) {
    sofa::defaulttype::Vec1Types::VecCoord &ps =
        *sofa_scene->sofa_instrument_switch->d_posture.beginEdit();
    ComputePositions(translationDevice, orientationDevice, ps);
    sofa_scene->sofa_instrument_switch->d_posture.endEdit();
  }
}
```
```C++
void ComputePositions(Vec3 &p_device, Quat &r_device,
                        sofa::defaulttype::Vec1Types::VecCoord &ps,
                        bool is_callback = false) {
  ps[0][0] = euler[1];
  ps[1][0] = euler[0];
  ps[2][0] = z_endoscope;
  ps[3][0] = z_endoscope_rot;
  ps[4][0] = z_tube;
  ps[5][0] = z_tube_rot;
  ps[6][0] = dz + (0) + delta_z;
  ps[7][0] = euler[2]; 
}                
```

Here is the data driver.

`euler[1]`和`euler[0]` are the overall rotation of the instrument around the pivot.

`eluer[2]` is the self rotation of instrument.

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


