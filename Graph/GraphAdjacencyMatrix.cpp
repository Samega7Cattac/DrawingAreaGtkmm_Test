#include <algorithm>
#include <numeric>

#include "GraphAdjacencyMatrix.hh"

GraphAdjacencyMatrix::GraphAdjacencyMatrix(unsigned int size) : Graph(size)
{
    InitEdgeContainer();
}

void
GraphAdjacencyMatrix::InitEdgeContainer()
{
    m_arcs.resize(GetSize()*GetSize());
}

GraphAdjacencyMatrix::~GraphAdjacencyMatrix()
{
}

void
GraphAdjacencyMatrix::AddEdge(const GraphNode& node_n, const GraphNode& node_m)
{
    m_arcs.at(node_m.GetNodeId() + node_n.GetNodeId()*GetSize()) = true;
}

void
GraphAdjacencyMatrix::AddEdge(unsigned int node_n_id, unsigned int node_m_id)
{
    m_arcs.at(node_m_id + node_n_id*GetSize()) = true;
}

void
GraphAdjacencyMatrix::RemoveEdge(const GraphNode& node_n, const GraphNode& node_m)
{
    m_arcs.at(node_m.GetNodeId() + node_n.GetNodeId()*GetSize()) = false;
}


bool
GraphAdjacencyMatrix::HasIncoming(const GraphNode& node_n) const
{
    for (auto node : m_nodes) {
        if (IsEdge(*node, node_n)) {
            return true;
        }
    }
    return false;
}


void
GraphAdjacencyMatrix::GetStartingNodes(std::list<std::shared_ptr<GraphNode>>& starting_nodes)
{
    starting_nodes.insert(starting_nodes.end(), m_nodes.begin(), m_nodes.end());
    
    for (auto node_i : m_nodes) {
        if (HasIncoming(*node_i)) {
            starting_nodes.remove(node_i);
        }
    }
}

void
GraphAdjacencyMatrix::GetStartingNodesAsDeque(std::deque<std::shared_ptr<GraphNode>>& starting_nodes)
{
    starting_nodes.insert(starting_nodes.end(), m_nodes.begin(), m_nodes.end());
    
    for (const std::shared_ptr<GraphNode>& node_i : m_nodes) {
        if (HasIncoming(*node_i)) {
            starting_nodes.erase(std::find(starting_nodes.begin(), starting_nodes.end(), node_i));
        }
    }
}


bool
GraphAdjacencyMatrix::HasOutgoing(const GraphNode& node_n) const
{   
    for (const std::shared_ptr<GraphNode>& node_j : m_nodes) {
        if (IsEdge(node_n, *node_j)) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<GraphNode>
GraphAdjacencyMatrix::PopOutgoingEdge(const GraphNode& node_n)
{
    
    for (const std::shared_ptr<GraphNode>& node_j : m_nodes) {
        if (IsEdge(node_n, *node_j)) {
            RemoveEdge(node_n, *node_j);
            return node_j;
        }
    }
    return nullptr;
}

inline bool
GraphAdjacencyMatrix::IsEdge(const GraphNode& node_n, const GraphNode& node_m) const
{
    return m_arcs.at(node_m.GetNodeId() + node_n.GetNodeId()*GetSize());
}

inline bool
GraphAdjacencyMatrix::IsEdge(unsigned int node_n_id, unsigned int node_m_id) const
{
    return m_arcs.at(node_m_id + node_n_id*GetSize());
}

int
GraphAdjacencyMatrix::GetNodeOrder(const GraphNode& node_n) {
    int order = 0;
    for (const std::shared_ptr<GraphNode>& node_m : m_nodes) {
        order += IsEdge(node_n, *node_m);
    }
    return order;
}

unsigned int
GraphAdjacencyMatrix::CountArcs() {
    unsigned int result = 0;
    for (bool arc : m_arcs) {
        result += arc;
    }
    return result;
}

