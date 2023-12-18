#ifndef DATABRIDGEORTOGONALLINK_HH
#define DATABRIDGEORTOGONALLINK_HH

#include "LinkLine.hh" 
#include "DatabridgeLink.hh"
#include "../TopologyRuleSet.hh"

constexpr int SLOT_ARC_OFFSET = 20;

class DatabridgeOrthogonalLink : public DatabridgeLink
{
public:
    DatabridgeOrthogonalLink(std::shared_ptr<TopologyRuleSet> rule_set,
                            std::string start_node_hash,
                            std::string start_slot_hash,
                            int start_position_x,
                            int start_position_y,
                            std::string end_node_hash,
                            std::string end_slot_hash,
                            int end_position_x,
                            int end_position_y);
    
    ~DatabridgeOrthogonalLink();

    void Draw(const Cairo::RefPtr<Cairo::Context>& cairo_context) const override;

    void DragStartToOffset(const DatabridgePoint& offset);

    void DragEndToOffset(const DatabridgePoint& offset);

    void DragLineToOffset(const DatabridgePoint& pointer_position, const DatabridgePoint& offset);

    void UpdateLinkTopology();
    
    void SetInFocus() override;

    void SetOutFocus() override;

    void SetStatus(WidgetStatus status) override;

    void UpdateLineStatus();

private:
    std::shared_ptr<TopologyRuleSet> m_topology_rule_set;
};

#endif // DATABRIDGEORTOGONALLINK_HH

