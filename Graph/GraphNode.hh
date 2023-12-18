#ifndef TOPOLOGYSORT_GRAPHNODE_HH
#define TOPOLOGYSORT_GRAPHNODE_HH

#include <string>

class GraphNode {

    public:
        GraphNode(unsigned int id);
        GraphNode(const GraphNode& node);
        ~GraphNode();

        unsigned int GetNodeId() const;
    private:
        unsigned int m_id;

};

#endif // TOPOLOGYSORT_GRAPHNODE