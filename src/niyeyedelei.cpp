#include "niyeyedelei.h"




void NXinterface::Open_wr() {
    using namespace NXOpen;



    Session* theSession = Session::GetSession();
    // ----------------------------------------------
    //   Menu: File->Open...
    // ----------------------------------------------
    BasePart* basePart1;
    PartLoadStatus* partLoadStatus1;
    basePart1 = theSession->Parts()->OpenBaseDisplay("D:\\env_tr\\u\\trptr\\tanhuangguan-shiyanmoxing.prt", &partLoadStatus1);

    Part* workPart(theSession->Parts()->Work());
    Part* displayPart(theSession->Parts()->Display());
    delete partLoadStatus1;


    Expression* expression1(dynamic_cast<Expression*>(workPart->Expressions()->FindObject("p2'10")));
    Unit* nUnit(NULL);
    workPart->Expressions()->EditWithUnits(expression1, nUnit, "3.933");

    // ----------------------------------------------
    //   Menu: File->Save
    // ----------------------------------------------
    PartSaveStatus* partSaveStatus1;
    partSaveStatus1 = workPart->Save(BasePart::SaveComponentsTrue, BasePart::CloseAfterSaveFalse);

    delete partSaveStatus1;
    // ----------------------------------------------
    //   Menu: Tools->Journal->Stop Recording
    // ----------------------------------------------

}

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


