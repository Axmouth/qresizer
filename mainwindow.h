#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectImages();
    void selectOutputFolder();
    void resizeImage();
    void toggleKeepAspectRatio(bool checked);
    void toggleConvertToJPEG(bool checked);
    void toggleUseInputFolder(bool checked);
    void onHeightChange(int height);
    void onWidthChange(int width);
    void onQualityChange(int quality);
    void optionsImageExtensionsSelect();

private:
    Ui::MainWindow *ui;
    QString outputFolder;
    QList<QString> inputImages;
};
#endif // MAINWINDOW_H
