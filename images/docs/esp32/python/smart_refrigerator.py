import cv2
import serial
import time
from datetime import datetime
import csv
import os

# 🔌 SERIAL PORT (CHANGE IF NEEDED)
ser = serial.Serial("COM91", 115200, timeout=1)
time.sleep(2)

# 📷 QR DETECTOR
detector = cv2.QRCodeDetector()

# SESSION
sessionActive = False
currentUser = None
items = {}

SCAN_DELAY = 2
TIMEOUT = 30   # ⏳ Auto logout after 30 sec idle
lastScanTime = 0

cap = None

# 📁 CSV FILE
CSV_FILE = "fridge_log.csv"

# CREATE CSV IF NOT EXISTS
if not os.path.exists(CSV_FILE):
    with open(CSV_FILE, "w", newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Date","Time","User","Item","Qty"])

print("Waiting for RFID...")

# 🎥 CAMERA FUNCTION (AUTO FIX)
def start_camera():
    for i in range(3):   # try index 0,1,2
        cam = cv2.VideoCapture(i)
        if cam.isOpened():
            print(f"✅ Camera opened at index {i}")
            return cam
    print("❌ Camera not found!")
    return None


while True:

    # 📡 SERIAL FROM ESP32
    while ser.in_waiting:
        line = ser.readline().decode().strip()

        # 🟢 SESSION START
        if line.startswith("SESSION_STARTED"):
            _, user = line.split(',')

            currentUser = user
            sessionActive = True
            items = {}
            lastScanTime = time.time()

            print(f"\n===== SESSION STARTED: {user} =====\n")

            cap = start_camera()

        # 🔴 SESSION END → FINAL BILL
        elif line.startswith("SESSION_END"):

            print("\n----- FINAL BILL -----")

            ser.write("BILL_START\n".encode())

            total = 0
            now = datetime.now()

            with open(CSV_FILE, "a", newline='') as f:
                writer = csv.writer(f)

                for item in sorted(items):
                    qty = items[item]

                    writer.writerow([now.date(), now.time(), currentUser, item, qty])

                    print(f"{item} x{qty}")
                    ser.write(f"BILL,{item},{qty}\n".encode())
                    time.sleep(0.3)

                    total += qty

            print(f"\nTotal Items: {total}")
            print("\nThank You! 🙏")
            print("\n===== NEXT USER: START SCANNING =====")

            # SEND TO ESP32
            ser.write(f"TOTAL,{total}\n".encode())
            time.sleep(0.5)
            ser.write("THANKYOU\n".encode())

            # RESET
            sessionActive = False
            items = {}

            if cap:
                cap.release()
                cv2.destroyAllWindows()

        # ❌ SESSION BUSY
        elif "SESSION_BUSY" in line:
            print("❌ Access Denied (Session Active)")

    # 🎥 CAMERA PROCESS
    if sessionActive and cap is not None:

        ret, frame = cap.read()

        if not ret:
            print("❌ Camera frame error")
            continue

        data, bbox, _ = detector.detectAndDecode(frame)

        # 🟩 DRAW BOX
        if bbox is not None:
            bbox = bbox.astype(int)
            for i in range(len(bbox[0])):
                cv2.line(frame,
                         tuple(bbox[0][i]),
                         tuple(bbox[0][(i+1)%len(bbox[0])]),
                         (0,255,0), 3)

        # 📦 QR DETECTED
        if data != "":
            now = time.time()

            if now - lastScanTime > SCAN_DELAY:
                item = data.split('|')[0].strip()

                items[item] = items.get(item, 0) + 1

                print(f"✔ {item} x{items[item]}")

                # SEND TO ESP32
                ser.write(f"ITEM,{item},{items[item]}\n".encode())

                lastScanTime = now

        # ⏳ AUTO LOGOUT
        if time.time() - lastScanTime > TIMEOUT:
            print("\n⏳ AUTO TIMEOUT → Generating Bill")
            ser.write("SESSION_END\n".encode())

        cv2.imshow("Smart Fridge", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

# CLEANUP
if cap:
    cap.release()

cv2.destroyAllWindows()
ser.close()