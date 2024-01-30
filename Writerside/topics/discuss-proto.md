# discuss_proto

## Server To Physics

| Message           | object    | variable | type                                           |
|-------------------|-----------|----------|------------------------------------------------|
| Instrument        | endoscope | pos      | float3                                         |
|                   | tube      | rot      | euler angle <br/> quaternion <br/> matrix4x4 ? |
|                   | rongeur   |          |                                                |
| Rongeur_animation | value     | value    | float `[0,1]`                                  |

## FE To Physics

| Message                      |               |
|------------------------------|---------------|
| Instrument_type              | rongeur       |
|                              | rongeur45     |
| pivot_set_times              | whether need? |
| pivot_pos                    |               |
| ablation_can_born_a_hole     |               |
| hemostasis_count             |               |
| hemostasis_index             |               |
| can_nerve_root_dance         |               |
| can_soft_tissue_can_be_grasp | NERVE         |
|                              | FLOCCULE      |
|                              | DFSPACE       |
|                              | FAT           |
|                              | VESSEL        |
|                              | POSTERIOR     |
|                              | ANNULUS       |
|                              | NUCLEUS       |
|                              | DURA          |

## Physics To Server

| Message | type   |
|---------|--------|
| force   | float3 |
|         | float  |

## Physics To FE

| Message            | type             | object                             |
|--------------------|------------------|------------------------------------|
| InstrumentData     | float3 pos       | Endoscope                          |
|                    | float3 rot       | Tube                               |
|                    | float2 rxs       | Rongeur                            |
|                    |                  | AblationCatheter                   |
| Current Instrument | enum:            | cur_ins                            |
|                    | Endoscope        |                                    |
|                    | Tube             |                                    |
|                    | Rongeur          |                                    |
|                    | AblationCatheter |                                    |
| SoftBodyStatus     | float            | touch_type                         |
|                    | float            | torn_type                          |
|                    | float            | torn_proportion                    |
|                    | float            | grasp_status                       |
|                    | float            | touch_vessel_type                  |
|                    | float3           | blood_point                        |
|                    | float3           | tip_point                          |
| SoftBodies         | SoftBody:        | `float` positions                  |
|                    | floccule         | `float` normals                    |
|                    | nerve            | `uint32` pts_vertices              |
|                    | dfspace          | `uint32` textured_triangle_indices |
|                    | fat              | `float2` texture_uv                |
|                    | vessel_1         | `uint32` coincident_points         |
|                    | vessel_2         |                                    |
|                    | posterior        |                                    |
|                    | annulus          |                                    |
|                    | nucleus          |                                    |
|                    | dura             |                                    |

