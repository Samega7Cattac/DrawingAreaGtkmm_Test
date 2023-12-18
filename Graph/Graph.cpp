#include "Graph.hh"

Graph::Graph(unsigned int size)
{
    m_nodes.reserve(size);
    m_size = size;
    for (unsigned int i = 0; i < size; i++) {
        m_nodes.push_back(std::make_shared<GraphNode>(i));
    }
}

Graph::~Graph()
{

}

unsigned int
Graph::GetSize() const
{
    return m_size;
}

std::shared_ptr<GraphNode>
Graph::GetNode(unsigned int id) const
{
    return m_nodes.at(id);
}