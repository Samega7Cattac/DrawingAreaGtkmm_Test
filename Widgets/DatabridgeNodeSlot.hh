#ifndef DATABRIDGENODESLOT_HH
#define DATABRIDGENODESLOT_HH

#include "DatabridgeBlock.hh"
#include "DatabridgeOrthogonalLink.hh"

// GDKMM includes
#include <gdkmm/rectangle.h>
#include <cairomm/context.h>

#include <memory>

enum class SlotType
{
    INBOUND,
    OUTBOUND
};


class DatabridgeNodeSlot : public DatabridgeBlock
{
public:
    DatabridgeNodeSlot(int x,
                       int y,
                       SlotType type);

    ~DatabridgeNodeSlot();


    // void SetPosition(int position_x, int position_y);

    void DragToOffset(const DatabridgePoint& offset) override;

    void Draw(const Cairo::RefPtr<Cairo::Context>& cairo_context) const override;

    void SetLink(const std::shared_ptr<DatabridgeOrthogonalLink>& link);

    void ClearLink();

    DatabridgePoint GetPosition() const override;

    DatabridgePoint GetLinkOrigin() const;

    bool IsAvailable() const;

private:
    SlotType m_slot_type;

    std::shared_ptr<DatabridgeOrthogonalLink> m_link;

    DatabridgePoint m_position;
};



#endif