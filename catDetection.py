
import cv2
import numpy as np
import urllib.request
import tkinter as tk
from tkinter import Button, simpledialog, Toplevel
from PIL import Image, ImageTk
import datetime
import paho.mqtt.client as mqtt

url = 'http://192.168.0.106/cam-hi.jpg'

cap = cv2.VideoCapture(url)
whT = 320
confThreshold = 0.5
nmsThreshold = 0.3
classesfile = 'coco.names'
classNames = []
with open(classesfile, 'rt') as f:
    classNames = f.read().rstrip('\n').split('\n')

modelConfig = 'yolov3.cfg'
modelWeights = 'yolov3.weights'
net = cv2.dnn.readNetFromDarknet(modelConfig, modelWeights)
net.setPreferableBackend(cv2.dnn.DNN_BACKEND_OPENCV)
net.setPreferableTarget(cv2.dnn.DNN_TARGET_CPU)

# Thời gian hẹn giờ cho việc nhận diện mèo
scheduled_time = None

# Tạo cửa sổ giao diện người dùng
root = tk.Tk()
root.title("Cat Feeding System")

# Hàm hiển thị cửa sổ nhập thời gian
def show_schedule_dialog():
    global scheduled_time
    scheduled_time = simpledialog.askstring("Hẹn giờ", "Nhập thời gian hẹn giờ (định dạng HH:MM):")
    scheduled_time = datetime.datetime.strptime(scheduled_time, "%H:%M").time()
    print(scheduled_time)

# Tạo nút "Cho ăn ngay"
feed_now_button = Button(root, text="Cho ăn ngay", command=lambda: feed_now())
feed_now_button.pack()

# Tạo nút "Hẹn giờ"
schedule_feed_button = Button(root, text="Hẹn giờ", command=lambda: show_schedule_dialog())
schedule_feed_button.pack()

# Tạo khung hiển thị hình ảnh
image_label = tk.Label(root)
image_label.pack()

# Khởi tạo MQTT client
client = mqtt.Client("python_pub")

def send_signal_to_esp32():
    # Gửi tín hiệu đến ESP32 để cấp điện cho một chân GPIO
    client.connect("broker.hivemq.com", 1883)
    client.publish("esp32/gpio", "ON")
    client.disconnect()

def feed_now():
    # Gửi tín hiệu đến ESP32 ngay lập tức
    send_signal_to_esp32()

def findObject(outputs, im):
    hT, wT, cT = im.shape
    bbox = []
    classIds = []
    confs = []
    found_cat = False

    for output in outputs:
        for det in output:
            scores = det[5:]
            classId = np.argmax(scores)
            confidence = scores[classId]
            if confidence > confThreshold and classNames[classId] == 'cat':
                w, h = int(det[2] * wT), int(det[3] * hT)
                x, y = int((det[0] * wT) - w / 2), int((det[1] * hT) - h / 2)
                bbox.append([x, y, w, h])
                classIds.append(classId)
                confs.append(float(confidence))

    indices = cv2.dnn.NMSBoxes(bbox, confs, confThreshold, nmsThreshold)

    for idx in indices:
        i = idx.item()  # Chuyển đổi numpy scalar thành Python scalar
        box = bbox[i]
        x, y, w, h = box[0], box[1], box[2], box[3]

        cv2.rectangle(im, (x, y), (x + w, y + h), (255, 0, 255), 2)
        cv2.putText(im, f'{classNames[classIds[i]].upper()} {int(confs[i] * 100)}%', (x, y - 10),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 255), 2)

        found_cat = True

    if found_cat and scheduled_time is not None:
        # Nếu đã hẹn giờ và nhận diện được mèo, kiểm tra xem thời gian hiện tại có phù hợp không
        current_time = datetime.datetime.now().time()
        if scheduled_time <= current_time < (datetime.datetime.combine(datetime.date.today(), scheduled_time) + datetime.timedelta(minutes=1)).time():
            send_signal_to_esp32()

def update_stream():
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    im = cv2.imdecode(imgnp, -1)
    success, img = cap.read()
    blob = cv2.dnn.blobFromImage(im, 1 / 255, (whT, whT), [0, 0, 0], 1, crop=False)
    net.setInput(blob)
    layernames = net.getLayerNames()
    outputNames = [layernames[i - 1] for i in net.getUnconnectedOutLayers()]
    outputs = net.forward(outputNames)
    findObject(outputs, im)

    # Chuyển đổi hình ảnh từ OpenCV sang định dạng có thể hiển thị được trong Tkinter
    im = cv2.cvtColor(im, cv2.COLOR_BGR2RGB)
    im = Image.fromarray(im)
    im = ImageTk.PhotoImage(im)

    # Hiển thị hình ảnh lên giao diện người dùng
    image_label.config(image=im)
    image_label.image = im

    # Lặp lại hàm cập nhật sau mỗi khoảng thời gian nhất định (ví dụ: 100ms)
    root.after(100, update_stream)

# Bắt đầu vòng lặp chính của giao diện người dùng
update_stream()
root.mainloop()
