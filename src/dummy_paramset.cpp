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
	case 0: return QString{ "p62'10" };
	//case 1: return m_inputParams[index].m_value;
	case 1: return QString{ "-0.5" };

	//case 2: return m_inputParams[index].m_unit;
	case 2: return QString{ "replace with  m_unit" };

	//case 3: return m_inputParams[index].m_type;
	case 3: return QString{ "replace with  m_type" };

	default: return "";
	}



}
// "p62'10";

void ParamSet::SetInputParam(size_t index, size_t param_index, QString value)
{
	////TODO 中间数据为空如何处理 有效性检查 添加确定按钮？
	//if (index >= m_inputParams.size())
	//{
	//	for (int i = m_inputParams.size(); i <= index; i++)
	//	{
	//		m_inputParams.append(Param());
	//	}
	//}
	//switch (param_index)
	//{
	//	//case 0: m_inputParams[index].m_displayName = value; break;
	//case 0: m_inputParams[index].m_name = value; break;
	//case 1: m_inputParams[index].m_value = value; break;
	//case 2: m_inputParams[index].m_unit = value; break;
	//case 3: m_inputParams[index].m_type = value; break;
	//default: break;
	//}

	return;


}