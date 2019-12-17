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
    void resizeImages();
    void toggleKeepAspectRatio(bool checked);
    void toggleConvertToJPEG(bool checked);
    void toggleUseInputFolder(bool checked);
    void onHeightChange(int height);
    void onWidthChange(int width);
    void onQualityChange(int quality);
    void optionsImageExtensionsSelect();
    void endResizeJob();
    void resizeJobProgress(int totalTargets);

private:
    Ui::MainWindow *ui;
    QString outputFolder;
    QList<QString> inputImages;
    int targetsDone = 0;
signals:
    void targetResized(int totalTargets);
    void resizeJobDone();
};
#endif // MAINWINDOW_H
