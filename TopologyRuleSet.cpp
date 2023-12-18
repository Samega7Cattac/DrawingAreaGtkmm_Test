#include "TopologyRuleSet.hh"
#include <any>
#include <iostream>

TopologyRuleSet::TopologyRuleSet()
{
    m_rules.insert(std::make_pair(Rule::HORIZONTAL_HORIZONTAL_XY,
[](std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
            WidgetStatus status = (*l_1)->GetStatus();
            if ((*l_1) == lines.front())
            {
                DatabridgePoint mid_point;
                mid_point.SetX((*l_1)->GetEndPoint().GetX());
                mid_point.SetY((*l_2)->GetStartPoint().GetY());
                
                l_2 = lines.insert(l_2,
                        std::make_unique<VerticalLinkLine>(
                        status,
                        (*l_1)->GetEndPoint(),
                        mid_point
                        ));

                l_2 = lines.insert(l_2,
                        std::make_unique<HorizontalLinkLine>(
                        status,
                        mid_point,
                        (*l_2)->GetStartPoint()
                        ));
                l_1++;
                l_2++;
            }
            else if ((*l_2) == lines.back())
            {
                DatabridgePoint mid_point;
                mid_point.SetX((*l_2)->GetStartPoint().GetX());
                mid_point.SetY((*l_1)->GetEndPoint().GetY());
                
                (*l_1)->SetEndPoint(mid_point);

                l_2 = lines.insert(l_2,
                        std::make_unique<VerticalLinkLine>(
                        status,
                        (*l_1)->GetEndPoint(),
                        (*l_2)->GetStartPoint()
                        ));
                l_1++;
                l_2++;
            }
            else if (!(*l_1)->IsLocked() && (*l_2)->IsLocked())
            {
                DatabridgePoint mid_point;
                mid_point.SetX((*l_1)->GetStartPoint().GetX());
                mid_point.SetY((*l_2)->GetStartPoint().GetY());

                (*l_2)->SetStartPoint(mid_point);
                l_1 = lines.erase(l_1);
                l_2 = l_1;
                l_2++;
            }
            else if ((*l_1)->IsLocked() && !(*l_2)->IsLocked())
            {
                DatabridgePoint mid_point;
                mid_point.SetX((*l_1)->GetStartPoint().GetX());
                mid_point.SetY((*l_2)->GetStartPoint().GetY());

                (*l_1)->SetEndPoint(mid_point);
                l_2 = lines.erase(l_2);
            }
            else
            {
                DatabridgePoint mid_point;
                mid_point.SetX((*l_2)->GetStartPoint().GetX());
                mid_point.SetY((*l_1)->GetEndPoint().GetY());
                
                (*l_1)->SetEndPoint(mid_point);

                l_2 = lines.insert(l_2,
                        std::make_unique<VerticalLinkLine>(
                        status,
                        (*l_1)->GetEndPoint(),
                        (*l_2)->GetStartPoint()
                        ));
                l_1++;
                l_2++;
            }
        }));

    m_rules.insert(std::make_pair(Rule::HORIZONTAL_HORIZONTAL_Y,
[](std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
            WidgetStatus status = (*l_1)->GetStatus();
            if ((*l_2) == lines.back())
            {
                if ((*l_1)->IsLocked())
                {
                    l_2 = lines.insert(l_2,
                            std::make_unique<VerticalLinkLine>(
                                status,
                                (*l_1)->GetEndPoint(),
                                (*l_2)->GetStartPoint()
                            ));
                        l_1++;
                        l_2++;

                }
                else
                {
                    DatabridgePoint mid_point;
                    mid_point.SetX((*l_1)->GetStartPoint().GetX());
                    mid_point.SetY((*l_2)->GetStartPoint().GetY());

                    (*l_1)->SetStartPoint(mid_point);
                    (*l_1)->SetEndPoint((*l_2)->GetStartPoint());
                    l_1--;
                    l_2--;
                }
            }
            else
            {
            l_2 = lines.insert(l_2,
                    std::make_unique<VerticalLinkLine>(
                        status,
                        (*l_1)->GetEndPoint(),
                        (*l_2)->GetStartPoint()
                    ));
                l_1++;
                l_2++;
            }
        }));

    m_rules.insert(std::make_pair(Rule::HORIZONTAL_HORIZONTAL_X,
[](std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
            if ((*l_1) == lines.front())
            {
                (*l_2)->SetStartPoint((*l_1)->GetEndPoint());
                l_1++;
                l_2++;
            }
            else if ((*l_2) == lines.back())
            {
                (*l_1)->SetEndPoint((*l_2)->GetStartPoint());
                l_1++;
                l_2++;
            }
            else if (!(*l_1)->IsLocked() && (*l_2)->IsLocked())
            {
                (*l_2)->SetStartPoint((*l_1)->GetStartPoint());
                l_1 = lines.erase(l_1);
                l_2 = l_1;
                l_2++;
            }
            else
            {
                (*l_1)->SetEndPoint((*l_2)->GetEndPoint());
                l_2 = lines.erase(l_2);
                l_1++;
                l_2++;
            }
        }));

    m_rules.insert(std::make_pair(Rule::VERTICAL_VERTICAL_XY,
[](std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
            WidgetStatus status = (*l_1)->GetStatus();
            if (!(*l_1)->IsLocked() && (*l_2)->IsLocked())
            {
                DatabridgePoint mid_point;
                mid_point.SetX((*l_2)->GetStartPoint().GetX());
                mid_point.SetY((*l_1)->GetStartPoint().GetY());

                (*l_2)->SetStartPoint(mid_point);
                l_2 = lines.erase(l_1);
                l_1 = l_2;
                l_1--;
            }
            else if ((*l_1)->IsLocked() && !(*l_2)->IsLocked())
            {
                DatabridgePoint mid_point;
                mid_point.SetX((*l_2)->GetStartPoint().GetX());
                mid_point.SetY((*l_1)->GetStartPoint().GetY());

                (*l_1)->SetEndPoint(mid_point);
                l_2 = lines.erase(l_2);
                l_1++;
                l_2++;
            }
            else
            {
                DatabridgePoint mid_point;
                mid_point.SetX((*l_2)->GetStartPoint().GetX());
                mid_point.SetY((*l_1)->GetEndPoint().GetY());
                
                (*l_1)->SetEndPoint(mid_point);

                l_2 = lines.insert(l_2,
                        std::make_unique<HorizontalLinkLine>(
                        status,
                            (*l_1)->GetEndPoint(),
                            (*l_2)->GetStartPoint()
                        ));
                l_1++;
                l_2++;
            }
        }));

    m_rules.insert(std::make_pair(Rule::VERTICAL_VERTICAL_Y,
[](std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
            if (!(*l_1)->IsLocked() && (*l_2)->IsLocked())
            {
                (*l_2)->SetStartPoint((*l_1)->GetStartPoint());
                l_1 = lines.erase(l_1);
                l_2 = l_1;
                l_2++;
            }
            else
            {
                (*l_1)->SetEndPoint((*l_2)->GetEndPoint());
                l_2 = lines.erase(l_2);
            }
        }));

    m_rules.insert(std::make_pair(Rule::VERTICAL_VERTICAL_X,
[](std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
            WidgetStatus status = (*l_1)->GetStatus();
            l_2 = lines.insert(l_2,
                    std::make_unique<HorizontalLinkLine>(
                        status,
                        (*l_1)->GetStartPoint(),
                        (*l_2)->GetEndPoint()
                    ));
            l_1++;
            l_2++;
        }));

    m_rules.insert(std::make_pair(Rule::HORIZONTAL_VERTICAL_XY,
        []([[maybe_unused]] std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
            if ((*l_1) == lines.front())
            {
                DatabridgePoint new_l2_end;
                new_l2_end.SetX((*l_1)->GetEndPoint().GetX());
                new_l2_end.SetY((*l_2)->GetEndPoint().GetY());

                (*l_2)->SetStartPoint((*l_1)->GetEndPoint());
                (*l_2)->SetEndPoint(new_l2_end);
            }
            else if (((*l_1)->IsLocked() && (*l_2)->IsLocked()) ||
                (!(*l_1)->IsLocked() && !(*l_2)->IsLocked()))
            {
                DatabridgePoint mid_point;
                mid_point.SetX((*l_2)->GetStartPoint().GetX());
                mid_point.SetY((*l_1)->GetEndPoint().GetY());

                (*l_1)->SetEndPoint(mid_point);
                (*l_2)->SetStartPoint(mid_point);
            }
            else if ((*l_1)->IsLocked() && !(*l_2)->IsLocked())
            {
                DatabridgePoint new_l2_end;
                new_l2_end.SetX((*l_1)->GetEndPoint().GetX());
                new_l2_end.SetY((*l_2)->GetEndPoint().GetY());

                (*l_2)->SetStartPoint((*l_1)->GetEndPoint());
                (*l_2)->SetEndPoint(new_l2_end);
            }
            else if (!(*l_1)->IsLocked() && (*l_2)->IsLocked())
            {
                DatabridgePoint new_l1_start;
                new_l1_start.SetX((*l_1)->GetStartPoint().GetX());
                new_l1_start.SetY((*l_2)->GetStartPoint().GetY());

                (*l_1)->SetStartPoint(new_l1_start);
                (*l_1)->SetEndPoint((*l_2)->GetStartPoint());
                l_1--;
                l_2--;
                return;
            }
            l_1++;
            l_2++;
        }));
    
    m_rules.insert(std::make_pair(Rule::HORIZONTAL_VERTICAL_Y,
        []([[maybe_unused]] std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
                (*l_2)->SetStartPoint((*l_1)->GetEndPoint());
                l_1++;
                l_2++;
            
        }));

    m_rules.insert(std::make_pair(Rule::HORIZONTAL_VERTICAL_X,
        []([[maybe_unused]] std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
            WidgetStatus status = (*l_1)->GetStatus();
            if ((*l_1) == lines.front())
            {
                l_2 = lines.insert(l_2,
                        std::make_unique<HorizontalLinkLine>(
                        status,
                        (*l_1)->GetEndPoint(),
                        (*l_2)->GetStartPoint()
                        ));
            }
            else /*if ((*l_2)->IsLocked())*/
            {
                (*l_1)->SetEndPoint((*l_2)->GetStartPoint());
            }
            /*else
            {
                DatabridgePoint new_l2_end;
                new_l2_end.SetX((*l_1)->GetEndPoint().GetX());
                new_l2_end.SetY((*l_2)->GetEndPoint().GetY());

                (*l_2)->SetStartPoint((*l_1)->GetEndPoint());
                (*l_2)->SetEndPoint(new_l2_end);
            }*/
            l_1++;
            l_2++;
        }));

    m_rules.insert(std::make_pair(Rule::VERTICAL_HORIZONTAL_XY,
        []([[maybe_unused]] std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
            if ((*l_2) == lines.back())
            {
                DatabridgePoint new_l1_start;
                new_l1_start.SetX((*l_2)->GetStartPoint().GetX());
                new_l1_start.SetY((*l_1)->GetStartPoint().GetY());

                (*l_1)->SetStartPoint(new_l1_start);
                (*l_1)->SetEndPoint((*l_2)->GetStartPoint());
                l_1--;
                l_2--;
                return;
            }
            else if (((*l_1)->IsLocked() && (*l_2)->IsLocked()) ||
                (!(*l_1)->IsLocked() && !(*l_2)->IsLocked()))
            {
                DatabridgePoint mid_point;
                mid_point.SetX((*l_1)->GetEndPoint().GetX());
                mid_point.SetY((*l_2)->GetStartPoint().GetY());

                (*l_1)->SetEndPoint(mid_point);
                (*l_2)->SetStartPoint(mid_point);
            }
            else if ((*l_1)->IsLocked() && !(*l_2)->IsLocked())
            {
                DatabridgePoint new_l2_end;
                new_l2_end.SetX((*l_2)->GetEndPoint().GetX());
                new_l2_end.SetY((*l_1)->GetEndPoint().GetY());

                (*l_2)->SetStartPoint((*l_1)->GetEndPoint());
                (*l_2)->SetEndPoint(new_l2_end);
            }
            else if (!(*l_1)->IsLocked() && (*l_2)->IsLocked())
            {
                DatabridgePoint new_l1_start;
                new_l1_start.SetX((*l_2)->GetStartPoint().GetX());
                new_l1_start.SetY((*l_1)->GetStartPoint().GetY());

                (*l_1)->SetStartPoint(new_l1_start);
                (*l_1)->SetEndPoint((*l_2)->GetStartPoint());
            }
            l_1++;
            l_2++;
        }));

    m_rules.insert(std::make_pair(Rule::VERTICAL_HORIZONTAL_Y,
        []([[maybe_unused]] std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
            if ((*l_2)->IsLocked() || (*l_2) == lines.back())
            {
                (*l_1)->SetEndPoint((*l_2)->GetStartPoint());
                l_1++;
                l_2++;
            }
            else
            {
                (*l_1)->SetEndPoint((*l_2)->GetStartPoint());
                l_1++;
                l_2++;
                /*DatabridgePoint new_l2_end;
                new_l2_end.SetX((*l_2)->GetEndPoint().GetX());
                new_l2_end.SetY((*l_1)->GetEndPoint().GetY());

                (*l_2)->SetStartPoint((*l_1)->GetEndPoint());
                (*l_2)->SetEndPoint(new_l2_end);
                l_1++;
                l_2++;*/
            }
        }));

    m_rules.insert(std::make_pair(Rule::VERTICAL_HORIZONTAL_X,
        []([[maybe_unused]] std::list<std::unique_ptr<LinkLine>>& lines, std::list<std::unique_ptr<LinkLine>>::iterator& l_1, std::list<std::unique_ptr<LinkLine>>::iterator& l_2)
        -> void {
            WidgetStatus status = (*l_1)->GetStatus();
            if ((*l_2) == lines.back())
            {
                if ((*l_1)->IsLocked())
                {
                    l_2 = lines.insert(l_2,
                        std::make_unique<HorizontalLinkLine>(
                        status,
                        (*l_1)->GetEndPoint(),
                        (*l_2)->GetStartPoint())
                        );
                    return;
                }
                else
                {
                    DatabridgePoint new_l1_start;
                    new_l1_start.SetX((*l_2)->GetStartPoint().GetX());
                    new_l1_start.SetY((*l_1)->GetStartPoint().GetY());

                    (*l_1)->SetEndPoint((*l_2)->GetStartPoint());
                    (*l_1)->SetStartPoint(new_l1_start);
                    l_1--;
                    l_2--;
                }
            }
            else if ((*l_1)->IsLocked())
            {
                (*l_2)->SetStartPoint((*l_1)->GetEndPoint());
                l_1++;
                l_2++;
            }
            else
            {
                (*l_2)->SetStartPoint((*l_1)->GetEndPoint());
                l_1++;
                l_2++; 
            }
        }));
}

TopologyRuleSet::~TopologyRuleSet()
{

}

void
TopologyRuleSet::ExecuteRule(std::list<std::unique_ptr<LinkLine>>& lines,
                            std::list<std::unique_ptr<LinkLine>>::iterator& line_1,
                            std::list<std::unique_ptr<LinkLine>>::iterator& line_2)
{
    std::underlying_type<TopologyRuleSet::Rule>::type e =
        (((*line_1)->IsVertical()) << 3) | ((*line_2)->IsVertical() << 2) | (((*line_1)->GetEndPoint().GetY() != (*line_2)->GetStartPoint().GetY()) << 1) | (((*line_1)->GetEndPoint().GetX() != (*line_2)->GetStartPoint().GetX()));

    auto iterator = m_rules.find(static_cast<TopologyRuleSet::Rule>(e));

    if (iterator == m_rules.end())
    {
        if ((*line_1)->GetEndPoint() == (*line_2)->GetStartPoint())
        {
            if ((*line_1) != lines.front() && (*line_2) != lines.back())
            {
                if ((((*line_1)->IsVertical() && (*line_2)->IsVertical()) 
                || ((*line_1)->IsHorizontal() && (*line_2)->IsHorizontal()))
                && ((*line_1)->IsLocked() == false && (*line_2)->IsLocked() == false))
                {
                    (*line_1)->SetEndPoint((*line_2)->GetEndPoint());
                    line_2 = lines.erase(line_2);
                    return;
                }
            }
            line_1++;
            line_2++;
            return;
        }
    }

    iterator->second(lines, line_1, line_2);
}


