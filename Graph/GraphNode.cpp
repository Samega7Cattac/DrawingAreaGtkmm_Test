#include "GraphNode.hh"

GraphNode::GraphNode(unsigned int id) : m_id(id)
{

};

GraphNode::GraphNode(const GraphNode& node) : m_id(node.GetNodeId())
{

};

GraphNode::~GraphNode()
{

}

unsigned int
GraphNode::GetNodeId() const
{
    return m_id;
}