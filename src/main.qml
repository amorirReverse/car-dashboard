/**
 * @file main.qml
 * @brief Interface graphique utilisateur du tableau de bord numérique.
 * @details Affiche deux jauges circulaires interactives (RPM et Vitesse) 
 * liées en temps réel au singleton C++ CanReceiver.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CarDashboard.Backend 1.0

ApplicationWindow {
    id: window
    width: 1024
    height: 600
    visible: true
    title: "Digital Cockpit - SocketCAN Receiver"
    color: "#0f111a"

    /** @brief Arrière-plan dégradé du cockpit */
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#16192b" }
            GradientStop { position: 1.0; color: "#090a10" }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 20

        // =============================================================
        // 1. JAUGE RÉGIME MOTEUR (RPM)
        // =============================================================
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                anchors.centerIn: parent
                width: Math.min(parent.width, parent.height) * 0.85
                height: width
                radius: width / 2
                color: "transparent"
                border.color: "#2a2e45"
                border.width: 8

                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width - 20
                    height: width
                    radius: width / 2
                    color: "#131524"
                    border.color: CanReceiver.rpm > 6000 ? "#ff0055" : "#00f0ff"
                    border.width: 3

                    Behavior on border.color {
                        ColorAnimation { duration: 200 }
                    }
                }

                /** @brief Aiguille du compte-tours */
                Rectangle {
                    id: rpmNeedle
                    width: 4
                    height: parent.width / 2 - 25
                    color: CanReceiver.rpm > 6000 ? "#ff0055" : "#00f0ff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.verticalCenter
                    transformOrigin: Item.Bottom
                    rotation: -120 + (Math.min(CanReceiver.rpm, 8000) / 8000) * 240

                    Behavior on rotation {
                        SmoothedAnimation { velocity: 250 }
                    }
                }

                Column {
                    anchors.centerIn: parent
                    spacing: 2

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: CanReceiver.rpm
                        color: "#ffffff"
                        font.pixelSize: 36
                        font.bold: true
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "RPM"
                        color: "#8a8fbc"
                        font.pixelSize: 14
                    }
                }
            }
        }

        // =============================================================
        // 2. BLOC CENTRAL DE STATUT
        // =============================================================
        ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            spacing: 20

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "SOCKETCAN VCAN0"
                color: "#00f0ff"
                font.pixelSize: 12
                font.letterSpacing: 2
                font.bold: true
            }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width: 180
                height: 2
                color: "#2a2e45"
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 10

                Rectangle {
                    width: 10
                    height: 10
                    radius: 5
                    color: "#00ff66"
                }
                Text {
                    text: "BUS ACTIF"
                    color: "#ffffff"
                    font.pixelSize: 12
                    font.bold: true
                }
            }
        }

        // =============================================================
        // 3. JAUGE DE VITESSE (KM/H)
        // =============================================================
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                anchors.centerIn: parent
                width: Math.min(parent.width, parent.height) * 0.85
                height: width
                radius: width / 2
                color: "transparent"
                border.color: "#2a2e45"
                border.width: 8

                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width - 20
                    height: width
                    radius: width / 2
                    color: "#131524"
                    border.color: "#00f0ff"
                    border.width: 3
                }

                /** @brief Aiguille du tachymètre */
                Rectangle {
                    id: speedNeedle
                    width: 4
                    height: parent.width / 2 - 25
                    color: "#00f0ff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.verticalCenter
                    transformOrigin: Item.Bottom
                    rotation: -120 + (Math.min(CanReceiver.speed, 240) / 240) * 240

                    Behavior on rotation {
                        SmoothedAnimation { velocity: 200 }
                    }
                }

                Column {
                    anchors.centerIn: parent
                    spacing: 2

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: CanReceiver.speed
                        color: "#ffffff"
                        font.pixelSize: 42
                        font.bold: true
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "KM/H"
                        color: "#8a8fbc"
                        font.pixelSize: 14
                    }
                }
            }
        }
    }
}