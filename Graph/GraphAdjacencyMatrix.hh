#include "Graph.hh"

#ifndef TOPOLOGYSORT_GRAPHADJACENCYMATRIX_HH
#define TOPOLOGYSORT_GRAPHADJACENCYMATRIX_HH

class GraphAdjacencyMatrix : public Graph
{
public:
    GraphAdjacencyMatrix(unsigned int size);

    ~GraphAdjacencyMatrix();

    void InitEdgeContainer();

    void AddEdge(const GraphNode &node_n, const GraphNode &node_m) override;
    void AddEdge(unsigned int node_n_id, unsigned int node_m_id) override;

    void RemoveEdge(const GraphNode &node_n, const GraphNode &node_m) override;

    inline bool HasIncoming(const GraphNode &node_n) const override;
    inline bool HasOutgoing(const GraphNode &node_n) const override;

    void GetStartingNodes(std::list<std::shared_ptr<GraphNode>>& result) override;
    void GetStartingNodesAsDeque(std::deque<std::shared_ptr<GraphNode>>& result) override;

    std::shared_ptr<GraphNode> PopOutgoingEdge(const GraphNode &node_n) override;

    int GetNodeOrder(const GraphNode &node_n) override;

    inline bool IsEdge(const GraphNode &node_n, const GraphNode &node_m) const override;
    inline bool IsEdge(unsigned int node_n_id, unsigned int node_m_id) const override;

    unsigned int CountArcs() override;

private:
    std::vector<bool> m_arcs;
};

#endif // TOPOLOGYSORT_GRAPHADJACENCYMATRIX_HH