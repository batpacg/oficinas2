import time
import cv2
import numpy as np
from ai_edge_litert.interpreter import Interpreter

labels = open("labels.txt").read().splitlines()
net = Interpreter("model.tflite", num_threads=4)
net.allocate_tensors()
inp = net.get_input_details()[0]
out = net.get_output_details()[0]

cam = cv2.VideoCapture(0)
cam.set(cv2.CAP_PROP_BUFFERSIZE, 1)

while True:
    start = time.time()
    ok, frame = cam.read()
    if not ok:
        break
    img = cv2.resize(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB), (224, 224))
    net.set_tensor(inp["index"], img[np.newaxis])
    net.invoke()
    scale, zero = out["quantization"]
    scores = (net.get_tensor(out["index"])[0].astype(np.float32) - zero) * scale
    i = int(scores.argmax())
    took = time.time() - start
    print(f"{labels[i]}  {scores[i]:.2f}  {took * 1000:.0f} ms")
    time.sleep(max(0, 0.2 - took))  # cap at 5 FPS
