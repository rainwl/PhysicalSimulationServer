# CodeAna

## SampleMesh

```C++
// creates mesh interior and surface sample points and clusters them into particles
void SampleMesh(const Vec3 *vertices, int numVertices, const int *indices, int numIndices, float radius,
                float volumeSampling, float surfaceSampling, std::vector<Vec3> &outPositions) {
  Vec3 meshLower(FLT_MAX);
  Vec3 meshUpper(-FLT_MAX);

  for (int i = 0; i < numVertices; ++i) {
    meshLower = Min(meshLower, vertices[i]);
    meshUpper = Max(meshUpper, vertices[i]);
  }

  std::vector<Vec3> samples;

  if (volumeSampling > 0.0f) {
    // recompute expanded edges
    Vec3 edges = meshUpper - meshLower;

    // use a higher resolution voxelization as a basis for the particle decomposition
    float spacing = radius / volumeSampling;

    // tweak spacing to avoid edge cases for particles laying on the boundary
    // just covers the case where an edge is a whole multiple of the spacing.
    float spacingEps = spacing * (1.0f - 1e-4f);

    // make sure to have at least one particle in each dimension
    int dx , dy , dz;
    dx = spacing > edges.x ? 1 : int(edges.x / spacingEps);
    dy = spacing > edges.y ? 1 : int(edges.y / spacingEps);
    dz = spacing > edges.z ? 1 : int(edges.z / spacingEps);

    int maxDim = max(max(dx, dy), dz);

    // expand border by two voxels to ensure adequate sampling at edges
    meshLower -= 2.0f * Vec3(spacing);
    meshUpper += 2.0f * Vec3(spacing);
    maxDim += 4;

    vector<uint32_t> voxels(maxDim * maxDim * maxDim);

    // we shift the voxelization bounds so that the voxel centers
    // lie symmetrically to the center of the object. this reduces the 
    // chance of missing features, and also better aligns the particles
    // with the mesh
    Vec3 meshOffset;
    meshOffset.x = 0.5f * (spacing - (edges.x - (dx - 1) * spacing));
    meshOffset.y = 0.5f * (spacing - (edges.y - (dy - 1) * spacing));
    meshOffset.z = 0.5f * (spacing - (edges.z - (dz - 1) * spacing));
    meshLower -= meshOffset;

    ::Voxelize(vertices, numVertices, indices, numIndices, maxDim, maxDim, maxDim, &voxels[0], meshLower,
               meshLower + Vec3(maxDim * spacing));
    // Voxelize(vertices, numVertices, indices, numIndices, maxDim, maxDim, maxDim, &voxels[0], meshLower,
    //          meshLower + Vec3(maxDim * spacing));

    // sample interior
    for (int x = 0; x < maxDim; ++x) {
      for (int y = 0; y < maxDim; ++y) {
        for (int z = 0; z < maxDim; ++z) {
          const int index = z * maxDim * maxDim + y * maxDim + x;

          // if voxel is marked as occupied the add a particle
          if (voxels[index]) {
            Vec3 position = meshLower + spacing * Vec3(float(x) + 0.5f, float(y) + 0.5f,
                                                       float(z) + 0.5f);

            // normalize the sdf value and transform to world scale
            samples.push_back(position);
          }
        }
      }
    }
  }

  if (surfaceSampling > 0.0f) {
    // sample vertices
    for (int i = 0; i < numVertices; ++i) samples.push_back(vertices[i]);

    // random surface sampling (non-uniform)
    const int numSamples = int(50000 * surfaceSampling);
    const int numTriangles = numIndices / 3;

    RandInit();

    for (int i = 0; i < numSamples; ++i) {
      int t = Rand() % numTriangles;
      float u = Randf();
      float v = Randf() * (1.0f - u);
      float w = 1.0f - u - v;

      int a = indices[t * 3 + 0];
      int b = indices[t * 3 + 1];
      int c = indices[t * 3 + 2];

      Vec3 p = vertices[a] * u + vertices[b] * v + vertices[c] * w;

      samples.push_back(p);
    }
  }

  std::vector<int> clusterIndices;
  std::vector<int> clusterOffsets;
  std::vector<Vec3> clusterPositions;
  std::vector<float> priority(samples.size());

  // cluster mesh sample points into actual particles
  CreateClusters(&samples[0], &priority[0], int(samples.size()), clusterOffsets, clusterIndices, outPositions,
                 radius);
}
```
{collapsible="true" collapsed-title="SampleMesh"}

该代码是一个函数`SampleMesh`，用于对给定的网格进行采样并生成粒子群。

函数接受以下参数：
- `vertices`：包含网格顶点坐标的数组。
- `numVertices`：网格顶点的数量。
- `indices`：包含网格三角形索引的数组。
- `numIndices`：网格三角形索引的数量。
- `radius`：粒子的半径。
- `volumeSampling`：体积采样参数。如果大于0，将在网格内进行体积采样。
- `surfaceSampling`：表面采样参数。如果大于0，将在网格表面进行非均匀采样。
- `outPositions`：存储生成的粒子位置的向量。

函数的主要步骤如下：

1. 计算网格的边界框（`meshLower`和`meshUpper`）。
2. 如果`volumeSampling`大于0，进行体积采样：
    - 计算扩展后的边界框大小。
    - 根据粒子的半径和体积采样参数计算体素的间距（`spacing`）。
    - 根据间距和网格边界调整体素的数量（`dx`、`dy`、`dz`）。
    - 扩展网格边界框并增加体素数量，确保在边界处进行足够的采样。
    - 创建一个大小为`maxDim * maxDim * maxDim`的体素数组（`voxels`）。
    - 将网格体素化，并将结果存储在体素数组中。
    - 遍历体素数组，如果体素被标记为占用，则将对应的粒子位置添加到采样数组（`samples`）中。
3. 如果`surfaceSampling`大于0，进行表面采样：
    - 将网格顶点添加到采样数组中。
    - 根据表面采样参数计算需要采样的三角形数量（`numSamples`）。
    - 随机选择三角形并在其上进行重心坐标插值，得到一个表面采样点，并将其添加到采样数组中。
4. 创建聚类：
    - 创建一些用于存储聚类信息的数组（`clusterIndices`、`clusterOffsets`、`clusterPositions`）。
    - 创建一个优先级数组（`priority`），与采样数组大小相同。
    - 调用`CreateClusters`函数，将采样数组中的点聚类成实际的粒子，并将结果存储在`outPositions`中。

总的来说，这段代码通过对网格进行体积采样和表面采样，生成一组粒子位置，然后将这些位置聚类成实际的粒子。