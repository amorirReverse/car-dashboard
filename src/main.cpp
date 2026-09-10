#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "CanReceiver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qDebug() << "Initialisation du Dashboard Automobile...";

    /// 1. Instanciation du récepteur SocketCAN
    CanReceiver canReceiver;

    /// 2. Déclaration du type C++ dans le système de types QML
    qmlRegisterSingletonInstance("CarDashboard.Backend", 1, 0, "CanReceiver", &canReceiver);

    /// 3. Démarrage de l'écoute sur vcan0 dans son thread dédié
    canReceiver.startListening("vcan0");

    /// 4. Chargement du moteur QML
    QQmlApplicationEngine engine;

    /// Chargement de l'interface graphique (fichier src/main.qml)
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    
    /// Alternative si le fichier QML est chargé directement depuis le disque :
    /// const QUrl url = QUrl::fromLocalFile("src/main.qml");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}