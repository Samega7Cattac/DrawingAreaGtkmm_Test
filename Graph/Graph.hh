#include <vector>
#include <deque>
#include <list>
#include <memory>
#include <functional>
#include "GraphNode.hh"

#ifndef TOPOLOGYSORT_GRAPH_HH
#define TOPOLOGYSORT_GRAPH_HH

class GraphAdjacencyList;
class GraphAdjacencyArray;

class Graph {

    public:
        
        Graph(unsigned int size);
    
        ~Graph();

        unsigned int GetSize() const;
        std::shared_ptr<GraphNode> GetNode(unsigned int id) const;

        virtual void InitEdgeContainer() = 0;

        virtual void AddEdge(const GraphNode& node_n, const GraphNode& node_m) = 0;
        virtual void AddEdge(unsigned int node_n_id, unsigned int node_m_id) = 0;

        virtual void RemoveEdge(const GraphNode& node_n, const GraphNode& node_m) = 0;
        
        virtual inline bool HasIncoming(const GraphNode& node_n) const = 0;
        virtual inline bool HasOutgoing(const GraphNode& node_n) const = 0;

        virtual void GetStartingNodes(std::list<std::shared_ptr<GraphNode>>& result) = 0;
        virtual void GetStartingNodesAsDeque(std::deque<std::shared_ptr<GraphNode>>& result) = 0;

        virtual std::shared_ptr<GraphNode> PopOutgoingEdge(const GraphNode& node_n) = 0;

        virtual int GetNodeOrder(const GraphNode& node_n) = 0;

        virtual inline bool IsEdge(const GraphNode& node_n, const GraphNode& node_m) const = 0;
        virtual inline bool IsEdge(unsigned int node_n_id, unsigned int node_m_id) const = 0;

        virtual unsigned int CountArcs() = 0;

        std::vector<std::shared_ptr<GraphNode>>& GetNodes() {
            return m_nodes;
        } 
    
    protected:
        std::vector<std::shared_ptr<GraphNode>> m_nodes;

    private:
        unsigned int m_size;


        
};

#define ConvertGraphP(g2_t, g1) GraphConverter<std::pointer_traits<decltype(g1)>::element_type, g2_t>::convert(*g1)
#define ConvertGraph(g2_t, g1) GraphConverter<std::remove_reference<decltype(g1)>::type, g2_t>::convert(g1)


template<class T, class I>
class GraphConverter {

    static_assert(std::is_base_of_v<Graph, T>, "Graph Converter source class is NOT derived from Graph class");
    static_assert(std::is_base_of_v<Graph, I>, "Graph Converter target class is NOT derived from Graph class");

    public:
     static std::unique_ptr<I> convert(T& graph) {

        std::unique_ptr<I> new_graph = std::make_unique<I>(graph.GetSize(), [graph](const unsigned int i) { return *(graph.GetNode(i));});

        new_graph->InitEdgeContainer();

        for (unsigned int i = 0; i < new_graph->GetSize(); i++) {
            for (unsigned int j = 0; j < new_graph->GetSize(); j++) {
                if (graph.IsEdge(i, j)) {
                    new_graph->AddEdge(i, j);
                }
            }
        }
        return new_graph;
    }
};

#endif // TOPOLOGYSORT_GRAPH