#ifndef NODE_SELECTOR_HH
#define NODE_SELECTOR_HH

// GTKMM headers
#include <gtkmm/notebook.h>
#include <gtkmm/gridview.h>
#include <gtkmm/singleselection.h>
#include <giomm/liststore.h>
#include <gtkmm/picture.h>
#include <gtkmm/listitem.h>

// STD headers
#include <map>

class NodeSelector : public Gtk::Notebook
{
public:
    NodeSelector();
    ~NodeSelector();

private:
    class IconModel : public Glib::Object
    {
    public:
        // TODO: Removing this triggers a compile error for no reason
        IconModel() = default;

        static Glib::RefPtr<IconModel> create(const Glib::RefPtr<Gdk::Pixbuf>& image,
                                              const std::string& name);

        Glib::RefPtr<Gdk::Pixbuf> GetImage() const;

        std::string GetName() const;

    protected:
        IconModel(const Glib::RefPtr<Gdk::Pixbuf>& image,
                  const std::string& name);

    private:
        Glib::RefPtr<Gdk::Pixbuf> m_image;

        std::string m_name;
    };

    std::map<unsigned int, Glib::RefPtr<Gtk::SingleSelection>> m_selection_model;

    void LoadPages();

    void OnImageSetup(const Glib::RefPtr<Gtk::ListItem>& list_item);

    void OnImageBind(const Glib::RefPtr<Gtk::ListItem>& list_item);

    Glib::RefPtr<Gdk::ContentProvider> PrepareDataToDrag(double, double, unsigned int page_number);

    void OnEndDrag(Gdk::EventSequence*, unsigned int page_number);
};

#endif // NODE_SELECTOR_HH