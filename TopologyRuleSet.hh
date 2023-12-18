#ifndef TOPOLOGYRULESET_HH
#define TOPOLOGYRULESET_HH

#include "Widgets/LinkLine.hh"

#include <list>
#include <functional>

class TopologyRuleSet
{
public:
    TopologyRuleSet();
    ~TopologyRuleSet();

    void ExecuteRule(std::list<std::unique_ptr<LinkLine>>&,
                     std::list<std::unique_ptr<LinkLine>>::iterator&,
                     std::list<std::unique_ptr<LinkLine>>::iterator&);
private:

    enum Rule
    {
        HORIZONTAL_HORIZONTAL_X = 0b0001,
        HORIZONTAL_HORIZONTAL_Y = 0b0010,
        HORIZONTAL_HORIZONTAL_XY = 0b0011,
        VERTICAL_VERTICAL_X = 0b1101,
        VERTICAL_VERTICAL_Y = 0b1110,
        VERTICAL_VERTICAL_XY = 0b1111,
        HORIZONTAL_VERTICAL_X = 0b0101,
        HORIZONTAL_VERTICAL_Y = 0b0110,
        HORIZONTAL_VERTICAL_XY = 0b0111,
        VERTICAL_HORIZONTAL_X = 0b1001,
        VERTICAL_HORIZONTAL_Y = 0b1010,
        VERTICAL_HORIZONTAL_XY = 0b1011
    };

    std::unordered_map<Rule, 
                       std::function<void(
                                std::list<std::unique_ptr<LinkLine>>&,
                                std::list<std::unique_ptr<LinkLine>>::iterator&,
                                std::list<std::unique_ptr<LinkLine>>::iterator&
                                )>> m_rules;
};


#endif // TOPOLOGYRULESET_HH