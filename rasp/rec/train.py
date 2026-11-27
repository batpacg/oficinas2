import tensorflow as tf
from tensorflow.keras import layers

ds = tf.keras.utils.image_dataset_from_directory(
    "data", image_size=(224, 224), batch_size=16
)
names = ds.class_names

base = tf.keras.applications.MobileNetV2(
    input_shape=(224, 224, 3), include_top=False, weights="imagenet"
)
base.trainable = False

inp = layers.Input((224, 224, 3))
x = layers.Rescaling(1 / 127.5, offset=-1)(inp)
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
