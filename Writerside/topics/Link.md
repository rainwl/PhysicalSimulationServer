# Link

```C++
int CreateLinks(
const Vec3* particles, 
int numParticles, 
std::vector<int>& outSpringIndices, 
std::vector<float>& outSpringLengths, 
std::vector<float>& outSpringStiffness, 
float radius, 
float stiffness = 1.0f)
```

| **input**        |              | **output**          |                      |
|------------------|--------------|---------------------|----------------------|
| *particles       | `const Vec3` | &outSpringIndices   | `std::vector<int>`   |
| numParticles     | `int`        | &outSpringLengths   | `std::vector<float>` |
| radius           | `float`      | &outSpringStiffness | `std::vector<float>` |
| stiffness = 1.0f | `float`      |                     |                      |

## Imp {collapsible="true"}

```C++
struct Tetrahedron {
    int indices[4]; // The index of the four vertices of a tetrahedron
};

int CreateTetrahedralLinks(
    const Vec3* vertices,
    int numVertices,
    const Tetrahedron* tetrahedra,
    int numTetrahedra,
    std::vector<int>& outSpringIndices,
    std::vector<float>& outSpringLengths,
    std::vector<float>& outSpringStiffness,
    float stiffness = 1.0f);
```

{collapsible="true" collapsed-title="CreateTetrahedralLinks"}

```C++
int CreateTetrahedralLinks(
    const Vec3* vertices,
    int numVertices,
    const Tetrahedron* tetrahedra,
    int numTetrahedra,
    std::vector<int>& outSpringIndices,
    std::vector<float>& outSpringLengths,
    std::vector<float>& outSpringStiffness,
    float stiffness = 1.0f)
{
    int count = 0;

    for (int t = 0; t < numTetrahedra; ++t) {
        // 对于每个四面体
        const Tetrahedron& tetra = tetrahedra[t];

        // 对于四面体中的每条边
        for (int i = 0; i < 4; ++i) {
            for (int j = i + 1; j < 4; ++j) {
                int indexA = tetra.indices[i];
                int indexB = tetra.indices[j];

                // 确保不重复添加同一条边
                bool edgeAlreadyAdded = false;
                for (size_t k = 0; k < outSpringIndices.size(); k += 2) {
                    if ((outSpringIndices[k] == indexA && outSpringIndices[k + 1] == indexB) ||
                        (outSpringIndices[k] == indexB && outSpringIndices[k + 1] == indexA)) {
                        edgeAlreadyAdded = true;
                        break;
                    }
                }

                if (!edgeAlreadyAdded) {
                    // 保存边的索引
                    outSpringIndices.push_back(indexA);
                    outSpringIndices.push_back(indexB);

                    // 计算并保存边的长度
                    Vec3 diff = vertices[indexA] - vertices[indexB];
                    float length = Length(diff);
                    outSpringLengths.push_back(length);

                    // 保存弹簧的刚度
                    outSpringStiffness.push_back(stiffness);

                    ++count;
                }
            }
        }
    }

    return count;
}
```

{collapsible="true" collapsed-title="CreateTetrahedralLinks"}

## optimized imp {collapsible="true"}

如果您的模型包含大量四面体，那么在添加边时使用一个更高效的数据结构来检查边是否已经存在会显著提高性能。一个常见的方法是使用哈希表（在
C++ 中通常使用 std::unordered_set）来存储已添加的边。

下面是一个使用 std::unordered_set 重写的 CreateTetrahedralLinks 函数的示例，该函数可以更高效地处理大量四面体：

```C++
#include <unordered_set>
#include <vector>
#include <utility>

struct Vec3 {
    float x, y, z;

    // ... Vec3的定义和相关函数 ...
};

struct Tetrahedron {
    int indices[4]; // 四面体的四个顶点的索引
    // ... Tetrahedron的定义和相关函数 ...
};

// 为了在unordered_set中使用自定义类型的pair<int, int>，需要定义hash函数和比较函数
struct EdgeHash {
    size_t operator()(const std::pair<int, int>& edge) const {
        return std::hash<int>()(edge.first) ^ std::hash<int>()(edge.second);
    }
};

struct EdgeEqual {
    bool operator()(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2) const {
        return edge1.first == edge2.first && edge1.second == edge2.second;
    }
};

int CreateTetrahedralLinks(
    const Vec3* vertices,
    const Tetrahedron* tetrahedra,
    int numTetrahedra,
    std::vector<int>& outSpringIndices,
    std::vector<float>& outSpringLengths,
    std::vector<float>& outSpringStiffness,
    float stiffness = 1.0f)
{
    std::unordered_set<std::pair<int, int>, EdgeHash, EdgeEqual> edgeSet;
    int count = 0;

    for (int t = 0; t < numTetrahedra; ++t) {
        const Tetrahedron& tetra = tetrahedra[t];
        for (int i = 0; i < 4; ++i) {
            for (int j = i + 1; j < 4; ++j) {
                int indexA = tetra.indices[i];
                int indexB = tetra.indices[j];

                // 保证第一个索引小于第二个索引，以便正确地存储和查找边
                auto edge = indexA < indexB ? std::make_pair(indexA, indexB) : std::make_pair(indexB, indexA);

                // 检查边是否已经存在于集合中
                if (!edgeSet.count(edge)) {
                    // 将新边添加到集合中
                    edgeSet.insert(edge);

                    // 保存边的索引
                    outSpringIndices.push_back(indexA);
                    outSpringIndices.push_back(indexB);

                    // 计算并保存边的长度
                    Vec3 diff = vertices[indexA] - vertices[indexB];
                    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z); // Length(diff);
                    outSpringLengths.push_back(length);

                    // 保存弹簧的刚度
                    outSpringStiffness.push_back(stiffness);

                    ++count;
                }
            }
        }
    }

    return count;
}
```

{collapsible="true" collapsed-title="CreateTetrahedralLinks"}

在这段代码中，我们使用了一个 unordered_set 来存储每条边。每条边都被存储为一个包含两个顶点索引的 pair，其中较小的索引始终作为
first 元素，以确保边的唯一性。

请注意，为了在 unordered_set 中使用 std::pair<int, int>，我们需要提供自定义的哈希函数和等价判断函数。在 EdgeHash
结构中，我们定义了如何为边计算哈希值，而在 EdgeEqual 结构中，我们定义了如何比较两条边是否相等。

这种方法将大大加快边的查找和插入速度，特别是在处理具有大量四面体的模型时。

## FlocculeCreateSoftFromMesh {collapsible="true"}

```C++

// 假设 Vec3 是一个用于存储3D坐标的向量类
// Tetrahedra 是一个包含四面体顶点索引的数组

bool FlocculeCreateSoftFromMesh(
    Vec3* vertices,             // 模型的顶点数组
    int numVertices,            // 顶点数量
    int* tetrahedra,            // 四面体的顶点索引数组
    int numTetrahedra,          // 四面体数量
    float linkRadius,           // 用于创建弹簧的链接半径
    float linkStiffness,        // 弹簧的刚度
    bool tearable               // 是否可以撕裂
) {
    // ... [其他代码] ...

    // 创建弹簧
    std::vector<int> springIndices;
    std::vector<float> springLengths;
    std::vector<float> springStiffness;

    // 遍历所有四面体来生成弹簧
    for (int i = 0; i < numTetrahedra; ++i) {
        // 对于每个四面体，获取四个顶点的索引
        int* tet = &tetrahedra[i * 4]; // 每个四面体有4个顶点

        // 创建四面体的六条边上的弹簧
        for (int j = 0; j < 3; ++j) {
            for (int k = j + 1; k < 4; ++k) {
                int idx1 = tet[j];
                int idx2 = tet[k];

                // 添加弹簧
                springIndices.push_back(idx1);
                springIndices.push_back(idx2);

                // 计算并添加弹簧的静止长度
                float restLength = (vertices[idx1] - vertices[idx2]).length();
                springLengths.push_back(restLength);

                // 添加弹簧的刚度
                springStiffness.push_back(linkStiffness);
            }
        }
    }

    // ... [根据 springIndices, springLengths 和 springStiffness 创建物理模型的代码] ...

    // ... [其他代码] ...

    return tearable;
}
```

{collapsible="true" collapsed-title="FlocculeCreateSoftFromMesh"}

## Tri Link {collapsible="true"}

```C++
  int CreateLinks(
    const Vec3* particles,
    const std::vector<int>& triangleIndices,
    std::vector<int>& outSpringIndices,
    std::vector<float>& outSpringLengths,
    std::vector<float>& outSpringStiffness,
    float stiffness = 1.0f)
	{
	  int count = 0;

	  // 计算每条边，确保每个弹簧只被添加一次
	  std::set<std::pair<int, int>> edges;

	  for (size_t i = 0; i < triangleIndices.size(); i += 3)
	  {
	    int indices[3] = { triangleIndices[i], triangleIndices[i + 1], triangleIndices[i + 2] };

	    // 对于每个三角形的每个边
	    for (int j = 0; j < 3; ++j)
	    {
	      int a = indices[j];
	      int b = indices[(j + 1) % 3];

	      // 以排序的顺序存储边，以避免重复
	      std::pair<int, int> edge = a < b ? std::make_pair(a, b) : std::make_pair(b, a);

	      // 如果这条边还没有被处理过
	      if (edges.find(edge) == edges.end())
	      {
	        outSpringIndices.push_back(edge.first);
	        outSpringIndices.push_back(edge.second);

	        float length = sqrtf(LengthSq(particles[edge.first] - particles[edge.second]));
	        outSpringLengths.push_back(length);
	        outSpringStiffness.push_back(stiffness);

	        // 标记这条边已处理
	        edges.insert(edge);

	        ++count;
	      }
	    }
	  }

	  return count;
	}
```

## Tetra Link {collapsible="true"}

Take fat as an example.

<video src="fat_no_link.mp4" preview-src="fat_nolink.png" width="500"/>

<video src="fat_link.mp4" preview-src="fat_link.png" width="500"/>

217721 springs