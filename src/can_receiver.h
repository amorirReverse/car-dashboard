/**
 * @file CanReceiver.h
 * @brief Classe de gestion de la réception des trames SocketCAN.
 * @author amorirReverse
 * @date 2026
 */

#ifndef CANRECEIVER_H
#define CANRECEIVER_H

#include <QObject>   ///Classe de base de Qt. Nécessaire pour utiliser les signaux et slots.
#include <QThread>  ///Permet de gérer le multithreading avec Qt.
#include <atomic>  ///Fournit des variables thread-safe natives C++ pour éviter les accès concurrents non synchronisés.

/**
 * @class CanReceiver
 * @brief Classe responsable de l'écoute du bus CAN et de l'exposition de la télémétrie à QML.
 * @details Cette classe hérite de QObject et utilise un thread secondaire pour effectuer 
 * des lectures bloquantes sur la socket Linux CAN sans figer l'interface utilisateur.
 */
class CanReceiver : public QObject
{
    Q_OBJECT
    /**
     * @brief Vitesse actuelle du véhicule en km/h
     */
    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)
    /**
     * @brief Régime moteur actuel en RPM
     */
    Q_PROPERTY(int rpm READ rpm NOTIFY rpmChanged)

public:
/**
 * @brief Constructeur de CanReceiver.
 * @param parent Objet parent Qt pour la gestion de l'arbre mémoire.
 */
    explicit CanReceiver(QObject *parent = nullptr);
    /** 
     * @brief Destructeur de CanReceiver. Arrête le thread d'écoute réseau.
     */
    ~CanReceiver();

    /** 
     * @brief Lit la vitesse actuelle du véhicule.
     * @return Vitesse en km/h.
     */
    int speed() const { return m_speed; }
    /**
     * @brief Lit le régime moteur actuel.
     * @return Régime en RPM.
     */
    int rpm() const { return m_rpm; }

    /**
     * @brief Démarre la boucle de lecture asynchrone sur l'interface CAN spécifiée.
     * @param interfaceName : Nom de l'interface CAN à écouter.
     */
    Q_INVOKABLE void startListening(const QString &interfaceName = "vcan0");

    /**
     * @brief Arrête la boucle de lecture du bus CAN.
     */
    Q_INVOKABLE void stopListening();

signals:
    /**
     * @brief Signal émis lorsque la vitesse change.
     * @param newSpeed : Nouvelle vitesse en km/h.
     */
    void speedChanged(int newSpeed);
    /**
     * @brief Signal émis lorsque le régime moteur change.
     * @param newRpm : Nouveau régime en RPM.
     */
    void rpmChanged(int newRpm);

private:
    /**
     * @brief Boucle de lecture du bus CAN exécutée dans un thread séparé.
     * @param interfaceName : Nom de l'interface CAN à écouter.
     */
    void readLoop(const QString &interfaceName);

    std::atomic<bool> m_running; ///< Indique si la boucle de lecture est active.
    int m_speed;                 ///< Vitesse actuelle du véhicule en km/h.
    int m_rpm;                   ///< Régime moteur actuel en RPM.
    QThread m_thread;            ///< Thread secondaire pour la lecture asynchrone.
};
#endif // CANRECEIVER_H