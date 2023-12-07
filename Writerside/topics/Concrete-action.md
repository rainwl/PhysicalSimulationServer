# Concrete action

## Floccule samples

**FlocculeSoftBody.h**

```C++
float mLinkRadius = 2.0f;

// add link data to the solver 
for (int i = 0; i < asset->numSprings; ++i)
{
  g_buffers->springIndices.push_back(asset->springIndices[i * 2 + 0]);
  g_buffers->springIndices.push_back(asset->springIndices[i * 2 + 1]);

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

## Conflicts

why only floccule has springs

Up to now ,

nerve will conflict with floccule.
so as dfspace
so as fat
`vesel is ok`
so as posterior ,but a little on the top
so as annulus
so as nucleus
so as durad


if we close floccule

the first softbody will crash also.

if all soft body open link,about 30ms
if only floccule,about 25ms (ifeCAL , 37ms)