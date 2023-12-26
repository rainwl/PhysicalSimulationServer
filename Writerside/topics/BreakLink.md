# BreakLink

## Interface

|                                                                             | interface        |     |
|-----------------------------------------------------------------------------|------------------|-----|
| `void UpdateSpringsForTornParticles(const std::vector<int>& tornParticles)` | `torn particles` |     |
|                                                                             |                  |     |
|                                                                             |                  |     |
|                                                                             |                  |     |
|                                                                             |                  |     |

## FleX tearing demo

首先在Initialize()中定义了布料网格,并生成了弹簧,并且复制给了g_buffers

```C++
//复制弹簧索引到g_buffers的springIndices数组中
g_buffers->springIndices.assign(mCloth->springIndices, mCloth->numSprings * 2);
g_buffers->springStiffness.assign(mCloth->springCoefficients, mCloth->numSprings);
g_buffers->springLengths.assign(mCloth->springRestLengths, mCloth->numSprings);
```

在Update中更新了弹簧

```C++
// update constraints
g_buffers->springIndices.assign(mCloth->springIndices, mCloth->numSprings * 2);
g_buffers->springStiffness.assign(mCloth->springCoefficients, mCloth->numSprings);
g_buffers->springLengths.assign(mCloth->springRestLengths, mCloth->numSprings);
```

可是这个是结果,不是弹簧的断裂更新过程.

## SpineEndoscope

在TetraSoftTearing.h中,创建了Spring

```C++
if (linkRadius > 0.0f) {
      std::vector<int> springIndices;
      std::vector<float> springLengths;
      std::vector<float> springStiffness;

      // create links between particles
      int numLinks = CreateLinks(
          relativeVertices, // the array of Vec3 representing vertices positions
          num_mesh_vertices,// the number of vertices
          tetraIndices,     // the tetrahedron indices
          springIndices,    // output for spring indices
          springLengths,    // output for spring lengths
          springStiffness,  // output for spring stiffness
          linkRadius,       // radius within which to create links
          linkStiffness     // stiffness of the links);
          );
      // assign links
      if (numLinks) {
        tearable_asset->springIndices = new int[numLinks * 2];
        memcpy(tearable_asset->springIndices, &springIndices[0], sizeof(int) * springIndices.size());

        tearable_asset->springCoefficients = new float[numLinks];
        memcpy(tearable_asset->springCoefficients, &springStiffness[0], sizeof(float) * numLinks);

        tearable_asset->springRestLengths = new float[numLinks];
        memcpy(tearable_asset->springRestLengths, &springLengths[0], sizeof(float) * numLinks);

        tearable_asset->numSprings = numLinks;
      }
```

{collapsible="true"}

在TearableTetraSoftBody.h中,做了赋值弹簧的操作.

```C++
// add link data to the solver
for (int i = 0; i < asset->numSprings; ++i) {
    g_buffers->springIndices.push_back(asset->springIndices[i * 2 + 0] + m_particleOffset);
    g_buffers->springIndices.push_back(asset->springIndices[i * 2 + 1] + m_particleOffset);

    g_buffers->springStiffness.push_back(asset->springCoefficients[i]);
    g_buffers->springLengths.push_back(asset->springRestLengths[i]);
}
```

in `release without debugging` mode

| linkRadius | minimum radius | spring count | frame | stiffness | penetration      |
|------------|----------------|--------------|-------|-----------|------------------|
| 2          | 0.1            |              |       |           |                  |
|            | 0.2            |              |       |           |                  |
|            | 0.5            | 2851352      | 33ms  | 不错        | 不错               |
|            | 1.0            | 2455777      | 31ms  | 不错        | 会穿一点点吧           |
|            | 1.5            | 1541173      | 26ms  | 还行吧       | 有一点点穿            |
|            | 1.75           | 832745       | 22ms  | 凑合,比1.5略差 | 比1.5穿的多一些,但是效果还在 |
| 3          | 1.75           | 6476138      | 53ms  | 不太好       | 很穿               |
|            | 2.0            | 4931021      | 45ms  | 不太好       | 很穿               |
| 4          |                |              |       |           |                  |
|            |                |              |       |           |                  |













