# Penetration problems of instruments and softbody

## Overview

The above video is the original effect, and the below is the improved one.

<video src="softorigin.mp4" preview-src="softorigincover.png" width="500"/> 

<video src="softlink.mp4" preview-src="softlinkcover.png" width="500"/>

|   |                                    |   |
|---|------------------------------------|---|
|   | particles properties               |   |
|   | Add cluster link                   |   |
|   | cluster show                       |   |
|   | Only focclue layer                 |   |
|   | Pure surface soft tissue           |   |
|   | see g_params config of rod demo    |   |
|   | compare octopus,rod,endoscope diff |   |
|   | CreateCluster function comprehend  |   |
|   |                                    |   |

## TODO

|   |                                                   |   |
|---|---------------------------------------------------|---|
|   | Add link to CreateCluster function                |   |
|   | Show cluster region                               |   |
|   | Only keep foccule layer ,remove other soft tissue |   |
|   |                                                   |   |
|   |                                                   |   |
|   |                                                   |   |
|   |                                                   |   |

## Analysis

### Bending and Stretch constraints

> Clothing in Flex is modeled using networks of springs. The cloth model is up to the user, but the extensions library
> provides an example cloth cooker (NvFlexExtCreateClothFromMesh()) that implements a common approach where the 1-ring
> neighbors are used to control stretch, and 2-ring neighbors are used to control bending.

> Note: because all collisions in Flex are performed at the particle level, the cloth mesh must be sufficiently well
> tessellated to avoid particle tunneling. Care must especially be taken for clothing with self-collision, the cloth
> mesh
> should be authored so that the mesh has a uniform edge length close to the solid particle rest distance. If
> self-collision is enabled, and particles are closer than this in the mesh then erroneous buckling or folding might
> occur
> as the distance and collision constraints fight each other.

### How does FleX simulate cloth

### Density of particles (Particle gap)

### Particle layer count

### Multi thread detection

### SDF with particles

**conclusion**

- [x] I. the sdf objects in the FleX example scenario are static.
- II. if the mouse drags the sphere to hit the sdf object, there is no resistance can be arbitrarily penetrated, but if
  there is no external force applied to the sphere, only do free fall, it can collide with the sdf.

like this:

<video src="pen.mp4" preview-src="penpre.png"/>

Here's the official explanation.Nothing useful.

![](flexsdf.png)

SDF example scenes
: Goo Gun
: Viscosity High
: Shape Collision
: ...

Click the ![][check]{width="16"} icon to mark an item as done.

[check]: ok.png

[ok]: ok1.png

### FleX Spine Endoscope Cluster Determination

```C++
const auto floccule_soft_body = std::make_shared<FlocculeSoftBody>("FlocculeSoftBody");
  floccule_soft_body->mIntergrated = integrated;
  floccule_soft_body->m_useSharedMemory = use_shared_memory;
  floccule_soft_body->m_fileName = "floccule";
  floccule_soft_body->m_type = 1;
  floccule_soft_body->m_resolution = 4;
  floccule_soft_body->mTranslation = body_translation;
  floccule_soft_body->mOrientation = body_orientation;
  floccule_soft_body->mScale = 0.1f;
  floccule_soft_body->m_globalStiffness = 0.5f;
  floccule_soft_body->m_ablatable = true;
  //flocculeSoftBody->m_eligibleTornProportion = 0.0f;
  scenes.push_back(floccule_soft_body);

  floccule_soft_body->initCallback = [](FlocculeSoftBody *softBody)->void {
    Vec3 center1 , center2;
    std::vector<Vec3> axes1 , axes2;
    std::vector<float> extents1 , extents2;
    defBox(softBody->m_fileName, "D_HRD_0.obj", softBody->mTranslation, softBody->mOrientation,
           softBody->mScale,
           center1, axes1, extents1);
    defBox(softBody->m_fileName, "D_HRD_1.obj", softBody->mTranslation, softBody->mOrientation,
           softBody->mScale,
           center2, axes2, extents2);

    for (int i = softBody->m_particleOffset; i < softBody->m_particleOffset + softBody->GetNumParticles(); ++i) {
      if (isInBox(g_buffers->positions[i], center1, axes1, extents1) || isInBox(
          g_buffers->positions[i], center2, axes2, extents2))
        g_buffers->positions[i].w = 0.0;
    }
  };

  instrument_switch->m_floccules.push_back(floccule_soft_body);
```

{collapsible="true"}