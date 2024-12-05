//#define XCS_GOOD_LUCK
#ifdef  XCS_GOOD_LUCK

#include <uf_defs.h>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/BasePart.hxx>
#include <NXOpen/Expression.hxx>
#include <NXOpen/ExpressionCollection.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/PartLoadStatus.hxx>
#include <NXOpen/PartSaveStatus.hxx>
#include <NXOpen/Preferences_SessionModeling.hxx>
#include <NXOpen/Preferences_SessionPreferences.hxx>
#include <NXOpen/Unit.hxx>
#include <NXOpen/UnitCollection.hxx>
#include <NXOpen/Update.hxx>
#include <NXOpen/NXString.hxx>
#include <iostream>
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

//#include "../618/core/hy_context.h"

 //传入模型文件、参数名、参数值

 int nx_test()
 {	
 	// 初始化
 	NXOpen::Session* session = NXOpen::Session::GetSession();
 
 	// 导入模型
 	NXOpen::BasePart* base_part;
 	NXOpen::PartLoadStatus* status;
 	try {//打开*.prt文件
 		base_part = session->Parts()->OpenBaseDisplay("../tmp/model2.prt", &status);
 	}
 	catch (NXOpen::NXException& e) {
 		std::cout << e.Message() << std::endl;
 		return -1;
 	}
 	NXOpen::Part* work_part = session->Parts()->Work();
 	delete status;
 	std::cout << "Open model successfully." << std::endl;
 
 	//更新参数
 	NXOpen::Expression* expression(dynamic_cast<NXOpen::Expression*>(work_part->Expressions()->FindObject("H1")));
 	work_part->Expressions()->Edit(expression, "2");
 
 	for (auto p = work_part->Expressions()->begin(); p != work_part->Expressions()->end(); p++) {
 		std::cout << (*p)->Name().GetText() << ", ";
 		std::cout << (*p)->Equation().GetLocaleText() << ", ";
 		std::cout << (*p)->Type().GetLocaleText() << std::endl;
 	}
 
 	std::cout << "Updated model successfully." << std::endl;
 
 	//保存
 	NXOpen::PartSaveStatus* save_status;
 	save_status = work_part->Save(NXOpen::BasePart::SaveComponentsTrue, NXOpen::BasePart::CloseAfterSaveFalse);
 
 	//检查保存状态
 	if (int unsaved = save_status->NumberUnsavedParts()) {
 		std::cout << unsaved << " part(s) cannot be saved." << std::endl;
 	}
 	std::cout << "All parts saved successfully." << std::endl;
 	delete save_status;
 
 	return 0;
 }

int UgRun(const ParamSet& param_set,int times,QString save_file_name,QString &error_message)
{
	if (times > (int)(param_set.InputParamsCount() / param_set.GetInputParamSize()))
	{
		qDebug() << "number of runs is over range.";
		return -1;
	}
	try {//打开*.prt文件
		// 初始化
		NXOpen::Session* session = NXOpen::Session::GetSession();
		//关闭原有Parts
		auto* parts = session->Parts();
		NXOpen::PartCloseResponses* response{ nullptr };
		parts->CloseAll(NXOpen::BasePart::CloseModifiedUseResponses, response);
		// 导入模型
		NXOpen::PartLoadStatus* status{ nullptr };
		auto file_name = param_set.GetModelName();
		qDebug() << file_name;
		NXOpen::BasePart* base_part = parts->OpenBaseDisplay(file_name.toLocal8Bit().constData(), &status);
		NXOpen::Part* work_part = parts->Work();
			
		//TODO 判定status	
		//qInfo() << "Open model successfully.";
		delete status;

		
		//更新参数
		QString error_msg;
		bool is_success = true;

		int param_size = (int)param_set.GetInputParamSize();
		int param_index = times * param_size;

		for (size_t i = param_index; i < (param_index+param_size); i++)
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
			catch (NXOpen::NXException& e)
			{
				qDebug() << e.what();
				error_msg += QString(u8"更新操作失败，请检查值是否在有效范围内，变量名：") + QString(name.c_str());
				is_success = false;
			}
			
			//判断参数是否更新成功
// 			int n_errs = 0;
// 			try
// 			{
// 				NXOpen::Session::UndoMarkId markId5 = session->SetUndoMark(NXOpen::Session::MarkVisibilityInvisible, "Update Expression Data");
// 				n_errs = session->UpdateManager()->DoUpdate(markId5);
// 			}
// 			catch (NXOpen::NXException& e)
// 			{
// 				qDebug() << e.what();
// 				error_msg += QString(u8"更新操作失败，变量名：") + QString(name.c_str());
// 				is_success = false;
// 			}
		}
		if (!is_success)
		{
			qInfo() << error_msg;
			error_message = error_msg;
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
			NXOpen::Expression* expression = work_part->Expressions()->FindObject(name.c_str());

			qInfo() << expression->Equation().GetLocaleText() << 
				" " << expression->Type().GetLocaleText() 
				<< " " 
				<< expression->Units()->Name().getLocaleText();
		}
// 		for (auto p = work_part->Expressions()->begin(); p != work_part->Expressions()->end(); p++)
// 		{ 			
// 			qInfo() << (*p)->Equation().GetLocaleText() << " " << (*p)->Type().GetLocaleText();
// 		}
		
		qInfo() << "Updated model successfully.";

		//保存
		NXOpen::PartSaveStatus* save_status;
		save_status = work_part->Save(NXOpen::BasePart::SaveComponentsTrue, NXOpen::BasePart::CloseAfterSaveFalse);

		//检查保存状态
		if (int unsaved = save_status->NumberUnsavedParts())
		{
			qWarning() << unsaved << " part(s) cannot be saved.";
// 			auto tp = save_status->GetPart(unsaved-1);
// 			QString fp = tp->FullPath().GetText();
// 			qWarning() << fp;
		}
		else
		{
			qInfo() << "All parts saved successfully.";
		}
		delete save_status;
		
		//save prt file for workbench
		//file_name = QCoreApplication::applicationDirPath() + "/ug_temp.prt";
		//qDebug() << QDir::currentPath();
		//file_name = "../tmp/ug_temp.prt";
		//file_name = "D:/03_code/618/tmp/ug_temp.prt";
		QFile temp_ug_file(save_file_name);
		if (temp_ug_file.exists())
		{
			temp_ug_file.remove();
		}
		std::string tmp = save_file_name.toUtf8().constData();
		
		work_part->SaveAs(tmp.c_str());
		qInfo() << "prt file saved successfully.";
	}
	catch (NXOpen::NXException& e)
	{
		qWarning() << e.what();
		error_message = e.what();
		return -1;
	}
	return 0;
}


int GetUgUnitType(const ParamSet& param_set, QString& error_message)
{
	try
	{
		NXOpen::Session* session = NXOpen::Session::GetSession();//获取对象
		auto* parts = session->Parts();
		NXOpen::PartCloseResponses* response{ nullptr };
		parts->CloseAll(NXOpen::BasePart::CloseModifiedUseResponses, response);//关闭已打开的部件
		// 导入模型
		NXOpen::PartLoadStatus* status{ nullptr };
		auto file_name = param_set.GetModelName();
		NXOpen::BasePart* base_part = parts->OpenBaseDisplay(file_name.toLocal8Bit().constData(), &status);//打开部件

		NXOpen::Part* work_part = parts->Work();
		delete status;
		//获取参数  
		int parmsize = (int)param_set.GetInputParamSize();
		//更新参数
		QString error_msg;
		bool is_success = true;
		int all_param_size = param_set.InputParamsCount();
		//int param_size = (int)param_set.GetInputParamSize();
		//int param_index = times * param_size;
		for (size_t i = 0; i < all_param_size; ++i)
		{
			QString param_name = param_set.GetInputParam(i, PARAM_NAME);
			QString param_value = param_set.GetInputParam(i, PARAM_VALUE);
			std::string name = param_name.toUtf8().constData();
			std::string value = param_value.toUtf8().constData();
			//回填参数
			try {
				NXOpen::Expression* expression = work_part->Expressions()->FindObject(name.c_str());  //异常处理，如果存在不在模型参数集的参数
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
				error_msg += QString(u8"参数设置错误,参数名：") + QString(name.c_str());
				is_success = false;
			}
		}
		if (!is_success)
		{
			qInfo() << error_msg;
            error_message = error_msg;
			return -1;
		}
	}
	catch (NXOpen::NXException& e)
	{
		qWarning() << e.what();
		return -1;
	}
	return 0;
}

QStringList SelectUgParams(const QString& file_name)
{
    QStringList param_list;
    try
    {
        NXOpen::Session* session = NXOpen::Session::GetSession();//获取对象
        auto* parts = session->Parts();
        NXOpen::PartCloseResponses* response{ nullptr };
        parts->CloseAll(NXOpen::BasePart::CloseModifiedUseResponses, response);//关闭已打开的部件
        // 导入模型
        NXOpen::PartLoadStatus* status{ nullptr };
        NXOpen::BasePart* base_part = parts->OpenBaseDisplay(file_name.toLocal8Bit().constData(), &status);//打开部件

        NXOpen::Part* work_part = parts->Work();
        //关闭
        delete status;

        NXOpen::ExpressionCollection* expressionCollection = work_part->Expressions();
        
        for (auto it= expressionCollection->begin(); it != expressionCollection->end(); ++it)
        {
            NXOpen::Expression* expression = dynamic_cast<NXOpen::Expression*>(*it);
            QString tmp = expression->Name().GetLocaleText();
            //过滤p开头+后第一位跟数字
            QRegularExpression regex("^p\\d");
            QRegularExpressionMatch filter = regex.match(tmp);
            if (filter.hasMatch()){
                continue;
            }
            else {
                param_list.append(tmp);
            }
        }
    }
    catch (NXOpen::NXException& e)
    {
        qWarning() << e.what();
        return param_list;
    }
    return param_list;
}

#endif