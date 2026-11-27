Then use a **MobileNetV2 classifier**, int8 quantized, at 224x224. Train it on your own classes on a PC or Google Colab, then run it on the Pi.

**1. Prepare images** in one folder per class, such as `data/cat/` and `data/dog/`. Include a `data/nothing/` class, because the model always picks one of the classes it knows.

**2. Train on the PC** (`pip install tensorflow`):

```python
import tensorflow as tf
from tensorflow.keras import layers

ds = tf.keras.utils.image_dataset_from_directory("data", image_size=(224, 224), batch_size=16)
names = ds.class_names

base = tf.keras.applications.MobileNetV2(input_shape=(224, 224, 3), include_top=False, weights="imagenet")
base.trainable = False

inp = layers.Input((224, 224, 3))
x = layers.Rescaling(1/127.5, offset=-1)(inp)
x = base(x, training=False)
x = layers.GlobalAveragePooling2D()(x)
x = layers.Dropout(0.2)(x)
out = layers.Dense(len(names), activation="softmax")(x)
model = tf.keras.Model(inp, out)
model.compile("adam", "sparse_categorical_crossentropy", metrics=["accuracy"])
model.fit(ds, epochs=10)

def samples():
    for img, _ in ds.unbatch().batch(1).take(100):
        yield [img]

conv = tf.lite.TFLiteConverter.from_keras_model(model)
conv.optimizations = [tf.lite.Optimize.DEFAULT]
conv.representative_dataset = samples
conv.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
conv.inference_input_type = tf.uint8
conv.inference_output_type = tf.uint8
open("model.tflite", "wb").write(conv.convert())
open("labels.txt", "w").write("\n".join(names))
```

**3. Run on the Pi** (64-bit OS, in a virtual environment: `pip install ai-edge-litert opencv-python-headless numpy`). Copy over `model.tflite` and `labels.txt`.

```python
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
    print(f"{labels[i]}  {scores[i]:.2f}  {took*1000:.0f} ms")
    time.sleep(max(0, 0.2 - took))  # cap at 5 FPS
```

I haven't been able to run these here. The camera code assumes a USB webcam; the ribbon-cable Pi camera needs `picamera2` instead.

If you only need common things (the 1,000 ImageNet categories), you can skip training and use a pretrained MobileNetV2. What are your classes, and which camera do you have?

---


