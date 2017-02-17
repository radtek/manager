#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include "mylibrary_global.h"

#define DB_FILE_NAME_EXTENSION ".cfg"

class MYLIBRARYSHARED_EXPORT database_connection
{

private:

  QString md_driver;

  QString md_host;
  QString md_port;

  QString md_user;
  QString md_pass;
  QString md_name;
  QString md_options;

public:
  database_connection();
  ~database_connection();

  void mf_set_driver(QString driver);
  void mf_set_host(QString host);
  void mf_set_port(QString port);
  void mf_set_user(QString user);
  void mf_set_pass(QString pass);
  void mf_set_name(QString name);
  void mf_set_options(QString options);

  QString mf_get_driver();
  QString mf_get_host();
  QString mf_get_port();
  QString mf_get_user();
  QString mf_get_pass();
  QString mf_get_name();
  QString mf_get_options();

  //void mf_set_configuration(QString host, QString db_name, QString user, QString pass);
  void mf_setConfig();
  void mf_setConfig(QString host, QString port, QString user, QString pass, QString name);
  void mf_load_configuration(QString file_config);

  bool mf_connect();
  //bool mf_connect(QString host, QString db_name, QString user, QString pass);


};

template class MYLIBRARYSHARED_EXPORT singleton<database_connection>;

#endif // DATABASE_CONNECTION_H
