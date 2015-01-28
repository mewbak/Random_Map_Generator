#include "mainwindow.h"
#include "map.h"
#include "MapGenerator.h"
#include "ui_mainwindow.h"
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    map_pointer            = new Map;
    map_pointer->room      = new Room[ROOM_MAX];
    map_pointer->tile      = new GenTile[100*100];
    app_data.algorithm     = ALGORITHM_CAVE;
    app_data.file_name     = "default";
    app_data.gen_chests    = false;
    app_data.gen_enemies   = false;
    app_data.gen_exits     = false;
    app_data.output_format = OUTPUT_FORMAT_FLARE;
    app_data.progress      = 0;
    app_data.size_x        = 100;
    app_data.size_y        = 100;
    app_data.status        = "Ready";
    app_data.tile_set      = TILESET_CAVE;
}

MainWindow::~MainWindow()
{
    delete[] map_pointer->room;
    delete[] map_pointer->tile;
    delete   map_pointer;
    delete ui;
}

void MainWindow::on_comboBox_algorithm_currentIndexChanged(int index)
{
    MainWindow::app_data.algorithm = index;
}

void MainWindow::on_comboBox_tileSet_currentIndexChanged(int index)
{
    MainWindow::app_data.tile_set = index;
}

void MainWindow::on_comboBox_mapSize_currentIndexChanged(int index)
{
    switch (index)
    {
        case 0:
        default:
        MainWindow::app_data.size_x = 100;
        MainWindow::app_data.size_y = 100;
        break;
    }
}

void MainWindow::on_comboBox_outputFormat_currentIndexChanged(int index)
{
    MainWindow::app_data.output_format = index;
}

void MainWindow::on_checkBox_genExits_stateChanged(int arg1)
{
    if (arg1 == 0) MainWindow::app_data.gen_exits = false;
    else MainWindow::app_data.gen_exits = true;
}

void MainWindow::on_checkBox_genChests_stateChanged(int arg1)
{
    if (arg1 == 0) MainWindow::app_data.gen_chests = false;
    else MainWindow::app_data.gen_chests = true;
}

void MainWindow::on_checkBox_genEnemies_stateChanged(int arg1)
{
    if (arg1 == 0) MainWindow::app_data.gen_enemies = false;
    else MainWindow::app_data.gen_enemies = true;
}

void MainWindow::on_pushButton_released()
{
    std::stringstream temp_stream;
    MainWindow::app_data.file_name = MainWindow::ui->lineEdit->text();
    MainWindow::ui->progressBar->setValue(0);
    MainWindow::ui->label_status->setText("Seeding random...");
    int seed = time(NULL);
    srand(seed);
    MainWindow::ui->progressBar->setValue(1);
    MainWindow::ui->label_status->setText("Initializing intermediate map...");
    MainWindow::ui->progressBar->setValue(10);
    MainWindow::ui->label_status->setText("Generating map...");
    MapGenerate(MainWindow::map_pointer,MainWindow::app_data.algorithm, MainWindow::app_data.size_x, MainWindow::app_data.size_y);
    MainWindow::ui->textEdit_mapview->clear();
    for (int i = 0; i < map_pointer->size(); i++)
    {
        if (map_pointer->tile[i].data == Tile_Type::TILE_FLOOR) temp_stream << "<font color=#006600>";
        if (map_pointer->tile[i].data == Tile_Type::TILE_WALL)  temp_stream << "<font color=#F8F8F8>";
        temp_stream << "&#9632;";//map_pointer->tile[i].data;
        temp_stream << "</font>";
        if ((i % map_pointer->w) == (map_pointer->w-1)) temp_stream << std::endl;
    }
    MainWindow::ui->textEdit_mapview->insertHtml(temp_stream.str().c_str());
    switch (MainWindow::app_data.output_format)
    {
    case OUTPUT_FORMAT_FROST_AND_FLAME:
        //file_export_flare(map_pointer,file_name);
    break;
    case OUTPUT_FORMAT_TILED:
        //file_export_flare(map_pointer,file_name);
    break;
    default:
    case OUTPUT_FORMAT_FLARE:
        flare_map_type* flare_map_pointer = new flare_map_type;
        MainWindow::ui->progressBar->setValue(80);
        MainWindow::ui->label_status->setText("Applying tile set...");
        MainWindow::ui->progressBar->setValue(90);
        // should be removed
        MapGenerator::map_to_flare_map(MainWindow::map_pointer,flare_map_pointer,MainWindow::app_data.tile_set);

        MainWindow::ui->label_status->setText("Exporting to file...");

        // use MapGenerator::Export(Map* map_pointer, std::string file_name);
        MapGenerator::file_export_flare(flare_map_pointer,MainWindow::app_data.file_name.toStdString());

        delete[] flare_map_pointer->layer_background;
        delete[] flare_map_pointer->layer_collision;
        delete[] flare_map_pointer->layer_foreground;
        delete[] flare_map_pointer->layer_fringe;
        delete[] flare_map_pointer->layer_object;
        delete   flare_map_pointer;
    break;
    }
    MainWindow::ui->progressBar->setValue(100);
    MainWindow::ui->label_status->setText("Done.");
}
