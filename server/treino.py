import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import classification_report, confusion_matrix
import tensorflow as tf
from tensorflow.keras.callbacks import EarlyStopping

# === CONFIGURAÇÕES ===
CSV_PATH = "dataset_bomba.csv"
HEADER_FILE = "weigths.h"
NORMALIZACAO_FILE = "fft_norm_parms.h"

INPUT_SIZE = 10     # 10 bins da FFT (bins 2 a 11)
HIDDEN_SIZE = 8
OUTPUT_SIZE = 3     # 3 classes: desligado, ligado, anomalia

# === 1. CARREGAR E PRÉ-PROCESSAR DADOS ===
df = pd.read_csv(CSV_PATH, header=0)
X = df.iloc[:, :-1].values  # FFT bins (já sem os 2 primeiros)
y = df.iloc[:, -1].values   # classes

# Normalização padrão (zero média, desvio unitário)
scaler = StandardScaler()
X_norm = scaler.fit_transform(X)

# One-hot encoding das classes
y_categorical = tf.keras.utils.to_categorical(y, num_classes=OUTPUT_SIZE)

# Dividir em treino/teste
X_train, X_test, y_train, y_test = train_test_split(X_norm, y_categorical, test_size=0.2, random_state=42)

# === 2. DEFINIR E TREINAR A REDE ===
model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(INPUT_SIZE,)),
    tf.keras.layers.Dense(HIDDEN_SIZE, activation='relu'),
    tf.keras.layers.Dense(OUTPUT_SIZE, activation='softmax')
])

model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

early_stop = EarlyStopping(
    monitor='val_accuracy',
    mode='max',
    patience=20,
    min_delta=1e-5,
    restore_best_weights=True,
    verbose=1
)

history = model.fit(
    X_train, y_train,
    epochs=100,
    batch_size=8,
    validation_data=(X_test, y_test),
    callbacks=[early_stop]
)

# === 3. AVALIAÇÃO DO MODELO ===
print("\n[Relatório de Classificação]")
y_pred = np.argmax(model.predict(X_test), axis=1)
y_true = np.argmax(y_test, axis=1)
print(classification_report(y_true, y_pred))
print("[Matriz de Confusão]\n", confusion_matrix(y_true, y_pred))

# === 4. EXPORTAR PESOS PARA C ===
def write_weights_to_header(model, filename):
    with open(filename, 'w') as f:
        f.write("// Pesos e bias exportados da rede neural treinada\n")
        f.write("#ifndef PESOS_REDE_NEURAL_H\n#define PESOS_REDE_NEURAL_H\n\n")
        f.write(f"#define INPUT_SIZE {INPUT_SIZE}\n")
        f.write(f"#define HIDDEN_SIZE {HIDDEN_SIZE}\n")
        f.write(f"#define OUTPUT_SIZE {OUTPUT_SIZE}\n\n")

        # Camada oculta
        weights_1 = model.layers[0].get_weights()[0].T  # [HIDDEN_SIZE][INPUT_SIZE]
        biases_1  = model.layers[0].get_weights()[1]
        f.write("const float hidden_weights[HIDDEN_SIZE][INPUT_SIZE] = {\n")
        for row in weights_1:
            f.write("    {" + ", ".join(f"{w:.6f}f" for w in row) + "},\n")
        f.write("};\n\n")
        f.write("const float hidden_biases[HIDDEN_SIZE] = {" + ", ".join(f"{b:.6f}f" for b in biases_1) + "};\n\n")

        # Camada de saída
        weights_2 = model.layers[1].get_weights()[0].T  # [OUTPUT_SIZE][HIDDEN_SIZE]
        biases_2  = model.layers[1].get_weights()[1]
        f.write("const float output_weights[OUTPUT_SIZE][HIDDEN_SIZE] = {\n")
        for row in weights_2:
            f.write("    {" + ", ".join(f"{w:.6f}f" for w in row) + "},\n")
        f.write("};\n\n")
        f.write("const float output_biases[OUTPUT_SIZE] = {" + ", ".join(f"{b:.6f}f" for b in biases_2) + "};\n\n")

        f.write("#endif // PESOS_REDE_NEURAL_H\n")

write_weights_to_header(model, HEADER_FILE)
print(f"✅ Pesos exportados para '{HEADER_FILE}' com sucesso.")

# === 5. EXPORTAR MÉDIAS E DESVIOS PARA NORMALIZAÇÃO EM C ===
def export_scaler_to_c(scaler, filename=NORMALIZACAO_FILE):
    with open(filename, "w") as f:
        f.write("// Médias e desvios dos 10 bins da FFT (para normalização)\n")
        f.write("#ifndef NORMALIZACAO_FFT_H\n#define NORMALIZACAO_FFT_H\n\n")
        f.write(f"#define INPUT_SIZE {INPUT_SIZE}\n\n")
        f.write("const float fft_means[INPUT_SIZE] = {" + ", ".join(f"{m:.6f}f" for m in scaler.mean_) + "};\n")
        f.write("const float fft_scales[INPUT_SIZE] = {" + ", ".join(f"{s:.6f}f" for s in scaler.scale_) + "};\n\n")
        f.write("#endif // NORMALIZACAO_FFT_H\n")

export_scaler_to_c(scaler)
print(f"✅ Normalização exportada para '{NORMALIZACAO_FILE}' com sucesso.")
