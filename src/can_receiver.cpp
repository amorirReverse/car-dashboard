/**
 * @file can_receiver.cpp
 * @brief Implémentation de la classe CanReceiver pour la gestion de SocketCan sous Linux
 */

 #include "can_receiver.h"
 #include <QDebug>

 #include <sys/socket.h>
 #include <sys/ioctl.h>
 #include <net/if.h>
 #include <linux/can.h>
 #include <linux/can/raw.h>
 #include <unistd.h>
 #include <cstring>

 /**
  * @brief Constructeur de la classe CanReceiver
  * @param parent Objet QObjet parent pour l'arbre de mémoire Qt
  */
 CanReceiver::CanReceiver(QObject *parent) : QObject(parent) {}

 /**
  * @brief Destructeur de la classe. Arrête l'écoute avant destruction.
  */
 CanReceiver::~CanReceiver() 
 {
     stopListening();
 }

 /**
  * @brief Démarre la lecture des trames CAN dans un thread dédié.
  * @param interfaceName Nom de l'interface CAN à écouter.
  */
 void CanReceiver::startListening(const QString &interfaceName)
 {
    if (m_running) return;

    m_running = true;

    /// Instanciation et lancement du thread de lecture asynchrome
    QThread *thread = QThread::create([this, interfaceName](){
        readLoop(interfaceName);
    })

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();
 }

 /**
  * @brief Demande l'arrêt de la boucle de lecture réseau.
  */
 void CanReceiver::stopListening()
 {
    m_running = false;
 }

 /**
  * @brief Boucle principale exécutée hors du thread IHM pour lire les sockets CAN.
  * @details Ouvre une socket RAW, effectue le blind sur l'interface puis décode les trames.
  * @param interfaceName Nom de l'interface CAN à écouter.
  */
 Void CanReceiver::readLoop(const QString &interfaceName)
 {
    /// Création de la socket CAN native Linux (AF_CAN / PF_CAN)
    int socketFd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socketFd < 0) {
        qWarning() << "Erreur lors de la création de la socket CAN:" << strerror(errno);
        return;
    }
    /// Récupération de l'index de l'interface CAN spécifiée
    ifreq ifr;
    std::strncpy(ifr.ifr_name, interfaceName.toLatin1().constData(), IFNAMSIZ - 1);
    if (ioctl(socketFd, SIOCGIFINDEX, &ifr) < 0) {
        qwarning() << "Erreur ; Interface" << interfaceName << "non trouvée.";
        close(socketFd);
        return;
    }
    
    /// Liaison (bind) de la socket à l'interface CAN
    sockaddr_can addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socketFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        qWarning() << "Erreur lors du bind de la socket CAN:" << interfaceName;
        close(socketFd);
        return;
    }
    qDebug() << "Ecoute activé sur" << interfaceName;

    can_frame frame;

    /// Boucle d'écoute bloquante jusqu'à demande d'arrêt
    while (m_running) {
        ssize_t bytesRead = read(socketFd, &frame, sizeof(struct can_frame));
        if (bytesRead < 0) {
            break;
        }

        /**
         * @note Décodage du message de télémétrie (ID 0x123) :
         * - Byte 0 : Vitesse du véhicule (0 - 240 km/h)
         * - Bytes 1-2 : Régime moteur / RPM (0 - 8000 RPM)
         */
        if (frame.can_id == 0x123 && frame.can_dlc >= 3) {
            int newSpeed = frame.data[0];
            int new Rpm = (frame.data[1] << 8) | frame.data[2];

            if (m_speed != newSpeed) {
                m_speed = newSpeed;
                emit speedUpdated(m_speed);
            }
            if (m_rpm != newRpm) {
                m_rpm = newRpm;
                emit rpmUpdated(m_rpm);
            }
        }

    }

    /// Nettoyage des ressources système à la sortie
    close(socketFd);
    qDebug() << "Ecoute arrêtée sur" << interfaceName;
}
