# Simulador de Codificacao da Camada Fisica

Projeto da disciplina **Teleinformatica e Redes 1** do Departamento de Ciencia da Computacao da Universidade de Brasilia. Simulador em C++ que codifica e decodifica mensagens em tres esquemas de sinalizacao da camada fisica (Binaria, Manchester e Bipolar), exibindo o sinal resultante como grafico ASCII no terminal.

---

## Sumario

- [Participantes](#participantes)
- [Tecnologias](#tecnologias)
- [Escopo do Projeto](#escopo-do-projeto)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Requisitos](#requisitos)
- [Como Executar](#como-executar)
- [Codificacoes Implementadas](#codificacoes-implementadas)
- [Arquitetura](#arquitetura)
- [Exemplos de Execucao](#exemplos-de-execucao)
- [Melhorias Implementadas](#melhorias-implementadas)

---

## Participantes

| Nome                              | Matricula |
|-----------------------------------|-----------|
| Gustavo Vieira de Araujo          | 211068440 |
| Pedro Ximenes                     | 200026071 |

---

## Tecnologias

| Tecnologia | Uso                                              |
|------------|--------------------------------------------------|
| C++        | Linguagem de implementacao                       |
| g++ (GCC)  | Compilador                                       |
| make       | Automacao de build                               |

---

## Escopo do Projeto

| Requisito                                          | Implementacao                                                     |
|----------------------------------------------------|-------------------------------------------------------------------|
| Codificacao binaria (NRZ)                          | Mapeamento direto de bits para niveis alto/baixo                  |
| Codificacao Manchester                             | Transicao no meio do bit (0 = subida, 1 = descida)               |
| Codificacao Bipolar (AMI)                          | Zeros em nivel zero, uns alternando entre +1 e -1                 |
| **Extra:** Codificacao 4B/5B                       | Mapeamento de 4 bits para 5 bits com tabela padrao                |
| Decodificacao de todos os esquemas                 | Processo inverso para cada codificacao                            |
| Visualizacao grafica do sinal                      | Grafico ASCII no terminal com moldura                             |
| Simulacao de transmissao e recepcao                | Mensagem → quadro → codificacao → meio → decodificacao → mensagem |
| **Extra:** Simulacao de ruido                      | Inversao aleatoria de bits com taxa configuravel                  |
| **Extra:** Deteccao de erro com paridade           | Bit de paridade par por byte                                      |
| **Extra:** Deteccao de erro com CRC-8              | Checksum por redundancia ciclica (polinomio x^8+x^2+x+1)        |
| **Extra:** Correcao de erro com Hamming(7,4)       | Detecta e corrige automaticamente erros de 1 bit                  |
| **Extra:** Enquadramento com bit stuffing          | Flags 01111110 + insercao de 0 apos 5 uns consecutivos            |
| **Extra:** Metricas de comparacao                  | Overhead, eficiencia, sincronizacao e componente DC               |
| **Extra:** Modo interativo                         | Loop de simulacoes sem reiniciar o programa                       |

---

## Estrutura do Projeto

| Diretorio / Arquivo            | Descricao                                                         |
|--------------------------------|-------------------------------------------------------------------|
| `src/Simulador.cpp`            | Ponto de entrada — menu interativo, orquestra o pipeline           |
| `src/CamadaFisica.cpp`         | Codificacao e decodificacao (Binaria, Manchester, Bipolar)         |
| `src/MontagemGrafico.cpp`      | Construcao dos graficos ASCII para cada tipo de sinal              |
| `src/Melhorias.cpp`            | 4B/5B, ruido, paridade, CRC-8, Hamming, bit stuffing, metricas    |
| `include/CamadaFisica.hpp`     | Declaracoes das funcoes da camada fisica                           |
| `include/MontagemGrafico.hpp`  | Declaracoes das funcoes de grafico                                 |
| `include/Melhorias.hpp`        | Declaracoes das funcoes de melhorias                               |
| `makefile`                     | Compilacao, execucao e limpeza                                     |

---

## Requisitos

- Compilador C++: `g++` (GCC)
- `make` (opcional)

```bash
# Ubuntu/Debian
sudo apt install build-essential
```

---

## Como Executar

```bash
# Compilar
make

# Executar
make run

# Limpar binario
make clean
```

Sem make:

```bash
g++ -std=c++11 -Wall src/*.cpp -o executar
./executar
```

O programa pede uma mensagem e o tipo de codificacao (0 = Binaria, 1 = Manchester, 2 = Bipolar), exibe o sinal codificado como grafico ASCII e mostra a mensagem decodificada.

---

## Codificacoes Implementadas

### Binaria (NRZ)

Mapeamento direto: bit `1` = nivel alto, bit `0` = nivel baixo. Sem transicoes adicionais.

### Manchester

Cada bit e representado por uma transicao no meio do intervalo:
- `0` → subida (baixo para alto)
- `1` → descida (alto para baixo)

Garante sincronizacao de clock embutida no sinal.

### Bipolar (AMI)

- `0` → nivel zero (sem sinal)
- `1` → alterna entre +1 e -1 a cada ocorrencia

Elimina a componente DC do sinal.

---

## Arquitetura

```
Simulador.cpp (main — menu interativo)
    |
    +--- CamadaFisica (codificacao / decodificacao)
    |        +--- Binaria, Manchester, Bipolar
    |        +--- aplicacao_transmissora / receptora
    |        +--- meio_de_comunicacao
    |
    +--- Melhorias (funcionalidades extras)
    |        +--- codificar/decodificar_4b5b
    |        +--- meio_de_comunicacao_com_ruido
    |        +--- adicionar/verificar_paridade
    |        +--- adicionar/verificar_crc8
    |        +--- codificar/decodificar_hamming
    |        +--- aplicar/remover_enquadramento
    |        +--- exibir_metricas
    |
    +--- MontagemGrafico (visualizacao ASCII)
             +--- construir_sinal_binario
             +--- construir_sinal_manchester
             +--- construir_sinal_bipolar
```

---

## Exemplos de Execucao

### Formas de onda por codificacao

**Binaria (NRZ)** — nivel alto = 1, nivel baixo = 0:

```
  Bits: 0   1   0   0   0   0   0   1   0   1   0   0   0   0   1   0   0   1   0   0   0   0   1   1
   H      ┌───┐                   ┌───┐   ┌───┐               ┌───┐       ┌───┐               ┌──────
   L  ────┘   └───────────────────┘   └───┘   └───────────────┘   └───────┘   └───────────────┘
```

**Manchester** — transicao no meio: 0 = subida, 1 = descida:

```
  Bits: 0     1     0     0     0     0     0     1     0     1     0     0     0     0     1     0
   H     ┌─────┐     ┌──   ┌──   ┌──   ┌──   ┌─────┐     ┌─────┐     ┌──   ┌──   ┌──   ┌─────┐
   L  ───┘     └─────┘  ───┘  ───┘  ───┘  ───┘     └─────┘     └─────┘  ───┘  ───┘  ───┘     └─────
```

**Bipolar (AMI)** — zeros em 0, uns alternando entre +1 e -1:

```
  Bits: 0   1   0   0   0   0   0   1   0   1   0   0   0   0   1   0   0   1   0   0   0   0   1   1
  +1      ┌───┐                           ┌───┐                           ┌───┐                   ┌──
   0  ────┘   └───────────────────┐   ┌───┘   └───────────────┐   ┌───────┘   └───────────────┐   │
  -1                              └───┘                       └───┘                           └───┘
```

### Pipeline completo — Binaria, sem ruido

```
=============== TRANSMISSAO ===============

>>> 1. Mensagem em bits (24 bits)
    01000001 01000010 01000011

>>> 4. Apos codificacao Binaria (24 bits)
    01000001 01000010 01000011

=============== CANAL DE COMUNICACAO ===============
>>> 5. Canal sem ruido — sinal intacto

  Bits: 0   1   0   0   0   0   0   1   0   1   0   0   0   0   1   0   0   1   0   0   0   0   1   1
   H      ┌───┐                   ┌───┐   ┌───┐               ┌───┐       ┌───┐               ┌──────
   L  ────┘   └───────────────────┘   └───┘   └───────────────┘   └───────┘   └───────────────┘

=============== RECEPCAO ===============

>>> 6. Apos decodificacao Binaria (24 bits)
    01000001 01000010 01000011

=============== RESULTADO ===============
Mensagem original:  ABC
Mensagem recebida:  ABC
Mensagens iguais:   SIM
```

### Pipeline completo — Manchester + Hamming + enquadramento

Mensagem `"Ola"` passando por todas as camadas: Hamming(7,4), enquadramento com bit stuffing e codificacao Manchester:

```
=============== TRANSMISSAO ===============

>>> 1. Mensagem em bits (24 bits)
    01001111 01101100 01100001

>>> 2. Apos Hamming(7,4) (42 bits)
    10011001 11111111 00110011 11001100 11011010 01

>>> 3. Apos enquadramento (bit stuffing) (59 bits)
    01111110 10011001 11110111 10011001 11100110 01101101 00101111 110

>>> 4. Apos codificacao Manchester (118 bits)
    01101010 10101001 10010110 10010110 10101010 01101010 10010110 ...

=============== RECEPCAO ===============

>>> 6. Apos decodificacao Manchester (59 bits)
>>> 7. Apos remover enquadramento (42 bits)
[HAMMING] Nenhum erro detectado.
>>> 8. Apos verificacao Hamming(7,4) (24 bits)
    01001111 01101100 01100001

=============== RESULTADO ===============
Mensagem original:  Ola
Mensagem recebida:  Ola
Mensagens iguais:   SIM

------ METRICAS ------
Bits originais:   24
Bits codificados: 118
Overhead:         391.67%
Eficiencia:       20.34%
Codificacao:      Manchester
Sincronizacao:    Sim (clock embutido)
Componente DC:    Nao
----------------------
```

Note como 24 bits originais viram 42 apos Hamming, 59 apos enquadramento e 118 apos Manchester.

---

## Melhorias Implementadas

Alem das codificacoes obrigatorias (Binaria, Manchester, Bipolar), foram adicionadas funcionalidades extras para tornar o simulador mais completo e proximo de um cenario real de transmissao de dados.

### Codificacao 4B/5B

Esquema de codificacao que mapeia cada grupo de 4 bits em um simbolo de 5 bits, garantindo transicoes suficientes no sinal para sincronizacao de clock. Reduz a eficiencia para 80% mas elimina sequencias longas de zeros que causam perda de sincronismo.

| Entrada (4 bits) | Saida (5 bits) |
|------------------|----------------|
| `0000`           | `11110`        |
| `0001`           | `01001`        |
| `1111`           | `11101`        |

### Simulacao de Ruido

O `meio_de_comunicacao()` foi estendido para introduzir erros aleatorios no sinal transmitido, simulando interferencias do canal fisico. Uma taxa de erro configuravel (ex: 1%) inverte bits aleatoriamente, permitindo observar o impacto do ruido na mensagem recebida.

### Deteccao de Erro com Bit de Paridade

Adiciona um bit de paridade (par ou impar) a cada byte antes da codificacao. Na recepcao, o bit de paridade e verificado para detectar erros de 1 bit por byte. Erros detectados sao sinalizados no terminal.

### CRC (Cyclic Redundancy Check)

Implementacao de deteccao de erro por redundancia ciclica usando polinomio divisor (CRC-8). O transmissor calcula o checksum e o anexa ao quadro; o receptor recalcula e compara, detectando erros de rajada de ate 8 bits.

### Codigo de Hamming

Codificacao que nao apenas detecta mas **corrige automaticamente** erros de 1 bit. Bits de verificacao sao inseridos em posicoes de potencia de 2 (1, 2, 4, 8, ...) e calculados por XOR sobre grupos especificos de bits de dados. Na recepcao, a sindrome identifica a posicao exata do bit errado.

### Enquadramento com Bit Stuffing

Delimita quadros usando flags de inicio e fim (`01111110`). Para evitar que a sequencia da flag apareca nos dados, aplica bit stuffing: insere um `0` apos cada sequencia de 5 bits `1` consecutivos. O receptor remove os bits inseridos antes de decodificar.

### Metricas de Comparacao

Ao final da simulacao, exibe uma tabela comparativa entre as codificacoes:

| Metrica                    | Binaria | Manchester | Bipolar |
|----------------------------|---------|------------|---------|
| Bits por simbolo           | 1       | 0.5        | 1       |
| Overhead                   | 0%      | 100%       | 0%      |
| Sincronizacao de clock     | Nao     | Sim        | Parcial |
| Componente DC              | Sim     | Nao        | Nao     |

### Modo Interativo

Loop de chat onde o usuario pode enviar multiplas mensagens consecutivas, escolhendo a codificacao para cada uma, sem reiniciar o programa. Cada mensagem passa por todo o pipeline (codificacao → transmissao → decodificacao) com exibicao do grafico.
