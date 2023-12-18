#include "DragAction.hh"

DragAction::DragAction(DragType drag_type, const DatabridgePoint& start_position)
{
    m_start_position = start_position; 
    m_type = drag_type;  
}

DragAction::~DragAction()
{

}

void
DragAction::SetLastOffset(const DatabridgePoint& last_offset)
{
    m_last_offset = last_offset;
}

DatabridgePoint
DragAction::GetLastOffset() const
{
    return m_last_offset;
}


DatabridgePoint
DragAction::GetStartPosition() const
{
    return m_start_position;
}

std::string
DragAction::GetHash(unsigned int hash_id) const
{
    return m_hashes.at(hash_id);
}

DragType
DragAction::GetDragType() const
{
    return m_type;
}

NoDragAction::NoDragAction(const DatabridgePoint& position) : DragAction(DragType::NO_DRAG, position)
{
}

NoDragAction::~NoDragAction()
{

}

DragNodeAction::DragNodeAction(const DatabridgePoint& start_position, 
                               std::string node_hash)
                               : DragAction(DragType::DRAG_NODE, start_position)
{
    m_hashes.push_back(node_hash);
}

DragNodeAction::~DragNodeAction()
{

}


DragLinkAction::DragLinkAction(const DatabridgePoint& start_position, 
                               std::string link_hash,
                               std::string line_hash)
                               : DragAction(DragType::DRAG_LINK, start_position)
{
    m_hashes.push_back(link_hash);
    m_hashes.push_back(line_hash);
}

DragLinkAction::~DragLinkAction()
{

}
