# Smart Pump Monitoring | TinyML & Edge AI 🌊🤖

Este repositório contém o sistema completo de **Manutenção Preditiva** para bombas hidráulicas, desenvolvido com foco em alta performance e baixo consumo de energia. O projeto utiliza **Machine Learning embarcado** para classificar estados de operação e detectar anomalias em tempo real na "borda" (Edge).

---

## 🛠️ Especificações Técnicas

- **Microcontrolador:** RP2040 (Dual-core ARM Cortex-M0+).
- **Plataforma:** Raspberry Pi Pico W.
- **Linguagem:** Desenvolvido integralmente em **C utilizando o Pico SDK** (foco em determinismo e eficiência).
- **Sensor:** MPU6050 (Acelerômetro e Giroscópio) via barramento **I2C**.
- **Processamento de Sinais:** Implementação de **FFT (Fast Fourier Transform)** para análise espectral de vibração.
- **Conectividade:** Protocolo **MQTT** via Wi-Fi para envio de telemetria e alertas de estado.
- **Machine Learning:** Modelo de rede neural otimizado com **TensorFlow Lite for Microcontrollers (TFLM)**.

---

## 📂 Estrutura do Projeto

- `inc/` & `src/`: Firmware robusto em C (Pico SDK).
- `models/`: Arquivo de inferência `.tflite` convertido para array C.
- `server/`: Infraestrutura de backend (Python) para recepção e log de dados via MQTT.
- `training/`: Scripts em **Python** utilizados para o processamento de sinais, treinamento e validação do modelo.

---

## 🤖 Inteligência Artificial e Monitoramento

O modelo de TinyML foi treinado para identificar e classificar o estado do ativo em três categorias principais:

1. **Desligado:** Monitoramento de disponibilidade e tempo de inatividade.
2. **Operação Normal:** Baseline de vibração e funcionamento ideal do equipamento.
3. **Anomalia:** Identificação de desvios nos padrões de vibração que indicam falhas iminentes.

---

## 📊 Impacto Industrial (MTBF & MTTR)

O sistema foi projetado para elevar o **MTBF (Mean Time Between Failures)** através do monitoramento contínuo. Ao detectar o estado de **Anomalia** em tempo real, o sistema permite uma resposta proativa da equipe de manutenção, reduzindo drasticamente o **MTTR (Mean Time To Repair)** e evitando paradas catastróficas não planejadas.



---
*Pesquisa e desenvolvimento baseados no artigo "Monitoramento inteligente em bombas hidráulicas utilizando machine learning embarcado", apresentado no Universo IF 2025.*
