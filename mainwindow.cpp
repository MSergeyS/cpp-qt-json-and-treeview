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

    initLoadDump();
    initEdit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initLoadDump()
{
    // выберите путь к txt файлу
    connect(ui->btnLoadTxtPath,&QPushButton::clicked,this,[this](){
        const QString filepath = QFileDialog::getOpenFileName(this,"File Path");
        if(filepath.isEmpty()) return;
        ui->editLoadTxtPath->setText(filepath);
    });
    // импортировать файл txt
    connect(ui->btnLoadTxt,&QPushButton::clicked,this,[this](){
        const QString filepath = ui->editLoadTxtPath->text();

        // Определить путь и нормально ли он открывается
        if(filepath.isEmpty())
            return false;

        jsonModel->loadTxt(filepath);
        ui->treeView->expandAll();

        /*
        QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Файл не открылся!!!";
        }

        // Закрыть файл после чтения данных
        const QByteArray raw_data=file.readAll();
        file.close();

        // Создаем заголовки столбцов:
        QStringList headers;
        headers << tr("Заголовок") << tr("Описание");
        // Загружаем данные в модель:
        TreeModel *model = new TreeModel(headers, raw_data);
        //ui->treeView->expandAll();
        ui->treeView->setModel(model);
        */
    });

    // выберите путь к файлу для импорта
    connect(ui->btnLoadPath,&QPushButton::clicked,this,[this](){
        const QString jsonpath = QFileDialog::getOpenFileName(this,"File Path");
        if(jsonpath.isEmpty()) return;
        ui->editLoadPath->setText(jsonpath);
    });
    // импортировать файл Json
    connect(ui->btnLoadJson,&QPushButton::clicked,this,[this](){
        const QString loadpath = ui->editLoadPath->text();
        if(loadpath.isEmpty()) return;
        //parseJson.loadJson(loadpath);
        jsonModel->loadJson(loadpath);
        ui->treeView->expandAll();
    });

    // выберите путь к экспортируемому файлу
    connect(ui->btnDumpPath,&QPushButton::clicked,this,[this](){
        const QString jsonpath = QFileDialog::getSaveFileName(this,"Save Path");
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
        /*for (int column = 0; column < model->columnCount(index.parent()); ++column) {
            QModelIndex child = model->index(index.row()+1, column, index.parent());
            model->setData(child, QVariant("[No data]"), Qt::EditRole);
        }*/
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
        /*for (int column = 0; column < model->columnCount(index); ++column) {
            QModelIndex child = model->index(0, column, index);
            model->setData(child, QVariant("[No data]"), Qt::EditRole);
        }*/
        ui->treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                                        QItemSelectionModel::ClearAndSelect);
        updateIndex();
    });
    // Удалить дерево узлов
    connect(ui->btnRemove,&QPushButton::clicked,this,[this](){
        QModelIndex index = ui->treeView->selectionModel()->currentIndex();
        QAbstractItemModel *model = ui->treeView->model();
        if (model->removeRow(index.row(), index.parent()))
            updateIndex();
    });
}

void MainWindow::updateIndex()
{
    //Пример копирования
    //bool hasSelection = !ui->treeView->selectionModel()->selection().isEmpty();
    //removeRowAction->setEnabled(hasSelection);
    //removeColumnAction->setEnabled(hasSelection);

    bool hasCurrent = ui->treeView->selectionModel()->currentIndex().isValid();
    //insertRowAction->setEnabled(hasCurrent);
    //insertColumnAction->setEnabled(hasCurrent);

    if (hasCurrent) {
        ui->treeView->closePersistentEditor(ui->treeView->selectionModel()->currentIndex());

        int row = ui->treeView->selectionModel()->currentIndex().row();
        int column = ui->treeView->selectionModel()->currentIndex().column();
        if (ui->treeView->selectionModel()->currentIndex().parent().isValid())
            qDebug()<<tr("Position: (%1,%2)").arg(row).arg(column);
        else
            qDebug()<<tr("Position: (%1,%2) in top level").arg(row).arg(column);
    }
}
