#include "BVH.h"

struct aabb
{
  G_O bmin = G_O(1e30f, 1e30f, 1e30f, 0), bmax = G_O(-1e30f, -1e30f, -1e30f, 0);
  void grow(G_O p) { bmin = G_O(fminf(bmin.x, p.x), fminf(bmin.y, p.y), fminf(bmin.z, p.z), 0), bmax = G_O(fmaxf(bmax.x, p.x), fmaxf(bmax.y, p.y), fmaxf(bmax.z, p.z), 0); }
  float area()
  {
    G_O e = bmax - bmin; // box extent
    return e.x * e.y + e.y * e.z + e.z * e.x;
  }
};

float EvaluateSAH(BVHNode &node, int axis, float pos)
{
  // determine triangle counts and bounds for this split candidate

  aabb leftBox, rightBox;
  int leftCount = 0, rightCount = 0;
  for (size_t i = 0; i < node.primCount; i++)
  {
    Triangulo &triangle = tri[node.firstPrim + i];
    if (triangle.centroide[axis] < pos)
    {
      leftCount++;
      leftBox.grow(triangle.v0);
      leftBox.grow(triangle.v1);
      leftBox.grow(triangle.v2);
    }
    else
    {
      rightCount++;
      rightBox.grow(triangle.v0);
      rightBox.grow(triangle.v1);
      rightBox.grow(triangle.v2);
    }
  }
  float cost = leftCount * leftBox.area() + rightCount * rightBox.area();
  return cost > 0 ? cost : 1e30f;
}

void UpdateNodeBounds(int nodeIdx, vector<BVHNode> &bvhNode)
{
  bvhNode[nodeIdx].aabbMin = G_O(1e20f, 1e20f, 1e20f, 1);
  bvhNode[nodeIdx].aabbMax = G_O(-1e20f, -1e20f, -1e20f, 1);
  for (int first = bvhNode[nodeIdx].firstPrim, i = 0; i < bvhNode[nodeIdx].primCount; i++)
  {
    bvhNode[nodeIdx].aabbMin = G_O::fminf(bvhNode[nodeIdx].aabbMin, tri[first + i].v0);
    bvhNode[nodeIdx].aabbMin = G_O::fminf(bvhNode[nodeIdx].aabbMin, tri[first + i].v1);
    bvhNode[nodeIdx].aabbMin = G_O::fminf(bvhNode[nodeIdx].aabbMin, tri[first + i].v2);
    bvhNode[nodeIdx].aabbMax = G_O::fmaxf(bvhNode[nodeIdx].aabbMax, tri[first + i].v0);
    bvhNode[nodeIdx].aabbMax = G_O::fmaxf(bvhNode[nodeIdx].aabbMax, tri[first + i].v1);
    bvhNode[nodeIdx].aabbMax = G_O::fmaxf(bvhNode[nodeIdx].aabbMax, tri[first + i].v2);
  }
}

struct Bin
{
  aabb bounds;
  int triCount = 0;
};

float FindBestSplitPlane(BVHNode &node, int &axis, float &splitPos)
{
  float bestCost = 1e30f;
  for (int a = 0; a < 3; a++)
  {
    float boundsMin = 1e30f, boundsMax = -1e30f;
    for (int i = 0; i < node.primCount; i++)
    {
      Triangulo &triangle = tri[node.firstPrim + i];
      boundsMin = min(boundsMin, triangle.centroide[a]);
      boundsMax = max(boundsMax, triangle.centroide[a]);
    }
    if (boundsMin == boundsMax)
      continue;
    // populate the bins
    Bin bin[BINS];
    float scale = BINS / (boundsMax - boundsMin);
    for (size_t i = 0; i < node.primCount; i++)
    {
      Triangulo &triangle = tri[node.firstPrim + i];
      int binIdx = min(BINS - 1,
                       (int)((triangle.centroide[a] - boundsMin) * scale));
      bin[binIdx].triCount++;
      bin[binIdx].bounds.grow(triangle.v0);
      bin[binIdx].bounds.grow(triangle.v1);
      bin[binIdx].bounds.grow(triangle.v2);
    }
    // gather data for the 7 planes between the 8 bins
    float leftArea[BINS - 1], rightArea[BINS - 1];
    int leftCount[BINS - 1], rightCount[BINS - 1];
    aabb leftBox, rightBox;
    int leftSum = 0, rightSum = 0;
    for (int i = 0; i < BINS - 1; i++)
    {
      leftSum += bin[i].triCount;
      leftCount[i] = leftSum;
      leftBox.grow(bin[i].bounds.bmin);
      leftBox.grow(bin[i].bounds.bmax);
      leftArea[i] = leftBox.area();
      rightSum += bin[BINS - 1 - i].triCount;
      rightCount[BINS - 2 - i] = rightSum;
      rightBox.grow(bin[BINS - 1 - i].bounds.bmin);
      rightBox.grow(bin[BINS - 1 - i].bounds.bmax);
      rightArea[BINS - 2 - i] = rightBox.area();
    }
    // calculate SAH cost for the 7 planes
    scale = (boundsMax - boundsMin) / BINS;
    for (int i = 0; i < BINS - 1; i++)
    {
      float planeCost =
          leftCount[i] * leftArea[i] + rightCount[i] * rightArea[i];
      if (planeCost < bestCost)
        axis = a, splitPos = boundsMin + scale * (i + 1),
        bestCost = planeCost;
    }
  }
  return bestCost;
}

float CalculateNodeCost(BVHNode &node)
{
  G_O e = node.aabbMax - node.aabbMin; // extent of the node
  float surfaceArea = e.x * e.y + e.y * e.z + e.z * e.x;
  return node.primCount * surfaceArea;
}

int Subdivide(int nodeIdx, int &nodesUsed, bool ponerTextura, vector<BVHNode> &bvhNode)
{
  BVHNode &node = bvhNode[nodeIdx];
  // terminate recursion

  // determine split axis using SAH
  int axis;
  float splitPos;
  float splitCost = FindBestSplitPlane(node, axis, splitPos);

  float nosplitCost = CalculateNodeCost(node);
  if (splitCost >= nosplitCost)
    return 0;

  // // Split plane axis and position
  // G_O extent = node.aabbMax - node.aabbMin;
  // int axis = 0;
  // if (extent.y > extent.x)
  // {
  //   axis = 1;
  //   if (extent.z > extent.y)
  //   {
  //     axis = 2;
  //   }
  // }
  // else
  // {
  //   if (extent.z > extent.x)
  //   {
  //     axis = 2;
  //   }
  // }

  int i, j;

  if (ponerTextura)
  {
    if (axis == 0)
    {
      //*float splitPos = node.aabbMin.x + extent.x * 0.5f;

      // Split the group in two halves
      i = node.firstPrim;
      j = i + node.primCount - 1;
      while (i <= j)
      {
        if (tri[i].centroide.x < splitPos)
          i++;
        else
        {
          swap(tri[i], tri[j]);
          swap(triTxt.tri[i], triTxt.tri[j]);
          j--;
        }
      }
    }
    else if (axis == 1)
    {
      //*float splitPos = node.aabbMin.y + extent.y * 0.5f;

      // Split the group in two halves
      i = node.firstPrim;
      j = i + node.primCount - 1;
      while (i <= j)
      {
        if (tri[i].centroide.y < splitPos)
          i++;
        else
        {
          swap(tri[i], tri[j]);
          swap(triTxt.tri[i], triTxt.tri[j]);
          j--;
        }
      }
    }
    else if (axis == 2)
    {
      //*float splitPos = node.aabbMin.z + extent.z * 0.5f;

      // Split the group in two halves
      i = node.firstPrim;
      j = i + node.primCount - 1;
      while (i <= j)
      {
        if (tri[i].centroide.z < splitPos)
          i++;
        else
        {
          swap(tri[i], tri[j]);
          swap(triTxt.tri[i], triTxt.tri[j]);
          j--;
        }
      }
    }
  }
  else
  {
    if (axis == 0)
    {
      //*float splitPos = node.aabbMin.x + extent.x * 0.5f;

      // Split the group in two halves
      i = node.firstPrim;
      j = i + node.primCount - 1;
      while (i <= j)
      {
        if (tri[i].centroide.x < splitPos)
          i++;
        else
        {
          swap(tri[i], tri[j--]);
        }
      }
    }
    else if (axis == 1)
    {
      //*float splitPos = node.aabbMin.y + extent.y * 0.5f;

      // Split the group in two halves
      i = node.firstPrim;
      j = i + node.primCount - 1;
      while (i <= j)
      {
        if (tri[i].centroide.y < splitPos)
          i++;
        else
        {
          swap(tri[i], tri[j--]);
        }
      }
    }
    else if (axis == 2)
    {
      //*float splitPos = node.aabbMin.z + extent.z * 0.5f;

      // Split the group in two halves
      i = node.firstPrim;
      j = i + node.primCount - 1;
      while (i <= j)
      {
        if (tri[i].centroide.z < splitPos)
          i++;
        else
        {
          swap(tri[i], tri[j--]);
        }
      }
    }
  }

  // Creating child nodes for each half
  int leftCount = i - node.firstPrim;
  if (leftCount == 0 || leftCount == node.primCount)
  {
    return 0;
  }

  // create child nodes
  int leftChildIdx = nodesUsed++;
  int rightChildIdx = nodesUsed++;
  node.leftChild = leftChildIdx;
  BVHNode left, right;

  left.firstPrim = node.firstPrim;
  left.primCount = leftCount;
  right.firstPrim = i;
  right.primCount = node.primCount - leftCount;
  node.primCount = 0;

  bvhNode.push_back(left);
  bvhNode.push_back(right);
  UpdateNodeBounds(leftChildIdx, bvhNode);
  UpdateNodeBounds(rightChildIdx, bvhNode);

  // recurse
  Subdivide(leftChildIdx, nodesUsed, ponerTextura, bvhNode);

  Subdivide(rightChildIdx, nodesUsed, ponerTextura, bvhNode);

  return 1;
}

void BVH::rangoValores()
{
  G_O aabbMin = G_O(1e20f, 1e20f, 1e20f, 1);
  G_O aabbMax = G_O(-1e20f, -1e20f, -1e20f, 1);
  for (int i = 0; i < tri.size(); i++)
  {
    Triangulo &leafTri = tri[i];
    aabbMin = G_O::fminf(aabbMin, leafTri.v0);
    aabbMin = G_O::fminf(aabbMin, leafTri.v1);
    aabbMin = G_O::fminf(aabbMin, leafTri.v2);
    aabbMax = G_O::fmaxf(aabbMax, leafTri.v0);
    aabbMax = G_O::fmaxf(aabbMax, leafTri.v1);
    aabbMax = G_O::fmaxf(aabbMax, leafTri.v2);
  }
  cout << "\n🌐 Limites del Objeto 3D en cada dimension 🌐\n";
  cout << "X: [" << aabbMax.x << ", " << aabbMin.x << "]" << endl;
  cout << "Y: [" << aabbMax.y << ", " << aabbMin.y << "]" << endl;
  cout << "Z: [" << aabbMax.z << ", " << aabbMin.z << "]" << endl;
}

vector<BVHNode> BVH::BuildBVH(bool ponerTextura)
{

  for (int i = 0; i < tri.size(); i++)
    tri[i].centroide =
        (tri[i].v0 + tri[i].v1 + tri[i].v2) * 0.3333f;
  // assign all triangles to root node
  BVHNode root;
  vector<BVHNode> bvhNode;
  root.leftChild = 0;
  root.firstPrim = 0, root.primCount = tri.size();
  bvhNode.push_back(root);
  int rootNodeIdx = 0;
  int nodesUsed = 1;

  UpdateNodeBounds(rootNodeIdx, bvhNode);
  // subdivide recursively
  Subdivide(rootNodeIdx, nodesUsed, ponerTextura, bvhNode);
  return bvhNode;
}
