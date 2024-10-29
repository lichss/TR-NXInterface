
#include <iostream>
#include "niyeyedelei.h"
//#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QtWidgets/QApplication>


#include "dummy_paramset.h"




int UgRun(const ParamSet& param_set, int times, QString save_file_name, QString& error_message)
{
    if(0)   // (times > (int)(param_set.InputParamsCount() / param_set.GetInputParamSize()))
    {
        qDebug() << "number of runs is over range.";
        return -1;
    }

	try {//��*.prt�ļ�
		// ��ʼ��
		NXOpen::Session* session = NXOpen::Session::GetSession();
		//�ر�ԭ��Parts
		auto* parts = session->Parts();
		NXOpen::PartCloseResponses* response{ nullptr };
		parts->CloseAll(NXOpen::BasePart::CloseModifiedUseResponses, response);
		// ����ģ��
		NXOpen::PartLoadStatus* status{ nullptr };
		auto file_name = param_set.GetModelName();
		qDebug() << file_name;
		NXOpen::BasePart* base_part = parts->OpenBaseDisplay(file_name.toLocal8Bit().constData(), &status);
		NXOpen::Part* work_part = parts->Work();

		//TODO �ж�status	
		//qInfo() << "Open model successfully.";
		delete status;

		std::cout << "gengxin canshu" << '\n';


		//���²���
		QString error_msg;
		bool is_success = true;

		int param_size = (int)param_set.GetInputParamSize();
		int param_index = times * param_size;


		param_size = 1;
		param_index = 2;

		for (size_t i = param_index; i < (param_index + param_size); i++)
		{
			QString param_name = param_set.GetInputParam(i, PARAM_NAME);
			QString param_value = param_set.GetInputParam(i, PARAM_VALUE);
			//NXOpen::Expression* expression{ nullptr };
			//auto name_tmp = param_name.toLocal8Bit().constData();
			std::string name = param_name.toUtf8().constData();//
			std::string value = param_value.toUtf8().constData();

			name = "p2'10";				/* ģ��ParamSet */
			value = "3.939";			/* ģ��ParamSet */
			std::cout << name << '\n';	/* ģ��ParamSet */
			std::cout << value << '\n';	/* ģ��ParamSet */


			try {
				NXOpen::Expression* expression = work_part->Expressions()->FindObject(name.c_str());
				//work_part->Expressions()->Edit(expression, value.c_str());
				expression->SetRightHandSide(value.c_str());
			}
			catch (NXOpen::NXException& e){
				qDebug() << e.what();
				error_msg += QString(u8"���²���ʧ�ܣ�����ֵ�Ƿ�����Ч��Χ�ڣ���������") + QString(name.c_str());
				is_success = false;
			}

			if (!is_success){
				qInfo() << error_msg;
				error_message = error_msg;
				std::cout << error_msg.toUtf8().constData();;  /* ���Լӵ� ɾ�� */

				return -1;
			}


			/* ������һ��δ���ע�� ��ɾ��*/


			/* ���湦�� ԭ����û�е� Ϊ�˲��Լӽ��� */
			NXOpen::PartSaveStatus* partSaveStatus1;
			partSaveStatus1 = work_part->Save(NXOpen::BasePart::SaveComponentsTrue, NXOpen::BasePart::CloseAfterSaveFalse);
			delete partSaveStatus1;
		}
	}
	catch (NXOpen::NXException& e)
	{
		std::cout << "error11\n";  /* ���Լӵ� ɾ�� */
		qWarning() << e.what();
		error_message = e.what();
		return -1;
	}


}


int main(int argc, char *argv[])
{
    std::cout << "run begin\n";

	{// ug_run test
		QString save_file_name = "filename";
		QString error_message = "error msg";
		ParamSet paramset;
		paramset.m_modelName = "D:\\env_tr\\u\\trptr\\tanhuangguan-shiyanmoxing.prt";

		UgRun(paramset, 2,save_file_name, error_message);
	}

    std::cout << "run over\n";

    return 0;
}
