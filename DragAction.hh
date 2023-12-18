#ifndef DRAGACTION_HH
#define DRAGAction_HH

#include <vector>
#include <string>

#include "Widgets/DatabridgePoint.hh"

enum DragType {
    NO_DRAG,
    DRAG_NODE,
    DRAG_LINK
};

class DragAction
{

public:
    DragAction(DragType drag_type, const DatabridgePoint& start_position);
    virtual ~DragAction();

    std::string GetHash(unsigned int hash_id) const;

    DatabridgePoint GetStartPosition() const;
    void SetLastOffset(const DatabridgePoint& last_offset);
    DatabridgePoint GetLastOffset() const;
    DragType GetDragType() const;

protected:
    std::vector<std::string> m_hashes;

private:
    DatabridgePoint m_start_position;
    DatabridgePoint m_last_offset;
    DragType m_type;

};

class NoDragAction : public DragAction
{
public:
    NoDragAction(const DatabridgePoint& start_position);
    ~NoDragAction();
};


class DragNodeAction : public DragAction
{

public:
    enum HASH
    {
        NODE_HASH = 0
    };

    DragNodeAction(const DatabridgePoint& start_position,
                   std::string node_hash);
    ~DragNodeAction();

};


class DragLinkAction : public DragAction
{
public:
    enum HASH
    {
        LINK_HASH = 0,
        LINE_HASH = 1
    };

    DragLinkAction(const DatabridgePoint& start_position,
                   std::string link_hash,
                   std::string line_hash);
    ~DragLinkAction();
};


#endif // DRAGACTION_HH