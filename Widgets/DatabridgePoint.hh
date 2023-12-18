#ifndef DATABRIDGEPOINT_HH
#define DATABRIDGEPOINT_HH

class DatabridgePoint
{
    public:
        DatabridgePoint();
        DatabridgePoint(int x, int y);
        ~DatabridgePoint();

        void SetX(int x);
        void SetY(int y);

        int GetX() const;
        int GetY() const;
    
        bool operator==(const DatabridgePoint& point);
        DatabridgePoint operator+(const DatabridgePoint& point);
        DatabridgePoint operator-(const DatabridgePoint& point);

    private:
        int m_x;
        int m_y;
};


#endif // DATABRIDGEPOINT_HH