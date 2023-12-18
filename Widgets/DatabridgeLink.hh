#ifndef DATABRIDGELINK_HH
#define DATABRIDGELINK_HH

#include "DatabridgeWidget.hh"
#include "DatabridgePoint.hh"
#include "LinkLine.hh"

class DatabridgeLink : public DatabridgeWidget
{
public:
    DatabridgeLink(std::string start_node_hash, 
                   std::string start_slot_hash,
                   std::string end_node_hash,
                   std::string end_slot_hash
                   );
    
    virtual ~DatabridgeLink();

    virtual void DragStartToOffset(const DatabridgePoint& offset) = 0;
    virtual void DragEndToOffset(const DatabridgePoint& offset) = 0;

    virtual void UpdateLinkTopology() = 0;

    bool ContainPoint(const DatabridgePoint& point) const override;

    void SetLineStatus(WidgetStatus status, const DatabridgePoint& point);

    std::string GetLineHash(const DatabridgePoint& point);

    std::string GetStartNodeHash();
    std::string GetEndNodeHash();

    std::string GetStartNodeSlotHash();
    std::string GetEndNodeSlotHash();

    bool LineContainPoint(const std::string& line_hash, const DatabridgePoint& point);

protected:
    std::string m_start_node_hash;
    std::string m_start_slot_hash;
    std::string m_end_node_hash;
    std::string m_end_slot_hash;

    std::list<std::unique_ptr<LinkLine>> m_lines;
};


#endif // DATABRIDGELINK_HH
