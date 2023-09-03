from ultralytics import YOLO
import cvzone
import cv2
import math
import os
from pathlib import Path
from imgur.working_imgur import upload_image
import datetime
import requests


BASE_DIR = Path(__file__).resolve().parent

image_directory = os.path.join(BASE_DIR, "saved_infer_image")


# Running real time from webcam
cap = cv2.VideoCapture(0)
model = YOLO("fire.pt")

past = datetime.datetime.now()

# Reading the classes
classnames = ["fire"]

capture_image = True

while True:
    ret, frame = cap.read()
    frame = cv2.resize(frame, (640, 480))
    result = model(frame, stream=True)

    # Getting bbox,confidence and class names informations to work with
    for info in result:
        boxes = info.boxes
        for box in boxes:
            confidence = box.conf[0]
            confidence = math.ceil(confidence * 100)
            Class = int(box.cls[0])
            if confidence > 50:
                x1, y1, x2, y2 = box.xyxy[0]
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 0, 255), 5)
                cvzone.putTextRect(
                    frame,
                    f"{classnames[Class]} {confidence}%",
                    [x1 + 8, y1 + 100],
                    scale=1.5,
                    thickness=2,
                )

                if confidence > 65 and capture_image:
                    success, image = cap.read()
                    if success:
                        past = datetime.datetime.now()
                        cv2.imwrite(f"{image_directory}/cap_img.png", image)
                        result = upload_image(f"{image_directory}/cap_img.png")
                        phone_no = "+9779861285984"
                        try:
                            res = requests.get(
                                f"http://192.168.18.218/firedetected?number={phone_no}&data={result}"
                            )
                        except Exception as e:
                            pass
                        capture_image = False

    time_diff = datetime.datetime.now() - past
    if time_diff.total_seconds() / 60 > 1:
        capture_image = True
    cv2.imshow("frame", frame)
    cv2.waitKey(1)
