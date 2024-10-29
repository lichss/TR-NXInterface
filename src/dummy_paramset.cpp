#include "dummy_paramset.h"


//class ParamSet {
//public:
//	QString m_modelName;
//	int m_input_param_size = 0;
//
//	QString GetModelName() const { return m_modelName; }
//	QString GetInputParam(size_t index, size_t paramIndex) const;
//
//	size_t GetInputParamSize() const { return m_input_param_size; };
//
//};

QString ParamSet::GetInputParam(size_t index, size_t paramIndex) const
{
	//Q_ASSERT(m_inputParams.size() > 0);
	//if (index >= m_inputParams.size())
	//{
	//	return "nan";
	//}
	switch (paramIndex)
	{
		//case 0: return m_inputParams[index].m_displayName;
	//case 0: return m_inputParams[index].m_name;
	case 0: return QString{ "replace with m_name" };
	//case 1: return m_inputParams[index].m_value;
	case 1: return QString{ "replace with  m_value" };

	//case 2: return m_inputParams[index].m_unit;
	case 2: return QString{ "replace with  m_unit" };

	//case 3: return m_inputParams[index].m_type;
	case 3: return QString{ "replace with  m_type" };

	default: return "";
	}



}