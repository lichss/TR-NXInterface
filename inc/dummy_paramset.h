#ifndef __DUMMY_PARAMSET_H__
#define __DUMMY_PARAMSET_H__

#include "headers.h"

#include <QFile>
#include <QDebug>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QtWidgets/QApplication>


#define PARAM_VALUE 1
#define PARAM_NAME 0


//这个类没有任何实际作用
//只是为了进行测试 

class ParamSet {
public:
	QString m_modelName;
	int m_input_param_size = 1;


	void SetInputParam(size_t index, size_t param_index, QString value);

	QString GetModelName() const { return m_modelName; }
	QString GetInputParam(size_t index, size_t paramIndex) const;

	size_t GetInputParamSize() const { return m_input_param_size; };
	size_t InputParamsCount() const { return 1; }
};

#endif