# NewProto

|   | CopyToSharedMemory    |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 | type                                                                                                                                                                                                               |
|---|-----------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| √ | AblationCatheter      | CopyToSharedMemory(poses, offset); <br/>CopyToSharedMemory(rots, offset);<br/>CopyToSharedMemory(rxs, offset);                                                                                                                                                                                                                                                                                                                                                                                                                  | `vector<Vec3> poses`;<br/>`vector<Quat> rots;`<br/>`vector<float> rxs;`                                                                                                                                            |
| √ | Rongeur               | CopyToSharedMemory(poses, offset); <br/>CopyToSharedMemory(rots, offset);<br/>CopyToSharedMemory(rxs, offset);                                                                                                                                                                                                                                                                                                                                                                                                                  | `vector<Vec3> poses`;<br/>`vector<Quat> rots;`<br/>`vector<float> rxs;`                                                                                                                                            |
| √ | Endoscope             | CopyToSharedMemory(poses, offset); <br/>CopyToSharedMemory(rots, offset);                                                                                                                                                                                                                                                                                                                                                                                                                                                       | `vector<Vec3> poses;`<br/>   `vector<Quat> rots;`                                                                                                                                                                  |
| √ | Tube                  | CopyToSharedMemory(poses, offset); <bar/>CopyToSharedMemory(rots, offset);                                                                                                                                                                                                                                                                                                                                                                                                                                                      | `vector<Vec3> poses;` <bar/>    `vector<Quat> rots;`                                                                                                                                                               |
| √ | InstrumentSwitch      | CopyToSharedMemory(indices, offset);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            | vector<uint32_t> indices;   indices.push_back(m_curInstrumentIndex);                                                                                                                                               |
| √ | Evaluation            | CopyToSharedMemory(&evaluationData[0], evaluationData.size(), offset); <br/>CopyToSharedMemory(bloodingPoints, offset);<br/>CopyToSharedMemory(tipsPoints, offset);                                                                                                                                                                                                                                                                                                                                                             | `vector<float> evaluationData`  :m_worldXZForce[0]   ,m_worldXZForce[1]  ,m_touchingSoftBodyType,m_tornSoftBodyType ,m_tornProportion, m_graspingStatus, m_touchingVesselType <bar/>`vector<Vec3> bloodingPoints;` |
| √ | FlocculeSoftBody      | CopyToSharedMemory((float*)&g_buffers->positions[m_particleOffset], m_asset->numParticles * 4, offset); <bar/> CopyToSharedMemory((float*)&g_buffers->normals[m_particleOffset], m_asset->numParticles * 4, offset); <bar/> CopyToSharedMemory((float*)&m_lastNormals[0], m_asset->numParticles * 4, offset); <bar/>CopyToSharedMemory(ptsVertices, offset);<bar/> CopyToSharedMemory(m_texturedTriIndices, offset);<bar/>CopyToSharedMemory(m_texturedUVs, offset);<bar/>CopyToSharedMemory(m_coincidentPoints, offset);<bar/> | `float` <bar/>   `float` <bar/>   `float`<bar/>   ` vector<uint32_t>&` <bar/>  `std::vector<uint32_t>`   <bar/> `std::vector<Vec2>` <bar/>     `ector<uint32_t>`                                                   |
| √ | RopeSoftBody          | ditto (but no coincidentPoints)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 | ditto                                                                                                                                                                                                              |
| √ | TearableTetraSoftBody | ditto (but no coincidentPoints)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 | ditto                                                                                                                                                                                                              |
| √ | TriSoftBody           | ditto (but no coincidentPoints)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 | ditto                                                                                                                                                                                                              |

| CopyFromSharedMemory |                                                           |                   |
|----------------------|-----------------------------------------------------------|-------------------|
| GeomagicCalibration  | m_sharedMemory->CopyFromSharedMemory(m_data, offset);     | calibration count |
| SerialPort           | m_sharedMemory->CopyFromSharedMemory(m_toolData, offset); |                   |

and also need to design the soft body in assets submodule.


| **index** | **proto**                | **example** | **index** | **proto**                       | **example** |
|-----------|--------------------------|-------------|-----------|---------------------------------|-------------|
| **0**     | endoscope_pos.x          | 2.000015    | **20**    | rot_coord.w                     | 0.707106    |
| **1**     | endoscope_pos.y          | 3.000009    | **21**    | pivot_pos.x                     | -10         |
| **2**     | endoscope_pos.z          | 7.999690    | **22**    | pivot_pos.y                     | 4.9         |
| **3**     | endoscope_euler.x        | 338.5304    | **23**    | pivot_pos.z                     | -0.9        |
| **4**     | endoscope_euler.y        | 13.23317    | **24**    | ablation_count                  | 0           |
| **5**     | endoscope_euler.z        | 11.95613    | **25**    | haptic.haptic_state             | 3           |
| **6**     | tube_pos.x               | 1.000009    | **26**    | haptic.haptic_offset            | -1          |
| **7**     | tube_pos.y               | 2.000009    | **27**    | haptic.haptic_force             | 2           |
| **8**     | tube_pos.z               | 5.999765    | **28**    | hemostasis_count                | 0           |
| **9**     | tube_euler.x             | 338.5304    | **29**    | hemostasis_index                | 0           |
| **10**    | tube_euler.y             | 13.23317    | **30**    | soft_tissue.liga_flavum         | 1           |
| **11**    | tube_euler.z             | 17.15376    | **31**    | soft_tissue.disc_yellow_space   | 1           |
| **12**    | offset.endoscope_offset  | -1.22898    | **32**    | soft_tissue.veutro_vessel       | 1           |
| **13**    | offset.tube_offset       | -3.61975    | **33**    | soft_tissue.fat                 | 1           |
| **14**    | offset.instrument_switch | 60          | **34**    | soft_tissue.fibrous_rings       | 1           |
| **15**    | offset.animation_value   | 0.9         | **35**    | soft_tissue.nucleus_pulposus    | 1           |
| **16**    | offset.pivot_offset      | 2           | **36**    | soft_tissue.p_longitudinal_liga | 1           |
| **17**    | rot_coord.x              | 0           | **37**    | soft_tissue.dura_mater          | 1           |
| **18**    | rot_coord.y              | 0.707106    | **38**    | soft_tissue.nerve_root          | 1           |
| **19**    | rot_coord.z              | 0           | **39**    | nerve_root_dance                | 0           |
|           |                          |             |           |                                 |             |
| **40**    | rongeur_pos.x            |             |           |                                 |             |
| **41**    | rongeur_pos.y            |             |           |                                 |             |
| **42**    | rongeur_pos.z            |             |           |                                 |             |
| **43**    | rongeur_rot.x            |             |           |                                 |             |
| **44**    | rongeur_rot.y            |             |           |                                 |             |
| **45**    | rongeur_rot.z            |             |           |                                 |             |



Server2Physics

| Message                 | son       | variable | type                                   |
|-------------------------|-----------|----------|----------------------------------------|
| Instrument              | endoscope | pos      | float3                                 |
|                         | tube      | rot      | euler angle & quaternion & matrix4x4 ? |
|                         | rongeur   |          |                                        |
| Rongeur_animation_value |           | value    | float `[0,1]`                          |
|                         |           |          |                                        |

FE2Physics

|                              |                |   |
|------------------------------|----------------|---|
| Instrument_type              | rongeur        |   |
|                              | rongeur45      |   |
| pivot_set_times              | whether need?  |   |
| pivot_pos                    |                |   |
| ablation_can_born_a_hole     |                |   |
| hemostasis_count             |                |   |
| hemostasis_index             |                |   |
| can_nerve_root_dance         |                |   |
| can_soft_tissue_can_be_grasp | NERVE          |   |
|                              | FLOCCULE       |   |
|                              | DFSPACE        |   |
|                              | FAT            |   |
|                              | VESSEL         |   |
|                              | POSTERIOR      |   |
|                              | ANNULUS        |   |
|                              | NUCLEUS        |   |
|                              | DURA           |   |


Physics2Server

|              |                   |        |
|--------------|-------------------|--------|
| Haptic_force | norm              | float  |
|              | direction needed? | float3 |
|              |                   |        |


Physics2FE

|                   |                   |   |
|-------------------|-------------------|---|
| InstrumentData    | endoscope         |   |
|                   | tube              |   |
|                   | rongeur           |   |
|                   | ablation catheter |   |
| CurrentInstrument | cur_ins           |   |
| SoftBodyStatus    | soft_status       |   |
| SoftBodies        | soft_body         |   |


