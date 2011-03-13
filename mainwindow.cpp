#include "mainwindow.h"

void MainWindow::processData()
{
    while (globalSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress addr;
        qint8 stat, type;
        quint16 pport;
        datagram.resize(globalSocket->pendingDatagramSize());
        globalSocket->readDatagram(datagram.data(), datagram.size(),&addr,&pport);
       stat = datagram[0];
        type = datagram[1];
        datagram.remove(0,2);
        if (stat == 1)
            UserCheck(tr(datagram.data()),addr.toString());
        if (stat == 4)
            UserDel(tr(datagram.data()),addr.toString());
        if (stat == 0)
        {
          if (tr(datagram.data())=="!cs")
          {
              chatArea->insertHtml("cs<br>");
              cs->CSping();
          } else
          if (tr(datagram.data())=="!dota")
          {
              chatArea->insertHtml("dota<br>");
              war->WarPing();
          } else
          if (tr(datagram.data())=="!q3")
          {
              chatArea->insertHtml("q3<br>");
              q3->Q3ping();
          } else
          if ((tr(datagram.data())=="!start")||(tr(datagram.data())=="!�����"))
          {
              chatArea->insertHtml("start<br>");
              quiz->Start();
          } else
          if ((tr(datagram.data())=="!stop")||(tr(datagram.data())=="!����"))
          {
              chatArea->insertHtml("stop<br>");
              quiz->Stop();
          } else
          if ((tr(datagram.data())=="!hint")||(tr(datagram.data())=="!����")||(tr(datagram.data())=="!h")||(tr(datagram.data())=="!�"))
          {
              chatArea->insertHtml("hint<br>");
              quiz->GetHint();
          } else
          if ((tr(datagram.data())=="!next")||(tr(datagram.data())=="!������")||(tr(datagram.data())=="!n")||(tr(datagram.data())=="!�"))
          {
              chatArea->insertHtml("next<br>");
              if (quiz->ServerRun())
                  quiz->Next();
          } else
          if ((tr(datagram.data())=="!q")||(tr(datagram.data())=="!�"))
          {
              chatArea->insertHtml("quest<br>");
              if (quiz->ServerRun())
                  quiz->Repeat();
          } else
          if ((tr(datagram.data())=="!top")||(tr(datagram.data())=="!���"))
          {
              chatArea->insertHtml("top<br>");
              quiz->Top();
          } else
          if ((tr(datagram.data())=="!help")||(tr(datagram.data())=="!������"))
          {
              chatArea->insertHtml("help<br>");
              sendMessage("��������� �������:<br>"
                          "!cs - ������ �������� Counter-Strike<br>"
                          "!dota - ������ �������� Warcraft III<br>"
                          "!q3 - ������ �������� Quake 3<br><br>"
                          "������� ���������:<br>"
                          "!q, !� - ��������� ������<br>"
                          "!start, !����� - ������ ����<br>"
                          "!stop, !���� - ���������� ����<br>"
                          "!next, !������, !n, !� - ��������� ������<br>"
                          "!hint, !����, !h, !� - ���������<br>"
                          "!top, !��� - ��� ���������"
                          "!help, !������ - ������",bGeneral);
          }
          else
          {
              if (quiz->ServerRun())
                  quiz->Answer(tr(datagram.data()),addr.toString());

          }

        }

    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    chatArea = new QTextBrowser(this);
    chatArea->setOpenLinks(false);
    setCentralWidget(chatArea);

    globalSocket = new QUdpSocket(this);
    globalSocket->bind(port,QUdpSocket::ReuseAddressHint);

    connect(globalSocket,SIGNAL(readyRead()),this,SLOT(processData()));

    /*botLog = new QFile("bot.log");
    botLog->open(QIODevice::Append | QIODevice::Text);
*/
    cs = new CSCheck;
    war = new WarCheck;
    quiz = new QuizCheck;
    q3 = new Q3Check;
    //cs->CSping();

    sendMessage("run",bGeneral);
    //WriteLog("run");

    chatArea->insertHtml("run<br>");

}


MainWindow::~MainWindow()
{
    sendMessage("die",bGeneral);
}

