
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



#if 0
int UgRun(const ParamSet& param_set, int times, QString save_file_name, QString& error_message)
{
    if (times > (int)(param_set.InputParamsCount() / param_set.GetInputParamSize()))
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

		//���²���
		QString error_msg;
		bool is_success = true;

		int param_size = (int)param_set.GetInputParamSize();
		int param_index = times * param_size;

		//std::cout <<"param_size: "<< param_size << '\n';	/* ģ��ParamSet */
		//std::cout <<"param_index: " << param_index << '\n';	/* ģ��ParamSet */

		for (size_t i = param_index; i < (param_index + param_size); i++)
		{
			QString param_name = param_set.GetInputParam(i, PARAM_NAME);
			QString param_value = param_set.GetInputParam(i, PARAM_VALUE);
			//NXOpen::Expression* expression{ nullptr };
			//auto name_tmp = param_name.toLocal8Bit().constData();
			std::string name = param_name.toUtf8().constData();//
			std::string value = param_value.toUtf8().constData();

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
				//std::cout << error_msg.toUtf8().constData();;  /* ���Լӵ� ɾ�� */

				return -1;
			}

			for (size_t i = param_index; i < (param_index + param_size); i++)
			{
				QString param_name = param_set.GetInputParam(i, PARAM_NAME);
				QString param_value = param_set.GetInputParam(i, PARAM_VALUE);
				//NXOpen::Expression* expression{ nullptr };
				//auto name_tmp = param_name.toLocal8Bit().constData();
				std::string name = param_name.toUtf8().constData();//
				std::string value = param_value.toUtf8().constData();

				//name = "p62'10";				/* ģ��ParamSet */
				//value = "-0.5";			/* ģ��ParamSet */

				NXOpen::Expression* expression = work_part->Expressions()->FindObject(name.c_str());


				
				//qInfo() << expression->Equation().GetLocaleText() << " " << expression->Type().GetLocaleText() <<" ";


				if (expression->Units() == nullptr){
					qInfo() << expression->Equation().GetLocaleText() << " " 
						<< expression->Type().GetLocaleText() << " "
						<< "No Units \n";
				}
				else {
					qInfo() << expression->Equation().GetLocaleText() << " "
						<< expression->Type().GetLocaleText() << " " 
						<< expression->Units()->Name().getLocaleText() << "\n";
				}

				
				/*std::cout << expression->Equation().GetLocaleText() << " " << expression->Type().GetLocaleText()
					<< " " << expression->Units()->Name().getLocaleText();*/
			}
			qInfo() << "Updated model successfully.";

			
			NXOpen::PartSaveStatus* save_status;
			save_status = work_part->Save(NXOpen::BasePart::SaveComponentsTrue, NXOpen::BasePart::CloseAfterSaveFalse);
			if (int unsaved = save_status->NumberUnsavedParts()){
				qWarning() << unsaved << " part(s) cannot be saved.";
				// 			auto tp = save_status->GetPart(unsaved-1);
				// 			QString fp = tp->FullPath().GetText();
				// 			qWarning() << fp;


			}
			else{
				qInfo() << "All parts saved successfully.";
			}
			delete save_status;


			QFile temp_ug_file(save_file_name);
			if (temp_ug_file.exists()){
				temp_ug_file.remove();
			}
			std::string tmp = save_file_name.toUtf8().constData();

			work_part->SaveAs(tmp.c_str());
			qInfo() << "prt file saved successfully.";

		}
	}
	catch (NXOpen::NXException& e)
	{
		std::cout << "error11\n";  /* ���Լӵ� ɾ�� */
		qWarning() << e.what();
		error_message = e.what();
		return -1;
	}
	return 0;

}
#endif

int GetUgUnitType(const ParamSet& param_set, QString& error_message)
{
	try
	{
		NXOpen::Session* session = NXOpen::Session::GetSession();//��ȡ����
		auto* parts = session->Parts();
		NXOpen::PartCloseResponses* response{ nullptr };
		parts->CloseAll(NXOpen::BasePart::CloseModifiedUseResponses, response);//�ر��Ѵ򿪵Ĳ���
		// ����ģ��
		NXOpen::PartLoadStatus* status{ nullptr };
		auto file_name = param_set.GetModelName();
		NXOpen::BasePart* base_part = parts->OpenBaseDisplay(file_name.toLocal8Bit().constData(), &status);//�򿪲���

		qInfo() << "open:" << file_name;

		NXOpen::Part* work_part = parts->Work();
		delete status;
		//��ȡ����  
		int parmsize = (int)param_set.GetInputParamSize();
		//���²���
		QString error_msg;
		bool is_success = true;
		int all_param_size = param_set.InputParamsCount();
		//int param_size = (int)param_set.GetInputParamSize();
		//int param_index = times * param_size;

		#if 0
		for (size_t i = 0; i < all_param_size; ++i)
		{
			QString param_name = param_set.GetInputParam(i, PARAM_NAME);
			QString param_value = param_set.GetInputParam(i, PARAM_VALUE);
			std::string name = param_name.toUtf8().constData();
			std::string value = param_value.toUtf8().constData();
			//�������
			try {
				NXOpen::Expression* expression = work_part->Expressions()->FindObject(name.c_str());  //�쳣����������ڲ���ģ�Ͳ������Ĳ���
				const_cast<ParamSet&> (param_set).SetInputParam(i, 2, QString(expression->Units()->Name().getLocaleText()));
				const_cast<ParamSet&>(param_set).SetInputParam(i, 3, QString(expression->Type().GetLocaleText()));
			}
			catch (NXOpen::NXException& e) {
				qDebug() << param_name << e.what();
				if (QString(name.c_str()) == "nan") {
					continue;
				}
				if (error_msg.contains(QString(name.c_str()))) {
					continue;
				}
				error_msg += QString(u8"�������ô���,��������") + QString(name.c_str());
				is_success = false;
			}
		}
		if (!is_success)
		{
			qInfo() << error_msg;
			error_message = error_msg;
			return -1;
		}
		#endif
	}
	catch (NXOpen::NXException& e)
	{
		qWarning() << e.what();
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
    std::cout << "run begin\n";

	
	if (0)
	{// ug_run test
		QString save_file_name = "dummy_file.prt";
		QString error_message = "error msg";
		ParamSet paramset;
		paramset.m_modelName = "D:\\env_tr\\u\\trptr\\tanhuangguan-shiyanmoxing.prt";

		NXinterface nx;
		nx.UgRun(paramset, 1, save_file_name, error_message);

	}

	{
		QString save_file_name = "dummy_file.prt";
		QString error_message = "error msg";
		ParamSet paramset;
		paramset.m_modelName = "D:\\env_tr\\u\\trptr\\tanhuangguan-shiyanmoxing.prt";

		std::cout << "\nexcute return value:" << GetUgUnitType(paramset, error_message) << "\n";

	}

    std::cout << "run over\n";

    return 0;
}
