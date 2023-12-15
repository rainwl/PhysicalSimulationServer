# imp

|   | todo                                     | status           |
|---|------------------------------------------|------------------|
| 0 | understand link and cluster function     |                  |
| 1 | change link to tetra edge                |                  |
| 2 | explore g_params relative to cluster     | [processing](#2) |
| 3 | understand constraint between clusters   |                  |
| 4 | update clusters and links                |                  |
| 5 | understand soft body generate            | [processing](#5) |
| 6 | Fixed project to using on other computer |                  |
| 7 | NvFleXParams                             |                  |

## 5 {collapsible="true"}

```C++
void CreateSoftBody(SoftBody::Instance instance, int group = 0)
{
    Mesh *mesh = ImportMesh(GetFilePathByPlatform(instance.mFile).c_str());
    mesh->Normalize();
    mesh->Transform(TranslationMatrix(Point3(instance.mTranslation)) * ScaleMatrix(instance.mScale * 0.1f));
    
    // create soft body definition
    NvFlexExtAsset *asset = NvFlexExtCreateSoftFromMesh(
        (float *)&renderingInstance.mMesh->m_positions[0],
        renderingInstance.mMesh->m_positions.size(),
        (int *)&renderingInstance.mMesh->m_indices[0],
        renderingInstance.mMesh->m_indices.size(),
        0.1f,
        instance.mVolumeSampling,
        instance.mSurfaceSampling,
        instance.mClusterSpacing * 0.1f,
        instance.mClusterRadius * 0.1f,
        instance.mClusterStiffness,
        instance.mLinkRadius * 0.1f,
        instance.mLinkStiffness,
        instance.mGlobalStiffness,
        instance.mClusterPlasticThreshold,
        instance.mClusterPlasticCreep);

  
    const int particleOffset = g_buffers->positions.size();
    const int indexOffset = g_buffers->rigidOffsets.back();

    // add particle data to solver
    for (int i = 0; i < asset->numParticles; ++i)
    {
        g_buffers->positions.push_back(&asset->particles[i * 4]);
        g_buffers->velocities.push_back(0.0f);

        const int phase = NvFlexMakePhase(group, eNvFlexPhaseSelfCollide | eNvFlexPhaseSelfCollideFilter);
        g_buffers->phases.push_back(phase);
    }

    // add shape data to solver
    for (int i = 0; i < asset->numShapeIndices; ++i)
        g_buffers->rigidIndices.push_back(asset->shapeIndices[i] + particleOffset);

    for (int i = 0; i < asset->numShapes; ++i)
    {
        g_buffers->rigidOffsets.push_back(asset->shapeOffsets[i] + indexOffset);
        g_buffers->rigidTranslations.push_back(Vec3(&asset->shapeCenters[i * 3]));
        g_buffers->rigidRotations.push_back(Quat());
        g_buffers->rigidCoefficients.push_back(asset->shapeCoefficients[i]);
    }

    // add link data to the solver 
    for (int i = 0; i < asset->numSprings; ++i)
    {
        g_buffers->springIndices.push_back(asset->springIndices[i * 2 + 0]);
        g_buffers->springIndices.push_back(asset->springIndices[i * 2 + 1]);

        g_buffers->springStiffness.push_back(asset->springCoefficients[i]);
        g_buffers->springLengths.push_back(asset->springRestLengths[i]);
    }

    NvFlexExtDestroyAsset(asset);
}
```

{collapsible="true" collapsed-title="void CreateSoftBody(SoftBody::Instance instance, int group = 0)"}

Among it, there is an important method:
```C++
NvFlexExtAsset* NvFlexExtCreateSoftFromMesh(const float* vertices, int numVertices, const int* indices, int numIndices, float particleSpacing, float volumeSampling, float surfaceSampling, float clusterSpacing, float clusterRadius, float clusterStiffness, float linkRadius, float linkStiffness, float globalStiffness, float clusterPlasticThreshold, float clusterPlasticCreep)
{
	// Switch to relative coordinates by computing the mean position of the vertices and subtracting the result from every vertex position
	// The increased precision will prevent ghost forces caused by inaccurate center of mass computations
	Vec3 meshOffset(0.0f);
	for (int i = 0; i < numVertices; i++)
	{
		meshOffset += ((Vec3*)vertices)[i];
	}
	meshOffset /= float(numVertices);

	Vec3* relativeVertices = new Vec3[numVertices];
	for (int i = 0; i < numVertices; i++)
	{
		relativeVertices[i] += ((Vec3*)vertices)[i] - meshOffset;
	}

	// construct asset definition
	NvFlexExtAsset* asset = new NvFlexExtAsset();

	// create particle sampling
	std::vector<Vec3> samples;
	SampleMesh(relativeVertices, numVertices, indices, numIndices, particleSpacing, volumeSampling, surfaceSampling, samples);

	delete[] relativeVertices;

	const int numParticles = int(samples.size());	

	std::vector<int> clusterIndices;
	std::vector<int> clusterOffsets;
	std::vector<Vec3> clusterPositions;
	std::vector<float> clusterCoefficients;
	std::vector<float> clusterPlasticThresholds;
	std::vector<float> clusterPlasticCreeps;

	// priority (not currently used)
	std::vector<float> priority(numParticles);
	for (int i = 0; i < int(priority.size()); ++i)
		priority[i] = 0.0f;

	// cluster particles into shape matching groups
	int numClusters = CreateClusters(&samples[0], &priority[0], int(samples.size()), clusterOffsets, clusterIndices, clusterPositions, clusterSpacing, clusterRadius);
	
	// assign all clusters the same stiffness 
	clusterCoefficients.resize(numClusters, clusterStiffness);

	if (clusterPlasticCreep) 
	{
		// assign all clusters the same plastic threshold 
		clusterPlasticThresholds.resize(numClusters, clusterPlasticThreshold);

		// assign all clusters the same plastic creep 
		clusterPlasticCreeps.resize(numClusters, clusterPlasticCreep);
	}

	// create links between clusters 
	if (linkRadius > 0.0f)
	{		
		std::vector<int> springIndices;
		std::vector<float> springLengths;
		std::vector<float> springStiffness;

		// create links between particles
		int numLinks = CreateLinks(&samples[0], int(samples.size()), springIndices, springLengths, springStiffness, linkRadius, linkStiffness);

		// assign links
		if (numLinks)
		{			
			asset->springIndices = new int[numLinks * 2];
			memcpy(asset->springIndices, &springIndices[0], sizeof(int)*springIndices.size());

			asset->springCoefficients = new float[numLinks];
			memcpy(asset->springCoefficients, &springStiffness[0], sizeof(float)*numLinks);

			asset->springRestLengths = new float[numLinks];
			memcpy(asset->springRestLengths, &springLengths[0], sizeof(float)*numLinks);

			asset->numSprings = numLinks;
		}
	}

	// add an additional global cluster with stiffness = globalStiffness
	if (globalStiffness > 0.0f)
	{
		numClusters += 1;
		clusterCoefficients.push_back(globalStiffness);
		if (clusterPlasticCreep) 
		{
			clusterPlasticThresholds.push_back(clusterPlasticThreshold);
			clusterPlasticCreeps.push_back(clusterPlasticCreep);
		}

		for (int i = 0; i < numParticles; ++i)
		{
			clusterIndices.push_back(i);
		}
		clusterOffsets.push_back((int)clusterIndices.size());

		// the mean of the global cluster is the mean of all particles
		Vec3 globalMeanPosition(0.0f);
		for (int i = 0; i < numParticles; ++i)
		{
			globalMeanPosition += samples[i];
		}
		globalMeanPosition /= float(numParticles);
		clusterPositions.push_back(globalMeanPosition);
	}

	// Switch back to absolute coordinates by adding meshOffset to the centers of mass and to each particle positions
	for (int i = 0; i < numParticles; ++i)
	{
		 samples[i] += meshOffset;
	}
	for (int i = 0; i < numClusters; ++i)
	{
		clusterPositions[i] += meshOffset;
	}

	// assign particles
	asset->particles = new float[numParticles * 4];
	asset->numParticles = numParticles;
	asset->maxParticles = numParticles;

	for (int i = 0; i < numParticles; ++i)
	{
		asset->particles[i*4+0] = samples[i].x;
		asset->particles[i*4+1] = samples[i].y;
		asset->particles[i*4+2] = samples[i].z;
		asset->particles[i*4+3] = 1.0f;
	}

	// assign shapes
	asset->shapeIndices = new int[clusterIndices.size()];
	memcpy(asset->shapeIndices, &clusterIndices[0], sizeof(int)*clusterIndices.size());

	asset->shapeOffsets = new int[numClusters];
	memcpy(asset->shapeOffsets, &clusterOffsets[0], sizeof(int)*numClusters);

	asset->shapeCenters = new float[numClusters*3];
	memcpy(asset->shapeCenters, &clusterPositions[0], sizeof(float)*numClusters*3);

	asset->shapeCoefficients = new float[numClusters];
	memcpy(asset->shapeCoefficients, &clusterCoefficients[0], sizeof(float)*numClusters);

	if (clusterPlasticCreep) 
	{
		asset->shapePlasticThresholds = new float[numClusters];
		memcpy(asset->shapePlasticThresholds, &clusterPlasticThresholds[0], sizeof(float)*numClusters);

		asset->shapePlasticCreeps = new float[numClusters];
		memcpy(asset->shapePlasticCreeps, &clusterPlasticCreeps[0], sizeof(float)*numClusters);
	}
	else
	{
		asset->shapePlasticThresholds = NULL;
		asset->shapePlasticCreeps = NULL;
	}

	asset->numShapeIndices = int(clusterIndices.size());
	asset->numShapes = numClusters;

	return asset;
}
```

{collapsible="true" collapsed-title="NvFlexExtAsset* NvFlexExtCreateSoftFromMesh"}

Explanation
: First, the average position of all vertices is calculated as the center point of the local coordinate system
: At the same time, the new positions of all points in the local coordinate system are calculated and stored
: Then we sample all the points in the new local coordinate system
: The current priorities are all 0
: Then clusters are created based on the sampled points, and all clusters are assigned the same stiffness
: At the same time, links are created based on the sampled points, and the links are created between particles
: If there is a global stiffness, a global cluster is created and a stiffness is assigned

### Specific method interpretation

**SampleMesh**

```C++
// creates mesh interior and surface sample points and clusters them into particles
void SampleMesh(const Vec3* vertices, int numVertices, const int* indices, int numIndices, float radius, float volumeSampling, float surfaceSampling, std::vector<Vec3>& outPositions)
{
#pragma region Bounding Box
	Vec3 meshLower(FLT_MAX);
	Vec3 meshUpper(-FLT_MAX);

	for (int i = 0; i < numVertices; ++i)
	{
		meshLower = Min(meshLower, vertices[i]);
		meshUpper = Max(meshUpper, vertices[i]);
	}
#pragma endregion
	
	std::vector<Vec3> samples;

	if (volumeSampling > 0.0f)
	{
		// recompute expanded edges
		Vec3 edges = meshUpper - meshLower;

		// use a higher resolution voxelization as a basis for the particle decomposition
		float spacing = radius / volumeSampling;

		// tweak spacing to avoid edge cases for particles laying on the boundary
		// just covers the case where an edge is a whole multiple of the spacing.
		float spacingEps = spacing*(1.0f - 1e-4f);

		// make sure to have at least one particle in each dimension
		int dx, dy, dz;
		dx = spacing > edges.x ? 1 : int(edges.x / spacingEps);
		dy = spacing > edges.y ? 1 : int(edges.y / spacingEps);
		dz = spacing > edges.z ? 1 : int(edges.z / spacingEps);

		int maxDim = max(max(dx, dy), dz);

		// expand border by two voxels to ensure adequate sampling at edges
		meshLower -= 2.0f*Vec3(spacing);
		meshUpper += 2.0f*Vec3(spacing);
		maxDim += 4;

		vector<uint32_t> voxels(maxDim*maxDim*maxDim);

		// we shift the voxelization bounds so that the voxel centers
		// lie symmetrically to the center of the object. this reduces the 
		// chance of missing features, and also better aligns the particles
		// with the mesh
		Vec3 meshOffset;
		meshOffset.x = 0.5f * (spacing - (edges.x - (dx - 1)*spacing));
		meshOffset.y = 0.5f * (spacing - (edges.y - (dy - 1)*spacing));
		meshOffset.z = 0.5f * (spacing - (edges.z - (dz - 1)*spacing));
		meshLower -= meshOffset;

		Voxelize(vertices, numVertices, indices, numIndices, maxDim, maxDim, maxDim, &voxels[0], meshLower, meshLower + Vec3(maxDim*spacing));

		// sample interior
		for (int x = 0; x < maxDim; ++x)
		{
			for (int y = 0; y < maxDim; ++y)
			{
				for (int z = 0; z < maxDim; ++z)
				{
					const int index = z*maxDim*maxDim + y*maxDim + x;

					// if voxel is marked as occupied the add a particle
					if (voxels[index])
					{
						Vec3 position = meshLower + spacing*Vec3(float(x) + 0.5f, float(y) + 0.5f, float(z) + 0.5f);

						// normalize the sdf value and transform to world scale
						samples.push_back(position);
					}
				}
			}
		}
	}

	if (surfaceSampling > 0.0f)
	{
		// sample vertices
		for (int i = 0; i < numVertices; ++i)
			samples.push_back(vertices[i]);

		// random surface sampling (non-uniform)
		const int numSamples = int(50000 * surfaceSampling);
		const int numTriangles = numIndices/3;

		RandInit();

		for (int i = 0; i < numSamples; ++i)
		{
			int t = Rand() % numTriangles;
			float u = Randf();
			float v = Randf()*(1.0f - u);
			float w = 1.0f - u - v;

			int a = indices[t*3 + 0];
			int b = indices[t*3 + 1];
			int c = indices[t*3 + 2];

			Vec3 p = vertices[a] * u + vertices[b] * v + vertices[c] * w;

			samples.push_back(p);
		}
	}

	std::vector<int> clusterIndices;
	std::vector<int> clusterOffsets;
	std::vector<Vec3> clusterPositions;
	std::vector<float> priority(samples.size());

	// cluster mesh sample points into actual particles
	CreateClusters(&samples[0], &priority[0], int(samples.size()), clusterOffsets, clusterIndices, outPositions, radius);
}
```

{collapsible="true" collapsed-title="void SampleMesh"}

Is designed to create a set of sample points on the surface and within the volume of a mesh and then cluster these
sample points into groups that can be considered particles.

`Declarations`

- `radius` is the desired radius around each sample point to be considered for clustering.
- `volumeSampling` and `surfaceSampling` are multipliers that control the density of sampling within the volume and on
  the surface of the mesh, respectively.
- `outPositions` is a vector of `Vec3` that will store the final positions of particles after clustering.

#### Volume Sampling

If `volumeSampling` is greater than zero, the function will sample points within the volume of the mesh:

- It recalculates the size of the mesh by subtracting the lower bound from the upper bound.
- Determines the spacing between sample points based on the given `radius` and `volumeSampling` factor, with a small
  epsilon to prevent boundary issues.
- Ensures there's at least one sample in each dimension and calculates the number of samples along each
  axis (`dx`, `dy`, `dz`).
- Expands the mesh bounds and recalculates the number of samples needed to cover the expanded volume.
- Initializes a vector to represent a 3D grid (`voxels`) for voxelization.
- Adjusts `meshLower` further by an offset to ensure symmetric voxel centers relative to the mesh center.
- Calls `Voxelize`, a function not shown here, which likely fills the `voxels` grid to indicate which voxels are inside
  the mesh.
- Iterates over the voxel grid, adding a sample point to `samples` for each occupied voxel.

#### Surface Sampling

If `surfaceSampling` is greater than zero, the function adds surface samples:

- First, it pushes all vertex positions as surface samples.
- It calculates the number of random surface samples based on the `surfaceSampling` factor.
- Initializes random number generation with `RandInit`.
- Samples random points on the mesh's surface by randomly choosing triangles and then randomly selecting points within
  those triangles using barycentric coordinates (u, v, w).

#### Clustering

Finally, the sample points created from the volume and surface of the mesh are clustered into particles:

- Initializes vectors for clustering (`clusterIndices`, `clusterOffsets`, `clusterPositions`) and a vector for priority
  values.
- Calls `CreateClusters`, a function not shown here, which likely groups the sample points into clusters based on
  proximity and possibly the priority values, outputting the final particle positions into `outPositions`.

**CreateClusters**

```C++
int CreateClusters(Vec3* particles, const float* priority, int numParticles, std::vector<int>& outClusterOffsets, std::vector<int>& outClusterIndices, std::vector<Vec3>& outClusterPositions, float radius, float smoothing = 0.0f)
{
	std::vector<Seed> seeds;
	std::vector<Cluster> clusters;

	// flags a particle as belonging to at least one cluster
	std::vector<bool> used(numParticles, false);

	// initialize seeds
	for (int i = 0; i < numParticles; ++i)
	{
		Seed s;
		s.index = i;
		s.priority = priority[i];

		seeds.push_back(s);
	}

	// sort seeds on priority
	std::stable_sort(seeds.begin(), seeds.end());

	SweepAndPrune sap(particles, numParticles);

	while (seeds.size())
	{
		// pick highest unused particle from the seeds list
		Seed seed = seeds.back();
		seeds.pop_back();

		if (!used[seed.index])
		{
			Cluster c;

			sap.QuerySphere(Vec3(particles[seed.index]), radius, c.indices);
			
			// mark overlapping particles as used so they are removed from the list of potential cluster seeds
			for (int i=0; i < int(c.indices.size()); ++i)
				used[c.indices[i]] = true;

			c.mean = CalculateMean(particles, &c.indices[0], int(c.indices.size()));

			clusters.push_back(c);
		}
	}

	if (smoothing > 0.0f)
	{
		for (int i = 0; i < int(clusters.size()); ++i)
		{
			Cluster& c = clusters[i];

			// clear cluster indices
			c.indices.resize(0);

			// calculate cluster particles using cluster mean and smoothing radius
			sap.QuerySphere(c.mean, smoothing, c.indices);

			c.mean = CalculateMean(particles, &c.indices[0], int(c.indices.size()));
		}
	}

	// write out cluster indices
	int count = 0;

	for (int c = 0; c < int(clusters.size()); ++c)
	{
		const Cluster& cluster = clusters[c];

		const int clusterSize = int(cluster.indices.size());

		// skip empty clusters
		if (clusterSize)
		{
			// write cluster indices
			for (int i = 0; i < clusterSize; ++i)
				outClusterIndices.push_back(cluster.indices[i]);

			// write cluster offset
			outClusterOffsets.push_back(int(outClusterIndices.size()));

			// write center
			outClusterPositions.push_back(cluster.mean);

			++count;
		}
	}

	return count;
}

```

{collapsible="true" collapsed-title="int CreateClusters"}

The code given seems to be a part of a particle clustering system, where `particles` are gathered into clusters based on
their proximity to each other and some priority. The function `CreateClusters` is written in C++ and takes several
parameters to perform its task:

1. `Vec3* particles`: A pointer to an array of `Vec3` structures, which likely represent the 3D positions of particles.
2. `const float* priority`: A pointer to an array of floats that represent the priority of each particle. Particles with
   a higher priority will be used as seeds for clusters first.
3. `int numParticles`: The number of particles in the `particles` array.
4. `std::vector<int>& outClusterOffsets`: A reference to a vector that will store the offsets of each cluster in
   the `outClusterIndices` vector.
5. `std::vector<int>& outClusterIndices`: A reference to a vector that will store the indices of particles that belong
   to the clusters.
6. `std::vector<Vec3>& outClusterPositions`: A reference to a vector that will store the positions of the centers of
   each cluster.
7. `float radius`: The radius within which particles will be considered to belong to the same cluster.
8. `float smoothing = 0.0f`: An optional parameter that defines a smoothing radius for cluster calculation.

The function returns an integer representing the number of clusters created.

Here's a step-by-step explanation of what the function does:

1. **Initialization:**

- It initializes a `std::vector<Seed>` to store seed data, which includes the index and priority of each particle.
- It initializes a `std::vector<Cluster>` to store the resulting clusters.
- It creates a `std::vector<bool>` to flag whether a particle has been used in a cluster.

2. **Seed Sorting:**

- It populates the `seeds` vector with the index and priority of each particle and then sorts this vector based on the
  priority, with higher priority seeds at the end.

3. **Sweep And Prune:**

- An instance of `SweepAndPrune` is created to efficiently find particles within a certain radius.

4. **Seed Processing:**

- The function enters a loop that continues as long as there are seeds left to process.
- It selects the seed with the highest priority that hasn't been used yet and queries the `SweepAndPrune` instance to
  find all particles within the given `radius` of the seed particle. These particles are added to a new cluster.

5. **Marking Particles:**

- The function marks all particles found within the cluster's radius as used to ensure they are not included in any
  further clusters as seed particles.

6. **Cluster Center Calculation:**

- It calculates the mean (center) position of each cluster using the `CalculateMean` function.

7. **Optional Smoothing:**

- If a `smoothing` radius is provided (greater than 0.0f), the function recalculates each cluster's particles based on
  this smoothing radius and recalculates the mean.

8. **Output Generation:**

- The function then iterates through the clusters and outputs the indices of the particles in each cluster
  to `outClusterIndices`.
- It records the offset into `outClusterOffsets` for the start of each cluster in `outClusterIndices`.
- It also pushes the calculated mean positions of each cluster to `outClusterPositions`.

9. **Return Value:**

- Finally, the function returns the count of clusters created.

**CreateLinks**

```C++
// creates distance constraints between particles within some radius
int CreateLinks(const Vec3* particles, int numParticles, std::vector<int>& outSpringIndices, std::vector<float>& outSpringLengths, std::vector<float>& outSpringStiffness, float radius, float stiffness = 1.0f)
{
	int count = 0;

	std::vector<int> neighbors;
	SweepAndPrune sap(particles, numParticles);

	for (int i = 0; i < numParticles; ++i)
	{
		neighbors.resize(0);

		sap.QuerySphere(Vec3(particles[i]), radius, neighbors);

		for (int j = 0; j < int(neighbors.size()); ++j)
		{
			const int nj = neighbors[j];

			if (nj != i)
			{
				outSpringIndices.push_back(i);
				outSpringIndices.push_back(nj);
				outSpringLengths.push_back(Length(Vec3(particles[i]) - Vec3(particles[nj])));
				outSpringStiffness.push_back(stiffness);

				++count;
			}
		}
	}

	return count;
}
```

{ collapsible="true" collapsed-title="int CreateLinks"}

`CreateLinks` 函数的目的是在一定半径范围内的粒子之间创建距离约束，这些在物理模拟中通常被称为“弹簧”。弹簧定义了粒子之间的相互作用方式，当运行物理模拟时，粒子将如何移动和反应。下面是参数和函数的中文解释：

1. `const Vec3* particles`：指向一个包含 `Vec3` 结构的数组的指针，这些结构代表粒子的三维位置。
2. `int numParticles`：数组 `particles` 中粒子的数量。
3. `std::vector<int>& outSpringIndices`：一个引用，指向一个向量，该向量将存储有弹簧连接的粒子对的索引。
4. `std::vector<float>& outSpringLengths`：一个引用，指向一个向量，该向量将存储弹簧的原始长度。
5. `std::vector<float>& outSpringStiffness`：一个引用，指向一个向量，该向量将存储每个弹簧的刚度值。
6. `float radius`：定义了创建弹簧的粒子间的最大距离半径。
7. `float stiffness = 1.0f`：一个可选参数，定义了弹簧的刚度，默认值为 1.0f。

函数的处理流程如下：

1. **初始化：**
    - 初始化一个局部整数 `count` 来记录创建的弹簧数。
    - 声明一个 `std::vector<int> neighbors` 向量来存储邻近粒子的索引。

2. **空间查询：**
    - 使用 `particles` 数组和 `numParticles` 作为输入创建 `SweepAndPrune` 的实例（代码片段中没有定义）。这个对象用于高效地找出在一定半径内的邻近粒子。

3. **寻找邻居：**
    - 通过 for 循环迭代每个粒子。
    - 每次迭代都清空 `neighbors` 向量。
    - 调用 `sap.QuerySphere`，传入当前粒子的位置和指定的 `radius`，找到所有邻近粒子，并将它们的索引存储在 `neighbors` 中。

4. **创建弹簧：**
    - 迭代找到的每个邻居粒子。
    - 检查邻居粒子的索引 `nj` 是否不同于当前粒子的索引 `i`，以确保不会将粒子与自身连接。
    - 将当前粒子的索引 `i` 和邻居粒子的索引 `nj` 添加到 `outSpringIndices` 中。这些索引对代表了弹簧的两端。
    - 计算弹簧的原始长度，即两个粒子之间的当前距离（`Length(Vec3(particles[i]) - Vec3(particles[nj]))`
      ），并将这个值添加到 `outSpringLengths` 中。
    - 对于每个弹簧，将 `stiffness` 值添加到 `outSpringStiffness` 中。

5. **计数弹簧：**
    - 对于每个有效创建的弹簧，`count` 加 1。

6. **返回值：**
    - 返回总共 `count` 数的弹簧数。

该函数假设在一个物理环境中，通过弹簧连接粒子来模拟弹性等物理属性。`Length`
函数用于计算弹簧的原始长度，它应该能够计算两点在三维空间中的欧几里得距离。每个弹簧由两个索引（代表它连接的粒子）、它的原始长度和它的刚度值定义。

请注意该函数没有阻止如果粒子 A 在粒子 B 的邻域中被发现，然后稍后在粒子 A 的邻域中又发现了粒子
B，这种情况下重复创建弹簧。根据应用程序的不同，可能需要额外的检查以确保每个弹簧只创建一次。

The `CreateLinks` function creates distance constraints, often called springs in physical simulation contexts, between
particles that are within a certain radius from each other. These springs determine how particles will interact with
each other when a physics simulation is run. Here's an explanation of the parameters and the function:

1. `const Vec3* particles`: A pointer to an array of `Vec3` structures, which represent the 3D positions of particles.
2. `int numParticles`: The number of particles in the `particles` array.
3. `std::vector<int>& outSpringIndices`: A reference to a vector that will store the indices of particle pairs that have
   springs between them.
4. `std::vector<float>& outSpringLengths`: A reference to a vector that will store the rest lengths of the springs.
5. `std::vector<float>& outSpringStiffness`: A reference to a vector that will store the stiffness values for each
   spring.
6. `float radius`: The radius within which if other particles are found relative to a given particle, a spring will be
   created between them.
7. `float stiffness = 1.0f`: An optional parameter that defines the stiffness of the springs; defaulting to 1.0f.

Here's what the function does, step by step:

1. **Initialization:**
    - Initializes a local integer `count` to keep track of the number of springs created.
    - Declares a `std::vector<int> neighbors` to hold the indices of neighboring particles.

2. **Spatial Query:**
    - Creates an instance of `SweepAndPrune` (not defined in the snippet) with the `particles` array and `numParticles`
      as input. This object is used to efficiently find neighboring particles within a certain radius.

3. **Finding Neighbors:**
    - Iterates through each particle using a for loop.
    - Clears the `neighbors` vector for each particle.
    - Calls `sap.QuerySphere` with the current particle's position and the specified `radius` to find all neighboring
      particles and stores their indices in `neighbors`.

4. **Creating Springs:**
    - Iterates through each neighbor found for the current particle.
    - Checks if the neighbor's index `nj` is not the same as the current particle's index `i` to ensure that a particle
      is not linked to itself.
    - Adds the current particle's index `i` and the neighbor's index `nj` to `outSpringIndices`. These pairs of indices
      represent the ends of the springs.
    - Calculates the rest length of the spring, which is the current distance between the two
      particles (`Length(Vec3(particles[i]) - Vec3(particles[nj]))`), and adds this value to `outSpringLengths`.
    - Adds the `stiffness` value to `outSpringStiffness` for each spring.

5. **Counting Springs:**
    - Increments `count` for each valid spring created.

6. **Return Value:**
    - Returns the total `count` of springs created.

The function assumes a physics context where springs connect particles to simulate physical properties like elasticity.
The `Length` function, which is used to calculate the spring's rest length, presumably calculates the Euclidean distance
between two points in 3D space. Each spring is defined by two indices (representing the particles it connects), its rest
length, and its stiffness value.

Note that the function does not prevent duplicate springs from being created if particle A is found in the neighborhood
of particle B and then later particle B is found in the neighborhood of particle A. Depending on the application,
additional checks might be needed to ensure each spring is only created once.

**NvFlexExtCreateSoftFromMesh**

```C++
NvFlexExtAsset* NvFlexExtCreateSoftFromMesh(const float* vertices, int numVertices, const int* indices, int numIndices, float particleSpacing, float volumeSampling, float surfaceSampling, float clusterSpacing, float clusterRadius, float clusterStiffness, float linkRadius, float linkStiffness, float globalStiffness, float clusterPlasticThreshold, float clusterPlasticCreep)
{
	// Switch to relative coordinates by computing the mean position of the vertices and subtracting the result from every vertex position
	// The increased precision will prevent ghost forces caused by inaccurate center of mass computations
	Vec3 meshOffset(0.0f);
	for (int i = 0; i < numVertices; i++)
	{
		meshOffset += ((Vec3*)vertices)[i];
	}
	meshOffset /= float(numVertices);

	Vec3* relativeVertices = new Vec3[numVertices];
	for (int i = 0; i < numVertices; i++)
	{
		relativeVertices[i] += ((Vec3*)vertices)[i] - meshOffset;
	}

	// construct asset definition
	NvFlexExtAsset* asset = new NvFlexExtAsset();

	// create particle sampling
	std::vector<Vec3> samples;
	SampleMesh(relativeVertices, numVertices, indices, numIndices, particleSpacing, volumeSampling, surfaceSampling, samples);

	delete[] relativeVertices;

	const int numParticles = int(samples.size());	

	std::vector<int> clusterIndices;
	std::vector<int> clusterOffsets;
	std::vector<Vec3> clusterPositions;
	std::vector<float> clusterCoefficients;
	std::vector<float> clusterPlasticThresholds;
	std::vector<float> clusterPlasticCreeps;

	// priority (not currently used)
	std::vector<float> priority(numParticles);
	for (int i = 0; i < int(priority.size()); ++i)
		priority[i] = 0.0f;

	// cluster particles into shape matching groups
	int numClusters = CreateClusters(&samples[0], &priority[0], int(samples.size()), clusterOffsets, clusterIndices, clusterPositions, clusterSpacing, clusterRadius);
	
	// assign all clusters the same stiffness 
	clusterCoefficients.resize(numClusters, clusterStiffness);

	if (clusterPlasticCreep) 
	{
		// assign all clusters the same plastic threshold 
		clusterPlasticThresholds.resize(numClusters, clusterPlasticThreshold);

		// assign all clusters the same plastic creep 
		clusterPlasticCreeps.resize(numClusters, clusterPlasticCreep);
	}

	// create links between clusters 
	if (linkRadius > 0.0f)
	{		
		std::vector<int> springIndices;
		std::vector<float> springLengths;
		std::vector<float> springStiffness;

		// create links between particles
		int numLinks = CreateLinks(&samples[0], int(samples.size()), springIndices, springLengths, springStiffness, linkRadius, linkStiffness);

		// assign links
		if (numLinks)
		{			
			asset->springIndices = new int[numLinks * 2];
			memcpy(asset->springIndices, &springIndices[0], sizeof(int)*springIndices.size());

			asset->springCoefficients = new float[numLinks];
			memcpy(asset->springCoefficients, &springStiffness[0], sizeof(float)*numLinks);

			asset->springRestLengths = new float[numLinks];
			memcpy(asset->springRestLengths, &springLengths[0], sizeof(float)*numLinks);

			asset->numSprings = numLinks;
		}
	}

	// add an additional global cluster with stiffness = globalStiffness
	if (globalStiffness > 0.0f)
	{
		numClusters += 1;
		clusterCoefficients.push_back(globalStiffness);
		if (clusterPlasticCreep) 
		{
			clusterPlasticThresholds.push_back(clusterPlasticThreshold);
			clusterPlasticCreeps.push_back(clusterPlasticCreep);
		}

		for (int i = 0; i < numParticles; ++i)
		{
			clusterIndices.push_back(i);
		}
		clusterOffsets.push_back((int)clusterIndices.size());

		// the mean of the global cluster is the mean of all particles
		Vec3 globalMeanPosition(0.0f);
		for (int i = 0; i < numParticles; ++i)
		{
			globalMeanPosition += samples[i];
		}
		globalMeanPosition /= float(numParticles);
		clusterPositions.push_back(globalMeanPosition);
	}

	// Switch back to absolute coordinates by adding meshOffset to the centers of mass and to each particle positions
	for (int i = 0; i < numParticles; ++i)
	{
		 samples[i] += meshOffset;
	}
	for (int i = 0; i < numClusters; ++i)
	{
		clusterPositions[i] += meshOffset;
	}

	// assign particles
	asset->particles = new float[numParticles * 4];
	asset->numParticles = numParticles;
	asset->maxParticles = numParticles;

	for (int i = 0; i < numParticles; ++i)
	{
		asset->particles[i*4+0] = samples[i].x;
		asset->particles[i*4+1] = samples[i].y;
		asset->particles[i*4+2] = samples[i].z;
		asset->particles[i*4+3] = 1.0f;
	}

	// assign shapes
	asset->shapeIndices = new int[clusterIndices.size()];
	memcpy(asset->shapeIndices, &clusterIndices[0], sizeof(int)*clusterIndices.size());

	asset->shapeOffsets = new int[numClusters];
	memcpy(asset->shapeOffsets, &clusterOffsets[0], sizeof(int)*numClusters);

	asset->shapeCenters = new float[numClusters*3];
	memcpy(asset->shapeCenters, &clusterPositions[0], sizeof(float)*numClusters*3);

	asset->shapeCoefficients = new float[numClusters];
	memcpy(asset->shapeCoefficients, &clusterCoefficients[0], sizeof(float)*numClusters);

	if (clusterPlasticCreep) 
	{
		asset->shapePlasticThresholds = new float[numClusters];
		memcpy(asset->shapePlasticThresholds, &clusterPlasticThresholds[0], sizeof(float)*numClusters);

		asset->shapePlasticCreeps = new float[numClusters];
		memcpy(asset->shapePlasticCreeps, &clusterPlasticCreeps[0], sizeof(float)*numClusters);
	}
	else
	{
		asset->shapePlasticThresholds = NULL;
		asset->shapePlasticCreeps = NULL;
	}

	asset->numShapeIndices = int(clusterIndices.size());
	asset->numShapes = numClusters;

	return asset;
}

```

{collapsible="true" collapsed-title="NvFlexExtAsset* NvFlexExtCreateSoftFromMesh"}

这段代码是一个函数的实现，其功能是从一个给定的网格数据中创建一个软体动力学模型，可能用于物理模拟引擎中。以下是对代码段的逐行分析：

首先定义了一个函数`NvFlexExtCreateSoftFromMesh`，它接收了一系列的参数：

- `vertices`：指向网格顶点坐标数组的指针。
- `numVertices`：顶点数组中顶点的数量。
- `indices`：指向网格索引数组的指针，用于定义网格的三角面片。
- `numIndices`：索引数组中索引的数量。
- `particleSpacing`、`volumeSampling`、`surfaceSampling`：这些参数用于控制网格到粒子系统的采样过程。
- `clusterSpacing`、`clusterRadius`、`clusterStiffness`：这些参数用于定义形状匹配群组（Cluster）的创建。
- `linkRadius`、`linkStiffness`：控制粒子之间弹簧连接的创建。
- `globalStiffness`、`clusterPlasticThreshold`、`clusterPlasticCreep`：分别控制全局的刚度，塑性阈值和塑性蠕变。

函数体内的处理流程分为几个主要步骤：

1. 将网格顶点坐标转换为相对坐标，这样做是为了提高精度，防止由于质心计算不准确而产生的幽灵力。

2. 使用`SampleMesh`函数从输入网格中生成粒子采样，这些采样将会用于构建软体动力学模型的粒子系统。

3. 基于采样粒子创建聚类，这些聚类是用于形状匹配的群组。

4. 如果`linkRadius`大于0，将会创建粒子之间的弹簧连接。

5. 如果`globalStiffness`大于0，会添加一个全局的聚类，该聚类包含所有粒子，并且具有全局刚度。

6. 将之前转换为相对坐标的粒子位置和聚类中心位置，转换回绝对坐标。

7. 分配粒子、聚类、连接等数据到一个新创建的`NvFlexExtAsset`结构体中，用于表示创建的软体模型。

8. 返回这个新创建的`NvFlexExtAsset`实例。

这个函数实现了从三维网格到物理模拟中软体模型的转换过程，涉及到了粒子系统的创建、形状匹配聚类的定义，以及粒子间连接的设置。这些都是物理模拟中常见的技术，用于实现如布料、橡胶或其他软体物体的现实世界行为。

## 2 {collapsible="true"}

| item                    | desc                                                                                                                                                                                              |                 |
|-------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-----------------|
| gravity                 | Constant acceleration applied to all particles.                                                                                                                                                   | simulate water? |
| dynamicFriction         | Coefficient of friction used when colliding against shapes.                                                                                                                                       |                 |
| staticFriction          | Coefficient of static friction used when colliding against shapes.                                                                                                                                |                 |
| particleFriction        | Coefficient of friction used when colliding particles.                                                                                                                                            |                 |
| restitution             | Coefficient of restitution used when colliding against shapes, particle collisions are always inelastic.                                                                                          |                 |
| maxSpeed                | The magnitude of particle velocity will be clamped to this value at the end of each step.                                                                                                         |                 |
| maxAcceleration         | The magnitude of particle acceleration will be clamped to this value at the end of each step (limits max velocity change per-second), useful to avoid popping due to large interpenetrations.     |                 |
| shockPropagation        | Artificially decrease the mass of particles based on height from a fixed reference point, this makes stacks and piles converge faster.                                                            |                 |
| dissipation             | Damps particle velocity based on how many particle contacts it has.                                                                                                                               |                 |
| damping                 | Viscous drag force, applies a force proportional, and opposite to the particle velocity.                                                                                                          |                 |
| wind                    | Constant acceleration applied to particles that belong to dynamic triangles, drag needs to be > 0 for wind to affect triangles.                                                                   |                 |
| drag                    | Drag force applied to particles belonging to dynamic triangles, proportional to velocity^2*area in the negative velocity direction.                                                               |                 |
| lift                    | Lift force applied to particles belonging to dynamic triangles, proportional to velocity^2*area in the direction perpendicular to velocity and (if possible), parallel to the plane normal.       |                 |
| surfaceTension          | Controls how strongly particles attempt to minimize surface area, default: 0.0, range: [0.0, +inf].                                                                                               |                 |
| particleCollisionMargin | Increases the radius used during neighbor finding, this is useful if particles are expected to move significantly during a single step to ensure contacts aren't missed on subsequent iterations. |                 |
| shapeCollisionMargin    | Increases the radius used during contact finding against kinematic shapes.                                                                                                                        |                 |
