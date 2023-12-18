
#include <iostream>

#include "BlockArea.hh"

#include <gtkmm/droptarget.h>
#include <gdkmm/general.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/eventcontrollermotion.h>
#include <gtkmm/eventcontrollerkey.h>

#include <gdk/gdkkeysyms.h>

BlockArea::BlockArea()
{
    set_can_focus(true);

    set_draw_func(sigc::mem_fun(*this, &BlockArea::on_draw));

    // Drag left button input controller
    Glib::RefPtr<Gtk::GestureDrag> gesture_drag_controller = Gtk::GestureDrag::create();
    gesture_drag_controller->set_button(1);
    gesture_drag_controller->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    gesture_drag_controller->signal_drag_begin().connect(
        sigc::mem_fun(*this, &BlockArea::on_left_drag_start), false
    );
    gesture_drag_controller->signal_drag_end().connect(
        sigc::mem_fun(*this, &BlockArea::on_left_drag_end), false
    );
    gesture_drag_controller->signal_drag_update().connect(
        sigc::mem_fun(*this, &BlockArea::on_left_drag_update), false
    );
    add_controller(gesture_drag_controller);


    // Drag Middle-button input controller
    Glib::RefPtr<Gtk::EventControllerScroll> scroll_controller = Gtk::EventControllerScroll::create();
    scroll_controller->set_flags(Gtk::EventControllerScroll::Flags::VERTICAL);
    scroll_controller->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    scroll_controller->signal_scroll().connect(
        sigc::mem_fun(*this, &BlockArea::on_scroll), false
    );

    add_controller(scroll_controller);

    // Drag Middle-button input controller
    Glib::RefPtr<Gtk::GestureDrag> gesture_drag_middle = Gtk::GestureDrag::create();
    gesture_drag_middle->set_button(2);
    gesture_drag_middle->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    gesture_drag_middle->signal_drag_begin().connect(
        sigc::mem_fun(*this, &BlockArea::on_middle_drag_start), false
    );
    gesture_drag_middle->signal_drag_update().connect(
        sigc::bind(sigc::mem_fun(*this, &BlockArea::on_middle_drag_update), gesture_drag_middle), false
    );
    gesture_drag_middle->signal_drag_end().connect(
        sigc::mem_fun(*this, &BlockArea::on_middle_drag_end), false
    );

    add_controller(gesture_drag_middle);


    // Left-button click input controller
    Glib::RefPtr<Gtk::GestureClick> gesture_click_left = Gtk::GestureClick::create();
    gesture_click_left->set_button(1);
    gesture_click_left->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    gesture_click_left->signal_pressed().connect(
        sigc::mem_fun(*this, &BlockArea::on_button_left_press), false
    );
    gesture_click_left->signal_released().connect(
        sigc::mem_fun(*this, &BlockArea::on_button_left_released), false
    );

    add_controller(gesture_click_left);
    //add_controller(gesture_click_2);

    // Mouse position controller
    Glib::RefPtr<Gtk::EventControllerMotion> mouse_position_controller = Gtk::EventControllerMotion::create();
    mouse_position_controller->signal_motion().connect(
        sigc::mem_fun(*this, &BlockArea::on_motion), false
    );
    add_controller(mouse_position_controller);



    // Out-of-widget drag target controller
    const GType ustring_type = Glib::Value<Glib::ustring>::value_type();
    Glib::RefPtr<Gtk::DropTarget> drop_target = Gtk::DropTarget::create(ustring_type, Gdk::DragAction::COPY);
    drop_target->signal_drop().connect(
        sigc::mem_fun(*this, &BlockArea::on_drop_data), false
    );
    add_controller(drop_target);

    m_rule_set = std::make_shared<TopologyRuleSet>();
    m_drag_action = std::make_unique<NoDragAction>(DatabridgePoint());

}

BlockArea::~BlockArea()
{
    
}

void
BlockArea::RemoveNode(const std::string& node_hash)
{
    std::shared_ptr<DatabridgeNode> node = m_nodes.at(node_hash);

    std::unordered_map<std::string, std::shared_ptr<DatabridgeOrthogonalLink>>::iterator link_iter;

    link_iter = std::find_if(m_links.begin(),
                 m_links.end(),
                 [&node_hash](std::pair<const std::string, std::shared_ptr<DatabridgeOrthogonalLink>>& pair){
                    return pair.second->GetStartNodeHash() == node_hash || pair.second->GetEndNodeHash() == node_hash;
                 });

    while (link_iter != m_links.end())
    {
        std::shared_ptr<DatabridgeOrthogonalLink>& link_ptr = link_iter->second;

        std::string start_node_hash = link_ptr->GetStartNodeHash();
        std::string end_node_hash = link_ptr->GetEndNodeHash();

        std::string start_slot_hash = link_ptr->GetStartNodeSlotHash();
        std::string end_slot_hash = link_ptr->GetEndNodeSlotHash();

        m_nodes.at(start_node_hash)->ClearOutboundSlot(start_slot_hash);
        m_nodes.at(end_node_hash)->ClearInboundSlot(end_slot_hash);

        link_iter = m_links.erase(link_iter);

        link_iter = std::find_if(link_iter,
                 m_links.end(),
                 [&node_hash](std::pair<const std::string, std::shared_ptr<DatabridgeOrthogonalLink>>& pair){
                    return pair.second->GetStartNodeHash() == node_hash || pair.second->GetEndNodeHash() == node_hash;
                 });
    }

    
    m_nodes.erase(node_hash);
}


void
BlockArea::RemoveLink(const std::string& hash)
{
    std::shared_ptr<DatabridgeOrthogonalLink>& link_ptr = m_links.at(hash);

    std::string start_node_hash = link_ptr->GetStartNodeHash();
    std::string end_node_hash = link_ptr->GetEndNodeHash();

    std::string start_slot_hash = link_ptr->GetStartNodeSlotHash();
    std::string end_slot_hash = link_ptr->GetEndNodeSlotHash();

    m_nodes.at(start_node_hash)->ClearOutboundSlot(start_slot_hash);
    m_nodes.at(end_node_hash)->ClearInboundSlot(end_slot_hash);

    m_links.erase(hash);
}

void
BlockArea::RemoveNode(std::shared_ptr<DatabridgeNode> node_ptr)
{
    std::unordered_map<std::string, std::shared_ptr<DatabridgeOrthogonalLink>>::iterator link_iter;

    link_iter = std::find_if(m_links.begin(),
                 m_links.end(),
                 [&node_ptr](std::pair<const std::string, std::shared_ptr<DatabridgeOrthogonalLink>>& pair){
                    return pair.second->GetStartNodeHash() == node_ptr->GetHash() || pair.second->GetEndNodeHash() == node_ptr->GetHash();
                 });

    while (link_iter != m_links.end())
    {
        std::shared_ptr<DatabridgeOrthogonalLink>& link_ptr = link_iter->second;

        std::string start_node_hash = link_ptr->GetStartNodeHash();
        std::string end_node_hash = link_ptr->GetEndNodeHash();

        std::string start_slot_hash = link_ptr->GetStartNodeSlotHash();
        std::string end_slot_hash = link_ptr->GetEndNodeSlotHash();

        m_nodes.at(start_node_hash)->ClearOutboundSlot(start_slot_hash);
        m_nodes.at(end_node_hash)->ClearInboundSlot(end_slot_hash);

        link_iter = m_links.erase(link_iter);

        link_iter = std::find_if(link_iter,
                 m_links.end(),
                 [&node_ptr](std::pair<const std::string, std::shared_ptr<DatabridgeOrthogonalLink>>& pair){
                    return pair.second->GetStartNodeHash() == node_ptr->GetHash() || pair.second->GetEndNodeHash() == node_ptr->GetHash();
                 });
    }

    
    m_nodes.erase(node_ptr->GetHash());
}


void
BlockArea::RemoveLink(std::shared_ptr<DatabridgeLink> link_ptr)
{
    std::string start_node_hash = link_ptr->GetStartNodeHash();
    std::string end_node_hash = link_ptr->GetEndNodeHash();

    std::string start_slot_hash = link_ptr->GetStartNodeSlotHash();
    std::string end_slot_hash = link_ptr->GetEndNodeSlotHash();

    m_nodes.at(start_node_hash)->ClearOutboundSlot(start_slot_hash);
    m_nodes.at(end_node_hash)->ClearInboundSlot(end_slot_hash);

    m_links.erase(link_ptr->GetHash());
}


void
BlockArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, [[maybe_unused]] int width, [[maybe_unused]] int height)
{
    // Set line width to 1
    cr->set_line_width(1);
    cr->transform(m_camera.GetViewMatrix());

    for (const auto& [hash, node] : m_nodes)
    {
        node->Draw(cr);
    }

    for (const auto& [hash, link] : m_links)
    {
        link->Draw(cr);
    }

    // if (m_arc_builder != nullptr)
    // {
    //     const std::string& node_start_hash = m_arc_builder->GetStartNodeHash();
    //     DatabridgeNode& start_node = m_nodes.at(node_start_hash);
    //     std::vector<DatabridgeNodeSlot>& start_slots = start_node->GetOutboundSlots();

    //     DatabridgeNodeSlot& start_slot = start_slots.at(m_arc_builder->GetStartSlotId());

    //     cr->move_to(start_slot.GetX(), start_slot.GetY());
    //     cr->line_to(m_arc_builder->GetArcEndX(), m_arc_builder->GetArcEndY());
    //     cr->stroke();
    // }
}

void
BlockArea::on_motion(double pointer_x, double pointer_y)
{
    DatabridgePoint pointer(pointer_x, pointer_y);
    pointer = m_camera.ScreenToViewMatrix(pointer);

    for (auto& [hash, node] : m_nodes)
    {   
        if (node->ContainPoint(pointer) == true)
        {
            node->SetInFocus();
        }
        else
        {
            node->SetOutFocus();
        }
        if (m_link_builder != nullptr)
        {
            std::string slot_hash = node->GetInboundSlotHash(pointer);
            node->UpdateInboundSlotStatus(slot_hash, WidgetStatus::INFOCUS);
        }
        else
        {
            std::string slot_hash = node->GetOutboundSlotHash(pointer);
            node->UpdateOutboundSlotStatus(slot_hash, WidgetStatus::INFOCUS);
        }
    }

    if (m_drag_action->GetDragType() == DragType::NO_DRAG)
    {
        for (auto& [hash, link] : m_links)
        {
            if (link->ContainPoint(pointer) == false)
            {
                link->SetOutFocus();
                continue;
            }
            else
            {
                link->SetOutFocus();
                link->SetLineStatus(WidgetStatus::INFOCUS, pointer);
            }
        }
    }

    queue_draw();
}

bool
BlockArea::on_drop_data([[maybe_unused]]const Glib::ValueBase& value, [[maybe_unused]] double pointer_x, [[maybe_unused]] double pointer_y)
{
    DatabridgePoint pointer(pointer_x, pointer_y);

    pointer = m_camera.ScreenToViewMatrix(pointer);

    if (G_VALUE_HOLDS(value.gobj(), Glib::Value<Glib::ustring>::value_type()))
    {
        // We got the value type that we expected.
        Glib::Value<Glib::ustring> ustring_value;
        ustring_value.init(value.gobj());
        const Glib::ustring dropped_string = ustring_value.get();

        if (dropped_string != "")
        {
            std::shared_ptr<DatabridgeNode> node_ptr = 
                std::make_shared<DatabridgeNode>(dropped_string,
                                                 pointer);
            
            m_nodes.emplace(node_ptr->GetHash(), node_ptr);
        }

        queue_draw();
        // std::cout << "Received \"" << dropped_string << "\" in drawing area " << std::endl;
        return true;
    }
    return false;
}

void
BlockArea::on_button_left_press(int n_press, [[maybe_unused]] double pointer_x, [[maybe_unused]] double pointer_y)
{
    DatabridgePoint pointer_position = DatabridgePoint(pointer_x, pointer_y);
    pointer_position = m_camera.ScreenToViewMatrix(pointer_position);

    if (n_press == 1) 
    {
        bool has_performed_action = false;

        for (auto& [hash, node] : m_nodes)
        {
            if (node->ContainPoint(pointer_position) == true)
            {
                node->SetStatus(WidgetStatus::SELECTED);
                has_performed_action = true;
            }
            else
            {
                node->SetStatus(WidgetStatus::OUTFOCUS);
                continue;    
            }


            if (m_link_builder != nullptr)
            {
                
                if (node->ContainInboundSlot(pointer_position))
                {
                    std::string hash = node->GetInboundSlotHash(pointer_position);

                    if (node->IsSlotAvailable(hash))
                    {
                        DatabridgePoint slot_link_position = 
                            node->GetSlotLinkOrigin(hash);

                        auto varptr = m_nodes.at(m_link_builder->GetStartNodeHash());
                        varptr->SetSlotStatus(m_link_builder->GetStartSlotHash(),
                                            WidgetStatus::OUTFOCUS);
                        
                        m_link_builder->endArc(node->GetHash(), hash, slot_link_position.GetX() , slot_link_position.GetY() );
                        m_link_builder->build(m_nodes, m_links);

                        node->UpdateInboundSlotStatus(hash, WidgetStatus::OUTFOCUS);

                        m_link_builder.release();

                        continue;
                    }

                }
            }
            else
            {
                if (node->ContainOutboundSlot(pointer_position))
                {
                    std::string hash = node->GetOutboundSlotHash(pointer_position);

                    if (node->IsSlotAvailable(hash))
                    {
                        DatabridgePoint slot_link_position =
                            node->GetSlotLinkOrigin(hash);

                        m_link_builder =
                            std::make_unique<LinkBuilder>(
                                                        m_rule_set,
                                                        node->GetHash(), 
                                                        hash,
                                                        slot_link_position.GetX() ,
                                                        slot_link_position.GetY() );

                        node->SetSlotStatus(hash, WidgetStatus::SELECTED);

                        continue;
                    }
                }
            }
        
        }

        for (auto& [hash, link] : m_links)
        {
            if (link->ContainPoint(pointer_position) && m_drag_action->GetDragType() == DragType::NO_DRAG)
            {
                link->SetStatus(WidgetStatus::SELECTED);
            } else
            {
                link->SetStatus(WidgetStatus::OUTFOCUS);
            }
        }

        if (!has_performed_action && m_link_builder != nullptr)
        {
            auto varptr = m_nodes.at(m_link_builder->GetStartNodeHash());
            varptr->SetSlotStatus(m_link_builder->GetStartSlotHash(),
                                    WidgetStatus::OUTFOCUS);

            m_link_builder.release();
            m_link_builder = nullptr;
        }
    }
    
    queue_draw();
}

void
BlockArea::on_button_left_released([[maybe_unused]] int n_press,
                              [[maybe_unused]] double pointer_x,
                              [[maybe_unused]] double pointer_y)
{
}

bool
BlockArea::on_scroll([[maybe_unused]]double d_scroll_x, double d_scroll_y)
{
    

    double scale_exp = exp(-0.05*d_scroll_y);
    m_camera.ScaleViewMatrix(scale_exp);

    /*double width = get_width();
    width = m_camera.ScreenDistanceToViewMatrixDistance(width);
    DatabridgePoint offset(width/2, width/2);

    m_camera.TranslateViewMatrix(offset);*/
    queue_draw();
    return true;
}



void
BlockArea::on_middle_drag_start([[maybe_unused]] double pointer_x,
                                [[maybe_unused]] double pointer_y)
{
   
}


void
BlockArea::on_middle_drag_update([[maybe_unused]] double offset_x,
                                 [[maybe_unused]] double offset_y,
                                 [[maybe_unused]] Glib::RefPtr<Gtk::GestureDrag> gesture)
{
    
    /*double start_x, start_y;
    gesture->get_start_point(start_x, start_y);
    DatabridgePoint start(start_x, start_y);*/
    DatabridgePoint offset(offset_x, offset_y);

    m_camera.TranslateViewMatrix(offset - m_lastPanOffset);

    m_lastPanOffset = offset;

}


void
BlockArea::on_middle_drag_end([[maybe_unused]] double pointer_x,
                              [[maybe_unused]] double pointer_y)
{
    m_lastPanOffset = DatabridgePoint();
}



void
BlockArea::on_left_drag_start(double pointer_x, double pointer_y)
{
    DatabridgePoint pointer_position((int)pointer_x, (int)pointer_y);
    pointer_position = m_camera.ScreenToViewMatrix(pointer_position);

    if (m_drag_action->GetDragType() == DragType::NO_DRAG)
    {
        for (auto& [hash, link] : m_links)
        {
            if (link->ContainPoint(pointer_position) == true)
            {
                m_drag_action =
                    std::make_unique<DragLinkAction>(pointer_position,
                                                     hash,
                                                     link->GetLineHash(pointer_position));
                return;
            }
        }

        for (auto& [hash, node] : m_nodes)
        {
            if (node->ContainPoint(pointer_position) == true)
            {
                if (node->HasSlotInFocus() == true) return;

                m_drag_action =
                    std::make_unique<DragNodeAction>(pointer_position,
                                                     hash);
                return;
            }
        }
    }
}


void
BlockArea::on_left_drag_update([[maybe_unused]] double pointer_offset_x, [[maybe_unused]] double pointer_offset_y)
{
    DatabridgePoint pointer_offset(pointer_offset_x, pointer_offset_y);

    if (m_drag_action->GetDragType() == DragType::DRAG_NODE)
    {

        std::string node_hash = m_drag_action->GetHash(DragNodeAction::NODE_HASH);

        std::shared_ptr<DatabridgeNode>& node = m_nodes.at(node_hash);
        
        DatabridgePoint last_offset = m_drag_action->GetLastOffset();

        DatabridgePoint new_offset = pointer_offset - last_offset;

        node->DragToOffset(new_offset);

        m_drag_action->SetLastOffset(pointer_offset);
    } 
    else if (m_drag_action->GetDragType() == DragType::DRAG_LINK)
    {

        DatabridgePoint start_position = m_drag_action->GetStartPosition();
        DatabridgePoint last_offset = m_drag_action->GetLastOffset();


        DatabridgePoint new_offset = pointer_offset - last_offset;

        DatabridgePoint current_position = start_position + last_offset;

        
        std::string link_hash = m_drag_action->GetHash(DragLinkAction::LINK_HASH);
        std::string line_hash = m_drag_action->GetHash(DragLinkAction::LINE_HASH);


        std::shared_ptr<DatabridgeOrthogonalLink>& link = m_links.at(link_hash);

        if (link->LineContainPoint(line_hash, current_position) == true)
        {
            link->SetLineStatus(WidgetStatus::SELECTED, current_position);
            link->DragLineToOffset(current_position, new_offset);
        }


        m_drag_action->SetLastOffset(pointer_offset);
    }
    queue_draw();
}

void
BlockArea::on_left_drag_end([[maybe_unused]] double pointer_offset_x,
                       [[maybe_unused]] double pointer_offset_y)
{
    DatabridgePoint pointer_offset(pointer_offset_x, pointer_offset_y);
    
    m_drag_action = std::make_unique<NoDragAction>(pointer_offset);

    queue_draw();
}

bool
BlockArea::on_key_pressed(guint keyval, guint, Gdk::ModifierType)
{

    switch (keyval)
    {
    case GDK_KEY_Escape:

        if (m_link_builder != nullptr)
        {
            std::shared_ptr<DatabridgeNode> node_ptr = m_nodes.at(m_link_builder->GetStartNodeHash());
            node_ptr->SetSlotStatus(m_link_builder->GetStartSlotHash(),
                                    WidgetStatus::OUTFOCUS);

            m_link_builder.release();
            m_link_builder = nullptr;            
        }

        return true;

    case GDK_KEY_Delete:

        std::vector<std::string> link_to_remove;

        for (auto [hash, link] : m_links)
        {
            if (link->GetStatus() == WidgetStatus::SELECTED)
            {
                link_to_remove.push_back(hash);
            }
        }

        if (!link_to_remove.empty())
        {
            for (std::string& hash : link_to_remove)
            {
                RemoveLink(hash);
            }
            queue_draw();
        }
        return true;



    }
    return false;
}



std::unordered_map<std::string, std::shared_ptr<DatabridgeNode>>&
BlockArea::GetNodes()
{
    return m_nodes;
}

std::unordered_map<std::string, std::shared_ptr<DatabridgeOrthogonalLink>>&
BlockArea::GetLinks()
{
    return m_links;
}

