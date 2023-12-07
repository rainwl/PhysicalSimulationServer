# Penetration problems of instruments and softbody

<video src="new.mp4" preview-src="newcover.png" width="500"/>

## Overview

The above video is the original effect, and the below is the improved one.

<video src="softorigin.mp4" preview-src="softorigincover.png" width="500"/> 

<video src="softlink.mp4" preview-src="softlinkcover.png" width="500"/>



## Case

|   | case                                    | status                                                 |
|---|-----------------------------------------|--------------------------------------------------------|
| 0 | Add links between clusters              | [done](#add-links-between-clusters)                    |
| 1 | Particle properties                     | [done](#particle-properties)                           |
| 2 | Native soft body testing                | [processing](#native-soft-body-testing)                |
| 3 | Explore the cluster and link parameters | [processing](#explore-the-cluster-and-link-parameters) |

## Imp

### Add links between clusters {collapsible="true"}

Take the floccule for example.

**FlocculeSoftBody.h**

```C++
float mLinkRadius = 2.0f;

// add link data to the solver 
for (int i = 0; i < asset->numSprings; ++i)
{
      g_buffers->springIndices.push_back(asset->springIndices[i * 2 + 0] + m_particleOffset);
      g_buffers->springIndices.push_back(asset->springIndices[i * 2 + 1] + m_particleOffset);

  g_buffers->springStiffness.push_back(asset->springCoefficients[i]);
  g_buffers->springLengths.push_back(asset->springRestLengths[i]);
}
```

{collapsible="true" collapsed-title="void CreateSoftBody(int group = 0)"}

The default value of `mLinkRadius` is `0.0f` and take no use.We just modify it to about `2.0f`,and add some params
to `g_buffers`,
then we can get a soft body performance.

**FlocculeSoftTearing.h**

```C++
// create links between clusters 
if (linkRadius > 0.0f)
// if (false)
{
  std::vector<int> springIndices;
  std::vector<float> springLengths;
  std::vector<float> springStiffness;

  // create links between particles
  int numLinks = CreateLinks(&relativeVertices[0], numMeshVertices, springIndices, springLengths, springStiffness,
                             linkRadius, linkStiffness);

  // assign links
  if (numLinks)
  {
    tearable->springIndices = new int[numLinks * 2];
    memcpy(tearable->springIndices, &springIndices[0], sizeof(int) * springIndices.size());

    tearable->springCoefficients = new float[numLinks];
    memcpy(tearable->springCoefficients, &springStiffness[0], sizeof(float) * numLinks);

    tearable->springRestLengths = new float[numLinks];
    memcpy(tearable->springRestLengths, &springLengths[0], sizeof(float) * numLinks);

    tearable->numSprings = numLinks;
  }
}
```

{collapsible="true" collapsed-title="NvFlexExtAsset* FlocculeCreateSoftFromMesh"}

We can add links here.And we can modify `linkStiffness` to adjust stiffness.

The improved video is based on this approach.

**However**

Adding constraints between clusters can result in significant performance `degradation` and uncontrollable `jitter`.

### Particle properties {collapsible="true"}

No practical effect.

The problem is that constrains using shape matching between the particles is too weak, and the instrument will simply
burst open, rather than maintaining the effect of a mesh or nearly rigid body.

### Native soft body testing {collapsible="true"}

[FleX Native Repository](https://github.com/rainwl/FleX_penetrate.git)

I used the `dev` branch of the following project to test.

### Explore the cluster and link parameters {collapsible="true"}

**default status**

![default](e1.png){width="1000"}

| **default params**         | **value**                |
|----------------------------|--------------------------|
| mRadius                    | 0.1f                     |
| mScale                     | (2.0f,2.0f,20.0f)        |
| g_params.radius *= 1.5f;   | 0.15f                    |
| g_params.dynamicFriction   | 0.35f                    |
| g_params.particleFriction  | 0.25f                    |
| g_params.collisionDistance | radius * 0.75f = 0.1125f |
| mClusterSpacing            | 2.0f                     |
| mClusterRadius             | 2.0f                     |
| mClusterStiffness          | 0.225f                   |
| mLinkRadius                | 0.0f                     |
| mLinkStiffness             | 0.0f                     |

- the `Cluster count` is about `Scale/mClusterSpacing`
- particles count is 119
- now ,`rigid/cluster count` equals to 11 (20/2+1)

**Cluster Conclusion**

| **relationship**                             | **result**                      |
|----------------------------------------------|---------------------------------|
| Spacing > Radius                             | Will break                      |
| Spacing = Radius                             | all right,good elasticity       |
| Spacing < Radius                             | stiffness ,elasticity--,rigid++ |
| Spacing and Radius are proportionally larger | stiffness ,elasticity--,rigid++ |

| **cluster stiffness** | **result**      |
|-----------------------|-----------------|
| 0                     | break           |
| 0.225                 | soft elastic    |
| 0.5                   | full elasticity |
| 1                     | explode         |

**Link Conclusion**

| **mLinkRadius** | **spring count** | **result**                             |
|-----------------|------------------|----------------------------------------|
| 1.0f            | 590              | more soft elastic,soft droop           |
| 2.0f            | 2298             | more and more soft elastic,soft droop  |
| 5.0f            | 5930             | more and more soft elastic,soft droop. |

![](e2.png) {width="800"}

![](e3.png) {width="800"}

![](e4.png) {width="800"}

The larger the `link radius` is, the more and softer the spring will be.

Moreover, after deformation, the larger the `jitter` will be, the more difficult it will be to `stabilize` quickly,
and the force will continue to `transfer` between the springs.

**Link stiffness**

Take link raduis equal to 2 as an example.

If we set `mLinkStiffness = 0.5f`,the stiffness of spring has been increased.
Overall performance is close to what it was before the addition of link.

![](e5.png) {width="800"}

**Grab**

If we grab the particle in the middle of the rope and lift it to the highest, then release it and wait for the rope to
balance itself.

- No spring takes 16-20 strokes
- With spring takes 10-14 strokes with spring

In all cases with link/spring, it is easier to consume energy to reach the resting state.
But the initial vibration amplitude, with spring will be larger, and then quickly consume energy.
And accompanied by a certain sense of shaking.

![](e6.png) {width="800"}

If we increase the link radius to 5.0f,The amplitude of the vibration will also decrease as we expected.

![](e7.png) {width="800"}