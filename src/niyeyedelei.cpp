#include "niyeyedelei.h"




int NXinterface::nx_test() {

    const char   dst_ptrfile[] = "D:\\env_tr\\u\\trptr\\tanhuangguan-shiyanmoxing.prt";

    // 初始化
    NXOpen::Session* session = NXOpen::Session::GetSession();

    // 导入模型
    NXOpen::BasePart* base_part;
    NXOpen::PartLoadStatus* status;

    try {       //打开*.prt文件
        base_part = session->Parts()->OpenBaseDisplay(dst_ptrfile, &status);
    }
    catch (NXOpen::NXException& e) {
        std::cout << e.Message() << std::endl;
        std::cout << "Open model successfully.\n";
        return -1;
    }



    NXOpen::Part* work_part = session->Parts()->Work();
    delete status;
    std::cout << "Open model successfully." << std::endl;

    //NXOpen::Expression* expression(dynamic_cast<NXOpen::Expression*>(work_part->Expressions()->FindObject("H1")));
    //work_part->Expressions()->Edit(expression, "2");

    NXOpen::Expression* expression(dynamic_cast<NXOpen::Expression*>(work_part->Expressions()->FindObject("p2'10")));
    work_part->Expressions()->Edit(expression, "3.922");

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


int NXinterface::UgRun(const ParamSet& param_set, int times, QString save_file_name, QString& error_message)
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
		qDebug()  << file_name;
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
			catch (NXOpen::NXException& e) {
				qDebug() << e.what();
				error_msg += QString(u8"更新操作失败，请检查值是否在有效范围内，变量名：") + QString(name.c_str());
				is_success = false;
			}

			if (!is_success) {
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

				//name = "p62'10";				/* 模拟ParamSet */
				//value = "-0.5";			/* 模拟ParamSet */

				NXOpen::Expression* expression = work_part->Expressions()->FindObject(name.c_str());



				//qInfo() << expression->Equation().GetLocaleText() << " " << expression->Type().GetLocaleText() <<" ";


				if (expression->Units() == nullptr) {
					qInfo() << expression->Equation().GetLocaleText() << " "
						<< expression->Type().GetLocaleText() << " "
						<< "No Units \n";
				}
				else {
					qInfo() << expression->Equation().GetLocaleText() << " "
						<< expression->Type().GetLocaleText() << " "
						<< expression->Units()->Name().getLocaleText() << "\n";
				}


				/*qInfo() << expression->Equation().GetLocaleText() << " " << expression->Type().GetLocaleText()
					<< " " << expression->Units()->Name().getLocaleText();*/
			}
			qInfo() << "Updated model successfully.";


			NXOpen::PartSaveStatus* save_status;
			save_status = work_part->Save(NXOpen::BasePart::SaveComponentsTrue, NXOpen::BasePart::CloseAfterSaveFalse);
			if (int unsaved = save_status->NumberUnsavedParts()) {
				qWarning() << unsaved << " part(s) cannot be saved.";
				// 			auto tp = save_status->GetPart(unsaved-1);
				// 			QString fp = tp->FullPath().GetText();
				// 			qWarning() << fp;


			}
			else {
				qInfo() << "All parts saved successfully.";
			}
			delete save_status;


			QFile temp_ug_file(save_file_name);
			if (temp_ug_file.exists()) {
				temp_ug_file.remove();
			}
			std::string tmp = save_file_name.toUtf8().constData();

			work_part->SaveAs(tmp.c_str());
			qInfo() << "prt file saved successfully.";

		}
	}
	catch (NXOpen::NXException& e)
	{
		
		qWarning() << e.what();
		error_message = e.what();
		return -1;
	}
	return 0;

}

//获取全部表达式，不加过滤
QStringList NXinterface::GetExpression(QString file_name) {
	QStringList Expression_list;
	try {
		NXOpen::Session* session = NXOpen::Session::GetSession();//获取对象
		auto* parts = session->Parts();
		NXOpen::PartCloseResponses* response{ nullptr };
		parts->CloseAll(NXOpen::BasePart::CloseModifiedUseResponses, response);//关闭已打开的部件

		NXOpen::PartLoadStatus* status{ nullptr };
		NXOpen::BasePart* base_part = parts->OpenBaseDisplay(file_name.toLocal8Bit().constData(), &status);//打开部件
		NXOpen::Part* work_part = parts->Work();

		delete status;
		NXOpen::ExpressionCollection* expressionCollection = work_part->Expressions();
		for (auto it = expressionCollection->begin(); it != expressionCollection->end(); it++) {
			NXOpen::Expression* expression = dynamic_cast<NXOpen::Expression*>(*it);
			QString tmp = expression->Name().GetLocaleText();
			tmp += "\t";
			tmp += expression->Equation().GetLocaleText();
			tmp += "\t";
			if (expression->Units())
				tmp += expression->Units()->Name().GetLocaleText();

			Expression_list.append(tmp);

		}

	}
	catch(NXOpen::NXException& e){ 
		qWarning() << e.what();
		return Expression_list;
	}


	return Expression_list;
}

int NXinterface::GetUgUnitType(const ParamSet& param_set, QString& error_message)
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

		qInfo() << file_name;

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

QStringList NXinterface::SelectUgParams(const QString& file_name)
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

		for (auto it = expressionCollection->begin(); it != expressionCollection->end(); ++it)
		{
			NXOpen::Expression* expression = dynamic_cast<NXOpen::Expression*>(*it);
			QString tmp = expression->Name().GetLocaleText();
			//过滤p开头+后第一位跟数字
			QRegularExpression regex("^p\\d");
			QRegularExpressionMatch filter = regex.match(tmp);
			//qInfo() << tmp;
			if (filter.hasMatch()) {
				//param_list.append(tmp);
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


QStringList NXinterface::SelectUgParams(const QString& file_name, const QString& filter_RegularExpression)
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

		for (auto it = expressionCollection->begin(); it != expressionCollection->end(); ++it)
		{
			NXOpen::Expression* expression = dynamic_cast<NXOpen::Expression*>(*it);
			QString tmp = expression->Name().GetLocaleText();
			//过滤p开头+后第一位跟数字
			//QRegularExpression regex("^p\\d");
			QRegularExpression regex(filter_RegularExpression);

			QRegularExpressionMatch filter = regex.match(tmp);
			//qInfo() << tmp;
			if (filter.hasMatch()) {
				//param_list.append(tmp);
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

int NXinterface::wirte_single(const QString& file_name, const QString& obejecName, const QString& rightHandSide) {

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
		NXOpen::Expression* expression = work_part->Expressions()->FindObject( NXOpen::NXString( obejecName.toStdString() ) );
		expression->SetRightHandSide( NXOpen::NXString("p219=k1"));
		//work_part->Expressions()->Edit(expression, "3.9");


		NXOpen::PartSaveStatus* save_status;
		save_status = work_part->Save(NXOpen::BasePart::SaveComponentsTrue, NXOpen::BasePart::CloseAfterSaveFalse);
		if (int unsaved = save_status->NumberUnsavedParts()) {
			qWarning() << unsaved << " part(s) cannot be saved.";
	
		}
		else {
			qInfo() << "All parts saved successfully.";
		}
		delete save_status;


		QFile temp_ug_file("D:\\env_tr\\u\\trptr\\C_moog_qianzhiji_cpa.prt");
		if (temp_ug_file.exists()) {
			temp_ug_file.remove();
		}
		std::string tmp = file_name.toUtf8().constData();
		//tmp = "D:\\env_tr\\u\\trptr\\C_moog_qianzhiji_cp.prt";
		work_part->SaveAs("D:\\env_tr\\u\\trptr\\C_moog_qianzhiji_cpa.prt" );
		qInfo() << "prt file saved successfully.";


	}
	catch (NXOpen::NXException& e) {
		qDebug() << obejecName << e.what();

		
	}
		


	return 0;
}

int NXinterface::writeExpressions(const QString ptrFilePath, const QStringList& expressionList,const QString saveFilePath) {
	

	{
		NXOpen::Session* session = NXOpen::Session::GetSession();//获取对象
		auto* parts = session->Parts();
		NXOpen::PartCloseResponses* response{ nullptr };
		parts->CloseAll(NXOpen::BasePart::CloseModifiedUseResponses, response);//关闭已打开的部件
		// 导入模型
		NXOpen::PartLoadStatus* status{ nullptr };
		NXOpen::BasePart* base_part = parts->OpenBaseDisplay(ptrFilePath.toLocal8Bit().constData(), &status);//打开部件

		NXOpen::Part* work_part = parts->Work();
		
		delete status;//关闭

		NXOpen::ExpressionCollection* expressionCollection = work_part->Expressions();

		for (auto item:expressionList) {
			try {
				QStringList namevalue = item.split('\t');
				NXOpen::Expression* expression = work_part->Expressions()->FindObject(namevalue[0].toStdString().c_str());
				expression->SetRightHandSide(namevalue[1].toStdString().c_str());
				qInfo() << "write" << namevalue[0] << namevalue[1];
			}
			catch (NXOpen::NXException& e) {
				qDebug() << e.what();
				//error_msg += QString(u8"更新操作失败，请检查值是否在有效范围内，变量名：") + QString(name.c_str());
				//is_success = false;
			}

		}
		NXOpen::PartSaveStatus* save_status;
		save_status = work_part->Save(NXOpen::BasePart::SaveComponentsTrue, NXOpen::BasePart::CloseAfterSaveFalse);
		if (int unsaved = save_status->NumberUnsavedParts()) {
			qWarning() << unsaved << " part(s) cannot be saved.";
		}
		else {
			qInfo() << "All parts saved successfully.";
		}
		delete save_status;

		QString savef ;
		savef = QDir::toNativeSeparators(saveFilePath);
		QFile temp_ug_file(savef);

		if (temp_ug_file.exists()) {
			temp_ug_file.remove();
		}
		std::string tmp = savef.toStdString();
		//savef = QDir::toNativeSeparators(savef);
		work_part->SaveAs(savef.toStdString());

		//work_part->SaveAs("D:\\env_tr\\u\\trptr\\C_moog_qianzhiji_cpaa.prt");
		qInfo() << "prt file saved successfully.";

	}
	return 0;
}