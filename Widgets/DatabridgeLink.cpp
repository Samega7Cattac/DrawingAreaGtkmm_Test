#include "DatabridgeLink.hh"

DatabridgeLink::DatabridgeLink(std::string start_node_hash, 
                               std::string start_slot_hash,
                               std::string end_node_hash,
                               std::string end_slot_hash)
                               : DatabridgeWidget()
{
    m_start_node_hash = start_node_hash;
    m_start_slot_hash = start_slot_hash;
    m_end_node_hash = end_node_hash;
    m_end_slot_hash = end_slot_hash;
}

DatabridgeLink::~DatabridgeLink()
{

}



bool
DatabridgeLink::ContainPoint(const DatabridgePoint& point) const
{
    return std::any_of(m_lines.begin(),
                       m_lines.end(),
                       [&point](const std::unique_ptr<LinkLine>& line)
                       {
                            return line->ContainPoint(point);
                       });
}


void
DatabridgeLink::SetLineStatus(WidgetStatus status, const DatabridgePoint& point)
{
    std::list<std::unique_ptr<LinkLine>>::iterator line_itr =
        std::find_if(m_lines.begin(),
                       m_lines.end(),
                       [&point](std::unique_ptr<LinkLine>& line)
                       {
                            return line->ContainPoint(point);
                       });
    
    if (line_itr != m_lines.end())
    {
        (*line_itr)->SetStatus(status);
    }
}

std::string
DatabridgeLink::GetLineHash(const DatabridgePoint& point)
{
    std::list<std::unique_ptr<LinkLine>>::iterator line_itr =
        std::find_if(m_lines.begin(),
                       m_lines.end(),
                       [&point](std::unique_ptr<LinkLine>& line)
                       {
                            return line->ContainPoint(point);
                       });

    if (line_itr == m_lines.end())
    {
        return "";
    }
    return (*line_itr)->GetHash();
}

std::string
DatabridgeLink::GetStartNodeHash()
{
    return m_start_node_hash;
}

std::string
DatabridgeLink::GetEndNodeHash()
{
    return m_end_node_hash;
}


std::string
DatabridgeLink::GetStartNodeSlotHash()
{
    return m_start_slot_hash;
}

std::string
DatabridgeLink::GetEndNodeSlotHash()
{
    return m_end_slot_hash;
}

bool
DatabridgeLink::LineContainPoint(const std::string& line_hash, const DatabridgePoint& point)
{
    return std::any_of(m_lines.begin(),
                       m_lines.end(),
                       [&line_hash, &point](std::unique_ptr<LinkLine>& line)
                       {    
                            return line_hash == line->GetHash() && line->ContainPoint(point);
                       });
}


