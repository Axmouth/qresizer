#ifndef IMAGEEXTENSIONSSELECTDIALOG_H
#define IMAGEEXTENSIONSSELECTDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QSettings>

namespace Ui {
class ImageExtensionsSelectDialog;
}

class ImageExtensionsSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageExtensionsSelectDialog(QWidget *parent = nullptr);
    ~ImageExtensionsSelectDialog();

public slots:
    void accept();

private:
    Ui::ImageExtensionsSelectDialog *ui;

    QStringListModel *ImageExtensionsListModel;
private slots:
    void removeExtension();
    void addExtension();
};

#endif // IMAGEEXTENSIONSSELECTDIALOG_H
