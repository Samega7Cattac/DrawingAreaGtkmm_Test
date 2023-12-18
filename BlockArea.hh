#ifndef BLOCKAREA_HH
#define BLOCKAREA_HH

#include <vector>

// Project headers
#include "Widgets/DatabridgeWidget.hh"
#include "Widgets/DatabridgePoint.hh"
#include "Widgets/DatabridgeOrthogonalLink.hh"
#include "LinkBuilder.hh"
#include "DragAction.hh"
#include "Camera.hh"

// GTKMM headers
#include <gtkmm/drawingarea.h>
#include <gtkmm/eventcontroller.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/gesturedrag.h>
#include <cairomm/cairomm.h>


class BlockArea : public Gtk::DrawingArea
{
public:
    BlockArea();

    virtual ~BlockArea();

    bool on_key_pressed(guint keyval, guint, Gdk::ModifierType);

    std::unordered_map<std::string, std::shared_ptr<DatabridgeNode>>& GetNodes();
    std::unordered_map<std::string, std::shared_ptr<DatabridgeOrthogonalLink>>& GetLinks();


private:
    DatabridgePoint m_lastPanOffset;

    Camera m_camera;

    std::unique_ptr<DragAction> m_drag_action;

    std::unordered_map<std::string, std::shared_ptr<DatabridgeNode>> m_nodes;
    std::unordered_map<std::string, std::shared_ptr<DatabridgeOrthogonalLink>> m_links;

    std::unique_ptr<LinkBuilder> m_link_builder;

    std::shared_ptr<TopologyRuleSet> m_rule_set;

    void RemoveNode(const std::string& hash);

    void RemoveLink(const std::string& hash);

    void RemoveNode(std::shared_ptr<DatabridgeNode> node);

    void RemoveLink(std::shared_ptr<DatabridgeLink> link);

    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr,
                 int width,
                 int height);


    void on_button_left_press(int n_press, double pointer_x, double pointer_y);

    void on_button_left_released(int n_press, double pointer_x, double pointer_y);
    

    bool on_scroll(double d_scroll_x, double d_scroll_y);


    void on_middle_drag_start(double pointer_x, double pointer_y);

    void on_middle_drag_update(double pointer_x,
                               double pointer_y,
                               Glib::RefPtr<Gtk::GestureDrag> gesture);

    void on_middle_drag_end(double pointer_x, double pointer_y);



    void on_left_drag_start(double pointer_x, double pointer_y);

    void on_left_drag_end(double pointer_x, double pointer_y);

    void on_left_drag_update(double pointer_x, double pointer_y);


    void on_motion(double pointer_x, double pointer_y);

    bool on_drop_data(const Glib::ValueBase& value,
                      double pointer_x,
                      double pointer_y);


};

#endif // BLOCKAREA_HH