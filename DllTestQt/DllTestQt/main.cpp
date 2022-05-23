#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <QLocale>
#include <QTranslator>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QFont>

#include "vmusbwave.h"
#include "VmSimplePlot.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQuickWindow::setSceneGraphBackend("software");

    qmlRegisterType<VmUsbWave>("Vimu.Qml.Ctrls", 1, 0, "VmUsbWave");
    qmlRegisterType<VmSimplePlot>("Vimu.Qml.Ctrls", 1, 0, "VmSimplePlot");

    QQuickStyle::setStyle("Universal");  //Universal Material Fusion

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "VMMSO_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qml/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QList<QObject*> root_objs = engine.rootObjects();
    if(root_objs.size()!=0)
    {
        QObject* vmdsoplot = root_objs.at(0)->findChild<QObject*>("objectvmdsoplot");
        QObject* vmusbwave = root_objs.at(0)->findChild<QObject*>("objectvmusbwave");

         //QVariant rValue;
        //QVariant msg = "Hello for C++ Add";
         //QMetaObject::invokeMethod(root_objs->at(0), "usbDevCallBack",  Q_RETURN_ARG(QVariant,rValue),  Q_ARG(QVariant, msg));
    }

    int exe_res = app.exec();
    return exe_res;
}
