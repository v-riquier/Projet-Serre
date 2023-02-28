#include "Projetserre.h"

Projetserre::Projetserre(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	socket = new QTcpSocket(this);
	QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
	/*
	chercher envoi http post url("http:"//server/fichier.php")
	chercher formattage données en JSON
	*/
}

Projetserre::~Projetserre()
{
}

void Projetserre::onConnectButtonClicked()
{
	socket->connectToHost(ip, port);
	if (!socket->waitForConnected(1000))
		ui.lblConnect->setText("La carte n'est pas allumée");
}

void Projetserre::onDisconnectButtonClicked()
{
	socket->disconnectFromHost();
}

void Projetserre::onSocketConnected()
{
	ui.lblConnect->setText("Connecte");
}

void Projetserre::onSocketDisconnected()
{
	ui.lblConnect->setText("Deconnecte");
}

void Projetserre::AffichageDonnees()
{
	if (socket->state() == QAbstractSocket::ConnectedState) {
		char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x03, 0x46, 0xB4, 0x00, 0x20 };
		QByteArray data(trame, 12);
		socket->write(data);
		socket->flush();
	}
}

void Projetserre::receiveData()
{
	QByteArray data = socket->readAll();
	ui.lblDonnees->setText(data);
}

/*float Projetserre::calculHumidite(int Vout) {
	float humid = -1.91 * pow(10,-9) * pow(Vout,3);
	humid += 1.33 * pow(10,-5) * pow(Vout,2);
	humid += 9.56 * pow(10,-3) * Vout;
	humid -= 2.16 * pow(10,1);
	return humid;
}

void Projetserre::problemes()
{
	QNetworkAccessManager* manager = new QNetworkAccessManager(this);

	QUrl url("http:///192.168.64.158/Serre/fonctions/api.php");
	QNetworkRequest request(url);

	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QUrl params;
	params.setQuery("client_id");
	params.setQuery("client_secret");
	params.setQuery("code");
	// etc

	QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

	manager->post(request, params.query(QUrl::FullyEncoded).toLatin1());
}*/
