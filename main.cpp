
#include <iostream>
#include "niyeyedelei.h"
//#include <QApplication>
#include <QtWidgets/QApplication>

#include <QFile>
#include <QDebug>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "dummy_paramset.h"



int read_json(QString jsonPath) {

	
	QString NjsonPath = QDir::fromNativeSeparators(jsonPath);
	qInfo() << "openning" << NjsonPath << "\n";

	QFile file(NjsonPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Failed to open JSON file:" << NjsonPath;
		return -1;
	}

	QByteArray jsonData = file.readAll();
	file.close();

	QJsonParseError jsonError;
	QJsonDocument doc = QJsonDocument::fromJson(jsonData, &jsonError);
	if (jsonError.error != QJsonParseError::NoError) {
		qDebug() << "Failed to parse JSON:" << jsonError.errorString();
		return -1;
	}

	QJsonObject jsonObject = doc.object();

	// 处理 JSON 数据
	// 示例：获取 "name" 字段的值
	QString name = jsonObject["city"].toString();
	int order1 = jsonObject.value("order1").toInt();
	qDebug() << "order1:" << order1;

	return 0;
}


int main(int argc, char* argv[])
{
	
	if (argc < 2) {
		qInfo() << "No extra arguments.\m";
		return -1;
	}
	std::cout << "run begin\n";

	NXinterface nx;

	read_json(QString::fromLatin1(argv[1]) );

	

	std::cout << "run over\n";
	return 0;
}
