#include "DatabridgeOrthogonalLink.hh"
#include <iostream>

DatabridgeOrthogonalLink::DatabridgeOrthogonalLink(std::shared_ptr<TopologyRuleSet> rule_set,
                                                std::string start_node_hash,
                                                std::string start_slot_hash,
                                                int start_position_x,
                                                int start_position_y,
                                                std::string end_node_hash,
                                                std::string end_slot_hash,
                                                int end_position_x,
                                                int end_position_y)

                                                 : DatabridgeLink(
                                                    start_node_hash, 
                                                    start_slot_hash, 
                                                    end_node_hash, 
                                                    end_slot_hash)
{
    m_topology_rule_set = rule_set;
    //int central_arrow_x_position = start_node.GetOutboundNum()/2;
    //int start_slot_offset =  central_arrow_x_position - abs(start_slot.GetId() - start_node.GetOutboundNum());
    int start_slot_offset_length = SLOT_ARC_OFFSET /* * start_slot_offset*/;

    DatabridgePoint start_slot_point(start_position_x, start_position_y);
    const int offset_length = start_position_x + start_slot_offset_length;
    DatabridgePoint start_fixed_point(offset_length, start_position_y);

    //int end_slot_offset = end_node.GetInboundNum()/2 - abs(end_slot.GetId() - end_node.GetInboundNum());
    int end_slot_offset_length = -SLOT_ARC_OFFSET /* * end_slot_offset*/; 

    DatabridgePoint end_slot_point(end_position_x, end_position_y);
    DatabridgePoint end_fixed_point(end_position_x + end_slot_offset_length, end_position_y);


    m_lines.push_back(std::make_unique<HorizontalLinkLine>(GetStatus(), start_slot_point, start_fixed_point));
    m_lines.push_back(std::make_unique<HorizontalLinkLine>(GetStatus(), end_fixed_point, end_slot_point));

    UpdateLinkTopology();
}

DatabridgeOrthogonalLink::~DatabridgeOrthogonalLink()
{

}

void
DatabridgeOrthogonalLink::Draw(const Cairo::RefPtr<Cairo::Context>& cairo_context) const
{

    if (GetStatus() == WidgetStatus::SELECTED)
    {
        cairo_context->set_source_rgba(0.3, 0.3, 0.8, 1.0);
    }
    else
    {
        cairo_context->set_source_rgba(0, 0, 0, 1);
    }

    for (const std::unique_ptr<LinkLine>& line : m_lines)
    {
        
        line->Draw(cairo_context);
        
    }
}

void
DatabridgeOrthogonalLink::DragStartToOffset(const DatabridgePoint& offset)
{
    std::list<std::unique_ptr<LinkLine>>::iterator end = m_lines.end();
    end--; 

    bool can_drag_x = true;
    bool can_drag_y = true;

    int drag_x, drag_y;

    for (std::list<std::unique_ptr<LinkLine>>::iterator line = m_lines.begin();
         line != end; line++)
    {   
        if ((*line)->IsLocked() == false)
        {
            

            if (can_drag_x)
            {
                drag_x = offset.GetX();
            } 
            else 
            {
                drag_x = 0;
            }

            if (can_drag_y)
            {
                drag_y = offset.GetY();
            } 
            else 
            {
                drag_y = 0;
            }

            if ((*line)->IsHorizontal())
            {
                can_drag_x = false;
            }
            if ((*line)->IsVertical())
            {
                can_drag_y = false;
            }

            (*line)->DragToOffset(DatabridgePoint(drag_x, drag_y));
        }
        else
        {
            break;
        }
    }    
    UpdateLinkTopology();
}


void
DatabridgeOrthogonalLink::DragEndToOffset(const DatabridgePoint& offset)
{
    std::list<std::unique_ptr<LinkLine>>::reverse_iterator end = m_lines.rend();
    end--; 

    bool can_drag_x = true;
    bool can_drag_y = true;

    int drag_x, drag_y;

    for (std::list<std::unique_ptr<LinkLine>>::reverse_iterator line =
            m_lines.rbegin();
         line != end;
         line++)
    {   
        if ((*line)->IsLocked() == false)
        {
            

            if (can_drag_x)
            {
                drag_x = offset.GetX();
            } 
            else 
            {
                drag_x = 0;
            }

            if (can_drag_y)
            {
                drag_y = offset.GetY();
            } 
            else 
            {
                drag_y = 0;
            }

            if ((*line)->IsHorizontal())
            {
                can_drag_x = false;
            }
            if ((*line)->IsVertical())
            {
                can_drag_y = false;
            }

            (*line)->DragToOffset(DatabridgePoint(drag_x, drag_y));
        }
        else
        {
            break;
        }
    }
    UpdateLinkTopology();
}

void
DatabridgeOrthogonalLink::UpdateLinkTopology()
{

    if (m_lines.size() < 4)
    {
        int start_position_x = m_lines.front()->GetStartPoint().GetX() ;
        int start_position_y = m_lines.front()->GetStartPoint().GetY() ;

        int end_position_x = m_lines.back()->GetEndPoint().GetX() ;
        int end_position_y = m_lines.back()->GetEndPoint().GetY() ;

        int start_slot_offset_length = SLOT_ARC_OFFSET /* * start_slot_offset*/;

        DatabridgePoint start_slot_point(start_position_x, start_position_y);
        const int offset_length = start_position_x + start_slot_offset_length;
        DatabridgePoint start_fixed_point(offset_length, start_position_y);

        //int end_slot_offset = end_node.GetInboundNum()/2 - abs(end_slot.GetId() - end_node.GetInboundNum());
        int end_slot_offset_length = -SLOT_ARC_OFFSET /* * end_slot_offset*/; 

        DatabridgePoint end_slot_point(end_position_x, end_position_y);
        DatabridgePoint end_fixed_point(end_position_x + end_slot_offset_length, end_position_y);

        int mid_x = (end_fixed_point.GetX()  - start_fixed_point.GetX() )/2;
        int mid_y = (end_fixed_point.GetY()  - start_fixed_point.GetY() );

        DatabridgePoint mid_1(start_fixed_point.GetX()  + mid_x, start_fixed_point.GetY() );
        DatabridgePoint mid_2(start_fixed_point.GetX()  + mid_x, start_fixed_point.GetY()  + mid_y);

        WidgetStatus status = GetStatus();

        m_lines.clear();
        m_lines.push_back(std::make_unique<HorizontalLinkLine>(status, start_slot_point, start_fixed_point));
        m_lines.push_back(std::make_unique<HorizontalLinkLine>(status, start_fixed_point, mid_1));
        m_lines.push_back(std::make_unique<VerticalLinkLine>(status, mid_1, mid_2));
        m_lines.push_back(std::make_unique<HorizontalLinkLine>(status, mid_2, end_fixed_point));
        m_lines.push_back(std::make_unique<HorizontalLinkLine>(status, end_fixed_point, end_slot_point));

        return;
    }


    std::list<std::unique_ptr<LinkLine>>::iterator line_1_iter = 
        m_lines.begin();

    std::list<std::unique_ptr<LinkLine>>::iterator line_2_iter = 
        m_lines.begin();
    line_2_iter++;

    std::list<std::unique_ptr<LinkLine>>::iterator end = m_lines.end();

    while (line_2_iter != end && line_1_iter != end)
    {
        m_topology_rule_set->ExecuteRule(m_lines, line_1_iter, line_2_iter);
    }
}

void
DatabridgeOrthogonalLink::DragLineToOffset(const DatabridgePoint& pointer_position, const DatabridgePoint& offset)
{
    std::list<std::unique_ptr<LinkLine>>::iterator line_iter = 
        std::find_if(m_lines.begin(),
                m_lines.end(),
                [&pointer_position](std::unique_ptr<LinkLine>& line)
                {
                    return line->ContainPoint(pointer_position);
                });

    if (*line_iter == m_lines.back() || *line_iter == m_lines.front())
    {
        return;
    }

    if ((*line_iter)->IsHorizontal()) {
        (*line_iter)->DragToOffset(DatabridgePoint(0, offset.GetY()));
    } else {
        (*line_iter)->DragToOffset(DatabridgePoint(offset.GetX(), 0));
    }

    (*line_iter)->lock();

    UpdateLinkTopology();
}


void
DatabridgeOrthogonalLink::SetInFocus()
{
    if (GetStatus() == WidgetStatus::OUTFOCUS && GetStatus() == WidgetStatus::INFOCUS)
    {
        SetStatus(WidgetStatus::INFOCUS);
    }
}

void
DatabridgeOrthogonalLink::SetOutFocus()
{
    if (GetStatus() == WidgetStatus::OUTFOCUS && GetStatus() == WidgetStatus::INFOCUS)
    {
        SetStatus(WidgetStatus::OUTFOCUS);
    }
    UpdateLineStatus();
}

void
DatabridgeOrthogonalLink::SetStatus(WidgetStatus status)
{
    DatabridgeLink::SetStatus(status);
}

void
DatabridgeOrthogonalLink::UpdateLineStatus()
{
    for (std::unique_ptr<LinkLine>& line : m_lines)
    {
        line->SetStatus(GetStatus());
    }
}