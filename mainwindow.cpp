#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    jsonModel = new JsonTreeModel(this);
    ui->treeView->setModel(jsonModel);
    QHeaderView* headerView = ui->treeView->header();
    QFont font("Arial", 9, 100);
    font.setBold(true);
    headerView->setFont(font);

    ui->treeView->header()->setStretchLastSection(false);
    ui->treeView->setHeaderHidden(false);

    initLoadDump();
    initEdit();

    connect(ui->insertColumnAction, SIGNAL(clicked()), this, SLOT(insertColumn()));
    connect(ui->removeColumnAction, SIGNAL(clicked()), this, SLOT(removeColumn()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initLoadDump()
{
    // выберите путь к txt файлу с данными
    connect(ui->btnLoadTxtPath,&QPushButton::clicked,this,[this](){
        const QString jsonpath = QFileDialog::getOpenFileName(
            this,
            "Open File",
            QDir::currentPath(),
            "text files(*.txt);; All files(*.*)");
        if (jsonpath.isEmpty()) return;
        ui->editLoadPath->setText(jsonpath);
    });

    // импортировать файл c данными
    connect(ui->btnLoadTxt, &QPushButton::clicked, this, [this]() {
        const QString loadpath = ui->editLoadTxtPath->text();
        if (loadpath.isEmpty()) return;

        // Определить путь и нормально ли он открывается
        if (loadpath.isEmpty()) return;
        QFile file(loadpath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

        // Закрыть файл после чтения данных
        const QByteArray raw_data = file.readAll();
        file.close();

        jsonModel->loadData(raw_data);
    });

    // выберите путь к файлу для импорта
    connect(ui->btnLoadPath,&QPushButton::clicked,this,[this](){
        const QString jsonpath = QFileDialog::getOpenFileName(
            this,
            "Open File",
            QDir::currentPath(),
            "Json files(*.json);; text files(*.txt);; All files (*.*)");
        if(jsonpath.isEmpty()) return;
        ui->editLoadPath->setText(jsonpath);
    });

    // импортировать файл Json или txt
    connect(ui->btnLoadJson,&QPushButton::clicked,this,[this](){
        const QString loadpath = ui->editLoadPath->text();
        if(loadpath.isEmpty()) return;

        // Определить путь и нормально ли он открывается
        if (loadpath.isEmpty()) return;
        QFile file(loadpath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

        // Закрыть файл после чтения данных
        const QByteArray raw_data = file.readAll();
        file.close();

        QFileInfo file_info(file);
        QString file_extension = file_info.suffix();

        if (file_extension == "json") {
            //parseJson.loadJson(loadpath);
            jsonModel->loadJson(raw_data);
        }
        else if (file_extension == "txt") {
            jsonModel->loadTxt(raw_data);
        }
        //ui->treeView->expandAll(); // развернуть все вложения дерева
    });

    // выберите путь к экспортируемому файлу
    connect(ui->btnDumpPath,&QPushButton::clicked,this,[this](){
        const QString jsonpath = QFileDialog::getSaveFileName(this,
        "Save File",
        QDir::currentPath(),
        "Json files(*.json);; All files (*.*)");;
        if(jsonpath.isEmpty()) return;
        ui->editDumpPath->setText(jsonpath);
    });
    // экспортировать файл Json
    connect(ui->btnDumpJson,&QPushButton::clicked,this,[this](){
        const QString dumppath = ui->editDumpPath->text();
        if(dumppath.isEmpty()) return;
        //parseJson.dumpJson(dumppath);
        jsonModel->dumpJson(dumppath);
    });
}

void MainWindow::initEdit()
{
    // Справочный пример добавления и удаления деталей: редактируемое дерево

    // Мой элемент по умолчанию имеет значение enum:none, но в настоящее время нет делегата для настройки операции
    // Изменено на enum:value в добавлении элементов элемента только для целей тестирования

    // Добавляем узел
    connect(ui->btnInsert,&QPushButton::clicked,this,[this](){
        QModelIndex index = ui->treeView->selectionModel()->currentIndex();
        if(!index.isValid())
            return;
        QAbstractItemModel *model = ui->treeView->model();
        if(!model->insertRow(index.row()+1,index.parent()))
            return;
        updateIndex();
        // Изменить содержимое вставки
        for (int column = 0; column < model->columnCount(index.parent()); ++column) {
            QModelIndex child = model->index(index.row()+1, column, index.parent());
            model->setData(child, QVariant("[No data]"), Qt::EditRole);
        }
    });
    // Добавить дочерний узел
    connect(ui->btnInsertChild,&QPushButton::clicked,this,[this](){
        QModelIndex index = ui->treeView->selectionModel()->currentIndex();
        QAbstractItemModel *model = ui->treeView->model();

        if (model->columnCount(index) == 0) {
            if (!model->insertColumn(0, index))
                return;
        }

        if (!model->insertRow(0, index))
            return;
        // Изменить содержимое вставки
        for (int column = 0; column < model->columnCount(index); ++column) {
            QModelIndex child = model->index(0, column, index);
            model->setData(child, QVariant("[No data]"), Qt::EditRole);
        }
        ui->treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                                        QItemSelectionModel::ClearAndSelect);
        updateIndex();
    });
    // Удалить дерево узлов
    connect(ui->btnRemove, &QPushButton::clicked, this, [this]() {
        QModelIndex index = ui->treeView->selectionModel()->currentIndex();
        QAbstractItemModel *model = ui->treeView->model();
        if (model->removeRow(index.row(), index.parent()))
            updateIndex();
    });
}

void MainWindow::updateIndex()
{
    // Обновим состояние кнопок:
    bool hasSelection = !ui->treeView->selectionModel()->selection().isEmpty();
    ui->removeColumnAction->setEnabled(hasSelection);
    bool hasCurrent = ui->treeView->selectionModel()->currentIndex().isValid();
    ui->insertColumnAction->setEnabled(hasCurrent);
    // Покажем информацию в заголовке окна:
    if (hasCurrent) {
        ui->treeView->closePersistentEditor(ui->treeView->selectionModel()->currentIndex());
        int row = ui->treeView->selectionModel()->currentIndex().row();
        int column = ui->treeView->selectionModel()->currentIndex().column();
        if (ui->treeView->selectionModel()->currentIndex().parent().isValid())
            qDebug() << tr("Position: (%1,%2)").arg(row).arg(column);
        else
            qDebug() << tr("Position: (%1,%2) in top level").arg(row).arg(column);
    }
}

bool MainWindow::insertColumn() {
    QAbstractItemModel* model = ui->treeView->model();
    int column = ui->treeView->selectionModel()->currentIndex().column();
    bool changed = model->insertColumn(column);
    if (changed)
        model->setHeaderData(column, Qt::Horizontal, QVariant("Столбец"), Qt::EditRole);
    updateIndex();
    return changed;
}

bool MainWindow::removeColumn() {
    QAbstractItemModel* model = ui->treeView->model();
    int column = ui->treeView->selectionModel()->currentIndex().column();
    bool changed = model->removeColumn(column); // Удалить столбец для каждого потомка
    if (changed)
        updateIndex();
    return changed;
}
