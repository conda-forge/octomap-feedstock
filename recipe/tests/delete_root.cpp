// Regression test for conda-forge/octomap-feedstock#27: deleting the last
// occupied node must free the root node. Counts live nodes through a node
// type derived from OcTreeNode, like OcTreeStamped does.
#include <octomap/OcTreeNode.h>
#include <octomap/OccupancyOcTreeBase.h>

#include <cstdio>
#include <string>

class CountedNode : public octomap::OcTreeNode {
public:
  CountedNode() { ++alive; }
  CountedNode(const CountedNode& rhs) : octomap::OcTreeNode(rhs) { ++alive; }
  ~CountedNode() { --alive; }
  static long alive;
};
long CountedNode::alive = 0;

class CountedTree : public octomap::OccupancyOcTreeBase<CountedNode> {
public:
  explicit CountedTree(double res) : octomap::OccupancyOcTreeBase<CountedNode>(res) {}
  CountedTree* create() const override { return new CountedTree(resolution); }
  std::string getTreeType() const override { return "CountedTree"; }
};

int main() {
  {
    CountedTree tree(0.1);
    const octomap::point3d point(0.0F, 0.0F, 0.0F);
    for (int i = 0; i < 3; ++i) {
      tree.updateNode(point, true);
      tree.deleteNode(point);
    }
  }
  std::printf("live nodes after the tree is destroyed: %ld\n", CountedNode::alive);
  return CountedNode::alive == 0 ? 0 : 1;
}
