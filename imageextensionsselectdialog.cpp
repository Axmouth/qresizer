#include "imageextensionsselectdialog.h"
#include "ui_imageextensionsselectdialog.h"
#include <constants.h>

ImageExtensionsSelectDialog::ImageExtensionsSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageExtensionsSelectDialog)
{
    ui->setupUi(this);

    qRegisterMetaType<QList<QString>>("QList<QString>");

#if QT_VERSION < 0x060000
    qRegisterMetaTypeStreamOperators<QList<QString>>("QList<QString>");
#endif

    QSettings MySettings;

    ImageExtensionsListModel = new QStringListModel(this);
    ui->listView->setModel(ImageExtensionsListModel);
    ImageExtensionsListModel->setStringList(MySettings.value(IMAGE_EXTENSIONS_SETTINGS_KEY, QVariant::fromValue(DEFAULT_IMAGE_EXTENSIONS)).value<QList<QString>>());

    connect(ui->pushButton_add, &QPushButton::clicked, this, &ImageExtensionsSelectDialog::addExtension);
    connect(ui->pushButton_removeSelected, &QPushButton::clicked, this, &ImageExtensionsSelectDialog::removeExtension);
}

ImageExtensionsSelectDialog::~ImageExtensionsSelectDialog()
{
    delete ui;
}

void ImageExtensionsSelectDialog::accept()
{
    QSettings MySettings;
    MySettings.setValue(IMAGE_EXTENSIONS_SETTINGS_KEY, QVariant::fromValue<QList<QString>>(ImageExtensionsListModel->stringList()));

    QDialog::accept();
}

void ImageExtensionsSelectDialog::removeExtension()
{
    QList<QModelIndex> indexes = ui->listView->selectionModel()->selectedIndexes();
    if (!indexes.isEmpty())
    {
        QStringList strList = ImageExtensionsListModel->stringList();
        strList.removeAt(indexes.first().row());
        ImageExtensionsListModel->setStringList(strList);
    }
}

void ImageExtensionsSelectDialog::addExtension()
{
    ImageExtensionsListModel->insertRow(ImageExtensionsListModel->rowCount());
    QModelIndex index = ImageExtensionsListModel->index(ImageExtensionsListModel->rowCount()-1);
    ImageExtensionsListModel->setData(index, ui->lineEdit_extension->text());
}
