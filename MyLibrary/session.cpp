#include "session.h"

session::session()
{

}

session::session(QString code, QString name, QString nick_name, QString permissions)
{
    md_code = code;
    md_name = name;
    md_nick_name = nick_name;
    md_permissions = permissions;
}

void session::mf_set_code(QString code)
{
    md_code = code;
}

void session::mf_set_name(QString name)
{
    md_name = name;
}

void session::mf_set_nick_name(QString nick_name)
{
    md_nick_name = nick_name;
}

void session::mf_set_permissions(QString permissions)
{
    md_permissions = permissions;
}

QString session::mf_get_code()
{
    return md_code;
}

QString session::mf_get_name()
{
    return md_name;
}

QString session::mf_get_nick_name()
{
    return md_nick_name;
}

QString session::mf_get_permissions()
{
    return md_permissions;
}

