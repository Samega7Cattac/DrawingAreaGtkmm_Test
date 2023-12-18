#include "LinkLine.hh"

/*LinkLine::LinkLine([[maybe_unused]] DatabridgePoint start_point, [[maybe_unused]] DatabridgePoint end_point)
    : m_locked(false)
{
    m_start_point = start_point;
    m_end_point = end_point;
}*/

LinkLine::LinkLine(WidgetStatus status, [[maybe_unused]] const DatabridgePoint& start_point, [[maybe_unused]] const DatabridgePoint& end_point)
    : m_locked(false)
{
    m_start_point = start_point;
    m_end_point = end_point;
    SetStatus(status);
}

LinkLine::~LinkLine()
{

}


bool
LinkLine::operator==(const LinkLine& line)
{
    return m_start_point == line.GetStartPoint()
        && m_end_point == line.GetEndPoint();
}

void
LinkLine::Draw(const Cairo::RefPtr<Cairo::Context>& cairo_context) const
{

    Cairo::RefPtr<Cairo::Pattern> parent_source = cairo_context->get_source();

    const DatabridgePoint& start_point = GetStartPoint();
    const DatabridgePoint& end_point = GetEndPoint();

    if (GetStatus() == WidgetStatus::INFOCUS)
    {
        cairo_context->set_line_width(3);
    }
    else if (GetStatus() == WidgetStatus::SELECTED)
    {
        cairo_context->set_source_rgba(0.3, 0.3, 0.8, 1.0);
        cairo_context->set_line_width(3);
    }

    cairo_context->move_to(start_point.GetX() , start_point.GetY());
    cairo_context->line_to(end_point.GetX() , end_point.GetY());
    
    cairo_context->stroke();

    cairo_context->set_line_width(1);
    
    cairo_context->set_source(parent_source);
}


void
LinkLine::DragToOffset(const DatabridgePoint& offset)
{
    m_start_point.SetX(m_start_point.GetX() + offset.GetX());
    m_start_point.SetY(m_start_point.GetY() + offset.GetY());

    m_end_point.SetX(m_end_point.GetX() + offset.GetX());
    m_end_point.SetY(m_end_point.GetY() + offset.GetY());

    UpdateLineBounds();
}

void
LinkLine::SetInFocus()
{
    SetStatus(WidgetStatus::INFOCUS);
}

void
LinkLine::SetOutFocus()
{
    SetStatus(WidgetStatus::OUTFOCUS);
}


DatabridgePoint
LinkLine::GetStartPoint() const
{
    return m_start_point;
}

DatabridgePoint
LinkLine::GetEndPoint() const
{
    return m_end_point;
}


void
LinkLine::SetStartPoint(const DatabridgePoint& new_point)
{
    m_start_point = new_point;
    UpdateLineBounds();
}

void
LinkLine::SetEndPoint(const DatabridgePoint& new_point)
{
    m_end_point = new_point;
    UpdateLineBounds();
}




void
LinkLine::lock()
{
    m_locked = true;
}

bool
LinkLine::IsLocked()
{
    return m_locked;
}

HorizontalLinkLine::HorizontalLinkLine(WidgetStatus status, const DatabridgePoint& start_point, const DatabridgePoint& end_point)
                                         : LinkLine(status, start_point, end_point)
{
    int bound_start_x, bound_start_y;
    int bound_width, bound_height;

    if (start_point.GetX() > end_point.GetX())
    {
        bound_start_x = end_point.GetX() ;
        bound_start_y = end_point.GetY()  - LINK_LINE_INTERACTION_WIDTH/2;
        bound_height = LINK_LINE_INTERACTION_WIDTH;
        bound_width = start_point.GetX() - end_point.GetX();
    }
    else
    {
        bound_start_x = start_point.GetX() ;
        bound_start_y = start_point.GetY()  - LINK_LINE_INTERACTION_WIDTH/2;
        bound_height = LINK_LINE_INTERACTION_WIDTH;
        bound_width = end_point.GetX() - start_point.GetX();
    }

    Gdk::Rectangle bound{bound_start_x, bound_start_y, bound_width, bound_height};

    GetBoundaries().push_back(bound);
}

HorizontalLinkLine::~HorizontalLinkLine()
{

}

void
HorizontalLinkLine::UpdateLineBounds()
{
    const DatabridgePoint& start_point = GetStartPoint();
    const DatabridgePoint& end_point = GetEndPoint();

    int bound_start_x, bound_start_y;
    int bound_width, bound_height;

    if (start_point.GetX() > end_point.GetX())
    {
        bound_start_x = end_point.GetX() ;
        bound_start_y = end_point.GetY()  - LINK_LINE_INTERACTION_WIDTH/2;
        bound_height = LINK_LINE_INTERACTION_WIDTH;
        bound_width = start_point.GetX() - end_point.GetX();
    }
    else
    {
        bound_start_x = start_point.GetX() ;
        bound_start_y = start_point.GetY()  - LINK_LINE_INTERACTION_WIDTH/2;
        bound_height = LINK_LINE_INTERACTION_WIDTH;
        bound_width = end_point.GetX() - start_point.GetX();
    }

    GetBoundaries()[0].set_x(bound_start_x);
    GetBoundaries()[0].set_y(bound_start_y);
    GetBoundaries()[0].set_width(bound_width);
    GetBoundaries()[0].set_height(bound_height);
}


bool
HorizontalLinkLine::IsHorizontal() const
{
    return true;
}

bool
HorizontalLinkLine::IsVertical() const
{
    return false;
}


VerticalLinkLine::VerticalLinkLine(WidgetStatus status, const DatabridgePoint& start_point, const DatabridgePoint& end_point)
                                         : LinkLine(status, start_point, end_point)
{
    int bound_start_x, bound_start_y;
    int bound_width, bound_height;

    if (start_point.GetY() > end_point.GetY())
    {
        bound_start_x = end_point.GetX() - LINK_LINE_INTERACTION_WIDTH/2;
        bound_start_y = end_point.GetY() ;
        bound_height = start_point.GetY() - end_point.GetY();
        bound_width = LINK_LINE_INTERACTION_WIDTH; 
    }
    else
    {
        bound_start_x = start_point.GetX() - LINK_LINE_INTERACTION_WIDTH/2;
        bound_start_y = start_point.GetY() ;
        bound_height = end_point.GetY() - start_point.GetY();;
        bound_width = LINK_LINE_INTERACTION_WIDTH; 
    }

    Gdk::Rectangle bound{bound_start_x, bound_start_y, bound_width, bound_height};

    GetBoundaries().push_back(bound);
}

VerticalLinkLine::~VerticalLinkLine()
{

}

void
VerticalLinkLine::UpdateLineBounds()
{
    const DatabridgePoint& start_point = GetStartPoint();
    const DatabridgePoint& end_point = GetEndPoint();

    int bound_start_x, bound_start_y;
    int bound_width, bound_height;

    if (start_point.GetY() > end_point.GetY())
    {
        bound_start_x = end_point.GetX() - LINK_LINE_INTERACTION_WIDTH/2;
        bound_start_y = end_point.GetY() ;
        bound_height = start_point.GetY() - end_point.GetY();
        bound_width = LINK_LINE_INTERACTION_WIDTH; 
    }
    else
    {
        bound_start_x = start_point.GetX() - LINK_LINE_INTERACTION_WIDTH/2;
        bound_start_y = start_point.GetY() ;
        bound_height = end_point.GetY() - start_point.GetY();;
        bound_width = LINK_LINE_INTERACTION_WIDTH; 
    }


    GetBoundaries()[0].set_x(bound_start_x);
    GetBoundaries()[0].set_y(bound_start_y);
    GetBoundaries()[0].set_width(bound_width);
    GetBoundaries()[0].set_height(bound_height);
}


bool
VerticalLinkLine::IsHorizontal() const
{
    return false;
}

bool
VerticalLinkLine::IsVertical() const
{
    return true;
}