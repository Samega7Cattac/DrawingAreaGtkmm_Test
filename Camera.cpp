#include "Camera.hh"

Camera::Camera()
{
    m_transform_matrix = Cairo::identity_matrix();
    m_inverse_transform_matrix = Cairo::identity_matrix();
}

Camera::~Camera()
{

}

void
Camera::TranslateViewMatrix(const DatabridgePoint& translate)
{
    m_transform_matrix.translate(translate.GetX(), translate.GetY());
    m_inverse_transform_matrix = m_transform_matrix;
    m_inverse_transform_matrix.invert();
}

void
Camera::ScaleViewMatrix(double scale)
{
    m_transform_matrix.scale(scale, scale);
    m_inverse_transform_matrix = m_transform_matrix;
    m_inverse_transform_matrix.invert();
}

DatabridgePoint
Camera::ScreenToViewMatrix(const DatabridgePoint& point) const 
{
    double point_x = point.GetX();
    double point_y = point.GetY();

    m_inverse_transform_matrix.transform_point(point_x, point_y);
    return {(int)point_x, (int)point_y};
}


double
Camera::ScreenDistanceToViewMatrixDistance(const double& distance) const 
{
    double distance_x = distance;
    double distance_y = distance;

    m_inverse_transform_matrix.transform_distance(distance_x, distance_y);
    return distance_y;
}

const Cairo::Matrix&
Camera::GetViewMatrix() const
{
    return m_transform_matrix;
}