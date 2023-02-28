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
		char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x03, 0x4D, 0x58, 0x00, 0x04 };
		QByteArray data(trame, 12);
		socket->write(data);
		socket->flush();
	}
}

void Projetserre::receiveData()
{
	QByteArray data = socket->readAll();
	data = data.right(8);//prend les 8 derniers caracteres
	QByteArray temp = data.left(4);//prend les 4 premiers caracteres
	QByteArray humid = data.right(4);
	float temperature = QByteArrayToFloat(temp);
	float humidite = QByteArrayToFloat(humid);
	QString affTemp = QString::number(temperature) + "°C";
	QString affHumid = QString::number(humidite) + "%";
	ui.lblTemp->setText(affTemp);
	ui.lblHumid->setText(affHumid);
	QJsonValue jsonTemp(affTemp);
	QJsonValue jsonHumid(affHumid);
	donneesJson.insert("Humidité", jsonHumid);
	donneesJson.insert("Temperature", jsonTemp);
	qDebug() << donneesJson;
}

float Projetserre::QByteArrayToFloat(QByteArray arr)
{
	static_assert(std::numeric_limits<float>::is_iec559, "Only supports IEC 559 (IEEE 754) float");

	quint32 temp = ((quint8)arr[0] << 24) | ((quint8)arr[1] << 16) | ((quint8)arr[2] << 8) | (quint8)arr[3];
	//La variable temp est un entier de 32 bits, arr est un tableau de 4 cases d'entiers de 8 bits.
	//En partant de la fin, le programme place arr[0] au bit 24 puis arr[0] au bit 16 puis arr[0] au bit 8 et arr[0] au bit 0

	float* out = reinterpret_cast<float*>(&temp);
	//le compilateur va forcer la transformation d'un quint32 en float

	return *out;
}

/*float Projetserre::calculHumidite(int Vout) {
	float humid = -1.91 * pow(10, -9) * pow(Vout, 3);
	humid += 1.33 * pow(10, -5) * pow(Vout, 2);
	humid += 9.56 * pow(10, -3) * Vout;
	humid -= 2.16 * pow(10, 1);
	return humid;
}*/

void Projetserre::problemes()
{
	QNetworkAccessManager* mgr = new QNetworkAccessManager(this);
	const QUrl url(QStringLiteral("http:/192.168.64.158/Serre/fonctions/api.json"));
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QJsonDocument document(donneesJson);
	QByteArray donnees = document.toJson();
	QNetworkReply* reply = mgr->post(request, donnees);

	QObject::connect(reply, &QNetworkReply::finished, [=]() {
		if (reply->error() == QNetworkReply::NoError) {
			QString contents = QString::fromUtf8(reply->readAll());
			qDebug() << contents;
		}
		else {
			QString err = reply->errorString();
			qDebug() << err;
		}
	reply->deleteLater();
		});
}
