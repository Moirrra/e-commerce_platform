#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "sqlhelp.h"

//设置只读代理
class ReadOnlyDelegate: public QItemDelegate
{
public:
    ReadOnlyDelegate(QWidget *parent = NULL):QItemDelegate(parent){}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override
    {
        Q_UNUSED(parent)
        Q_UNUSED(option)
        Q_UNUSED(index)
        return NULL;
    }
};

class UserIDDelegate: public QItemDelegate
{
    Q_OBJECT
public :
    UserIDDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        QLineEdit *editor =  new  QLineEdit(parent);
        QRegExp regExp( "[0-9]{0,10}" );
        editor->setValidator( new  QRegExpValidator(regExp, parent));
        return  editor;
    }
    void  setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *lineEdit = static_cast <QLineEdit*>(editor);
        lineEdit->setText(text);
    }
    void  setModelData(QWidget *editor, QAbstractItemModel *model,
                       const  QModelIndex &index) const
    {
        QLineEdit *lineEdit =  static_cast <QLineEdit*>(editor);
        QString text = lineEdit->text();
        model->setData(index, text, Qt::EditRole);
    }
    void  updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }
};

//数量列，利用QSpinBox委托进行输入限制，只能输入1－99999之间的数字
class  CountDelegate:  public  QItemDelegate
{
    Q_OBJECT
public :
    CountDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const  QModelIndex &index) const
    {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setMinimum(1);
        editor->setMaximum(99999);
        return editor;
    }
    void  setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast <QSpinBox*>(editor);
        spinBox->setValue(value);
    }
    void  setModelData(QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const
    {
        QSpinBox *spinBox = static_cast <QSpinBox*>(editor);
        spinBox->interpretText();
        int  value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
    void  updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                               const  QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }
};

//价格列，利用QSpinBox委托进行输入限制，只能输入0.01－9999.99之间的数字
class  PriceDelegate: public QItemDelegate
{
    Q_OBJECT
public :
    PriceDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const  QModelIndex &index) const
    {
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setDecimals(2);
        editor->setMinimum(0.01);
        editor->setMaximum(9999.99);

        return editor;
    }
    void  setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        double value = index.model()->data(index, Qt::EditRole).toDouble();
        QDoubleSpinBox *doublespinBox = static_cast <QDoubleSpinBox*>(editor);
        doublespinBox->setValue(value);
    }
    void  setModelData(QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const
    {
        QDoubleSpinBox *doublespinBox = static_cast <QDoubleSpinBox*>(editor);
        doublespinBox->interpretText();
        double value = doublespinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
    void  updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }
};

//商品种类列，利用QComboBox委托对输入进行限制
class  TypeDelegate:  public  QItemDelegate
{
    Q_OBJECT
public :
    TypeDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        QComboBox *editor = new QComboBox(parent);
        SqlHelp* sqlhelp = new SqlHelp();
        sqlhelp->BindDataToCombox("GoodsType","TypeName",editor);
        return  editor;
    }
    void  setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast <QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
    void  setModelData(QWidget *editor, QAbstractItemModel *model,
                       const  QModelIndex &index)  const
    {
        QComboBox *comboBox = static_cast <QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
    void  updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }
};

//商品规格列，利用QComboBox委托对输入进行限制
class  SizeDelegate:  public  QItemDelegate
{
    Q_OBJECT
public :
    SizeDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        QComboBox *editor = new QComboBox(parent);
        SqlHelp* sqlhelp = new SqlHelp();
        sqlhelp->BindDataToCombox("GoodsSize","Size",editor);
        return  editor;
    }
    void  setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast <QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
    void  setModelData(QWidget *editor, QAbstractItemModel *model,
                       const  QModelIndex &index)  const
    {
        QComboBox *comboBox = static_cast <QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
    void  updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }
};

//商品规格列，利用QComboBox委托对输入进行限制
class  UnitDelegate:  public  QItemDelegate
{
    Q_OBJECT
public :
    UnitDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        QComboBox *editor = new QComboBox(parent);
        SqlHelp* sqlhelp = new SqlHelp();
        sqlhelp->BindDataToCombox("GoodsUnit","UnitName",editor);
        return  editor;
    }
    void  setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast <QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
    void  setModelData(QWidget *editor, QAbstractItemModel *model,
                       const  QModelIndex &index)  const
    {
        QComboBox *comboBox = static_cast <QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
    void  updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }
};

#endif // TABLEDELEGATE_H
