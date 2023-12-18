// gtkmm HEADERS
#include <glibmm/init.h>

// project HEADERS
#include "MainWindow.hh"

int
main(int argc, char* argv[])
{
    // Creates a new application instance
    // Takes argc and argv as arguments for gtk standard command options.
    Glib::RefPtr<Gtk::Application> app =
        Gtk::Application::create("com.oncontrol.drawingarea");

    return app->make_window_and_run<MainWindow>(argc, argv);
}
