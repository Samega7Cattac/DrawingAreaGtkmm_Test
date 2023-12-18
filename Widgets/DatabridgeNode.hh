#ifndef DATABRIDGENODES_HH
#define DATABRIDGENODES_HH

#include "DatabridgeBlock.hh"
#include "DatabridgeNodeSlot.hh"

// GTKMM includes
#include <gdkmm/rectangle.h>
#include <gdkmm/pixbuf.h>
#include <cairomm/context.h>

class DatabridgeNode : public DatabridgeBlock
{
public:
    DatabridgeNode(Glib::ustring name, const DatabridgePoint& position);

    ~DatabridgeNode();

    Glib::ustring GetName() const;

    void DragToOffset(const DatabridgePoint& point) override;

    void UpdateSlotStatus(const std::string& slot_hash, WidgetStatus status);

    void UpdateInboundSlotStatus(const std::string& slot_hash, WidgetStatus status);
    void UpdateOutboundSlotStatus(const std::string& slot_hash, WidgetStatus status);


    void Draw(const Cairo::RefPtr<Cairo::Context>& cairo_context) const override;

    void ClearInboundSlot(const std::string& slot_hash);

    void ClearOutboundSlot(const std::string& slot_hash);

    void SetInboundSlotLink(const std::string& slot_hash, 
                            const std::shared_ptr<DatabridgeOrthogonalLink>& link);

    void SetOutboundSlotLink(const std::string& slot_hash,
                             const std::shared_ptr<DatabridgeOrthogonalLink>& link);
    
    void SetSlotStatus(const std::string& slot_hash, WidgetStatus status);

    void SetOrderId(unsigned int order);

    bool ContainInboundSlot(DatabridgePoint pointer) const;

    bool ContainOutboundSlot(DatabridgePoint pointer) const;

    std::string GetInboundSlotHash(DatabridgePoint pointer) const;

    std::string GetOutboundSlotHash(DatabridgePoint pointer) const;

    DatabridgePoint GetSlotLinkOrigin(const std::string& slot_hash) const;

    DatabridgePoint GetPosition() const override;

    unsigned int GetOrderId() const;

    bool IsSlotAvailable(const std::string& slot_hash) const;

    bool HasSlotInFocus() const;


private:
    Glib::ustring m_name;

    Glib::RefPtr<Gdk::Pixbuf> m_symbol;

    std::vector<DatabridgeNodeSlot> m_slots_inbound;

    std::vector<DatabridgeNodeSlot> m_slots_outbound;

    unsigned int m_order_id = 0;

    void InitNodeSlots(unsigned int input, unsigned int output);
};

#endif