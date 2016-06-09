#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include "QRegExp"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydb.sqlite");
    if(!db.open()){
        QMessageBox::information(this,"Message","Not connected");
    }
    model = new QSqlTableModel(0, db);
    model->setTable("DYSCYPL");
    model->select();
    ui->tableView->setModel(model);
    ui->radioButton->setChecked(true);
    ui->lineEdit->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->radioButton_4->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::getcode(QString &code) {
    ui->listWidget->clear();
    ui->listWidget->addItem("SYSTEM DEBUG");
    code = "";
    code = ui->lineEdit->text();
    code = code.trimmed();
    ui->listWidget->addItem("   ALERT Code: " + code);
    code += "$$";
}

void MainWindow::on_radioButton_2_clicked()
{
    ui->comboBox->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->lineEdit->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
}

void MainWindow::on_radioButton_clicked()
{
    ui->lineEdit->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->comboBox->setEnabled(true);
    ui->pushButton->setEnabled(true);
}

void MainWindow::on_pushButton_clicked()
{
    QSqlQueryModel *qmodel = new QSqlQueryModel;
    if(ui->listWidget->SelectedClicked){
        qmodel->setQuery(ui->comboBox->currentText());
    ui->tableView->setModel(qmodel);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QSqlQueryModel *qmodel = new QSqlQueryModel;
    qmodel->setQuery(ui->lineEdit->text());
    ui->tableView->setModel(qmodel);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString code;
    getcode(code);
    checkcode(ui->lineEdit->text());
}

void MainWindow::on_pushButton_4_clicked()
{
    QString code;
    getcode(code);
    checkreg(ui->lineEdit->text());
}

void MainWindow::checkreg(QString code)
{
    other.clear();
    //rx=new QRegExp("\\s+|\\s+&&\\(|\\s+&&\\)|\\s+&&\\,|\\s+&&\\=|\\,|\\(|\\)|\\,|\\=|\\\"|\\*|\\");
    QStringList wr =code.split(QRegExp("\\s+|\\s+&&\\(|\\s+&&\\)|\\s+&&\\,|\\s+&&|\\,|\\(|\\)|\\,|\\\"|\\;"),QString::SkipEmptyParts);
    foreach (QString word, wr) {
        int from = code.indexOf(word);
        int to = from + word.length();
        rx=new QRegExp("((S)|(s))((e)|(E))((l)|(L))((E)|(e))((C)|(c))((t)|(T))");
        if(rx->exactMatch(word))
        {
           ui->listWidget->addItem("Keyword: "+word+": "+QString::number(from)+", "+QString::number(to));
           continue;
        }
        rx=new QRegExp("((F)|(f))((r)|(R))((O)|(o))((M)|(m))");
        if(rx->exactMatch(word))
        {
           ui->listWidget->addItem("Keyword: "+word+": "+QString::number(from)+", "+QString::number(to));
           continue;
        }
        rx=new QRegExp("((W)|(w))((H)|(h))((E)|(e))((R)|(r))((E)|(e))");
        if(rx->exactMatch(word))
        {
           ui->listWidget->addItem("Keyword: "+word+": "+QString::number(from)+", "+QString::number(to));
           continue;
        }
        rx=new QRegExp("((A)|(a))((N)|(n))((D)|(d))");
        if(rx->exactMatch(word))
        {
           ui->listWidget->addItem("  Logic operator token: "+word+": "+QString::number(from)+", "+QString::number(to));
           continue;
        }
        rx=new QRegExp("((O)|(o))((r)|(R))");
        if(rx->exactMatch(word))
        {
           ui->listWidget->addItem("  Logic operator token: "+word+": "+QString::number(from)+", "+QString::number(to));
           continue;
        }
        other[QString::number(from)+", "+QString::number(to)]=word;
    }
    QString tmp=code.toLower();
    int pos1=tmp.indexOf("select")+6;
    int pos2=tmp.indexOf("from");
    int pos3=tmp.indexOf("where");
    if(ui->radioButton_3->isChecked())
    {

        QMapIterator<QString,QString> it(other);
        while (it.hasNext()) {
           it.next();
           //rx=new QRegExp("^[\_|a-zA-Z][0-9a-zA-Z]+");
           //rx=new QRegExp("^_[0-9a-zA-Z]+");
           rx=new QRegExp("^[a-zA-Z][0-9a-zA-Z]+");
           if((rx->exactMatch(it.value()) && (((code.indexOf(it.value())>pos2) && (code.indexOf(it.value())<pos3)) || code[code.indexOf(it.value())+it.value().length()]==';')) ){
               if(it.value().length()>12){
                  ui->listWidget->addItem("  Tablename token error, length >12: \""+it.value()+"\", "+it.key());
               }else{
               ui->listWidget->addItem("  Tablename token (id1): \""+it.value()+"\", "+it.key());
            }
          }else{
               rx=new QRegExp("^((M|m)(A|a)(X|x))|((M|m)(i|I)(N|n))|((A|a)(V|v)(G|g))$");
               if(rx->exactMatch(it.value())){
                   ui->listWidget->addItem("  Arithmetic_operation token: \""+it.value()+"\", "+it.key());
                }//ui->listWidget->addItem("  Other part: \""+it.key()+"\", "+it.value());
               else{
               // rx=new QRegExp("^[a-zA-Z][0-9a-zA-Z]+");
               rx=new QRegExp("^[a-zA-Z][0-9a-zA-Z\s]+");
               if(rx->exactMatch(it.value()) &&(((code.indexOf(it.value())>pos1) && (code.indexOf(it.value())<pos2)) || (code.indexOf(it.value())>pos3))){
                   if(it.value().length()>8){
                      ui->listWidget->addItem("  Attribute_name token error, length >8: \""+it.value()+"\", "+it.key());
                   }else{
                   ui->listWidget->addItem("  Attribute_name token (id6): \""+it.value()+"\", "+it.key());
                }
               }
               else{
                   rx=new QRegExp("^(\>)|(\<)|(\=)");
                   if(rx->exactMatch(it.value())){
                       ui->listWidget->addItem("  Relop token: \""+it.value()+"\", "+it.key());
                   }//ui->listWidget->addItem("  Other part: \""+it.key()+"\", "+it.value());
                   else{
                       rx=new QRegExp("^[0-9\.]+$");
                       if(rx->exactMatch(it.value())){
                          ui->listWidget->addItem("  String token: \""+it.value()+"\", "+it.key());
                       }//ui->listWidget->addItem("  Other part: \""+it.key()+"\", "+it.value());
                       else{
                           ui->listWidget->addItem("  Other part: \""+it.key()+"\", "+it.value());
                       }
                   }
               }
               }
           }
        }
    }
     //--------------------------------------------

    //--------------------------------------------
    if(ui->radioButton_4->isChecked())
    {
        QMapIterator<QString,QString> it(other);
        while (it.hasNext()) {
           it.next();
           //rx=new QRegExp("^[\_|a-zA-Z][0-9a-zA-Z]+");
           //rx=new QRegExp("^_[0-9a-zA-Z]+");
           rx=new QRegExp("^[a-zA-Z][0-9a-zA-Z]+");
           if(rx->exactMatch(it.value())){
               if(it.value().length()>12){
                  ui->listWidget->addItem("  Tablename token error, length >12: \""+it.value()+"\", "+it.key());
               }else{
               ui->listWidget->addItem("  Tablename token (id1): \""+it.value()+"\", "+it.key());
            }
          }else{
               rx=new QRegExp("^((M|m)(A|a)(X|x))|((M|m)(i|I)(N|n))|((A|a)(V|v)(G|g))$");
               if(rx->exactMatch(it.value())){
                   ui->listWidget->addItem("  Arithmetic_operation token: \""+it.value()+"\", "+it.key());
                }//ui->listWidget->addItem("  Other part: \""+it.key()+"\", "+it.value());
               else{
               // rx=new QRegExp("^[a-zA-Z][0-9a-zA-Z]+");
               rx=new QRegExp("^[a-zA-Z][0-9a-zA-Z\s]+");
               if(rx->exactMatch(it.value())){
                   if(it.value().length()>8){
                      ui->listWidget->addItem("  Attribute_name token error, length >8: \""+it.value()+"\", "+it.key());
                   }else{
                   ui->listWidget->addItem("  Attribute_name token (id6): \""+it.value()+"\", "+it.key());
                }
               }else{
                   rx=new QRegExp("^(\>)|(\<)|(\=)$");
                   if(rx->exactMatch(it.value())){
                       ui->listWidget->addItem("  Relop token: \""+it.value()+"\", "+it.key());
                   }//ui->listWidget->addItem("  Other part: \""+it.key()+"\", "+it.value());
                   else{
                       rx=new QRegExp("^[0-9\.]+$");
                       if(rx->exactMatch(it.value())){
                          ui->listWidget->addItem("  String token: \""+it.value()+"\", "+it.key());
                       }//ui->listWidget->addItem("  Other part: \""+it.key()+"\", "+it.value());
                       else{
                            ui->listWidget->addItem("  Other part: \""+it.key()+"\", "+it.value());
                       }
                   }
               }
               }
//               else{
//                   //ui->listWidget->addItem("  Other part: \""+it.key()+"\", "+it.value());
//                   //rx=new QRegExp("^((M|m)(A|a)(X|x))|((M|m)(i|I)(N|n))|((A|a)(V|v)(G|g))\([a-zA-Z][0-9a-zA-Z\s]+\)");
//                   rx=new QRegExp("^((M|m)(A|a)(X|x))|((M|m)(i|I)(N|n))|((A|a)(V|v)(G|g))");
//                   if(rx->exactMatch(it.key())){
//                       ui->listWidget->addItem("  Arithmetic_operation token: \""+it.key()+"\", "+it.value());
//                    }
//               }
           }
        }
    }
}


void MainWindow::checkcode(QString code) {
    int tmpB, tmpE,
        tmpB2, tmpE2,
        tmpB3, tmpE3,
        tmpB4, tmpE4,
        tmpB5, tmpE5,
        tmpB6, tmpE6,
        tmpB7, tmpE7,
        tmpB8, tmpE8,
        tmpB9, tmpE9,
        tmpB10, tmpE10;
    int error=99999999,success=100000000;
    keyword.clear();
    token.clear();
    state = 0;
    compile = true;
    ch=new QChar(code.length()+1);
    ch=code.data();
    int tmp=0;
    for (int i = 0; i < code.length()+1; i++) {
        switch (state) {
        case 0:
             if((ch[i]=='S')||(ch[i]=='s')){
                state = 22;
                break;
             }
                ui->listWidget->addItem("   ERROR Querry");
                state = error;
            break;
        case 22:
             if((ch[i]=='E')||(ch[i]=='e')){
                state = 23;
                break;
             }
                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'SELECT\'  \"");
                state = error;

            break;

        case 23:
             if((ch[i]=='L')||(ch[i]=='l')){
                state = 24;
                break;
             }

                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'SELECT\'  \"");
                state = error;

            break;

        case 24:
             if((ch[i]=='E')||(ch[i]=='e')){
                state = 25;
                break;
             }

                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'SELECT\'  \"");
                state = error;

            break;

        case 25:
             if((ch[i]=='C')||(ch[i]=='c')){
                state = 26;
                break;
             }
                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'SELECT\'  \"");
                state = error;

            break;

        case 26:
             if((ch[i]=='T')||(ch[i]=='t')){
                state = 27;
                keyword["SELECT"]=QString::number(i+1-6)+","+QString::number(i+1);
                break;
             }

                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'SELECT\'  \"");
                state = error;

            break;

        case 27:
             if(ch[i]==' '){
                tmpB7=i;
                state = 28;
                break;
             }

                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'space\' after \'SELECT\'  \"");
                state = error;

            break;

        case 28:
             if(ch[i]=='M' && ch[i+1]=='I' && ch[i+2]=='N'){
                 tmpB9=i;
                 state=97;
                 break;
             }
             if(ch[i]=='M' && ch[i+1]=='A' && ch[i+2]=='X'){
                 tmpB9=i;
                 state=97;
                 break;
             }
             if(ch[i]=='A' && ch[i+1]=='V' && ch[i+2]=='G'){
                 tmpB9=i;
                 state=97;
                 break;
             }
             if(ch[i]=='*'){
                 tmpE7=i+1;
                 QStringRef subString(&code, tmpB7+1, tmpE7-tmpB7-1);
                 token["Attribute_name token (id6): "+QString::number(tmpB7+1)+","+QString::number(tmpE7)]=subString.toString();
                state = 29;
                break;
             }
             rx=new QRegExp("[a-zA-Z]");
             if(rx->exactMatch(ch[i])){

                tmp++;
                state=281;
                break;
             }/*else{
             if(ch[i]==' '){
                  tmpE7=i+1;
                  QStringRef subString(&code, tmpB7+1, tmpE7-tmpB7-2);
                  token["Attribute_name token (id6): "+QString::number(tmpB7+1)+","+QString::number(tmpE7-1)]=subString.toString();
                  tmp=0;
                  state = 30;
                  break;
              }
             }
             if(ch[i]==','){
                tmpE7=i+1;
                QStringRef subString(&code, tmpB7+1, tmpE7-tmpB7-2);
                token["Attribute_name token (id6): "+QString::number(tmpB7+1)+","+QString::number(tmpE7-1)]=subString.toString();
                tmpB8=i;
                tmp=0;
                state=282;
                break;
             }*/
                ui->listWidget->addItem("   ERROR State \"EXPECTED ATTRIBUTES: like \'*\' or AttName or FUNCTION \"");
                state = error;

            break;
        case 281:
            rx=new QRegExp("[0-9\*a-zA-Z]");
            if(rx->exactMatch(ch[i])){
               if(tmp>8){
                   ui->listWidget->addItem("   ERROR State \"length of attribute >8 \"");
                   state = error;
                   tmp=0;
               }
               tmp++;
               state=281;
               break;
            }else{
            if(ch[i]==' '){
                 tmpE7=i+1;
                 QStringRef subString(&code, tmpB7+1, tmpE7-tmpB7-2);
                 token["Attribute_name token (id6): "+QString::number(tmpB7+1)+","+QString::number(tmpE7-1)]=subString.toString();
                 tmp=0;
                 state = 30;
                 break;
             }

            }
            if(ch[i]==','){
               tmpE7=i+1;
               QStringRef subString(&code, tmpB7+1, tmpE7-tmpB7-2);
               token["Attribute_name token (id6): "+QString::number(tmpB7+1)+","+QString::number(tmpE7-1)]=subString.toString();
               tmpB8=i;
               tmp=0;
               state=282;
               break;
            }
            //ui->listWidget->addItem("   ERROR State \"EXPECTED ATTRIBUTES: like \'*\' or AttName or FUNCTION \"");
            state = error;
            break;
        case 282:
            rx=new QRegExp("[0-9\*a-zA-Z]");
            if(rx->exactMatch(ch[i])){
               if(tmp>8){
                   ui->listWidget->addItem("   ERROR State \"length of attribute >8 \"");
                   state = error;
                   tmp=0;
               }
               tmp++;
               state=282;
               break;
            }else{
            if(ch[i]==' '){
                 tmpE8=i+1;
                 QStringRef subString(&code, tmpB8+1, tmpE8-tmpB8-2);
                 token["Attribute_name token (id6): "+QString::number(tmpB8+1)+","+QString::number(tmpE8-1)]=subString.toString();
                 tmp=0;
                 state = 30;
                 break;
             }
            }
            ui->listWidget->addItem("   ERROR State \"EXPECTED ATTRIBUTES: like \'*\' or AttName or FUNCTION \"");
            state = error;
            break;
//----------------------------------------------------------------------------------
        case 97:
            if(ch[i-1]=='M' && ch[i]=='I' && ch[i+1]=='N'){
                state=98;
                break;
            }
            if(ch[i-1]=='M' && ch[i]=='A' && ch[i+1]=='X'){
                state=98;
                break;
            }
            if(ch[i-1]=='A' && ch[i]=='V' && ch[i+1]=='G'){
                state=98;
                break;
            }
            break;
        case 98:
            if(ch[i-2]=='M' && ch[i-1]=='I' && ch[i]=='N'){
                state=99;
                break;
            }
            if(ch[i-2]=='M' && ch[i-1]=='A' && ch[i]=='X'){
                state=99;
                break;
            }
            if(ch[i-2]=='A' && ch[i-1]=='V' && ch[i]=='G'){
                state=99;
                break;
            }
            break;
        case 99:
            if(ch[i]=='('){
                tmpE9=i;
                QStringRef subString(&code, tmpB9, tmpE9-tmpB9);
                token["Arithmetic_operation token: "+QString::number(tmpB9)+","+QString::number(tmpE9)]=subString.toString();
                tmpB6=i;
                state=100;
                break;
            }
            ui->listWidget->addItem("   ERROR State \"EXPECTED ()\"");
            state = error;
            break;

        case 100:
            rx=new QRegExp("[0-9a-zA-Z]");
            if(rx->exactMatch(ch[i])){
               if(tmp>8){
                   ui->listWidget->addItem("   ERROR State \"length of attribute >8 \"");
                   state = error;
                   tmp=0;
               }
               tmp++;
               state=100;
               break;
            }else{
            if(ch[i]==')'){
                 tmpE6=i+1;
                 QStringRef subString(&code, tmpB6+1, tmpE6-tmpB6-2);
                 token["Attribute_name token (id6): "+QString::number(tmpB6+1)+","+QString::number(tmpE6-1)]=subString.toString();
                 tmp=0;
                 state = 101;
                 break;
             }
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED CONDITION IN '()'  \"");
            state = error;
            break;
        case 101:
            if(ch[i]==' '){
                state = 30;
                break;
            }
            ui->listWidget->addItem("   ERROR State \"EXPECTED \'space\' after \'FUNCTION()\'  \"");
            state = error;
            break;
        //------------------------------------------------------------
        case 29:
            if(ch[i]==' '){
                state = 30;
                break;
            }

                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'space\' after \'*\'  \"");
                state = error;

            break;
        case 30:
             if((ch[i]=='F')||(ch[i]=='f')){
                state = 31;
                break;
             }

                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'FROM\'  \"");
                state = error;

            break;

        case 31:
             if((ch[i]=='R')||(ch[i]=='r')){
                state = 32;
                break;
             }

                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'FROM\'  \"");
                state = error;

            break;

        case 32:
             if((ch[i]=='O')||(ch[i]=='o')){
                state = 33;
                break;
             }
                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'FROM\'  \"");
                state = error;

            break;

        case 33:
             if((ch[i]=='M')||(ch[i]=='m')){
                state = 34;
                keyword["FROM"]=QString::number(i+1-4)+","+QString::number(i+1);
                break;
             }
                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'FROM\'  \"");
                state = error;

            break;

        case 34:
            if(ch[i]==' '){
                tmpB5=i;
                state = 35;
                break;
            }

                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED \'space\' after \'FROM\'  \"");
                state = error;

            break;

        case 35:
            rx=new QRegExp("[a-zA-Z0-9]");
            if(rx->exactMatch(ch[i])){
               if(tmp>12){
                   ui->listWidget->addItem("   ERROR State \"length of TABLE_NAME >12 \"");
                   state = error;
                   tmp=0;
               }
               tmp++;
               state=35;
               break;
            }
            if(ch[i]==';'){
                tmpE5=i+1;
                QStringRef subString(&code, tmpB5+1, tmpE5-tmpB5-2);
                token["Table_name token (id1): "+QString::number(tmpB5+1)+","+QString::number(tmpE5-1)]=subString.toString();
               tmp=0;
               state = success;
               break;
            }
            if(ch[i]==' '){
                tmpE5=i+1;
                QStringRef subString(&code, tmpB5+1, tmpE5-tmpB5-2);
                token["Table_name token (id1): "+QString::number(tmpB5+1)+","+QString::number(tmpE5-1)]=subString.toString();
               tmp=0;
               state=36;
               break;
            }
            /////
            ui->listWidget->addItem("   ERROR State \"EXPECTED tablename\"");
            state = error;

            break;

        case 36:
            if(ch[i]=='W'||ch[i]=='w'){
                state=37;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED \'WHERE\'  \"");
            state = error;
            break;

        case 37:
            if(ch[i]=='H'||ch[i]=='h'){
                state=38;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED \'WHERE\'  \"");
            state = error;
            break;

        case 38:
            if(ch[i]=='E'||ch[i]=='e'){
                state=39;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED \'WHERE\'  \"");
            state = error;
            break;

        case 39:
            if(ch[i]=='R'||ch[i]=='r'){
                state=40;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED \'WHERE\'  \"");
            state = error;
            break;

        case 40:
            if(ch[i]=='E'||ch[i]=='e'){
                state=41;
                keyword["WHERE"]=QString::number(i+1-5)+","+QString::number(i+1);
                break;
            }else
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED \'WHERE\'  \"");
            state = error;
            break;

        case 41:
            if(ch[i]==' '){
                tmpB4=i;
                state=50;
                break;
            }
            if(ch[i]=='$'){
                state = error;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED \'space\' after \'WHERE\'  \"");
            state = error;
            break;

        case 50:
            rx=new QRegExp("[a-zA-Z0-9]");
            if(rx->exactMatch(ch[i])){
               if(tmp>8){
                   ui->listWidget->addItem("   ERROR State \"length of ATTRIBUTE >8 \"");
                   state = error;
                   tmp=0;
               }
               tmp++;
               state=50;
               break;
            }
            if(ch[i]==' '){
                state=519;
                break;
            }
            if(ch[i]=='=')
            {
                tmpE4=i+1;
                QStringRef subString(&code, tmpB4+1, tmpE4-tmpB4-2);
                token["Attribute_name token (id6): "+QString::number(tmpB4+1)+","+QString::number(tmpE4-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]="=";
                state=51;
                break;
            }
            if(ch[i]=='<')
            {
                tmpE4=i+1;
                QStringRef subString(&code, tmpB4+1, tmpE4-tmpB4-2);
                token["Attribute_name token (id6): "+QString::number(tmpB4+1)+","+QString::number(tmpE4-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]="<";
                state=51;
                break;
            }
            if(ch[i]=='>')
            {
                tmpE4=i+1;
                QStringRef subString(&code, tmpB4+1, tmpE4-tmpB4-2);
                token["Attribute_name token (id6): "+QString::number(tmpB4+1)+","+QString::number(tmpE4-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]=">";
                state=51;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED ATTRIBUTE NAME FOR CONDITION in \'WHERE\'  \"");
            state = error;
            break;
            /////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////
        case 519:
            if(ch[i]=='=')
            {
                tmpE4=i+1;
                QStringRef subString(&code, tmpB4+1, tmpE4-tmpB4-2);
                token["Attribute_name token (id6): "+QString::number(tmpB4+1)+","+QString::number(tmpE4-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]="=";
                state=51;
                break;
            }
            if(ch[i]=='<')
            {
                tmpE4=i+1;
                QStringRef subString(&code, tmpB4+1, tmpE4-tmpB4-2);
                token["Attribute_name token (id6): "+QString::number(tmpB4+1)+","+QString::number(tmpE4-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]="<";
                state=51;
                break;
            }
            if(ch[i]=='>')
            {
                tmpE4=i+1;
                QStringRef subString(&code, tmpB4+1, tmpE4-tmpB4-2);
                token["Attribute_name token (id6): "+QString::number(tmpB4+1)+","+QString::number(tmpE4-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]=">";
                state=51;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED =/</>  AFTER \'ATTRIBUTE NAME\'  \"");
            state = error;
            break;
            /////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////
        case 51:
//            if(ch[i]==';'){
//               state = success;
//               break;
//            }
            if(ch[i]==' '){
                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED CONDITION value after =/</> in \'WHERE\'  \"");
                state=error;
                break;
            }
            if(ch[i]=='"'){
                tmpB2=i;
                state=521;
                break;
            }
//            if(ch[i]==','){
//                state=51;
//                break;
//            }
            /*rx=new QRegExp("[a-zA-Z]");
            if(rx->exactMatch(ch[i])){
                state=51;
                break;
            }*/

            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED CONDITION value after =/</> in \'WHERE\'  \"");
            state=error;
            break;
        case 521:
            rx=new QRegExp("[a-zA-Z0-9]");
            if(rx->exactMatch(ch[i])){
                state=521;
                break;
            }
            if(ch[i]=='"'){
                tmpE2=i+1;
                QStringRef subString(&code, tmpB2+1, tmpE2-tmpB2-2);
                token["String token: "+QString::number(tmpB2+1)+","+QString::number(tmpE2-1)]=subString.toString();
                state=52;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"ERROR IN CONDITION value after =/</> in \'WHERE\'  \"");
            state=error;
            break;
        case 52:
            if(ch[i]==';'){
                state = success;
                break;
            }
            if(ch[i]==' '){
                state=53;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED ENDING OF CONDITION \';\' or \'space\'  \"");
            state = error;
            break;
        case 53:
            if(ch[i]=='A'||ch[i]=='a'){
                state=54;
                break;
            }
            if(ch[i]=='O'||ch[i]=='o'){
                state=56;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED \'AND\' OR \'OR\' \"");
            state = error;
            break;
        case 54:
            if(ch[i]=='N'||ch[i]=='n'){
                state=55;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED 'AND\'\"");
            state = error;
            break;
        case 55:
            if(ch[i]=='D'||ch[i]=='d'){
                tmpB3=i+1;
                state=57;
                //keyword[QString::number(i+1-3)+","+QString::number(i+1)]="AND";
                token["Logic operator token: "+QString::number(i+1-3)+","+QString::number(i+1)]="AND";
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED 'AND\'\"");
            break;
        case 56:
            if(ch[i]=='R'||ch[i]=='r'){
                tmpB3=i+1;
                state=58;
                //keyword[QString::number(i+1-2)+","+QString::number(i+1)]="OR";
                token["Logic operator token: "+QString::number(i+1-2)+","+QString::number(i+1)]="OR";
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED 'OR\'\"");
            break;
        case 57:
            //
            if(ch[i]==' '){

                state=59;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED 'SPACE\' AFTER AND\"");
            break;
            //
        case 58:
            if(ch[i]==' '){

                state=59;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED 'SPACE\' AFTER OR\"");
            break;
        //--------------
        case 59:

            tmp=0;
            rx=new QRegExp("[a-zA-Z]");
            if(rx->exactMatch(ch[i])){
               if(tmp>8){
                   ui->listWidget->addItem("   ERROR State \"length of ATTRIBUTE >8 \"");
                   state = error;
                   tmp=0;
               }
               tmp++;
               state=59;
               break;
            }
            if(ch[i]==' '){
                state=591;
                break;
            }
            if(ch[i]=='=')
            {
                tmpE3=i+1;
                QStringRef subString(&code, tmpB3+1, tmpE3-tmpB3-2);
                token["Attribute_name token (id6): "+QString::number(tmpB3+1)+","+QString::number(tmpE3-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]="=";
                state=60;
                break;
            }
            if(ch[i]=='<')
            {
                tmpE3=i+1;
                QStringRef subString(&code, tmpB3+1, tmpE3-tmpB3-2);
                token["Attribute_name token (id6): "+QString::number(tmpB3+1)+","+QString::number(tmpE3-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]="<";
                state=60;
                break;
            }
            if(ch[i]=='>')
            {
                tmpE3=i+1;
                QStringRef subString(&code, tmpB3+1, tmpE3-tmpB3-2);
                token["Attribute_name token (id6): "+QString::number(tmpB3+1)+","+QString::number(tmpE3-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]=">";
                state=60;
                break;
            }

            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED ATTRIBUTE NAME FOR CONDITION in \'WHERE\'  \"");
            state = error;
            break;
            /////////////////////////////////////////////////////////
        case 591:
            if(ch[i]=='=')
            {
                tmpE3=i+1;
                QStringRef subString(&code, tmpB3+1, tmpE3-tmpB3-2);
                token["Attribute_name token (id6): "+QString::number(tmpB3+1)+","+QString::number(tmpE3-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]="=";
                state=60;
                break;
            }
            if(ch[i]=='<')
            {
                tmpE3=i+1;
                QStringRef subString(&code, tmpB3+1, tmpE3-tmpB3-2);
                token["Attribute_name token (id6): "+QString::number(tmpB3+1)+","+QString::number(tmpE3-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]="<";
                state=60;
                break;
            }
            if(ch[i]=='>')
            {
                tmpE3=i+1;
                QStringRef subString(&code, tmpB3+1, tmpE3-tmpB3-2);
                token["Attribute_name token (id6): "+QString::number(tmpB3+1)+","+QString::number(tmpE3-1)]=subString.toString();
                token["Relop token: "+QString::number(i)+","+QString::number(i+1)]=">";
                state=60;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED =/</>  AFTER \'ATTRIBUTE NAME\'  \"");
            state = error;
            break;
            /////////////////////////////////////////////////////////
        case 60:
            if(ch[i]==' '){
                ui->listWidget->addItem(
                        "   ERROR State \"EXPECTED CONDITION value after =/</> in \'WHERE\'  \"");
                state=error;
                break;
            }
            if(ch[i]=='"'){
                tmpB=i;
                state=621;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED CONDITION value after =/</> in \'WHERE\'  \"");
            state=error;
            break;
        case 621:
            rx=new QRegExp("[a-zA-Z]");
            if(rx->exactMatch(ch[i])){
                state=621;
                break;
            }
            rx=new QRegExp("[0-9\.]");
            if(rx->exactMatch(ch[i])){
                state=621;
                break;
            }
            if(ch[i]=='"'){
                tmpE=i+1;
                QStringRef subString(&code, tmpB+1, tmpE-tmpB-2);
                token["String token: "+QString::number(tmpB+1)+","+QString::number(tmpE-1)]=subString.toString();
                state=62;
                break;
            }
            ui->listWidget->addItem(
                    "   ERROR State \"ERROR IN CONDITION value after =/</> in \'WHERE\'  \"");
            state=error;
            break;
        case 62:
            if(ch[i]==';'){
                state = success;
                break;
            }
//            if(ch[i]==' '){
//                state=53;
//                break;
//            }
            ui->listWidget->addItem(
                    "   ERROR State \"EXPECTED ENDING OF CONDITION \';\'\"");
            state = error;
            break;
            //--------------------------------------------
        case 99999999:
            ui->listWidget->addItem("   FATAL ERROR Querry");
            ui->listWidget->addItem("   WARNING Analyzing: Complete");

            compile = false;
            break;

        case 100000000:
            ui->listWidget->addItem("   NO ERRORS IN Querry");
            ui->listWidget->addItem("   WARNING Analyzing: Complete");

            compile = false;
            break;
        }
        if (compile == false) {
            break;
        }
    }
    QMapIterator<QString,QString> it(keyword);
    while (it.hasNext()) {
        it.next();
        ui->listWidget->addItem("     Keyword: "+it.key()+", "+it.value());
    }
    token.remove("\"");
    QMapIterator<QString,QString> it2(token);
    while (it2.hasNext()) {
        it2.next();
        ui->listWidget->addItem(it2.key()+", \""+it2.value()+"\"");
        /*rx=new QRegExp("^[0-9\.]+$");
        if(rx->exactMatch(it2.key())){
            ui->listWidget->addItem("     String token: "+it2.value()+", \""+it2.key()+"\"");
        }else{
            rx=new QRegExp("^[a-zA-Z0-9]+$");
            if(rx->exactMatch(it2.key())){
            ui->listWidget->addItem("     Text token: "+it2.value()+", \""+it2.key()+"\"");
            }else ui->listWidget->addItem("     "+it2.value()+", \""+it2.key()+"\"");
        }*/
    }

}

