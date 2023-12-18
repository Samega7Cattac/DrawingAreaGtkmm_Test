#ifndef TOPOLOGYSORT_HH
#define TOPOLOGY_SORT_HH

#include "Graph/Graph.hh"
#include "Widgets/DatabridgeNode.hh"
#include "Widgets/DatabridgeLink.hh"

class TopologySort
{

public:
    TopologySort();

    ~TopologySort();

    std::string ExecuteSorting(std::unordered_map<std::string, std::shared_ptr<DatabridgeNode>>& m_nodes,
                               std::unordered_map<std::string, std::shared_ptr<DatabridgeOrthogonalLink>>& m_links);

private:
    void Sort(Graph& graph, std::list<std::shared_ptr<GraphNode>>& result);

};

#endif // TOPOLOGY_SORT_HH