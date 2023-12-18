// PROJECT headers
#include "MainWindow.hh"

// GTKMM headers
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/textview.h>
#include <gtkmm/eventcontrollerkey.h>

#include <gdk/gdkkeysyms.h>

constexpr const char WINDOW_TITLE_NAME[] = "Drawing Area Test";

constexpr const int WINDOW_WIDTH = 800;

constexpr const int WINDOW_HEIGHT = 600;

MainWindow::MainWindow ()
{
    // Set base default window properties and add icon
    set_title(WINDOW_TITLE_NAME);
    set_default_size(WINDOW_WIDTH, WINDOW_HEIGHT);
    set_resizable();

    // Create Main box that will contain both the menu and drawing area
    Gtk::Box* main_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    set_child(*main_box);
    
    m_node_selector.set_vexpand(false);
    main_box->append(m_node_selector);
    
    // Create DrawArea
    m_draw_area.set_expand(true);
    m_draw_area.set_valign(Gtk::Align::FILL);

    Glib::RefPtr<Gtk::EventControllerKey> key_event_controller = Gtk::EventControllerKey::create();
    key_event_controller->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    key_event_controller->signal_key_pressed().connect(
        sigc::mem_fun(m_draw_area, &BlockArea::on_key_pressed), false
    );

    main_box->append(m_draw_area);

    // Create Box with sorting information
    Gtk::Box* sort_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);

    Gtk::Button* sort_button = Gtk::make_managed<Gtk::Button>("Sort");
    sort_button->signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::OnSortButtonPressed)
    );

    m_sort_result_text_buffer = Gtk::TextBuffer::create();

    Gtk::TextView* sort_result = Gtk::make_managed<Gtk::TextView>();
    sort_result->set_buffer(m_sort_result_text_buffer);
    sort_result->set_hexpand();

    sort_box->append(*sort_button);
    sort_box->append(*sort_result);

    main_box->append(*sort_box);


    add_controller(key_event_controller);

    
}


MainWindow::~MainWindow()
{
}


void
MainWindow::OnSortButtonPressed()
{
    std::string result = m_topology_sort.ExecuteSorting(m_draw_area.GetNodes(),
                                                        m_draw_area.GetLinks());

    m_sort_result_text_buffer->set_text(result);

    m_draw_area.queue_draw();
}