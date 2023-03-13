#pragma once

#include <QtWidgets/QMainWindow>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include "ui_Projetserre.h"
#include "CalculJson.h"

class Projetserre : public QMainWindow
{
    Q_OBJECT

public:
    Projetserre(QWidget *parent = nullptr);
    ~Projetserre();

public slots:
	void onConnectButtonClicked();
	void onDisconnectButtonClicked();
	void onSocketConnected();
	void onSocketDisconnected();
	void wSocketConnected();
	void wSocketDisconnected();
	void DonneesSensor();
	void DonneesCapteurs();
	void receiveData();
	void sendWebsocket();

private:
	//void problemes();
    Ui::ProjetserreClass ui;
	QTcpSocket* socket;
	QString ip = "192.168.65.8";
	quint16 port = 502;
	QJsonObject donneesJson;
	QWebSocketServer* wSocketServer;
	QWebSocket* wSocket = nullptr;
};
