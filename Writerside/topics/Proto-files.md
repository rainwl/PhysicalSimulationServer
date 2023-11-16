# Proto files

## fusion.proto

```C++
syntax = "proto3";

import "coord.proto";
import "tissue.proto";
import "haptic.proto";
import "offset.proto";

package pb.FusionData;

message FusionData
{
    Coord.Vector3 endoscope_pos = 1;
    Coord.Euler endoscope_euler = 2;
    Coord.Vector3 tube_pos = 3;
    Coord.Euler tube_euler = 4;
    Offset.Offset offset = 5;
    Coord.Quaternion rot_coord = 6;
    Coord.Vector3 pivot_pos = 7;
    float ablation_count = 8;
    Haptic.Haptic haptic = 9;
    float hemostasis_count = 10;
    float hemostasis_index = 11;
    Tissue.Tissue soft_tissue = 12;
    float nerve_root_dance = 13;
}
```

## coord.proto

```C++
syntax = "proto3";

package pb.Coord;

message Vector3
{
    float x = 1;
    float y = 2;
    float z = 3;
}

message Euler
{
    float x = 1;
    float y = 2;
    float z = 3;
}

message Quaternion
{
    float x = 1;
    float y = 2;
    float z = 3;
    float w = 4;
}
```

## tissue.proto

```C++
syntax = "proto3";

package pb.Tissue;

message Tissue
{
    float liga_flavum = 1;
    float disc_yellow_space = 2;
    float veutro_vessel = 3;
    float fat = 4;
    float fibrous_rings = 5;
    float nucleus_pulposus = 6;
    float p_longitudinal_liga = 7;
    float dura_mater = 8;
    float nerve_root = 9;
}
```

## haptic.proto

```C++
syntax = "proto3";

package pb.Haptic;

message Haptic
{
    float haptic_state = 1;
    float haptic_offset = 2;
    float haptic_force = 3;
}
```

## offset.proto

```C++
syntax = "proto3";

package pb.Offset;

message Offset
{
    float endoscope_offset = 1;
    float tube_offset = 2;
    float instrument_switch = 3;
    float animation_value = 4;
    float pivot_offset = 5;
}
```