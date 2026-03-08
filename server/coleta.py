import paho.mqtt.client as mqtt
import csv
import os
import ast

# Configurações
BROKER = "IP DO BROKER"
TOPIC = "pump/fft"
CSV_FILE = "dataset_bomba.csv"
SAMPLES_TO_COLLECT = 50  # Quantidade de amostras para coletar antes de parar

# Cabeçalho com colunas fft + label (ajuste o tamanho conforme sua FFT)
FFT_LENGTH = 10
HEADER = [f"fft_bin_{i+1}" for i in range(FFT_LENGTH)] + ["label"]

# Variáveis globais
samples_collected = 0
label_class = None
collected_samples = []

# Função para salvar as amostras no CSV (modo append)
def save_samples_to_csv(samples, label):
    file_exists = os.path.isfile(CSV_FILE)
    with open(CSV_FILE, 'a', newline='') as csvfile:
        writer = csv.writer(csvfile)
        if not file_exists:
            writer.writerow(HEADER)  # escreve o cabeçalho só se arquivo não existir
        for sample in samples:
            writer.writerow(sample + [label])
    print(f"{len(samples)} amostras da classe {label} salvas em {CSV_FILE}.")

# Callback quando conecta ao broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado ao broker MQTT com sucesso!")
        client.subscribe(TOPIC)
    else:
        print(f"Falha na conexão. Código: {rc}")

# Callback para mensagens recebidas
def on_message(client, userdata, msg):
    global samples_collected, collected_samples

    try:
        # Decodifica a mensagem recebida (espera uma lista de floats em string)
        data_str = msg.payload.decode('utf-8')
        fft_values = ast.literal_eval(data_str)  # converte string para lista

        if len(fft_values) != FFT_LENGTH:
            print(f"Mensagem com tamanho inválido: {fft_values}")
            return

        collected_samples.append(fft_values)
        samples_collected += 1
        print(f"Amostra {samples_collected} recebida: {fft_values}")

        if samples_collected >= SAMPLES_TO_COLLECT:
            # Salva no CSV e desconecta
            save_samples_to_csv(collected_samples, label_class)
            client.disconnect()

    except Exception as e:
        print(f"Erro ao processar mensagem: {e}")

def main():
    global label_class, samples_collected, collected_samples

    # Entrada do usuário para label da classe
    while True:
        try:
            label_class = int(input("Informe o número da classe (0-2): "))
            if label_class not in [0,1,2]:
                print("Classe inválida. Informe um número entre 0 e 2.")
                continue
            break
        except:
            print("Entrada inválida. Informe um número inteiro.")

    print(f"Colete {SAMPLES_TO_COLLECT} amostras para a classe {label_class}...")

    # Reseta variáveis
    samples_collected = 0
    collected_samples = []

    # Configura cliente MQTT
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    print(f"Conectando ao broker MQTT {BROKER}...")
    client.connect(BROKER)

    client.loop_forever()
    print("Coleta finalizada.")

if __name__ == "__main__":
    main()
