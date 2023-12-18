#ifndef LinkBuilder_HH
#define LinkBuilder_HH

#include "Widgets/DatabridgeNodeSlot.hh"
#include "Widgets/DatabridgeNode.hh"

#include "Widgets/DatabridgeOrthogonalLink.hh"

#include <cmath>
#include <memory>


class LinkBuilder
{
public:
    LinkBuilder(std::shared_ptr<TopologyRuleSet> rule_set,
               std::string node_start_hash, 
               std::string slot_start_hash,
               int start_position_x,
               int start_position_y);
    

    void endArc(std::string node_end_hash, std::string slot_end_hash, int end_position_x, int end_position_y);

    std::shared_ptr<DatabridgeOrthogonalLink> build(
                                            const std::unordered_map<std::string,
                                                           std::shared_ptr<DatabridgeNode>
                                                          >& nodes,
                                            std::unordered_map<std::string,
                                                     std::shared_ptr<DatabridgeOrthogonalLink>
                                                          >& links);


    std::string GetStartNodeHash();
    std::string GetStartSlotHash();

private:
    SlotType m_type;

    std::string m_start_node_hash;
    std::string m_start_slot_hash;

    std::string m_end_node_hash;
    std::string m_end_slot_hash;

    int m_start_position_x;
    int m_start_position_y;

    int m_end_position_x;
    int m_end_position_y;

    std::shared_ptr<TopologyRuleSet> m_rule_set;

    //int m_arc_mouse_position_x;
    //int m_arc_mouse_position_y;

};

#endif // LinkBuilder_HH