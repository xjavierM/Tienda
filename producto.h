#ifndef PRODUCTO_H
#define PRODUCTO_H
#include <QObject>
class Producto : public QObject
{
    Q_OBJECT
    //Establecer propiedades
    Q_PROPERTY(int codigo READ codigo)
    Q_PROPERTY(float precio READ precio)
    Q_PROPERTY(QString nombre READ nombre)
public:
    explicit Producto(QObject *parent = nullptr);
    Producto(int codigo, QString nombre, float precio);
    int codigo() const;
    float precio() const;
    QString nombre() const;
signals:
private:
    int m_codigo;
    float m_precio;
    QString m_nombre;
};
#endif // PRODUCTO_H
