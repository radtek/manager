#ifndef SESSION_H
#define SESSION_H

#include "mylibrary_global.h"

class MYLIBRARYSHARED_EXPORT session
{

private:

    QString md_code;
	QString md_name;
	QString md_nick_name;
	QString md_permissions;

public:

    session();
    session(QString code, QString name, QString nick_name, QString permissions);

    void mf_set_code(QString code);
    void mf_set_name(QString name);
    void mf_set_nick_name(QString nick_name);
    void mf_set_permissions(QString permissions);

	QString mf_get_code();
	QString mf_get_name();
	QString mf_get_nick_name();
	QString mf_get_permissions();





    ~session(){}
};

#endif // SESSION_H
