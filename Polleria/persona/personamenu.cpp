#include "personamenu.h"
#include "ui_personamenu.h"

PersonaMenu::PersonaMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonaMenu)
{
    ui->setupUi(this);

    QTreeWidgetItem* colaboradores = new QTreeWidgetItem(ui->treeWidget_menu);
    ui->treeWidget_menu->setItemExpanded(colaboradores, false);
}

PersonaMenu::~PersonaMenu()
{
    delete ui;
}
void PersonaMenu::on_lw_tipo_itemClicked(QListWidgetItem *item)
{
    if(SYSTEM->tipo_persona.compare(Persona::administrador) == 0){
        if(item->text().compare(Persona::administrador) == 0){
            QMessageBox::warning(this, "Advertencia", "No es posible esta acción.", "Aceptar");
            return;
        }
    }
    if(SYSTEM->tipo_persona.compare(Persona::cajero) == 0){
        if(item->text().compare(Persona::administrador) == 0 || item->text().compare(Persona::cajero) == 0){
            QMessageBox::warning(this, "Advertencia", "No es posible esta acción.", "Aceptar");
            return;
        }
    }
    Persona* w = new Persona(this);
    w->setWindowFlag(Qt::Dialog);
    w->setWindowModality(Qt::WindowModal);

    w->setOperacion(INGRESAR);
    w->setTipoPersona(item->text());
    w->show();
}
void PersonaMenu::on_treeWidget_menu_itemClicked(QTreeWidgetItem *item, int column)
{
    //qDebug()<<item->text(0)<<endl;
    if(item->text(0).compare(Persona::opc_clientes) == 0){
        if(column == 1){
            QListWidget* lw_tipo = new QListWidget(this);
            ((QWidget*)lw_tipo)->setWindowFlag(Qt::Dialog);
            ((QWidget*)lw_tipo)->setWindowModality(Qt::WindowModal);

            ((QAbstractItemView*)lw_tipo)->setIconSize(QSize(48, 48));
            lw_tipo->addItem(Persona::cliente_ruc);
            lw_tipo->item(0)->setIcon(QIcon(":/new/Iconos/sunat.png"));
            lw_tipo->addItem(Persona::cliente_dni);
            lw_tipo->item(1)->setIcon(QIcon(":/new/Iconos/dni.png"));
            lw_tipo->addItem(Persona::cliente_eventual);
            lw_tipo->item(2)->setIcon(QIcon(":/new/Iconos/eventual.png"));
            lw_tipo->addItem(Persona::cliente_delivery);
            lw_tipo->item(3)->setIcon(QIcon(":/new/Iconos/eventual.png"));
            QFont font;
            font.setPointSize(10);
            ((QWidget*)lw_tipo)->setFont(font);

            connect(lw_tipo, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_lw_tipo_itemClicked(QListWidgetItem*)));
            lw_tipo->show();
        }
        if(column == 2){
            PersonaBuscar* w = new PersonaBuscar(this);
            w->setAttribute(Qt::WA_DeleteOnClose);
            w->setWindowFlag(Qt::Dialog);
            w->setWindowModality(Qt::WindowModal);

            w->setOpcPersona(Persona::opc_clientes);
            w->show();
        }
    }
    if(item->text(0).compare(Persona::opc_proveedores) == 0){
        if(column == 1){
            QListWidget* lw_tipo = new QListWidget(this);
            ((QWidget*)lw_tipo)->setWindowFlag(Qt::Dialog);
            ((QWidget*)lw_tipo)->setWindowModality(Qt::WindowModal);

            ((QAbstractItemView*)lw_tipo)->setIconSize(QSize(48, 48));
            lw_tipo->addItem(Persona::proveedor);
            lw_tipo->item(0)->setIcon(QIcon(":/new/Iconos/proveedor_gorro.jpg"));
            lw_tipo->addItem(Persona::transportista);
            lw_tipo->item(1)->setIcon(QIcon(":/new/Iconos/transportista.png"));
            QFont font;
            font.setPointSize(10);
            ((QWidget*)lw_tipo)->setFont(font);

            connect(lw_tipo, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_lw_tipo_itemClicked(QListWidgetItem*)));
            lw_tipo->show();
        }
        if(column == 2){
            PersonaBuscar* w = new PersonaBuscar(this);
            w->setAttribute(Qt::WA_DeleteOnClose);
            w->setWindowFlag(Qt::Dialog);
            w->setWindowModality(Qt::WindowModal);

            w->setOpcPersona(Persona::opc_proveedores);
            w->show();
        }
    }
    if(item->text(0).compare(Persona::opc_usuarios) == 0){
        if(column == 1){
            QListWidget* lw_tipo = new QListWidget(this);
            ((QWidget*)lw_tipo)->setWindowFlag(Qt::Dialog);
            ((QWidget*)lw_tipo)->setWindowModality(Qt::WindowModal);

            ((QAbstractItemView*)lw_tipo)->setIconSize(QSize(48, 48));
            lw_tipo->addItem(Persona::administrador);
            lw_tipo->item(0)->setIcon(QIcon(":/new/Iconos/admin.png"));
            lw_tipo->addItem(Persona::cajero);
            lw_tipo->item(1)->setIcon(QIcon(":/new/Iconos/cajero.png"));
            QFont font;
            font.setPointSize(10);
            ((QWidget*)lw_tipo)->setFont(font);

            connect(lw_tipo, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_lw_tipo_itemClicked(QListWidgetItem*)));
            lw_tipo->show();
        }
        if(column == 2){
            PersonaBuscar* w = new PersonaBuscar(this);
            w->setAttribute(Qt::WA_DeleteOnClose);
            w->setWindowFlag(Qt::Dialog);
            w->setWindowModality(Qt::WindowModal);

            w->setOpcPersona(Persona::opc_usuarios);
            w->show();
        }
    }
}
