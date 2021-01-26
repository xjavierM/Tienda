#include "principal.h"
#include "ui_principal.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    m_subtotal=0;
    cargarDatos();
    inicializarWidgets();
}
Principal::~Principal()
{
    delete ui;
}
void Principal::mostrarPrecio(int indice)
{
    float precioProducto = m_productos.at(indice)->precio();
    ui->outPrecio->setText("$ " + QString::number(precioProducto));
}
void Principal::on_pushButton_2_clicked()
{
}
void Principal::cargarDatos()
{
    QString pathActual = QDir::currentPath();
    QFile archivo(pathActual + "/productos.csv");
    bool primeraLinea = true;
    if(archivo.open(QFile::ReadOnly)){
        QTextStream in(&archivo);
        while (!in.atEnd()) {
            QString linea = in.readLine();
            if(primeraLinea){
                primeraLinea = false;
                continue;
            }
            QStringList datos = linea.split(";");
            int codigo = datos[0].toInt();
            float precio = datos[2].toFloat();
            m_productos.append(new Producto(codigo,datos[1],precio));
        }
    }else{
        QMessageBox msgBox;
        msgBox.setText("El archivo de productos no se puede abrir");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}
void Principal::inicializarWidgets()
{
    for (int i=0;i<m_productos.length(); i++) {
        ui->inProducto->addItem(m_productos.at(i)->nombre());
    }
    QStringList cabecera = {"Cantidad", "Nombre", "Subtotal"};
    ui->outDetalle->setColumnCount(3);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);
    connect(ui->inProducto, SIGNAL(currentIndexChanged(int)),
            this, SLOT(mostrarPrecio(int)));
    mostrarPrecio(0);

}
void Principal::calcular(float subtotal)
{
    m_subtotal += subtotal;
    float iva = m_subtotal * IVA;
    float total = m_subtotal + iva;
    int ent = int(total * 1000);
    int num = int(total * 100);
    int ult_c = ent % 10;
    if(ult_c >=5 ){
        num = num + 1;
    }else{
        num = num + 0;
    }
    int entero = int(iva * 1000);
    int numero = int(iva * 100);

    int ult_cidra = entero % 10;
    if(ult_cidra >= 5){
        numero = numero+1;
    }else{
        numero= numero+0;
    }
    ui->outSubtotal->setText(QString::number(m_subtotal));
    ui->outIva->setText(QString::number(numero/100.00));
    ui->outTotal->setText(QString::number(num/100.00));
}
void Principal::on_cmdAgregar_clicked()
{
    int indice = ui->inProducto->currentIndex();
    Producto *p = m_productos.at(indice);
    //Obtener la cantidad
    int cantidad = ui->inCantidad->value();
    if (cantidad == 0){
        return;
    }
    float subtotal = cantidad * p->precio();
    if(!buscar(p, cantidad)){
        int posicion = ui->outDetalle->rowCount();
        ui->outDetalle->insertRow(posicion);
        ui->outDetalle->setItem(posicion,0,new QTableWidgetItem(QString::number(cantidad)));
        ui->outDetalle->setItem(posicion,1,new QTableWidgetItem(p->nombre()));
        ui->outDetalle->setItem(posicion,2,new QTableWidgetItem(QString::number(subtotal)));
    }
    ui->inCantidad->setValue(0);
    ui->inProducto->setFocus();
    calcular(subtotal);
}
void Principal::validarCedula()
{
    int   pares,impares,total,dec=0;
    int a,b,c,d,e,f,g,h,i,j;
    long cedula;
    cedula=ui->inCedula->text().toLong();
    a=cedula/1000000000;
    cedula=cedula-(a*1000000000);
    b=cedula/100000000;
    cedula=cedula-(b*100000000);
    c=cedula/10000000;
    cedula=cedula-(c*10000000);
    d=cedula/1000000;
    cedula=cedula-(d*1000000);
    e=cedula/100000;
    cedula=cedula-(e*100000);
    f=cedula/10000;
    cedula=cedula-(f*10000);
    g=cedula/1000;
    cedula=cedula-(g*1000);
    h=cedula/100;
    cedula=cedula-(h*100);
    i=cedula/10;
    cedula=cedula-(i*10);
    j=cedula/1;
    cedula=cedula-(j*1);
    if (cedula>9000000000){
        ui->inCedula->setStyleSheet("color : red");
    }else{
        pares=b+d+f+h;

        a=a*2;
        if (a>9){
            a=a%10+a/10;
        }

        c=c*2;
        if (c>9){
            c=c%10+c/10;
        }

        e=e*2;
        if (e>9){
            e=e%10+e/10;
        }

        g=g*2;
        if (g>9){
            g=g%10+g/10;
        }
        i=i*2;
        if (i>9){
            i=i%10+i/10;
        }
        impares=a+c+e+g+i;
        total=pares+impares;
        while (dec-total!=j && dec<total+10){
            dec=dec+10;
        }
        if (dec-total==j){
            ui->inCedula->setStyleSheet("color: green");
        }else {
            ui->inCedula->setStyleSheet("color: red");
            ui->statusbar->showMessage("Numero de cedula invalido",5000);
        }
    }
}
void Principal::validarNombre()
{
    QString nombre = ui->inNombre->text();
    if(nombre == "" ){
        ui->inNombre->setStyleSheet("background-color: rgb(239, 41, 41);");
    }else{
        ui->inNombre->setStyleSheet("background-color: rgb(216, 245, 162);");
    }
}
bool Principal::validarCampoCedula()
{
    QString cedula = ui->inCedula->text();
    if(cedula == ""){
        ui->inCedula->setStyleSheet("background-color: rgb(239, 41, 41);");
    }else{
        ui->inCedula->setStyleSheet("background-color: rgb(216, 245, 162);");
    }
}
bool Principal::buscar(Producto *producto, int cantidad)
{
    int numFilas= ui->outDetalle->rowCount();
    for (int i=0;i<numFilas;++i) {
        QTableWidgetItem *item=ui->outDetalle->item(i,1);
        QString dato= item->data(Qt::DisplayRole).toString();
        if(dato == producto->nombre()){
            QTableWidgetItem *item=ui->outDetalle->item(i,0);
            int cantidadActual = item->data(Qt::DisplayRole).toInt();
            int cantidadNueva= cantidadActual + cantidad;
            float subtotal = cantidadNueva *producto->precio();
            qDebug() << cantidadNueva<<subtotal;
            ui->outDetalle->setItem(i,0, new QTableWidgetItem(QString::number(cantidadNueva)));
            ui->outDetalle->setItem(i,2, new QTableWidgetItem(QString::number(subtotal)));
            return true;
        }
    }
    return false;
}
void Principal::on_inCedula_editingFinished()
{
    validarCedula();
    validarCampoCedula();
}
void Principal::on_cmdFinalizar_clicked()
{
    QString cedula = ui->inCedula->text();
    QString nombre = ui->inNombre->text();
    QString telefono = ui->inTelefono->text();
    QString email = ui->inEmail->text();
    QString direccion = ui->inDireccion->toPlainText();
    QString subTotal=ui->outSubtotal->text();
    QString iva = ui->outIva->text();
    QString total= ui->outTotal->text();
    if(nombre=="" || cedula=="" ){
        return;
    }else {
        Receipt *recibo = new Receipt;
        QString resultado= "Cliente: " + nombre + "\n";
        resultado += "-------------------------------";
        resultado += "\nCedula: " + cedula + "\n";
        resultado += "-------------------------------";
        resultado += "\nTelefono: " + telefono + "\n";
        resultado += "\nCorreo: " + email + "\n";
        resultado += "\nDireccion: "+ direccion +"\n";
        resultado += "-------------------------------";
        resultado += "\nSubtotal: $"+ subTotal +"\n";
        resultado += "\nIva: "+ iva +"\n";
        resultado += "\nTotal a pagar: $"+ total +"\n";
        resultado += "-------------------------------";
        recibo->setRecibo(resultado);
        recibo->show();
        ui->inNombre->setText("");
        ui->inCedula->setText("");
        ui->inTelefono->setText("");
        ui->inEmail->setText("");
        ui->inDireccion->setText("");
        ui->outDetalle->deleteLater();
    }
}
void Principal::on_inNombre_editingFinished()
{
    validarNombre();
}
void Principal::on_actionAcerca_de_Tienda_triggered()
{
    About *venta1 = new About;
    venta1->show();
}
