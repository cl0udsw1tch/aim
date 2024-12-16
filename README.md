# AIM

[AI Manipulator] This project enables manipulation of rendered Blender models via hand signs read by your webcame and processed by a CNN model. It's comprised of two parts, the model code (HandSigns Classifier) and the rendering code (Graphics Engine). 

# HandSigns Classifier

This subrepository is a simple implementation of a Convolutional Neural Network (CNN) model that recognizes hand signs using OpenCV and a custom neural network structure. It includes data preprocessing, model training, and real-time prediction functionality.

## Dependencies

Ensure you have the following libraries installed:

- OpenCV
- Numpy
- Tensorflow (or your preferred deep learning backend)
- Any additional dependencies for the custom code (NeuralNetStructure, preprocessor, etc.)

You can install dependencies using:

```bash
pip install opencv-python numpy tensorflow
```

## Dataset

This implementation uses the "HandSigns" dataset, which you should place in the `datasets/HandSigns` directory. The dataset consists of images of hand gestures for various signs:

- ARM_CROSS
- BACK_HAND
- FIST
- FRONT_HAND
- POINT_LEFT
- POINT_RIGHT
- PRAN_MUDRA

Each class is organized in its respective folder within the `datasets/HandSigns/Classes` directory.

## Preprocessing

Images are resized to (100, 100) for model input. To preprocess the dataset for training, the following method is used:

```python
prep = Preprocessor("HandSigns", (100, 100))
X, y, X_test, y_test = prep.preprocess(0.80, "2D")
```

This splits the data into training and testing sets with 80% of the data used for training.

## Model Training

Once preprocessing is completed, the model is constructed as follows:

```python
model = nnet.Model("2D", (100, 100))
model.add(layer.Layer_Conv(input_channels=3, num_filters=1, filter_dims = (5, 5), stride=(1, 1), weight_regularizer_L2=0, bias_regularizer_L2=0), "CONV")
model.add(activation.Activation_ReLU(), "CONV")
model.add(pool.Pool(pool_dims=(2, 2), stride=(2, 2)), "CONV")
model.add(layer.Layer_Conv(input_channels=1, num_filters=3, filter_dims = (5, 5), stride=(1, 1), weight_regularizer_L2=0, bias_regularizer_L2=0), "CONV")
model.add(activation.Activation_ReLU(), "CONV")
model.add(pool.Pool(pool_dims=(2, 2), stride=(2, 2)), "CONV")
model.add(layer.Layer_Flatten(), "CONV")
```

The training is conducted by calling:

```python
model.train(X, y_hot, epochs=100, batch_size=64, validation_data=(X_test, y_test_hot))
```

You can save the model's parameters after training:

```python
model.save("models/handsigns_classifier")
```

## Predicting on New Data

After saving the model, it can be loaded again for inference:

```python
model = nnet.Model.load("models/handsigns_classifier")
model.predict(np.expand_dims(X[0], 0))
```

For real-time prediction via webcam, run the provided loop below. This captures webcam input, resizes the image to match the input size, and outputs the model’s predictions on screen:

```python
cap = cv2.VideoCapture(0)
font = cv2.FONT_HERSHEY_SIMPLEX
bottomLeftCornerOfText = (250,250)
fontScale = 0.5
fontColor = (72,251,0)
thickness = 1
lineType = 2

while True:
    ret, frame = cap.read()
    img = cv2.resize(frame[100:300, 220:420, :], (100, 100))
    img = ImageTools.channelLastToFirst(img)
    output = model.forward(np.expand_dims(img, 0), False)
    prediction = CLASSES[np.argmax(output)] if np.max(output) > 0.99 else "NONE"

    cv2.putText(frame, f'{prediction}', bottomLeftCornerOfText, font, fontScale, fontColor, thickness, lineType)

    cv2.imshow("test", frame[100:300, 220:420, :])

    if cv2.waitKey(1) == 27:  # Press Esc to exit
        cv2.destroyAllWindows()
        break
```

## Model Architecture

The model architecture is a custom CNN with the following layers:

- Conv Layer (5x5 kernel, 1 filter)
- ReLU Activation
- Pooling (2x2)
- Conv Layer (5x5 kernel, 3 filters)
- ReLU Activation
- Pooling (2x2)
- Flatten Layer
- Dense Layers (64 neurons, dropout layer included)
- Final Dense Layer (7 neurons, one for each class)

### Saving/Loading Model

To save model parameters, use:

```python
model.save_parameters("params/handsigns.params")
```

To load a saved model:

```python
model = nnet.Model.load("models/handsigns_classifier")
```

# Graphics-Engine

This repository hosts a versatile and performant graphics engine written in C++ and built using CMake. The project is optimized for Visual Studio users, though it can be adapted for other IDEs.

## Features
- **Shader Programming**: Support for vertex and fragment shaders.
- **Model Rendering**: Integrates mesh and model management for efficient rendering.
- **Camera Controls**: Basic camera functionality included.
- **Utilities**: Helper functions for handling transforms, image processing (using `stb_image`), and OpenGL contexts.
- **Modular Codebase**: Structured to allow easy expansion.

---

## Requirements

### Prerequisites
Ensure you have the following installed:
- C++17 compatible compiler
- CMake (version 3.14 or later)
- Visual Studio (for convenient building and debugging)
- OpenGL runtime environment

### Dependencies
The engine relies on:
- [GLEW](http://glew.sourceforge.net/) for managing OpenGL extensions.
- [GLFW](https://www.glfw.org/) for handling window and input.
- [stb_image](https://github.com/nothings/stb) for image loading.
- GLM for mathematics.

These dependencies are included in the `dependencies/` folder.

---

## Build Instructions

### With Visual Studio (Recommended)

1. Open Visual Studio.
2. Select **File** > **Open** > **Folder...**.
   - Point to the root of the project directory.
3. Allow Visual Studio to detect and configure the project.
4. Select the appropriate build configuration (Debug/Release).
5. Press `Ctrl + Shift + B` to build the project.

### With CMake (CLI)

1. Create a build directory and navigate into it:
   ```sh
   mkdir build && cd build
   ```
2. Run the following commands:
   ```sh
   cmake ..
   cmake --build .
   ```

---

## File Structure

```plaintext
Graphics-Engine/
├── dependencies/
│   ├── include/
│   │   ├── config.h
│   │   ├── eglew.h
│   │   ├── glad/
│   │   └── ...
│   ├── glad.c
│   └── glm/
├── src/
│   ├── Camera.cpp
│   ├── Camera.h
│   ├── Engine.cpp
│   ├── Engine.h
│   ├── Shader.cpp
│   ├── Shader.h
│   └── vendor/
├── CMakeLists.txt
└── README.md
```

---

## Usage

### Running the Engine
After building successfully, you can run the resulting binary from the `build` directory.

### Adding New Features
- Include new headers in the `src/` folder.
- Add the appropriate file paths to the `CMakeLists.txt` file under `Source_Files` or `Header_Files`.
- Rebuild the project.


---


## License
This project is licensed under the MIT License.

