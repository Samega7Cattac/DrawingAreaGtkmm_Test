#include "LinkBuilder.hh"
#include "Widgets/DatabridgeOrthogonalLink.hh"

// BOTAN headers
#include <botan/hash.h>
#include <botan/hex.h>

#include <ctime>
#include <sstream>

LinkBuilder::LinkBuilder(std::shared_ptr<TopologyRuleSet> rule_set,
                       std::string start_node_hash,
                       std::string start_slot_hash,
                       int start_position_x,
                       int start_position_y)
{
    m_rule_set = rule_set;
    m_start_node_hash = start_node_hash;
    m_start_slot_hash = start_slot_hash;
    m_start_position_x = start_position_x;
    m_start_position_y = start_position_y;
}

void
LinkBuilder::endArc(std::string end_node_hash,
                   std::string end_slot_hash,
                   int end_position_x,
                   int end_position_y)
{
    m_end_node_hash = end_node_hash;
    m_end_slot_hash = end_slot_hash;
    m_end_position_x = end_position_x;
    m_end_position_y = end_position_y;
}

std::shared_ptr<DatabridgeOrthogonalLink>
LinkBuilder::build(const std::unordered_map<std::string,
                                  std::shared_ptr<DatabridgeNode>
                                 >& nodes,
                   std::unordered_map<std::string,
                                  std::shared_ptr<DatabridgeOrthogonalLink>
                                 >& links)
{
    std::shared_ptr<DatabridgeOrthogonalLink> link =
        std::make_shared<DatabridgeOrthogonalLink>(
                                   m_rule_set,
                                   m_start_node_hash,
                                   m_start_slot_hash,
                                   m_start_position_x,
                                   m_start_position_y,
                                   m_end_node_hash,
                                   m_end_slot_hash,
                                   m_end_position_x,
                                   m_end_position_y);

    nodes.at(m_start_node_hash)
          ->SetOutboundSlotLink(m_start_slot_hash, link);


    nodes.at(m_end_node_hash)
          ->SetInboundSlotLink(m_end_slot_hash, link);


    links.emplace(link->GetHash(), link);

    return link;
}


std::string 
LinkBuilder::GetStartNodeHash()
{
    return m_start_node_hash;
}

std::string 
LinkBuilder::GetStartSlotHash()
{
    return m_start_slot_hash;
}
