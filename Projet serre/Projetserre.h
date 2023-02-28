#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Projetserre.h"
#include <QTcpSocket>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

class Projetserre : public QMainWindow
{
    Q_OBJECT

public:
    Projetserre(QWidget *parent = nullptr);
    ~Projetserre();
	float QByteArrayToFloat(QByteArray arr);

public slots:
	void onConnectButtonClicked();
	void onDisconnectButtonClicked();
	void onSocketConnected();
	void onSocketDisconnected();
	void AffichageDonnees();
	void receiveData();

private:
	/*float calculHumidite(int);*/
	void problemes();
    Ui::ProjetserreClass ui;
	QTcpSocket* socket;
	QString ip = "192.168.65.8";
	quint16 port = 502;
	QJsonObject donneesJson;
};
