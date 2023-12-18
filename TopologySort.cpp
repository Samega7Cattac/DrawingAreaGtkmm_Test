
#include <sstream>

#include "TopologySort.hh"

#include "Graph/GraphAdjacencyMatrix.hh"


TopologySort::TopologySort()
{

}

TopologySort::~TopologySort()
{

}


std::string
TopologySort::ExecuteSorting(std::unordered_map<std::string, std::shared_ptr<DatabridgeNode>>& nodes,
                             std::unordered_map<std::string, std::shared_ptr<DatabridgeOrthogonalLink>>& links)
{
    int graph_size = nodes.size();

    std::unordered_map<std::string, unsigned int> hash_to_nodeid;

    unsigned int node_id = 0;

    for (const auto& [hash, node] : nodes)
    {
        hash_to_nodeid[hash] = node_id;
        node->SetOrderId(node_id);
        node_id++;
    }

    GraphAdjacencyMatrix graph(graph_size);

    for (const auto& [hash, link] : links)
    {
        std::string start_node_hash = link->GetStartNodeHash();
        unsigned int start_node_id = hash_to_nodeid[start_node_hash];

        std::string end_node_hash = link->GetEndNodeHash();
        unsigned int end_node_id = hash_to_nodeid[end_node_hash];

        graph.AddEdge(start_node_id, end_node_id);
    }

    std::list<std::shared_ptr<GraphNode>> sort_result;
    Sort(graph, sort_result);

    std::ostringstream result_stream;

    for (const std::shared_ptr<GraphNode>& node : sort_result)
    {
        result_stream << node->GetNodeId();
    }

    return result_stream.str();
}


void
TopologySort::Sort(Graph& graph, std::list<std::shared_ptr<GraphNode>>& result)
{
    
    std::list<std::shared_ptr<GraphNode>> S;

    graph.GetStartingNodes(S);

    while (!S.empty()) {
        auto n = S.front();

        S.pop_front();

        result.push_back(n);

        while (graph.HasOutgoing(*n)) {

            auto m = graph.PopOutgoingEdge(*n);

            if (!graph.HasIncoming(*m)) {
                
                S.push_back(m);
            }
        }
    }
}

