#ifndef DATABRIDGEBLOCK_HH
#define DATABRIDGEBLOCK_HH

#include "DatabridgeWidget.hh"
#include "DatabridgePoint.hh"

// GDKMM includes
#include <gdkmm/rectangle.h>
#include <gdkmm/pixbuf.h>

// STD headers
#include <vector>

class DatabridgeBlock : public DatabridgeWidget
{
public:
    DatabridgeBlock();

    virtual ~DatabridgeBlock();

    virtual void DragToOffset(const DatabridgePoint& offset) = 0;

    virtual DatabridgePoint GetPosition() const = 0;

};

#endif // DATABRIDGEBLOCK_HH