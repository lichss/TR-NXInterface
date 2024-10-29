#ifndef __DUMMY_PARAMSET_H__
#define __DUMMY_PARAMSET_H__

#include "headers.h"

class ParamSet {
public:
	QString GetModelName() const;
private:
	QString ModelName = "aaa";
};

#endif