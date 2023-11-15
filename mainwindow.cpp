#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>

#include <QDebug>

constexpr auto ENDL = "\x0D\x0A";

const QString HEADERS = "[Headers]";
const QString ROOT = "[Root]";

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

    connect(ui->treeView, SIGNAL(jsonModel->dataChanged), this, SLOT(sendMessage()));
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
        expandToKey("MODE1");
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

        expandToKey("MODE1");
    });

    // выберите путь к экспортируемому файлу
    connect(ui->btnDumpPath,&QPushButton::clicked,this,[this](){
        const QString jsonpath = QFileDialog::getSaveFileName(this,
        "Save File",
        QDir::currentPath(),
        "Json files(*.json);; All files (*.*)");

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
    connect(ui->btnInsert, &QPushButton::clicked, this, [this]()
        {
            QModelIndex index = ui->treeView->selectionModel()->currentIndex();
            if (!index.isValid())
                return;
            QAbstractItemModel* model = ui->treeView->model();
            if (!model->insertRow(index.row() + 1, index.parent()))
                return;
            updateIndex();
            // Изменить содержимое вставки
            for (int column = 0; column < model->columnCount(index.parent()); ++column) {
                QModelIndex child = model->index(index.row() + 1, column, index.parent());
                model->setData(child, QVariant("[No data]"), Qt::EditRole);
            }
        });

    // Добавить дочерний узел
    connect(ui->btnInsertChild, &QPushButton::clicked, this, [this]()
        {
            QModelIndex index = ui->treeView->selectionModel()->currentIndex();
            QAbstractItemModel* model = ui->treeView->model();

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
    connect(ui->btnRemove, &QPushButton::clicked, this, [this]()
        {
            QModelIndex index = ui->treeView->selectionModel()->currentIndex();
            QAbstractItemModel* model = ui->treeView->model();
            if (model->removeRow(index.row(), index.parent()))
                updateIndex();
        });

    // Добавляем столбец
    connect(ui->insertColumnAction, &QPushButton::clicked, this, [this]()
        {
            QAbstractItemModel* model = ui->treeView->model();
            int column = ui->treeView->selectionModel()->currentIndex().column();
            bool changed = model->insertColumn(column);
            if (changed)
                model->setHeaderData(column, Qt::Horizontal, QVariant("Столбец"), Qt::EditRole);
            updateIndex();
            return changed;
        });

    // Удалить столбец
    connect(ui->removeColumnAction, &QPushButton::clicked, this, [this]()
        {
            QAbstractItemModel* model = ui->treeView->model();
            int column = ui->treeView->selectionModel()->currentIndex().column();
            bool changed = model->removeColumn(column); // Удалить столбец для каждого потомка
            if (changed)
                updateIndex();
            return changed;
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

void MainWindow::expandToKey(QString key)
{
    QVector<int> indexes;
    jsonModel->findByKey(key, indexes);

    QModelIndex index;
    for (int inx = indexes.size() - 1; inx >= 0; inx--) {
        index = jsonModel->index(indexes.at(inx), 0, index);
    }
    ui->treeView->expand(index);
    ui->treeView->scrollTo(index);
    ui->treeView->setCurrentIndex(index);
    ui->treeView->resizeColumnToContents(0);
}

void MainWindow::sendMessage()
{
    QVector<QModelIndex> indexes;
    QString message = "PARAMS SET: ";

    QModelIndex index = ui->treeView->currentIndex();
    indexes << index;
    while (jsonModel->parent(index) != ui->treeView->rootIndex()) {
        index = index.parent();
        indexes << index;
    }
    if (indexes.size() < 2)
        return;

    for (int inx = indexes.size() - 2; inx >= 0; inx--) {
        message += jsonModel->data(indexes.at(inx)).toString();
        (inx != 0) ? (message += "/") : (message += ", ");
    }

    QModelIndex index_parameter = indexes.at(0);
    int row = index_parameter.row();
    QModelIndex index_data = jsonModel->index(row, 1, index_parameter.parent());
    message += jsonModel->data(index_data).toString();
    message += ENDL;

    qDebug() << message;
    message += ui->textEdit->toPlainText();
    ui->textEdit->setText(message);

    //ui->textEdit->setTextColor({255, 255, 0});
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << "=== press key";

    int keyValue = event->key();

    if (keyValue == Qt::Key_Enter) {
        qDebug() << "Press Enter";
    }

    if (keyValue == Qt::Key_Return) {
        //qDebug() << "Press Return";
        sendMessage();
    }

    if (keyValue == Qt::Key_Space) {
        qDebug() << "Press Space";
    }

    if (keyValue == Qt::Key_K) {
        qDebug() << "Press K";
    }

    if (keyValue == Qt::Key_Control) {
        // При нажатии клавиши Ctrl получается абсолютная позиция мыши на экране
        QPoint pt = QCursor::pos();
        QString str;
        str = QString("%1 , %2").arg(pt.x()).arg(pt.y());
        qDebug() << str;
    }
}
