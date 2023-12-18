#include "NodeSelector.hh"

// IMAGES headers
#include "images/cloudflare.imgh"
#include "images/dyndns.imgh"
#include "images/ioxhost.imgh"
#include "images/maxcdn.imgh"
#include "images/mybatis.imgh"
#include "images/plesk.imgh"

// GTKMM headers
#include <gtkmm/dragsource.h>
#include <gtkmm/signallistitemfactory.h>
#include <gtkmm/scrolledwindow.h>

NodeSelector::NodeSelector()
{
    LoadPages();   
}

NodeSelector::~NodeSelector()
{
    
}

Glib::RefPtr<NodeSelector::IconModel>
NodeSelector::IconModel::create(const Glib::RefPtr<Gdk::Pixbuf>& image,
                                const std::string& name)
{
    return Glib::make_refptr_for_instance<IconModel>(
                new IconModel(image, name));
}

Glib::RefPtr<Gdk::Pixbuf>
NodeSelector::IconModel::GetImage() const
{
    return m_image;
}

std::string
NodeSelector::IconModel::GetName() const
{
    return m_name;
}

NodeSelector::IconModel::IconModel(const Glib::RefPtr<Gdk::Pixbuf>& image,
                                   const std::string& name) :
    m_image(image),
    m_name(name)
{

}

void
NodeSelector::LoadPages()
{
    Gtk::ScrolledWindow* spi_scrolled = Gtk::make_managed<Gtk::ScrolledWindow>();
    spi_scrolled->set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    unsigned int page_number = append_page(*spi_scrolled, "SPI");

    Gtk::GridView* spi_icons = Gtk::make_managed<Gtk::GridView>();
    spi_scrolled->set_child(*spi_icons);

    Glib::RefPtr<Gtk::DragSource> spi_drag_source = Gtk::DragSource::create();
    spi_drag_source->set_actions(Gdk::DragAction::COPY);
    spi_drag_source->signal_prepare().connect(sigc::bind(sigc::mem_fun(*this, &NodeSelector::PrepareDataToDrag), page_number), false);
    spi_drag_source->signal_end().connect(sigc::bind(sigc::mem_fun(*this, &NodeSelector::OnEndDrag), page_number));
    spi_icons->add_controller(spi_drag_source);

    spi_icons->set_single_click_activate(true);

    Glib::RefPtr<Gio::ListStore<IconModel>> spi_icon_model = Gio::ListStore<IconModel>::create();

    m_selection_model[page_number] = Gtk::SingleSelection::create(spi_icon_model);
    m_selection_model[page_number]->set_autoselect(false);
    spi_icons->set_model(m_selection_model[page_number]);

    Glib::RefPtr<Gtk::SignalListItemFactory> spi_grid_factory = Gtk::SignalListItemFactory::create();
    spi_grid_factory->signal_setup().connect(
        sigc::mem_fun(*this, &NodeSelector::OnImageSetup));
    spi_grid_factory->signal_bind().connect(
        sigc::mem_fun(*this, &NodeSelector::OnImageBind));
    spi_icons->set_factory(spi_grid_factory);

    Glib::RefPtr<Gdk::Pixbuf> cloudflare =
        Gdk::Pixbuf::create_from_data(CLOUDFLARE_PIXEL_DATA,
                                    Gdk::Colorspace::RGB,
                                    true,
                                    8,
                                    CLOUDFLARE_WIDTH,
                                    CLOUDFLARE_HEIGHT,
                                    CLOUDFLARE_ROWSTRIDE);
    spi_icon_model->append(IconModel::create(cloudflare, "cloudflare"));

    Glib::RefPtr<Gdk::Pixbuf> dyndns =
        Gdk::Pixbuf::create_from_data(DYNDNS_PIXEL_DATA,
                                    Gdk::Colorspace::RGB,
                                    true,
                                    8,
                                    DYNDNS_WIDTH,
                                    DYNDNS_HEIGHT,
                                    DYNDNS_ROWSTRIDE);
    spi_icon_model->append(IconModel::create(dyndns, "dyndns"));

    Glib::RefPtr<Gdk::Pixbuf> ioxhost =
        Gdk::Pixbuf::create_from_data(IOXHOST_PIXEL_DATA,
                                    Gdk::Colorspace::RGB,
                                    true,
                                    8,
                                    IOXHOST_WIDTH,
                                    IOXHOST_HEIGHT,
                                    IOXHOST_ROWSTRIDE);
    spi_icon_model->append(IconModel::create(ioxhost, "ioxhost"));

    Glib::RefPtr<Gdk::Pixbuf> maxcdn =
        Gdk::Pixbuf::create_from_data(MAXCDN_PIXEL_DATA,
                                    Gdk::Colorspace::RGB,
                                    true,
                                    8,
                                    MAXCDN_WIDTH,
                                    MAXCDN_HEIGHT,
                                    MAXCDN_ROWSTRIDE);
    spi_icon_model->append(IconModel::create(maxcdn, "maxcdn"));

    Gtk::ScrolledWindow* modbus_scolled = Gtk::make_managed<Gtk::ScrolledWindow>();
    modbus_scolled->set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    page_number = append_page(*modbus_scolled, "Modbus");

    Gtk::GridView* modbus_icons = Gtk::make_managed<Gtk::GridView>();
    modbus_scolled->set_child(*modbus_icons);

    Glib::RefPtr<Gtk::DragSource> modbus_drag_source = Gtk::DragSource::create();
    modbus_drag_source->set_actions(Gdk::DragAction::COPY);
    modbus_drag_source->signal_prepare().connect(sigc::bind(sigc::mem_fun(*this, &NodeSelector::PrepareDataToDrag), page_number), false);
    modbus_drag_source->signal_end().connect(sigc::bind(sigc::mem_fun(*this, &NodeSelector::OnEndDrag), page_number));
    modbus_icons->add_controller(modbus_drag_source);

    modbus_icons->set_single_click_activate(true);

    Glib::RefPtr<Gio::ListStore<IconModel>> modbus_icon_model = Gio::ListStore<IconModel>::create();

    m_selection_model[page_number] = Gtk::SingleSelection::create(modbus_icon_model);
    m_selection_model[page_number]->set_autoselect(false);
    modbus_icons->set_model(m_selection_model[page_number]);

    Glib::RefPtr<Gtk::SignalListItemFactory> modbus_grid_factory = Gtk::SignalListItemFactory::create();
    modbus_grid_factory->signal_setup().connect(
        sigc::mem_fun(*this, &NodeSelector::OnImageSetup));
    modbus_grid_factory->signal_bind().connect(
        sigc::mem_fun(*this, &NodeSelector::OnImageBind));
    modbus_icons->set_factory(modbus_grid_factory);

    Glib::RefPtr<Gdk::Pixbuf> mybatis =
        Gdk::Pixbuf::create_from_data(MYBATIS_PIXEL_DATA,
                                    Gdk::Colorspace::RGB,
                                    true,
                                    8,
                                    MYBATIS_WIDTH,
                                    MYBATIS_HEIGHT,
                                    MYBATIS_ROWSTRIDE);
    modbus_icon_model->append(IconModel::create(mybatis, "mybatis"));

    Glib::RefPtr<Gdk::Pixbuf> plesk =
        Gdk::Pixbuf::create_from_data(PLESK_PIXEL_DATA,
                                    Gdk::Colorspace::RGB,
                                    true,
                                    8,
                                    PLESK_WIDTH,
                                    PLESK_HEIGHT,
                                    PLESK_ROWSTRIDE);
    modbus_icon_model->append(IconModel::create(plesk, "plesk"));
}

void
NodeSelector::OnImageSetup(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
    Gtk::Picture* image = Gtk::make_managed<Gtk::Picture>();
    image->set_can_shrink(false);
    image->set_expand(false);
    // image->set_valign(Gtk::Align::FILL);
    // image->set_halign(Gtk::Align::FILL);
    list_item->set_child(*image);
}

void
NodeSelector::OnImageBind(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
    Glib::RefPtr<IconModel> col = std::dynamic_pointer_cast<IconModel>(list_item->get_item());
    if (col == nullptr)
    {
        return;
    }
    Gtk::Picture* picture = dynamic_cast<Gtk::Picture*>(list_item->get_child());
    if (picture == nullptr)
    {
        return;
    }
    picture->set_pixbuf(col->GetImage());
}

Glib::RefPtr<Gdk::ContentProvider>
NodeSelector::PrepareDataToDrag(double, double, unsigned int page_number)
{
    Glib::RefPtr<Glib::ObjectBase> obj = m_selection_model[page_number]->get_selected_item();
    
    if (obj == nullptr) 
    {
        Glib::Value<Glib::ustring> ustring_value;
        ustring_value.init(ustring_value.value_type());
        ustring_value.set("");
        return Gdk::ContentProvider::create(ustring_value);
    }

    
    Glib::RefPtr<IconModel> col = std::dynamic_pointer_cast<IconModel>(obj);

    Glib::Value<Glib::ustring> ustring_value;
    ustring_value.init(ustring_value.value_type());
    ustring_value.set(col->GetName());
    return Gdk::ContentProvider::create(ustring_value);
}

void
NodeSelector::OnEndDrag(Gdk::EventSequence*, unsigned int page_number)
{
    // TODO: The point of this is to deselect when the drag is finished but it's still selected
    m_selection_model[page_number]->unselect_all();
}
