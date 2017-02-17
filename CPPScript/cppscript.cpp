#include "cppscript.h"

CPPScript::CPPScript()
{
}

void CPPScript::setHost(QString host){ this->host = host; }

void CPPScript::setUser(QString user){ this->user = user; }

void CPPScript::setPass(QString pass){ this->pass = pass; }

void CPPScript::setDB(QString db){ this->db = db; }

void CPPScript::set_port(QString port){ this->port = port; }

void CPPScript::setDirFolder(QString str){ dirFolder = str + "\\"; }

QString CPPScript::getHost(){ return host; }

QString CPPScript::getUser(){ return user; }

QString CPPScript::getPass(){ return pass; }

QString CPPScript::getDB(){ return db; }

QString CPPScript::getDirFolder(){ return dirFolder; }

bool CPPScript::generar_clases()
{
    // Create folder
    //mkdir(getDirFolder().c_str());

    QSqlDatabase db = QSqlDatabase::database();

    foreach(QString e,db.tables())
    {
        qDebug()<<e<<endl;
        data.push_back(QPair<QString, QVector<QVector<QString> > >(e, QVector<QVector<QString> >()));
    }

    extractColumnsOfData(data);

    QVector<QString> column_names;


    for(unsigned int i = 0; i < data.size(); i++){
        if(openFile_h(data[i].first)){
            column_names.clear();
            for(int n= 0; n < data[i].second.size(); n++)
                column_names.push_back(data[i].second[n][0]);

            fillFile_h(data[i].first, column_names, i);
        }
    }

    for(unsigned int i = 0; i < data.size(); i++){
        if(openFile_cpp(data[i].first)){            
            column_names.clear();
            for(int n= 0; n < data[i].second.size(); n++){
                column_names.push_back(data[i].second[n][0]);

            }

            fillFile_cpp(data[i].first, column_names, i);
        }
    }
    generate_class_objectGeneral_h(data);

    return false;
}
bool CPPScript::extractColumnsOfData(QVector<QPair<QString, QVector<QVector<QString> > > > &data)
{
    for(unsigned i = 0; i < data.size(); i++)
    {
        QSqlQuery describe_table;

        QString query = "DESCRIBE "+data[i].first;
        describe_table.prepare(query);

        if(describe_table.exec())
        {
            qDebug()<<"query successful"<<endl;

            data[i].second.resize(describe_table.size());
            for(int n= 0; n< describe_table.size(); n++)
                data[i].second[n].resize(6);

            int n= 0;
            while(describe_table.next())
            {
                data[i].second[n][0]= describe_table.value(0).toString();
                data[i].second[n][1]= describe_table.value(1).toString();
                data[i].second[n][2]= describe_table.value(2).toString();
                data[i].second[n][3]= describe_table.value(3).toString();
                data[i].second[n][4]= describe_table.value(4).toString();
                data[i].second[n][5]= describe_table.value(5).toString();
                n++;
            }
        }
    }
    return true;
}
bool CPPScript::openFile_h(QString class_name)
{
    QString str_name_class = getDirFolder()+STR_OBJECT+class_name+".h";
    QFile file(str_name_class);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"File_h openned"<<endl;
        return true;
    }else{
        qDebug()<<"File_h not openned"<<endl;
        return false;
    }

}
bool CPPScript::openFile_cpp(QString class_name)
{
    QString str_name_class = getDirFolder()+STR_OBJECT+class_name+".cpp";
    QFile file(str_name_class);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"File_h openned"<<endl;
        return true;
    }else{
        qDebug()<<"File_h not openned"<<endl;
        return false;
    }
}
void CPPScript::fillFile_h(QString class_name, QVector<QString>& column_names, int i)
{
    qDebug()<<"fillFile_h class_name: "<<class_name<<endl;
    QString file_class_name = getDirFolder()+STR_OBJECT+class_name+".h";
    QFile file(file_class_name);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"File_h to fill openned"<<endl;
    }else{
        qDebug()<<"File_h to fill not openned"<<endl;
        return;
    }

    QString str_class_name_upper = class_name.toUpper();
    QString str_class_name = class_name;

    QString input;
    input= "#ifndef "+str_class_name_upper+"_H"+"\n";
    input+= "#define "+str_class_name_upper+"_H"+"\n\n";
    input+= QString()+"#include \"mylibrary_global.h\""+"\n";
    input+= QString()+"#include \""+STR_INCLUDE+"share_static_include.h"+"\""+"\n";
    input+= QString()+"#include \""+"general.h"+"\""+"\n\n";
    input+= "#define TABLE_NAME_"+str_class_name_upper+" \""+str_class_name+"\""+"\n\n";
    input+= QString()+"class "+"MYLIBRARYSHARED_EXPORT"+" "+STR_OBJECT+str_class_name+":public "+STR_OBJECT+"general"+"\n";
    input+= QString()+"{"+"\n\n";

    if(class_name== "item"){
        QSqlQuery query;

        QString str_query= "SELECT id, nombre FROM group_of_items";

        if(query.exec(str_query)){

            input+= "/*enum group_of_items{";

            query.next();
            QString id= query.value(0).toString();
            QString nombre= query.value(1).toString();
            nombre= nombre.replace(' ', '_');
            nombre= nombre.toUpper();

            input+= nombre+" = "+id;
            //input+= nombre;

            int i=1;
            while(query.next()){
                QString id= query.value(0).toString();
                QString nombre= query.value(1).toString();
                nombre= nombre.replace(' ', '_');
                nombre= nombre.toUpper();
                //input+= ", "+nombre+" = "+id;
                input+= ", "+nombre+" = "+id;
                if((i%5)== 0){
                    input+= "\n\t";
                }
                i++;
            }
            input+= QString()+"};*/"+"\n";
        }
    }
    if(class_name== "item"){        
        QSqlQuery query;

        QString str_query= "SELECT id, nombre FROM item";

        if(query.exec(str_query)){

            input+= "/*enum items{";

            query.next();
            QString id= query.value(0).toString();
            QString nombre= query.value(1).toString();
            nombre= nombre.replace(' ', '_');
            nombre= nombre.toUpper();

            input+= nombre+" = "+id;
            //input+= nombre;

            int i=1;
            while(query.next()){
                QString id= query.value(0).toString();
                QString nombre= query.value(1).toString();
                nombre= nombre.replace(' ', '_');
                nombre= nombre.toUpper();
                //input+= ", "+nombre+" = "+id;
                input+= ", "+nombre+" = "+id;

                if((i%5)== 0){
                    input+= "\n\t";
                }
                i++;
            }
            input+= QString()+"};*/"+"\n";
        }
    }    
    input+= QString()+"private:"+"\n";

    for(unsigned int i = 0; i < column_names.size(); i++)
    {
        QString temp = column_names[i];
        input+= "\tQVector<QString> v_"+temp+";"+"\n";
    }
    input+= QString()+"\n\tQVector<int> "+"transactions"+";"+"\n\n";

    for(unsigned int i = 0; i < column_names.size(); i++)
    {
        QString temp = column_names[i];
        input+= "\tQVector<QString> v_"+temp+"_info;"+"\n";
    }

    input+= QString()+"\npublic:"+"\n";

    // CONSTRUCTOR #1
    input+= "\t"+STR_OBJECT+str_class_name+"();"+"\n";

    QString first_column = column_names[0];
    /*
    // CONSTRUCTOR #2

    input+= "\t"+STR_OBJECT+str_class_name+"(QString "+first_column;
    for(unsigned int i = 1; i < column_names.size(); i++)
    {
        QString temp = column_names[i];
        input+= ", QString "+temp;
    }
    input+= QString()+");"+"\n\n";
    */

    input+= "\t~"+STR_OBJECT+str_class_name+"();"+"\n\n";

    input+= QString()+"\tvoid insert_into"+"(QString "+first_column;
    for(unsigned int i = 1; i < column_names.size(); i++)
    {
        QString temp = column_names[i];
        input+= ", QString "+temp;
    }
    input+= QString()+");"+"\n";

    input+= QString()+"\tvoid update"+"(QString "+first_column;
    for(unsigned int i = 1; i < column_names.size(); i++)
    {
        QString temp = column_names[i];
        input+= ", QString "+temp;
    }
    input+= QString()+");"+"\n";

    input+= QString()+"\tvoid delete_from"+"(QString "+first_column+");"+"\n\n";

    QSqlQuery query;
    QString str_query= "SELECT TABLE_NAME, COLUMN_NAME, REFERENCED_TABLE_NAME"
                       ", REFERENCED_COLUMN_NAME FROM information_schema.key_column_usage"
                       " WHERE TABLE_NAME = '"+str_class_name+"'";

    QString f_transaction;
    if(query.exec(str_query)){
        query.next();
        f_transaction += "QString columns, QString alias_"+str_class_name+", ";
        // RELATIONS
        while(query.next()){
            QString str_column_name = query.value(1).toString();
            //QString str_r_table_name = query.value(2).toString();
            //f_transaction += "QString alias_"+str_r_table_name+"_"+str_column_name+", ";
            f_transaction += "QString alias_fk_"+str_column_name+", ";
        }
        f_transaction += "\n\tQString type_join, QString where_clause";
    }
    input+= QString()+"\tQString select_from("+f_transaction+");"+"\n\n";

    input+= QString()+"\tQString build_query();"+"\n\n";

    input+= QString()+"};"+"\n";
    input+= "#endif // "+str_class_name_upper+"_H"+"\n";

    QTextStream out(&file);
    out << input;
    file.close();

    qDebug()<<"End fill"<<endl;

}

void CPPScript::fillFile_cpp(QString class_name, QVector<QString>& column_names, int i)
{
    qDebug()<<"mf_fillFile_cpp class_name: "<<class_name<<endl;
    QString file_class_name = getDirFolder()+STR_OBJECT+class_name+".cpp";
    QFile file(file_class_name);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"File_h to fill openned"<<endl;
    }else{
        qDebug()<<"File_h to fill not openned"<<endl;
        return;
    }

    QString str_class_name_upper = class_name.toUpper();
    QString str_class_name = class_name;

    QString input;

    // CONSTRUCTOR #1
    input+= "#include \""+STR_OBJECT+str_class_name+".h\""+"\n\n";
    input+= STR_OBJECT+str_class_name+"::"+STR_OBJECT+str_class_name+"()"+"\n";
    input+= QString()+"{"+"\n";
    input+= "\t//file "+str_class_name+"\n";
    input+= QString()+"\t//function construct_0"+"\n";
    input+= QString()+"\t//w!"+"\n";

    for(int j = 0; j < data[i].second.size(); j++){
        QString temp = column_names[j];
        for(int k = 0; k < data[i].second[0].size(); k++){
            input+= "\tv_"+temp+"_info.push_back(\""+data[i].second[j][k]+"\");"+"\n";
        }
        input+= "\n";
    }
    input+= "\n";

    input+= QString()+"}"+"\n\n";

    /*
    // CONSTRUCTOR #2
    QString first_column = column_names[0];
    input+= STR_OBJECT+str_class_name+"::"+STR_OBJECT+str_class_name+"(QString "+first_column;
    for(unsigned int i = 1; i < column_names.size(); i++)
    {
        QString temp = column_names[i];
        input+= ", QString "+temp;
    }
    input+= QString()+")"+"\n";
    input+= QString()+"{"+"\n";
    input+= "\t//file "+class_name+"\n";
    input+= QString()+"\t//function construct_1"+"\n";
    input+= QString()+"\t//w!"+"\n\n";

    for(unsigned int i = 0; i < column_names.size(); i++)
    {
        QString temp = column_names[i];
        input+= "\tthis->v_"+temp+".push_back("+temp+");"+"\n";
    }

    input+= QString()+"}"+"\n\n";
    */

    // DESTRUCTOR
    input+= STR_OBJECT+str_class_name+"::"+"~"+STR_OBJECT+str_class_name+"()"+"\n";
    input+= QString()+"{"+"\n\n"+"}"+"\n\n";

    // FUNCTION INSERT INTO
    QString f_transaction= "void "+QString(STR_OBJECT)+QString(str_class_name)+"::insert_into(";
    QString part_1, part_2;
    for(unsigned int i = 0; i < column_names.size(); i++)
    {
        QString temp = column_names[i];
        part_1+= "QString "+QString(temp)+", ";
        part_2+= "\tthis->v_"+QString(temp)+".push_back("+QString(temp)+");\n";
    }
    f_transaction+= part_1.remove(part_1.length()-2, 2)+")\n{\n\t//function insert_into\n\t//w!\n";
    f_transaction+= part_2;
    input+= f_transaction;
    input+= QString()+"\n\tthis->transactions"+".push_back(INGRESAR);\n}\n\n";

    // FUNCTION UPDATE
    f_transaction= "void "+QString(STR_OBJECT)+QString(str_class_name)+"::update(";
    part_1= "";
    part_2= "";

    for(unsigned int i = 0; i < column_names.size(); i++)
    {
        QString temp = column_names[i];
        part_1+= "QString "+QString(temp)+", ";
        part_2+= "\tthis->v_"+QString(temp)+".push_back("+QString(temp)+");\n";
    }
    f_transaction+= part_1.remove(part_1.length()-2, 2)+")\n{\n\t//function update\n\t//w!\n";
    f_transaction+= part_2;
    input+= f_transaction;
    input+= QString()+"\n\tthis->transactions"+".push_back(MODIFICAR);\n}\n\n";

    // FUNCTION DELETE FROM
    f_transaction= "void "+QString(STR_OBJECT)+QString(str_class_name)+"::delete_from(";
    part_1= "";
    part_2= "";

    QString temp = column_names[0];
    part_1+= "QString "+QString(temp)+"";
    part_2+= "\tthis->v_"+QString(temp)+".push_back("+QString(temp)+");\n";
    for(unsigned int i = 1; i < column_names.size(); i++)
    {
        QString temp = column_names[i];
        part_2+= "\tthis->v_"+QString(temp)+".push_back(\"\");\n";
    }
    f_transaction+= part_1+")\n{\n\t//function delete_from\n\t//w!\n";
    f_transaction+= part_2;
    input+= f_transaction;
    input+= QString()+"\n\tthis->transactions"+".push_back(ELIMINAR);\n}\n\n";

    // FUNCTION SELECT FROM

    QSqlQuery query;
    QString str_query= "SELECT TABLE_NAME, COLUMN_NAME, REFERENCED_TABLE_NAME"
                       ", REFERENCED_COLUMN_NAME FROM information_schema.key_column_usage"
                       " WHERE TABLE_NAME = '"+str_class_name+"'";

    if(query.exec(str_query)){
        f_transaction= "QString "+QString(STR_OBJECT)+QString(str_class_name)+"::select_from(";
        part_1= "";
        part_2= "";

        query.next();
        part_1 += "QString columns, QString alias_"+str_class_name+", ";
        // RELATIONS
        while(query.next()){
            QString str_column_name = query.value(1).toString();
            QString str_r_table_name = query.value(2).toString();
            //part_1 += "QString alias_"+str_r_table_name+"fk"+str_column_name+", ";
            part_1 += "QString alias_fk_"+str_column_name+", ";
        }
        part_1 += "\nQString type_join, QString where_clause)";

        query.seek(0);
        part_2 += "\tQString str_query = \"SELECT \"+columns+\" FROM "+str_class_name+" \"+alias_"
                +str_class_name+"";
        while(query.next()){
            QString str_column_name = query.value(1).toString();
            QString str_r_table_name = query.value(2).toString();
            QString str_r_column_name = query.value(3).toString();
            part_2 += "\n\t+\" \"+type_join+\" "+str_r_table_name+" \"+alias"
                    "_fk_"+str_column_name
                    +"\n\t+\" ON \"+alias_"+str_class_name+"+\"."+str_column_name+"\"+\" = \""
                    //"\n\t+alias_"+str_r_table_name+"_"+str_column_name+"+\"."+str_r_column_name+"\" ";
                    "\n\t+alias_fk_"+str_column_name+"+\"."+str_r_column_name+"\" ";
        }
        part_2 += "\n\t+\" WHERE \"+where_clause;\n\treturn str_query;\n}\n";
        f_transaction+= part_1+"\n{\n\t//function select_from\n\t//w!\n";
        f_transaction+= part_2;
        input+= f_transaction;
        //input+= QString()+"\n\tthis->transactions"+".push_back(ELIMINAR);\n}\n\n";
    }


    // FUNCTION BUILD QUERY

    input+= QString()+"QString "+str_class_name+"::build_query()"+"\n{";

    input+= QString()+"\n";

    // PREPARE QUERY
    input += QString()+"\tfor(int j= 0; j < v_"+column_names[0]+".size(); j++){\n";

    for(int i = 0; i < column_names.size(); i++){
        QString temp= column_names[i];
        input += "\t\tv_"+temp+"[j] = "+"v_"+temp+"[j].replace(\"\\\"\", \"\\\\\\\"\");\n";
        input += "\t\tv_"+temp+"[j] = "+"v_"+temp+"[j].replace(\"'\", \"\\'\");\n";
    }
    input += "\t}\n\n";

    input += QString()+"\tbool isAvalaible = true;"+"\n";
    input += QString()+"\tfor(int j= 0; j < v_"+column_names[0]+".size(); j++){\n";

    for(int j = 0; j < data[i].second.size(); j++){
        QString temp = column_names[j];

        input += "\t\tif(v_"+temp+"_info[2] == \"YES\"){"+"\n";
        input += QString()+"\t\t\tif("+"v_"+temp+"[j] == \"$$NULL$$\"){\n";
        input += "\t\t\t\tv_"+temp+"[j] = \"NULL\";\n";
        input += "\t\t\t\tisAvalaible = false;\n";
        input += "\t\t\t}else{\n";
        input += "\t\t\t\tisAvalaible = true;\n";
        input += "\t\t\t}\n";
        input += "\t\t}else{\n";
        input += "\t\t\tisAvalaible = true;\n";
        input += "\t\t}\n";

        input += "\t\tif(v_"+temp+"_info[5] == \"auto_increment\"){\n";
        input += "\t\t\tif(v_"+temp+"[j] == \"\"){\n";
        input += "\t\t\t\tv_"+temp+"[j] = \"(SELECT MAX(t."+temp+")+1 FROM "
                +str_class_name+" t)\";\n";
        input += "\t\t\t\tisAvalaible = false;\n";
        input += "\t\t\t}else{\n";
        input += "\t\t\t\tisAvalaible = true;\n";
        input += "\t\t\t}\n";
        input += "\t\t}\n";
        input += QString()+"\t\tif(isAvalaible){\n";
        /*
        input += "\t\t\tbool isNumeric = v_"+temp+"_info[1].mid(0, 3) == \"int\"\n";
        input += "\t\t\t || v_"+temp+"_info[1].mid(0, 7) == \"decimal\"\n";
        input += "\t\t\t || v_"+temp+"_info[1].mid(0, 4) == \"bool\"\n";
        input += "\t\t\t || v_"+temp+"_info[1].mid(0, 5) == \"point\";\n";
        */
        input += "\t\t\tbool isDateTime = v_"+temp+"_info[1].mid(0, 4) == \"date\"\n";
        input += "\t\t\t || v_"+temp+"_info[1].mid(0, 8) == \"datetime\";\n";
        input += "\t\t\tbool isText = v_"+temp+"_info[1].mid(0, 4) == \"text\"\n";
        input += "\t\t\t || v_"+temp+"_info[1].mid(0, 7) == \"varchar\";\n";
        input += QString()+"\t\t\tif(isDateTime || isText){\n";
        input += "\t\t\t\tv_"+temp+"[j] = \"\\\"\"+v_"+temp+"[j]"+"+\"\\\"\";\n";
        input += QString()+"\t\t\t}\n";
        input += QString()+"\t\t}\n";
    }
    input+= QString()+"\t}\n\n";
    input+= QString()+"\tQString str_query;\n";
    input+= QString()+"\tfor(int j= 0; j < v_"+column_names[0]+".size(); j++){";
    input+= QString()+"\n";

        // PARTE INSERT INTO
    input+= QString()+"\t\tif(transactions[j] == INGRESAR)"+"{\n";
    input+= QString()+"\t\t\tstr_query+= \"INSERT INTO "+str_class_name+"(\"";
    QString str_columns;
    for(int i = 0; i < column_names.size(); i++){
        if((i%3) == 0){
            str_columns+= "\n\t\t\t\"";
        }
        QString temp= column_names[i];
        str_columns+= ""+temp+", ";

        if((i+1) == column_names.size()){
            str_columns= str_columns.remove(str_columns.length()-2, 2);
            str_columns+= "\"";
            break;
        }
        if((i%3) == 2){
            str_columns+= "\"";
        }
    }
    input+= str_columns;
    str_columns= "";
    input+= "\n\t\t\t\")VALUES(\"";
    for(int i = 0; i < column_names.size(); i++){
        if((i%3)== 0){
            str_columns+= "\n\t\t\t";
        }
        QString temp= column_names[i];
        str_columns+= "+v_"+temp+"[j]+\", \"";

    }
    input+= str_columns.remove(str_columns.length()-5, 5);
    input+= QString()+"\n\t\t\t+\")&&END_QUERY&&\";\n\t\t}\n";


        // PARTE UPDATE
    input+= QString()+"\t\tif(transactions[j]== MODIFICAR)"+"{\n";
    input+= QString()+"\t\t\tstr_query+= \"UPDATE "+str_class_name+" SET\"";
    str_columns= "";
    for(int i = 1; i < column_names.size(); i++){
        if(((i-1)%2)== 0){
            str_columns+= "\n\t\t\t";
        }
        QString temp= column_names[i];
        str_columns+= "\" "+temp+" = \"+v_"+temp+"[j]+\", \"+";
        if((i+1)== column_names.size()){
            str_columns= str_columns.remove(str_columns.length()-5, 5);
            str_columns+= "\"+";
            break;
        }

    }
    input+= str_columns.remove(str_columns.length()-2, 2);
    input+= "\n\t\t\t\" WHERE id= \"+v_"+column_names[0]+"[j]+\"&&END_QUERY&&\";";
    input+= "\n\t\t}\n";

        // PARTE DELETE FROM
    input+= QString()+"\t\tif(transactions[j]== ELIMINAR)"+"{\n";
    input+= QString()+"\t\t\tstr_query+= \"DELETE FROM "+str_class_name+" WHERE";

    input+= " id = \"+v_"+column_names[0]+"[j]+\"&&END_QUERY&&\";";
    input+= QString()+"\n\t\t}\n\t}\n\n\treturn str_query;\n}";


    QTextStream out(&file);
    out << input;
    file.close();
}

// Class Object General
void CPPScript::generate_class_objectGeneral_h(QVector<QPair<QString, QVector<QVector<QString> > > >& data)
{
    QString str_general = "general";

    QFile file(str_general+".h");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"general openned"<<endl;
    }else{
        qDebug()<<"general not openned"<<endl;
        return;
    }

    QString str_general_upper = str_general.toUpper();

    QString input;
    input += "#ifndef "+str_general_upper+"_H\n";
    input += "#define "+str_general_upper+"_H\n";
    input += "\n#include \""+STR_INCLUDE+"share_include.h\"\n";
    input += "\nclass "+STR_OBJECT+str_general+"\n";
    input += "{\n";
    input += "public:\n";
    input += "\t"+STR_OBJECT+str_general+"(){ }\n";
    input += QString("\t")+"QString get_null(){ return \"$$NULL$$\"; }\n";
    input += "private:\n";
    input += "\tvirtual QString build_query() = 0;\n";
    input += "};\n";
    input += "\n#endif // "+str_general_upper+"\n";

    input += "/*\n";
    for(unsigned i = 0; i < data.size(); i++)
    {
        input += "#include \"db/"+STR_OBJECT+data[i].first+".h\"\n";
    }
    input += "\n";
    input += "*///\n";

    QTextStream out(&file);
    out << input;
    file.close();

}

void CPPScript::generate_class_objectGeneral_cpp()
{
    /*
    string str_general = "general";
    if(!mf_openFile_cpp(str_general))
        return;

    qDebug()<<"mf_fillFile_h class_name: "<<str_general<<endl;
    string file_class_name = getDirFolder()+str_general+".h";
    fstream file(file_class_name.c_str());

    //string str_class_name_upper = class_name.toUpper().toStdString();

    file<<"#ifndef OBJECT_"<<str_class_name_upper<<"_H"<<endl;
    */
}
