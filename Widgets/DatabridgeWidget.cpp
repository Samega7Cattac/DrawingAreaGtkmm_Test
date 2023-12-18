#include "DatabridgeWidget.hh"

// BOTAN headers
#include <botan/hash.h>
#include <botan/hex.h>

#include <ctime>
#include <sstream>
#include <random>

constexpr const char HASH_ALGORITHM[] = "SHA-256";

DatabridgeWidget::DatabridgeWidget() :
    m_status(WidgetStatus::OUTFOCUS)
{
    // set local key
    // Create hash object of type SHA-256
    std::unique_ptr<Botan::HashFunction> hash(
        Botan::HashFunction::create(HASH_ALGORITHM));
    // create random string to create the hash number
    // string will contain the current time and random number which was seed by
    // it
    std::stringstream sstring;
    sstring << std::time(nullptr) << reinterpret_cast<unsigned long long>(this) << std::rand();
    std::string sha_seed_string = sstring.str();
    std::vector<uint8_t> sha_seed_vector;
    sha_seed_vector.resize(sha_seed_string.size());
    // convert string to uint8_t vector (used by botan)
    std::transform(sha_seed_string.begin(),
                sha_seed_string.end(),
                sha_seed_vector.begin(),
                [](char a) -> uint8_t
                {
                    return static_cast<uint8_t>(a);
                });
    // Generate hash
    hash->update(sha_seed_vector.data(), sha_seed_vector.size());
    m_hash = Botan::hex_encode(hash->final());
}

DatabridgeWidget::~DatabridgeWidget()
{

}

std::string
DatabridgeWidget::GetHash() const
{
    return m_hash;
}

bool
DatabridgeWidget::ContainPoint(const DatabridgePoint& point) const
{
    // Iterate the boundaries vector and check if any contains the points
    return std::any_of(m_boundaries.begin(),
                       m_boundaries.end(),
                       [&point](const Gdk::Rectangle& boundary)
                       {
                            return boundary.contains_point(point.GetX() , point.GetY() );
                       });
}

void
DatabridgeWidget::SetInFocus()
{
    if (GetStatus() == WidgetStatus::OUTFOCUS && GetStatus() == WidgetStatus::INFOCUS)
    {
        SetStatus(WidgetStatus::INFOCUS);
    }
}

void
DatabridgeWidget::SetOutFocus()
{
    if (GetStatus() == WidgetStatus::OUTFOCUS && GetStatus() == WidgetStatus::INFOCUS)
    {
        SetStatus(WidgetStatus::OUTFOCUS);
    }
}

void
DatabridgeWidget::SetStatus(WidgetStatus status)
{
    m_status = status;
}

WidgetStatus
DatabridgeWidget::GetStatus() const
{
    return m_status;
}

const std::vector<Gdk::Rectangle>&
DatabridgeWidget::GetBoundaries() const
{
    return m_boundaries;
}

std::vector<Gdk::Rectangle>&
DatabridgeWidget::GetBoundaries()
{
    return m_boundaries;
}
