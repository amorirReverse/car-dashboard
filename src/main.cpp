/**
 * @file main.cpp
 * @brief Point d'entrée de l'application Qt.
 * @details Initialise le moteur QML, instancie le récepteur C++ SocketCAN 
 * et enregistre le singleton auprès de l'IHM.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "can_receiver.h"

/**
 * @brief Fonction principale du programme.
 * @param argc Nombre d'arguments en ligne de commande.
 * @param argv Tableau des arguments.
 * @return Code de sortie de l'application Qt.
 */

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "xcb");
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
    const QUrl url(QStringLiteral("qrc:/qt/qml/CarDashboard/src/main.qml"));
    

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}