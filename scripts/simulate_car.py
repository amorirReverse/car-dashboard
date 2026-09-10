#!/usr/bin/env python3
import time
import can

def simulate_cockpit():
    try:
        bus = can.interface.Bus(channel='vcan0', bustype='socketcan')
        print("🚗 Simulation démarrée sur vcan0... (Appuyez sur Ctrl+C pour arrêter)")
        
        speed = 0
        rpm = 800
        gear = 1

        while True:
            # Envoi de la trame CAN ID 0x123
            rpm_bytes = rpm.to_bytes(2, byteorder='big')
            data = [speed, rpm_bytes[0], rpm_bytes[1]]
            msg = can.Message(arbitration_id=0x123, data=data, is_extended_id=False)
            
            bus.send(msg)
            
            # Simulation d'accélération
            speed += 1
            rpm += 40

            if rpm > 6000 and gear < 6:
                gear += 1
                rpm = 2500 # Chute de régime au passage de rapport

            if speed > 220:
                speed = 0
                rpm = 800
                gear = 1

            time.sleep(0.05) # 20 Hz

    except KeyboardInterrupt:
        print("\n🛑 Simulation arrêtée.")
    except OSError:
        print("❌ Erreur : vcan0 n'est pas activé. Exécutez 'sudo ip link add dev vcan0 type vcan && sudo ip link set up vcan0'")

if __name__ == "__main__":
    simulate_cockpit()