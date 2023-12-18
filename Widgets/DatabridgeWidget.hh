#ifndef DATABRIDGEWIDGET_HH
#define DATABRIDGEWIDGET_HH


#include "DatabridgePoint.hh"

// CAIROMM includes
#include <cairomm/context.h>

// GDKMM includes
#include <gdkmm/rectangle.h>
#include <gdkmm/pixbuf.h>

// STD headers
#include <vector>

struct Point
{
    int x;
    int y;
};

enum class WidgetStatus
{
    OUTFOCUS,
    INFOCUS,
    SELECTED
};

class DatabridgeWidget
{
public:
    DatabridgeWidget();

    virtual ~DatabridgeWidget();

    std::string GetHash() const;

    virtual bool ContainPoint(const DatabridgePoint& point) const;

    virtual void SetInFocus();

    virtual void SetOutFocus();

    virtual void SetStatus(WidgetStatus status);

    WidgetStatus GetStatus() const;

    virtual void Draw(const Cairo::RefPtr<Cairo::Context>& cairo_context) const = 0;

protected:

    const std::vector<Gdk::Rectangle>& GetBoundaries() const;
    std::vector<Gdk::Rectangle>& GetBoundaries();

private:

    std::string m_hash;

    WidgetStatus m_status;

    std::vector<Gdk::Rectangle> m_boundaries;
};

#endif // DATABRIDGEWIDGET_HH