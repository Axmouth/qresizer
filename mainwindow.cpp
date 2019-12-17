#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QDirIterator>
#include <QStack>
#include <imageextensionsselectdialog.h>
#include <constants.h>

QList<QString> findFilesRecursively(QDir rootDir) {
    QStack<QString> filenames;
    QDirIterator it(rootDir, QDirIterator::Subdirectories);
    while(it.hasNext()) {
        QString filename = it.next();
        qDebug() << filename;
        filenames.push(filename);
    }

    return filenames.toList();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<QList<QString>>("QList<QString>");
    qRegisterMetaTypeStreamOperators<QList<QString>>("QList<QString>");

    QSettings MySettings;
    ui->lineEdit_selectOutputFolder->setText(MySettings.value(DEFAULT_OUTPUT_DIR_SETTINGS_KEY, DEFAULT_OUTPUT_DIR_SETTINGS_KEY).toString());
    outputFolder = MySettings.value(DEFAULT_OUTPUT_DIR_SETTINGS_KEY, DEFAULT_OUTPUT_DIR_SETTINGS_KEY).toString();

    ui->spinBox_targetWidth->setValue(MySettings.value(TARGET_HEIGHT_SETTINGS_KEY, DEFAULT_TARGET_HEIGHT).toInt());
    ui->spinBox_targetHeight->setValue(MySettings.value(TARGET_WIDTH_SETTINGS_KEY, DEFAULT_TARGET_WIDTH).toInt());
    ui->spinBox_qualitySetting->setValue(MySettings.value(IMAGE_QUALITY_SETTINGS_KEY, DEFAULT_IMAGE_QUALITY).toInt());
    ui->checkBox_keepAspectRatio->setChecked(MySettings.value(KEEP_ASPECT_RATIO_SETTINGS_KEY, DEFAULT_KEEP_ASPECT_RATIO).toBool());
    ui->checkBox_convertToJPEG->setChecked(MySettings.value(CONVERT_TO_JPEG_SETTINGS_KEY, DEFAULT_CONVERT_TO_JPEG).toBool());
    ui->checkBox_useInputFolder->setChecked(MySettings.value(USE_INPUT_FOLDER_SETTINGS_KEY, DEFAULT_USE_INPUT_FOLDER).toBool());

    connect(ui->pushButton_selectImages, &QPushButton::clicked, this, &MainWindow::selectImages);
    connect(ui->pushButton_selectOutputFolder, &QPushButton::clicked, this, &MainWindow::selectOutputFolder);
    connect(ui->pushButton_Resize, &QPushButton::clicked, this, &MainWindow::resizeImage);
    connect(ui->checkBox_keepAspectRatio, &QCheckBox::toggled, this, &MainWindow::toggleKeepAspectRatio);
    connect(ui->checkBox_convertToJPEG, &QCheckBox::toggled, this, &MainWindow::toggleConvertToJPEG);
    connect(ui->checkBox_useInputFolder, &QCheckBox::toggled, this, &MainWindow::toggleUseInputFolder);
    connect(ui->spinBox_targetHeight, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onHeightChange);
    connect(ui->spinBox_targetWidth, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onWidthChange);
    connect(ui->actionImage_Extensions, &QAction::triggered, this, &MainWindow::optionsImageExtensionsSelect);

    ui->progressBar_converting->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectImages()
{
    QSettings MySettings;

    QList<QString> SelectedFiles;
    QString SelectedFolder = "";

    if (!ui->checkBox_useInputFolder->isChecked()) {
        SelectedFiles = QFileDialog::getOpenFileNames(this, "Select input images", \
MySettings.value(IMAGE_SELECT_DIR_SETTINGS_KEY, DEFAULT_IMAGE_SELECT_DIR).toString(), tr("Images (*." + \
MySettings.value(IMAGE_EXTENSIONS_SETTINGS_KEY, QVariant::fromValue(DEFAULT_IMAGE_EXTENSIONS)).value<QList<QString>>().join(" ").toLocal8Bit() \
+ ");;All files (*.*)"));

        if (!SelectedFiles.isEmpty()) {
            QDir CurrentDir;
            SelectedFolder = CurrentDir.absoluteFilePath(SelectedFiles[0]);
            MySettings.setValue(IMAGE_SELECT_DIR_SETTINGS_KEY, CurrentDir.absoluteFilePath(SelectedFiles[0]));
        }
    } else {
        SelectedFolder = QFileDialog::getExistingDirectory(this, "Select input folder", MySettings.value(IMAGE_SELECT_DIR_SETTINGS_KEY, DEFAULT_IMAGE_SELECT_DIR).toString());

        QDir selectedDir = QDir(SelectedFolder);
        selectedDir.setFilter(QDir::Files | QDir::NoDot | QDir::NoDotDot);
        QStringList qsl;
        for (QString extension : MySettings.value(IMAGE_EXTENSIONS_SETTINGS_KEY, QVariant::fromValue(DEFAULT_IMAGE_EXTENSIONS)).value<QList<QString>>()) {
            qsl.append("*." + extension);
        }
        selectedDir.setNameFilters(qsl);
        SelectedFiles = findFilesRecursively(selectedDir);
        MySettings.setValue(IMAGE_SELECT_DIR_SETTINGS_KEY, SelectedFolder);
    }

    if (SelectedFiles.isEmpty())
    {
        return;
    }

    if (!SelectedFiles.isEmpty()) {
        // QDir CurrentDir;
        // MySettings.setValue(IMAGE_SELECT_DIR_SETTINGS_KEY, CurrentDir.absoluteFilePath(SelectedFiles[0]));
    }

    QList<QString> filenames = SelectedFiles;
    inputImages = filenames;
    qDebug() << SelectedFiles.count();
    ui->lineEdit_selectImages->setText(SelectedFolder);
}

void MainWindow::selectOutputFolder()
{
    QSettings MySettings;

    QString SelectedFolder = QFileDialog::getExistingDirectory(this, "Select output folder", MySettings.value(DEFAULT_OUTPUT_DIR_SETTINGS_KEY, DEFAULT_DEFAULT_OUTPUT_DIR).toString());

    if (SelectedFolder == "")
    {
        return;
    }

    if (!SelectedFolder.isEmpty()) {
        QDir CurrentDir;
        MySettings.setValue(DEFAULT_OUTPUT_DIR_SETTINGS_KEY, CurrentDir.absoluteFilePath(SelectedFolder));
    }

    QString folderName = SelectedFolder;
    ui->lineEdit_selectOutputFolder->setText(folderName);
}

void MainWindow::resizeImage()
{

    if (outputFolder.isEmpty()) {

        QMessageBox::warning(this, tr("No output folder chosen"),
            tr("Please choose an output folder to proceed."));
        return;
    }
    if (inputImages.isEmpty()) {
        QMessageBox::warning(this, tr("No images chosen"),
            tr("Please choose some images to proceed."));
        return;
    }

    ui->progressBar_converting->setVisible(true);
    ui->pushButton_Resize->setDisabled(true);
    ui->pushButton_selectImages->setDisabled(true);
    ui->pushButton_selectOutputFolder->setDisabled(true);
    ui->actionImage_Extensions->setDisabled(true);
    int i = 0;
    ui->progressBar_converting->setValue(i);

    char *format = nullptr;
    int quality = -1;
    if (ui->checkBox_convertToJPEG->isChecked()) {
        format = "JPEG";
        quality = ui->spinBox_qualitySetting->value();
    }

    for (QString imagePath : inputImages) {
        QImage *tempImage = new QImage(imagePath);
        QFileInfo fi(imagePath);
        QString fileName = fi.fileName();
        if (ui->checkBox_convertToJPEG->isChecked()) {
            fileName = fi.completeBaseName() + ".jpg";
        }
        QString targetPath = outputFolder + QDir::separator() + fileName;
        QImage resizedImage;
        if (ui->checkBox_keepAspectRatio->isChecked()) {
            resizedImage = tempImage->scaled(ui->spinBox_targetWidth->value(), ui->spinBox_targetHeight->value(), Qt::AspectRatioMode::KeepAspectRatio);
        }
        else {
            resizedImage = tempImage->scaled(ui->spinBox_targetWidth->value(), ui->spinBox_targetHeight->value(), Qt::AspectRatioMode::IgnoreAspectRatio);
        }
        resizedImage.save(targetPath, format, quality);
        qDebug() << i << " === " << imagePath;
        delete tempImage;
        ui->progressBar_converting->setValue(100*++i/inputImages.count());
    }

    ui->progressBar_converting->setVisible(false);
    ui->pushButton_Resize->setDisabled(false);
    ui->pushButton_selectImages->setDisabled(false);
    ui->pushButton_selectOutputFolder->setDisabled(false);
    ui->actionImage_Extensions->setDisabled(false);
}

void MainWindow::toggleKeepAspectRatio(bool checked)
{
    QSettings MySettings;
    MySettings.setValue(KEEP_ASPECT_RATIO_SETTINGS_KEY, checked);
}

void MainWindow::toggleConvertToJPEG(bool checked)
{
    QSettings MySettings;
    MySettings.setValue(CONVERT_TO_JPEG_SETTINGS_KEY, checked);
}

void MainWindow::toggleUseInputFolder(bool checked)
{
    QSettings MySettings;
    MySettings.setValue(USE_INPUT_FOLDER_SETTINGS_KEY, checked);
}

void MainWindow::onHeightChange(int height)
{
    QSettings MySettings;
    MySettings.setValue(TARGET_HEIGHT_SETTINGS_KEY, height);
}

void MainWindow::onWidthChange(int width)
{
    QSettings MySettings;
    MySettings.setValue(TARGET_WIDTH_SETTINGS_KEY, width);
}

void MainWindow::onQualityChange(int quality)
{
    QSettings MySettings;
    MySettings.setValue(IMAGE_QUALITY_SETTINGS_KEY, quality);
}

void MainWindow::optionsImageExtensionsSelect()
{
    ImageExtensionsSelectDialog *dialog = new ImageExtensionsSelectDialog(this);

    dialog->exec();
}

