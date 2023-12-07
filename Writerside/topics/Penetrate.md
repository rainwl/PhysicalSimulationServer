# Penetration problems of instruments and softbody

## Overview

The above video is the original effect, and the below is the improved one.

<video src="softorigin.mp4" preview-src="softorigincover.png" width="500"/> 

<video src="softlink.mp4" preview-src="softlinkcover.png" width="500"/>

## Case

|   | case                       | status                                  |
|---|----------------------------|-----------------------------------------|
| 0 | Add links between clusters | [done](#add-links-between-clusters)     |
| 1 | Particle properties        | [done](#particle-properties)            |
| 2 | Native soft body testing   | [processing](#native-soft-body-testing) |

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

