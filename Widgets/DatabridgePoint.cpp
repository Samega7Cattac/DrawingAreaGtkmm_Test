#include "DatabridgePoint.hh"


DatabridgePoint::DatabridgePoint()
{
    m_x = 0;
    m_y = 0;
}

DatabridgePoint::DatabridgePoint(int x, int y)
{
    m_x = x;
    m_y = y;
}

DatabridgePoint::~DatabridgePoint()
{
}


void
DatabridgePoint::SetX(int x)
{
    m_x = x;
}

void
DatabridgePoint::SetY(int y)
{
    m_y = y;
}

int
DatabridgePoint::GetX() const
{
    return m_x;
}

int
DatabridgePoint::GetY() const
{
    return m_y;
}

bool
DatabridgePoint::operator==(const DatabridgePoint& point)
{
    return m_x == point.GetX() && m_y == point.GetY();
}

DatabridgePoint
DatabridgePoint::operator+(const DatabridgePoint& point)
{
    return DatabridgePoint(m_x + point.GetX(), m_y + point.GetY());
}

DatabridgePoint
DatabridgePoint::operator-(const DatabridgePoint& point)
{
    return DatabridgePoint(m_x - point.GetX(), m_y - point.GetY());;
}