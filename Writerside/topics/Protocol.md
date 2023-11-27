# Protocol

I use eCAL as the communication suite, which will add some new protocols as well as replace some of the native shared
memory.

## TODO

|   | **issue**                           | **status** |
|---|-------------------------------------|------------|
| 0 | rename the protocol                 | done       |
| 1 | rename `SerialPort` to `FusionData` | done       |
| 2 | add send fusion data function       | done       |
| 3 | add pos and rot for rongeur         |            |

## Protocol item

### FusionData {collapsible="true"}

`Interface data->Physics`

Use `ExternalData.h` to set and get external data.

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

explain
: `endoscope_offset` The upper and lower offset of the endoscope
: `tube_offset` The upper and lower offset of the tube
: `haptic_state` 0:Closing haptic 1:Open haptic 2:Closing haptic & physics 3: Open all
: `haptic_offset` Offset of pliers
: `soft_tissue` 0:Unable to grab 1:able to grab

You can refer to the following link for specific protocol definitions.

> [proto definition](Proto-files.md)
> {style="note"}

