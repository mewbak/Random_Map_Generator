#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MapHelper.h"

namespace Ui {
class MainWindow;
}

struct AppData
{
    // this is only used by QT front end.
    int     progress;
    int     algorithm;
    std::string tile_set;
    int     size_x;
    int     size_y;
    int     seed;
    bool    seed_set;
    bool    gen_exits;
    bool    gen_enemies;
    bool    gen_chests;
    std::string file_name;
    std::string status;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppData app_data;
    Map*  map_pointer;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBox_algorithm_currentIndexChanged(int index);
    void on_comboBox_tileSet_currentIndexChanged(int index);
    void on_comboBox_mapSize_currentIndexChanged(int index);
    void on_checkBox_genExits_stateChanged(int arg1);
    void on_checkBox_genChests_stateChanged(int arg1);
    void on_checkBox_genEnemies_stateChanged(int arg1);

    void on_pushButton_released();

    void renderPreview(Map* map_pointer);

    void on_lineEdit_seed_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
