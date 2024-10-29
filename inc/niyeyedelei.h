#ifndef __NIYEYEDELEI_H__
#define __NIYEYEDELEI_H__



#include <iostream>
#include <uf_defs.h>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/BasePart.hxx>
#include <NXOpen/Builder.hxx>
#include <NXOpen/Expression.hxx>
#include <NXOpen/ExpressionCollection.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/PartLoadStatus.hxx>
#include <NXOpen/PartSaveStatus.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Update.hxx>


#define NX_EXPORT_D _declspec(dllexport)



class NX_EXPORT_D NXinterface
{
public:

	void Open_wr();
	int nx_test();
private:

};


#endif // !__NIYEYEDELEI_H__