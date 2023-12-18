#include "DatabridgeNode.hh"
#include "../images/cloudflare.imgh"

// GTKMM headers
#include <glibmm/ustring.h>
#include <gdkmm/general.h>

#include <iostream>

constexpr int NODE_WIDTH = 100;
constexpr int NODE_SELECTED_OFFSET = 2;
constexpr int NODE_SLOT_MARGINS_OFFSET = 40;
constexpr int NODE_SLOT_IO_OFFSET = 25;

DatabridgeNode::DatabridgeNode(Glib::ustring name,
                               const DatabridgePoint& position) :
    m_name(name)
{

    // Create a new boundary
    // This widget only requires 1 boundary
    Gdk::Rectangle& boundary = GetBoundaries().emplace_back();

    boundary.set_width(NODE_WIDTH);
    boundary.set_height(2 * NODE_SLOT_MARGINS_OFFSET);
    boundary.set_x(position.GetX() - boundary.get_width() / 2);
    boundary.set_y(position.GetY() - boundary.get_height() / 2);

    // Set the icon
    m_symbol = Gdk::Pixbuf::create_from_data(CLOUDFLARE_PIXEL_DATA,
                                    Gdk::Colorspace::RGB,
                                    true,
                                    8,
                                    CLOUDFLARE_WIDTH,
                                    CLOUDFLARE_HEIGHT,
                                    CLOUDFLARE_ROWSTRIDE);

    // Create default slots
    InitNodeSlots(4, 4);    
}

DatabridgeNode::~DatabridgeNode()
{

}

Glib::ustring
DatabridgeNode::GetName() const
{
    return m_name;
}

void
DatabridgeNode::DragToOffset(const DatabridgePoint& offset)
{
    

    // Get boundary
    Gdk::Rectangle& boundary = GetBoundaries()[0];

    // Get current boundary position and add offset
    int new_x_pos = boundary.get_x() + offset.GetX();
    int new_y_pos = boundary.get_y() + offset.GetY();

    // update node position
    boundary.set_x(new_x_pos);
    boundary.set_y(new_y_pos);

    // Also update node's slot positions
    for (DatabridgeNodeSlot& node_slot : m_slots_inbound)
    {
        node_slot.DragToOffset(offset);
    }

    for (DatabridgeNodeSlot& node_slot : m_slots_outbound)
    {
        node_slot.DragToOffset(offset);
    }
}

void
DatabridgeNode::Draw(const Cairo::RefPtr<Cairo::Context>& cairo_context) const
{
    // Get boundary
    const Gdk::Rectangle& boundary = GetBoundaries()[0];
    
    // Highlight node when selected   
    if (GetStatus() == WidgetStatus::SELECTED)
    {
        cairo_context->set_source_rgba(0.3, 0.3, 0.8, 1.0);
        cairo_context->rectangle(boundary.get_x() - NODE_SELECTED_OFFSET,
                                boundary.get_y() - NODE_SELECTED_OFFSET,
                                boundary.get_width() + 2*NODE_SELECTED_OFFSET,
                                boundary.get_height() + 2*NODE_SELECTED_OFFSET);
        cairo_context->fill();
    }

    // Fill node background    WHITE
    cairo_context->set_source_rgba(1, 1, 1, 1.0);
    cairo_context->rectangle(boundary.get_x(),
                             boundary.get_y(),
                             boundary.get_width(),
                             boundary.get_height());
    cairo_context->fill();

    // Draw Node Outline       GRAY
    cairo_context->set_source_rgba(0.1, 0.1, 0.1, 1.0);
    cairo_context->rectangle(boundary.get_x(),
                             boundary.get_y(),\
                             boundary.get_width(),
                             boundary.get_height());
    cairo_context->stroke();

    // Font Settings
    cairo_context->select_font_face("Sans",
                                    Cairo::ToyFontFace::Slant::NORMAL,
                                    Cairo::ToyFontFace::Weight::NORMAL);
    cairo_context->set_font_size(10);
    Cairo::TextExtents text_extents;

    // Drawing node name
        // Gets the size of the text when it will be drawn to center
    cairo_context->get_text_extents(GetName(), text_extents);

    int text_position_x = boundary.get_x() + boundary.get_width() / 2;
    int text_position_y = boundary.get_y() + 10;

    int centering_text_position_x = text_position_x - text_extents.width / 2 - text_extents.x_bearing;
    int centering_text_position_y = text_position_y - text_extents.height / 2 - text_extents.y_bearing;
    
        // move the pointer to the position where, when drawn, the text will be centered
    cairo_context->move_to(centering_text_position_x, centering_text_position_y);

        // draw the text in the specified position
    cairo_context->show_text(GetName());

    // Drawing order number
        // Gets the size of the text when it will be drawn to center
    
    Cairo::TextExtents order_extents;
    cairo_context->get_text_extents(std::to_string(GetOrderId()), order_extents);

    int order_position_x = boundary.get_x() + boundary.get_width() / 2;
    int order_position_y = text_position_y + 10;

    int centering_order_position_x = order_position_x - order_extents.width / 2 - order_extents.x_bearing;
    int centering_order_position_y = order_position_y - order_extents.height / 2 - order_extents.y_bearing;
    
        // move the pointer to the position where, when drawn, the order will be centered
    cairo_context->move_to(centering_order_position_x, centering_order_position_y);

        // draw the text in the specified position
    cairo_context->show_text(std::to_string(GetOrderId()));
    
    // Draw Symbol
    int node_symbol_x = boundary.get_x() + boundary.get_width() / 2 - m_symbol->get_width() / 2;
    int node_symbol_y = boundary.get_y() + boundary.get_height() / 2 - m_symbol->get_height() / 2;

    Gdk::Cairo::set_source_pixbuf(cairo_context, m_symbol, node_symbol_x, node_symbol_y);
    cairo_context->paint();

    // Draw slots
    for (const DatabridgeNodeSlot& inbound_slot : m_slots_inbound)
    {
        inbound_slot.Draw(cairo_context);
    }
    for (const DatabridgeNodeSlot& outbound_slot : m_slots_outbound)
    {
        outbound_slot.Draw(cairo_context);
    }
}


void
DatabridgeNode::ClearInboundSlot(const std::string& slot_hash)
{
    std::vector<DatabridgeNodeSlot>::iterator inbound =
        std::find_if(m_slots_inbound.begin(),
                     m_slots_inbound.end(),
                     [&slot_hash](const DatabridgeNodeSlot& slot)
                     {
                        return slot_hash == slot.GetHash();
                     });
    if (inbound != m_slots_inbound.end())
    {
        inbound->ClearLink();
    }
}

void
DatabridgeNode::ClearOutboundSlot(const std::string& slot_hash)
{
    std::vector<DatabridgeNodeSlot>::iterator outbound =
        std::find_if(m_slots_outbound.begin(),
                     m_slots_outbound.end(),
                     [&slot_hash](const DatabridgeNodeSlot& slot)
                     {
                        return slot_hash == slot.GetHash();
                     });
    if (outbound != m_slots_outbound.end())
    {
        outbound->ClearLink();
    }
}


void
DatabridgeNode::SetOutboundSlotLink(const std::string& slot_hash,
                                    const std::shared_ptr<DatabridgeOrthogonalLink>& link)
{
    std::vector<DatabridgeNodeSlot>::iterator outbound =
        std::find_if(m_slots_outbound.begin(),
                     m_slots_outbound.end(),
                     [slot_hash](const DatabridgeNodeSlot& slot)
                     {
                        return slot_hash == slot.GetHash();
                     });
    if (outbound != m_slots_outbound.end())
    {
        outbound->SetLink(link);
    }
}

void
DatabridgeNode::SetInboundSlotLink(const std::string& slot_hash,
                                   const std::shared_ptr<DatabridgeOrthogonalLink>& link)
{
    std::vector<DatabridgeNodeSlot>::iterator inbound =
        std::find_if(m_slots_inbound.begin(),
                     m_slots_inbound.end(),
                     [slot_hash](const DatabridgeNodeSlot& slot)
                     {
                        return slot_hash == slot.GetHash();
                     });
    if (inbound != m_slots_inbound.end())
    {
        inbound->SetLink(link);
    }
}


void
DatabridgeNode::SetSlotStatus(const std::string& slot_hash, WidgetStatus status)
{
    std::vector<DatabridgeNodeSlot>::iterator inbound =
        std::find_if(m_slots_inbound.begin(),
                     m_slots_inbound.end(),
                     [slot_hash](const DatabridgeNodeSlot& slot)
                     {
                        return slot_hash == slot.GetHash();
                     });
    if (inbound != m_slots_inbound.end())
    {
        inbound->SetStatus(status);
        return;
    }

    std::vector<DatabridgeNodeSlot>::iterator outbound =
        std::find_if(m_slots_outbound.begin(),
                     m_slots_outbound.end(),
                     [slot_hash](const DatabridgeNodeSlot& slot)
                     {
                        return slot_hash == slot.GetHash();
                     });
    if (outbound != m_slots_outbound.end())
    {
        outbound->SetStatus(status);
        return;
    }
}



void
DatabridgeNode::SetOrderId(unsigned int order)
{
    m_order_id = order;
}

bool
DatabridgeNode::ContainInboundSlot(DatabridgePoint pointer) const
{
    for (const DatabridgeNodeSlot& slot : m_slots_inbound)
    {
        if (slot.ContainPoint(pointer))
        {
            return true;
        }
    }
    return false;
}

bool
DatabridgeNode::ContainOutboundSlot(DatabridgePoint pointer) const
{
    for (const DatabridgeNodeSlot& slot : m_slots_outbound)
    {
        if (slot.ContainPoint(pointer))
        {
            return true;
        }
    }
    return false;
}

std::string
DatabridgeNode::GetInboundSlotHash(DatabridgePoint pointer) const
{
    for (const DatabridgeNodeSlot& slot : m_slots_inbound)
    {
        if (slot.ContainPoint(pointer))
        {
            return slot.GetHash();
        }
    }
    return std::string();
}
std::string
DatabridgeNode::GetOutboundSlotHash(DatabridgePoint pointer) const
{
    for (const DatabridgeNodeSlot& slot : m_slots_outbound)
    {
        if (slot.ContainPoint(pointer))
        {
            return slot.GetHash();
        }
    }
    return std::string();
}

 
DatabridgePoint
DatabridgeNode::GetSlotLinkOrigin(const std::string& slot_hash) const
{
    std::vector<DatabridgeNodeSlot>::const_iterator inbound = 
        std::find_if(m_slots_inbound.cbegin(), 
                     m_slots_inbound.cend(), 
                     [slot_hash](const DatabridgeNodeSlot& link)
                     {
                        return link.GetHash() == slot_hash;
                     });
                     
    if (inbound != m_slots_inbound.cend())
    {
        return inbound->GetLinkOrigin();
    }

    std::vector<DatabridgeNodeSlot>::const_iterator outbound =
        std::find_if(m_slots_outbound.cbegin(),
                     m_slots_outbound.cend(), 
                     [slot_hash](const DatabridgeNodeSlot& link)
                     {
                        return link.GetHash() == slot_hash;
                     });

    if (outbound != m_slots_outbound.cend())
    {
        return outbound->GetLinkOrigin();
    }
    
    return {0,0};
}

DatabridgePoint
DatabridgeNode::GetPosition() const
{
    int position_x = GetBoundaries()[0].get_x();
    int position_y = GetBoundaries()[0].get_y();
    return {position_x, position_y};
}



unsigned int
DatabridgeNode::GetOrderId() const
{
    return m_order_id;
}

void
DatabridgeNode::InitNodeSlots(unsigned int num_input, unsigned int num_output)
{
    // Get boundary
    Gdk::Rectangle& boundary = GetBoundaries()[0];

    // Create inbound slots
    int slot_inbound_x = boundary.get_x();
    int slot_inbound_y = boundary.get_y();

    int slot_inbound_y_offset = NODE_SLOT_MARGINS_OFFSET;

    for (unsigned int input_id = 0; input_id < num_input; input_id++)
    {
        DatabridgeNodeSlot slot(slot_inbound_x, slot_inbound_y + slot_inbound_y_offset, SlotType::INBOUND);
        m_slots_inbound.push_back(slot);
        // update the offset for the next slot
        slot_inbound_y_offset += NODE_SLOT_IO_OFFSET;
    }

    slot_inbound_y_offset += NODE_SLOT_MARGINS_OFFSET - NODE_SLOT_IO_OFFSET;

    // Create outbound slots
    int slot_outbound_x = boundary.get_x() + boundary.get_width();
    int slot_outbound_y = boundary.get_y();

    int slot_outbound_y_offset = NODE_SLOT_MARGINS_OFFSET;

    for (unsigned int output_id = 0; output_id < num_output; output_id++)
    {
        DatabridgeNodeSlot slot(slot_outbound_x, slot_outbound_y + slot_outbound_y_offset, SlotType::OUTBOUND);
        m_slots_outbound.push_back(slot);
        slot_outbound_y_offset += NODE_SLOT_IO_OFFSET;
    }

    slot_outbound_y_offset += NODE_SLOT_MARGINS_OFFSET - NODE_SLOT_IO_OFFSET;

    // calculate resulting block height (max of the inbound and outbound face size)
    if (slot_inbound_y_offset > slot_outbound_y_offset)
    {
        boundary.set_height(slot_inbound_y_offset);
    }
    else
    {
        boundary.set_height(slot_outbound_y_offset);
    }


}


void
DatabridgeNode::UpdateSlotStatus(const std::string& slot_hash, WidgetStatus status)
{
    UpdateInboundSlotStatus(slot_hash, status);
    UpdateOutboundSlotStatus(slot_hash, status);
}


void
DatabridgeNode::UpdateInboundSlotStatus(const std::string& slot_hash, WidgetStatus status)
{
    
    for (DatabridgeNodeSlot& slot : m_slots_inbound)
    {
        if (slot.GetStatus() == WidgetStatus::SELECTED) continue;
        if (slot.GetHash() == slot_hash)
        {
            slot.SetStatus(status);
        } 
        else
        {
            slot.SetStatus(WidgetStatus::OUTFOCUS);
        }
    }
}


void
DatabridgeNode::UpdateOutboundSlotStatus(const std::string& slot_hash, WidgetStatus status)
{
    for (DatabridgeNodeSlot& slot : m_slots_outbound)
    {
        if (slot.GetStatus() == WidgetStatus::SELECTED) continue;
        if (slot.GetHash() == slot_hash)
        {
            slot.SetStatus(status);
        }
        else
        {
            slot.SetStatus(WidgetStatus::OUTFOCUS);
        }
    }
}


bool DatabridgeNode::IsSlotAvailable(const std::string&slot_hash) const
{
    std::vector<DatabridgeNodeSlot>::const_iterator inbound = 
        std::find_if(m_slots_inbound.cbegin(), 
                     m_slots_inbound.cend(), 
                     [slot_hash](const DatabridgeNodeSlot& slot)
                     {
                        return slot.GetHash() == slot_hash;
                     });
                     
    if (inbound != m_slots_inbound.cend())
    {
        return inbound->IsAvailable();
    }

    std::vector<DatabridgeNodeSlot>::const_iterator outbound =
        std::find_if(m_slots_outbound.cbegin(),
                     m_slots_outbound.cend(), 
                     [slot_hash](const DatabridgeNodeSlot& slot)
                     {
                        return slot.GetHash() == slot_hash;
                     });

    if (outbound != m_slots_outbound.cend())
    {
        return outbound->IsAvailable();
    }

    return false;
}

bool
DatabridgeNode::HasSlotInFocus() const
{
    std::vector<DatabridgeNodeSlot>::const_iterator inbound = 
        std::find_if(m_slots_inbound.cbegin(), 
                     m_slots_inbound.cend(), 
                     [](const DatabridgeNodeSlot& slot)
                     {
                        return slot.GetStatus();
                     });
                     
    if (inbound != m_slots_inbound.cend())
    {
        return inbound->IsAvailable();
    }

    std::vector<DatabridgeNodeSlot>::const_iterator outbound =
        std::find_if(m_slots_outbound.cbegin(),
                     m_slots_outbound.cend(), 
                     [](const DatabridgeNodeSlot& slot)
                     {
                        return slot.GetStatus();
                     });

    if (outbound != m_slots_outbound.cend())
    {
        return outbound->IsAvailable();
    }

    return false;
}


