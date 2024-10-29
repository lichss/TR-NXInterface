#ifdef XCS__GOOD_LUCK


#include <string>
#include<QStringList>
class ParamSet;

int nx_test();
int UgRun(const ParamSet& param_set,int times, QString save_file_name,QString &error_massage);
int GetUgUnitType(const ParamSet& param_set, QString& error_message);
QStringList SelectUgParams(const QString& file_name);

#endif