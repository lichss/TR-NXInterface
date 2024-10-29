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
