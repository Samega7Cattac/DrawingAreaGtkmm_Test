#ifndef CAMERA_HH
#define CAMERA_HH

#include "Widgets/DatabridgePoint.hh"

#include <cairomm/cairomm.h>

class Camera
{
public:
    Camera();
    ~Camera();

    void TranslateViewMatrix(const DatabridgePoint& point);

    void ScaleViewMatrix(double scale);

    DatabridgePoint ScreenToViewMatrix(const DatabridgePoint& point) const;
    double ScreenDistanceToViewMatrixDistance(const double& distance) const;

    const Cairo::Matrix& GetViewMatrix() const;

private:
    Cairo::Matrix m_transform_matrix;
    Cairo::Matrix m_inverse_transform_matrix;
};

#endif