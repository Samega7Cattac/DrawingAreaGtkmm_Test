#ifndef LINKLINE_HH
#define LINKLINE_HH

#include "DatabridgePoint.hh"
#include "DatabridgeWidget.hh"

constexpr int LINK_LINE_INTERACTION_WIDTH = 20;

class LinkLine : public DatabridgeWidget
{
    public:
        LinkLine(WidgetStatus status, const DatabridgePoint& start_point, const DatabridgePoint& end_point);
        //LinkLine(DatabridgePoint start_point, DatabridgePoint end_point);
        ~LinkLine();

        void Draw(const Cairo::RefPtr<Cairo::Context>& cairo_context) const override;

        bool operator==(const LinkLine& line);

        virtual void UpdateLineBounds() = 0;

        void SetStartPoint(const DatabridgePoint& new_point);
        void SetEndPoint(const DatabridgePoint& new_point);

        void SetInFocus() override;
        void SetOutFocus() override;

        DatabridgePoint GetStartPoint() const;
        DatabridgePoint GetEndPoint() const;

        void DragToOffset(const DatabridgePoint& offset);

        virtual bool IsHorizontal() const = 0;
        virtual bool IsVertical() const = 0;

        void lock();
        bool IsLocked();

    private:
        DatabridgePoint m_start_point;
        DatabridgePoint m_end_point;
        bool m_locked;
};

class HorizontalLinkLine : public LinkLine
{
    public:
        HorizontalLinkLine(WidgetStatus status, const DatabridgePoint& start_point, const DatabridgePoint& end_point);
        ~HorizontalLinkLine();

        void UpdateLineBounds() override;

        bool IsHorizontal() const override;
        bool IsVertical() const override;
        
};

class VerticalLinkLine : public LinkLine
{
    public:
        VerticalLinkLine(WidgetStatus status, const DatabridgePoint& start_point, const DatabridgePoint& end_point);
        ~VerticalLinkLine();

        void UpdateLineBounds() override;

        bool IsHorizontal() const override;
        bool IsVertical() const override;
};


#endif // LINKLINE_HH