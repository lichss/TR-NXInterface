
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

/*
TODO

SelectUgParams 过滤现在是固定的 把他参数化一下，引出来
做个图形化界面
把加载模型的参数都显出来


*/

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
		
	std::cout << "run begin\n";

	NXinterface nx;
	QString ptrPath;
	
	QString name = "p219";
	if (0)
		 ptrPath = "D:\\env_tr\\u\\trptr\\tanhuangguan-shiyanmoxing.prt";
	else
		 ptrPath = "D:\\env_tr\\u\\trptr\\C_moog_qianzhiji_cp.prt";

	//C_moog_qianzhiji.prt
	//QStringList qls = nx.SelectUgParams(ptrPath,"^p\\d");
	if(0){
		QStringList qls = nx.GetExpression(ptrPath);
		for (auto item : qls)
			std::cout << item.toStdString() << "\n";
		printf("%d parameter in all.\n", qls.size());
	}
	if (1) {
		QStringList qls;
		QStringList qls2;
		QString savef = "D:/env_tr/u/trptr/sss";
		qls.append("p220\t0.2");
		qls.append("p243\th1");
		qls.append("p246\tk1");
		qls.append("p265\t0.5");

		nx.writeExpressions(ptrPath,qls, savef);


	}
	

	//nx.wirte_single(ptrPath, name,righthands);

	//nx.UgRun()

	std::cout << "run over\n";
	return 0;
}
