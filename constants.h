#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QList>
#include <QString>

const QString IMAGE_EXTENSIONS_SETTINGS_KEY = "image_extensions_list";
const QString TARGET_HEIGHT_SETTINGS_KEY = "target_height";
const QString TARGET_WIDTH_SETTINGS_KEY = "target_width";
const QString KEEP_ASPECT_RATIO_SETTINGS_KEY = "keep_aspect_ratio";
const QString CONVERT_TO_JPEG_SETTINGS_KEY = "convert_to_jpeg";
const QString USE_INPUT_FOLDER_SETTINGS_KEY = "user_input_folder";
const QString IMAGE_SELECT_DIR_SETTINGS_KEY = "default_image_select_dir";
const QString DEFAULT_OUTPUT_DIR_SETTINGS_KEY = "default_output_dir";
const QString IMAGE_QUALITY_SETTINGS_KEY = "image_quality";

const QList<QString> DEFAULT_IMAGE_EXTENSIONS = QList<QString>{"png", "jpg", "jpeg", "xpm", "gif"};
const int DEFAULT_TARGET_HEIGHT = 500;
const int DEFAULT_TARGET_WIDTH = 500;
const bool DEFAULT_KEEP_ASPECT_RATIO = true;
const bool DEFAULT_CONVERT_TO_JPEG = true;
const bool DEFAULT_USE_INPUT_FOLDER = true;
const QString DEFAULT_IMAGE_SELECT_DIR = "";
const QString DEFAULT_DEFAULT_OUTPUT_DIR = "";
const int DEFAULT_IMAGE_QUALITY = -1;

#endif // CONSTANTS_H
