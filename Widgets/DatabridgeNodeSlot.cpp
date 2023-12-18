#include "DatabridgeNodeSlot.hh"

#include <ctime>

// BOTAN headers
#include <botan/hash.h>
#include <botan/hex.h>

// STD headers
#include <cmath>

// GTKMM headers
#include <gdkmm/general.h>

constexpr int SLOT_INTERACTION_BOUNDS_SIZE = 20;
constexpr int SLOT_CIRCLE_RADIUS = 5;

DatabridgeNodeSlot::DatabridgeNodeSlot(int position_x,
                                       int position_y,
                                       SlotType type) : DatabridgeBlock(),
                                       m_slot_type(type)
{
    // Create new boundary
    // This widget onyl requires 1 boundary
    Gdk::Rectangle& boundary = GetBoundaries().emplace_back();

    int bound_offset_y = -SLOT_INTERACTION_BOUNDS_SIZE / 2;

    int bound_offset_x;

    if (type == SlotType::INBOUND)
    {
        bound_offset_x = 0;
    }
    else
    {
        bound_offset_x = -SLOT_INTERACTION_BOUNDS_SIZE;
    }

    boundary.set_x(position_x + bound_offset_x);
    boundary.set_y(position_y + bound_offset_y);
    boundary.set_width(SLOT_INTERACTION_BOUNDS_SIZE);
    boundary.set_height(SLOT_INTERACTION_BOUNDS_SIZE);
}

// DatabridgeNodeSlot::DatabridgeNodeSlot(const DatabridgeNodeSlot& other)
// {
//     m_id = other.GetId();

//     m_bounds = other.GetBounds();
//     m_active = other.m_active;

//     m_is_inbound = other.m_is_inbound;

//     m_slot_position_x = other.m_slot_position_x;
//     m_slot_position_y = other.m_slot_position_y;
// }

DatabridgeNodeSlot::~DatabridgeNodeSlot()
{

}

void
DatabridgeNodeSlot::DragToOffset(const DatabridgePoint& offset)
{
    // Get current boundary position and add offset
    Gdk::Rectangle& boundary = GetBoundaries()[0];
    int new_x_pos = boundary.get_x() + offset.GetX() ;
    int new_y_pos = boundary.get_y() + offset.GetY() ;

    // Update the new boundary position
    boundary.set_x(new_x_pos);
    boundary.set_y(new_y_pos);

    if (m_link != nullptr)
    {
        if (m_slot_type == SlotType::OUTBOUND)
        {
            m_link->DragStartToOffset(offset);
        }
        else
        {
            m_link->DragEndToOffset(offset);
        }
    }
}

void
DatabridgeNodeSlot::Draw(const Cairo::RefPtr<Cairo::Context>& cairo_context) const
{
    // Get widget boundary
    const Gdk::Rectangle& boundary = GetBoundaries()[0];

    cairo_context->set_source_rgba(0.1, 0.1, 0.1, 1.0);
    
    int slot_center_y = boundary.get_y() + SLOT_INTERACTION_BOUNDS_SIZE/2;

    if (m_slot_type == SlotType::INBOUND)
    {
        cairo_context->move_to(boundary.get_x(), slot_center_y);

        cairo_context->line_to(boundary.get_x() + 5, slot_center_y);
        cairo_context->stroke();

        double slot_circle_center;

        switch (GetStatus())
        {

        case WidgetStatus::SELECTED:
            slot_circle_center = boundary.get_x() + SLOT_CIRCLE_RADIUS/2 + SLOT_CIRCLE_RADIUS + 1;
            cairo_context->set_source_rgba(0.4, 0.9, 0.4, 1.0);
            cairo_context->arc(slot_circle_center, slot_center_y, SLOT_CIRCLE_RADIUS, 0.0, M_PI*2);
            cairo_context->fill();
            cairo_context->set_source_rgba(0.1, 0.1, 0.1, 1.0);
            cairo_context->arc(slot_circle_center, slot_center_y, SLOT_CIRCLE_RADIUS, 0.0, M_PI*2);
            cairo_context->stroke();
            break;

        case WidgetStatus::INFOCUS:
            slot_circle_center = boundary.get_x() + SLOT_CIRCLE_RADIUS/2 + SLOT_CIRCLE_RADIUS + 1;
            cairo_context->arc(slot_circle_center, slot_center_y, SLOT_CIRCLE_RADIUS, 0.0, M_PI*2);
            cairo_context->stroke();
            break;

        default:
            slot_circle_center = boundary.get_x() + SLOT_CIRCLE_RADIUS + 1;
            cairo_context->arc(slot_circle_center, slot_center_y, SLOT_CIRCLE_RADIUS/2, 0.0, M_PI*2);
            cairo_context->fill();
            break;
        }

    }
    else if (m_slot_type == SlotType::OUTBOUND)
    {
        int slot_center_x = boundary.get_x() + SLOT_INTERACTION_BOUNDS_SIZE;
     
        cairo_context->move_to(slot_center_x, slot_center_y);
        cairo_context->line_to(slot_center_x - 5, slot_center_y);
        cairo_context->stroke();

        double slot_circle_center;

        switch (GetStatus())
        {

        case WidgetStatus::SELECTED:
            slot_circle_center = slot_center_x - (SLOT_CIRCLE_RADIUS/2 + SLOT_CIRCLE_RADIUS);
            cairo_context->set_source_rgba(0.4, 0.9, 0.4, 1.0);
            cairo_context->arc(slot_circle_center, slot_center_y, SLOT_CIRCLE_RADIUS, 0.0, M_PI*2);
            cairo_context->fill();
            cairo_context->set_source_rgba(0.1, 0.1, 0.1, 1.0);
            cairo_context->arc(slot_circle_center, slot_center_y, SLOT_CIRCLE_RADIUS, 0.0, M_PI*2);
            cairo_context->stroke();
            break;

        case WidgetStatus::INFOCUS:
            slot_circle_center = slot_center_x - (SLOT_CIRCLE_RADIUS/2 + SLOT_CIRCLE_RADIUS);
            cairo_context->arc(slot_circle_center, slot_center_y, SLOT_CIRCLE_RADIUS, 0.0, M_PI*2);
            cairo_context->stroke();
            break;

        default:
            slot_circle_center = slot_center_x - (SLOT_CIRCLE_RADIUS + SLOT_CIRCLE_RADIUS/2);
            cairo_context->arc(slot_circle_center, slot_center_y, SLOT_CIRCLE_RADIUS/2, 0.0, M_PI*2);
            cairo_context->fill();
            break;
        }
    }
}


void
DatabridgeNodeSlot::SetLink(const std::shared_ptr<DatabridgeOrthogonalLink>& link)
{
    m_link = link;
}

void
DatabridgeNodeSlot::ClearLink()
{
    m_link = nullptr;
}

DatabridgePoint
DatabridgeNodeSlot::GetPosition() const
{
    int position_x = GetBoundaries()[0].get_x();
    int position_y = GetBoundaries()[0].get_y();
    return {position_x, position_y};
}

DatabridgePoint
DatabridgeNodeSlot::GetLinkOrigin() const
{   
    int position_x;
    int position_y = GetBoundaries()[0].get_y() + SLOT_INTERACTION_BOUNDS_SIZE/2;

    if (m_slot_type == SlotType::OUTBOUND)
    {
        position_x = GetBoundaries()[0].get_x() + SLOT_INTERACTION_BOUNDS_SIZE;   
    }
    else
    {
        position_x = GetBoundaries()[0].get_x();
    }

    return {position_x, position_y};
}


bool
DatabridgeNodeSlot::IsAvailable() const
{
    return m_link == nullptr;
}