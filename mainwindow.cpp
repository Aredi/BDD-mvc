#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDialog>

QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonConnexion_clicked()
{
    db.setHostName("172.17.0.3");
    db.setDatabaseName("gestmagasin");
    db.setUserName("root");
    db.setPassword("tsiris");
    
    if (db.open() == 0) {
        qDebug() << "Impossible de se connecter a la BDD.";
    } else {
        qDebug() << "Connexion reussie";
    }
}


void MainWindow::on_pushButtonInsertBDD_clicked()
{
    QSqlQuery rayon("insert INTO rayon (id,nom) VALUES ('1','Fruits'),('2','Legumes'),('3','Viandes')");
    QSqlQuery produit("insert INTO produit (id,designation,prix,idrayon) VALUES ('1','Orange','3,5','1'),('2','pomme','2.1','1'),('3','banane','1,95','1'),('4','scarole','1.9','2'),('5','golden','2.25','2'),('6','cotelettes mouton','35','3'),('7','bifteak','25','3'),('8','jambon bayonne','33','3')");
    rayon.exec();
    produit.exec();
}



void MainWindow::on_pushButtonAfficherRayon_clicked()
{
    modelRayon = new QSqlTableModel(this,db);
    modelRayon->setTable("rayon");

    if (modelRayon->select() == 0) {
        qDebug() << "Echec du peuplement du modele";
    } else {
        qDebug() << "Peuplement du modele accompli";
    }

    modelProduitRel = new QSqlRelationalTableModel(this,db);
    modelProduitRel->setTable("produit");
    modelProduitRel->setRelation(modelProduitRel->fieldIndex("idrayon"), QSqlRelation("rayon","id","nom"));

    if (modelProduitRel->select() == 0) {
        qDebug() << "Echec du peuplement du modele produit";
    } else {
        qDebug() << "Peuplement du modele accompli produit";
    }
    
    ui->tableViewRayon->setModel(modelRayon);
    ui->tableViewRayon->resizeColumnsToContents();
    ui->tableViewArticle->setModel(modelProduitRel);
    ui->tableViewArticle->resizeColumnsToContents();

    connect(ui->tableViewRayon->selectionModel(),
    SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(clicTableChangementRayon(const QModelIndex &)));
}


void MainWindow::clicTableChangementRayon(const QModelIndex &index)
{
    if(index.isValid()) {
        int lignecliquee=index.row();
        QSqlRecord record1 =  modelRayon->record(lignecliquee);
        QString nom = record1.value("id").toString();
        QString filtre = "idrayon=" + nom;
        modelProduitRel->setFilter(filtre);
    }
}


void MainWindow::clicTableModificationArticle(const QModelIndex &index)
{
    // TODO: clicTableModificationArticle
}


void MainWindow::on_pushButtonEnregistrer_clicked()
{
    QSqlRecord RecordChangement;
    RecordChangement.record(modelProduitRel->rowCount());

    while (int i = 0, i < modelProduitRel->rowCount(), i++)
    {
        // TODO: While        
    }
}
