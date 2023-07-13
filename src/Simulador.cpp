#include "../include/CamadaFisica.hpp"
#include "../include/MontagemGrafico.hpp"
#include "../include/Melhorias.hpp"

static void exibir_bits(string rotulo, vector<char>& bits, int limite = 80) {
    cout << "\n>>> " << rotulo << " (" << bits.size() << " bits)" << endl;
    cout << "    ";
    for (size_t i = 0; i < bits.size() && (int)i < limite; i++) {
        cout << bits[i];
        if ((i + 1) % 8 == 0 && i + 1 < bits.size()) cout << " ";
    }
    if ((int)bits.size() > limite) cout << " ...";
    cout << endl;
}

static void exibir_separador(string titulo) {
    cout << "\n=============== " << titulo << " ===============" << endl;
}

void executar_simulacao() {
    srand(time(NULL));

    string mensagem_enviada = aplicacao_transmissora();

    int codificacao;
    cout << "\nCodificacao: [0] Binaria | [1] Manchester | [2] Bipolar | [3] 4B/5B" << endl;
    cout << "Escolha: ";
    cin >> codificacao;
    cin.ignore();

    int deteccao;
    cout << "\nDeteccao de erro: [0] Nenhuma | [1] Paridade | [2] CRC-8 | [3] Hamming" << endl;
    cout << "Escolha: ";
    cin >> deteccao;
    cin.ignore();

    int usar_enquadramento;
    cout << "\nEnquadramento com bit stuffing? [0] Nao | [1] Sim" << endl;
    cout << "Escolha: ";
    cin >> usar_enquadramento;
    cin.ignore();

    double taxa_ruido;
    cout << "\nTaxa de ruido (0.0 = sem ruido, 0.01 = 1%, 0.05 = 5%): ";
    cin >> taxa_ruido;
    cin.ignore();

    // ============ TRANSMISSAO ============
    exibir_separador("TRANSMISSAO");

    vector<char> quadro = camada_de_aplicacao_transmissora(mensagem_enviada);
    int tamanho_original = quadro.size();
    exibir_bits("1. Mensagem em bits", quadro);

    // Deteccao de erro (transmissor)
    if (deteccao > 0) {
        string nomes_det[] = {"", "Paridade", "CRC-8", "Hamming(7,4)"};
        switch (deteccao) {
            case 1: quadro = adicionar_paridade(quadro); break;
            case 2: quadro = adicionar_crc8(quadro); break;
            case 3: quadro = codificar_hamming(quadro); break;
        }
        exibir_bits("2. Apos " + nomes_det[deteccao], quadro);
    }

    // Enquadramento
    if (usar_enquadramento == 1) {
        quadro = aplicar_enquadramento(quadro);
        exibir_bits("3. Apos enquadramento (bit stuffing)", quadro);
    }

    // Codificacao
    vector<char> trem_de_bits;
    string nomes_cod[] = {"Binaria", "Manchester", "Bipolar", "4B/5B"};
    if (codificacao == 3) {
        trem_de_bits = codificar_4b5b(quadro);
    } else {
        trem_de_bits = camada_fisica_transmissora(quadro, codificacao);
    }
    int tamanho_codificado = trem_de_bits.size();
    exibir_bits("4. Apos codificacao " + nomes_cod[codificacao], trem_de_bits);

    // ============ MEIO DE COMUNICACAO ============
    exibir_separador("CANAL DE COMUNICACAO");

    vector<char> trem_recebido;
    if (taxa_ruido > 0) {
        trem_recebido = meio_de_comunicacao_com_ruido(trem_de_bits, taxa_ruido);
        exibir_bits("5. Sinal apos ruido", trem_recebido);

        // Comparar visualmente bits alterados
        int erros = 0;
        cout << "    ";
        for (size_t i = 0; i < trem_de_bits.size() && i < 80; i++) {
            if (trem_de_bits[i] != trem_recebido[i]) {
                cout << "^";
                erros++;
            } else {
                cout << " ";
            }
            if ((i + 1) % 8 == 0) cout << " ";
        }
        if (erros > 0) cout << " (" << erros << " erro(s))";
        cout << endl;
    } else {
        trem_recebido = meio_de_comunicacao(trem_de_bits);
        cout << ">>> 5. Canal sem ruido — sinal intacto" << endl;
    }

    // Grafico do sinal
    if (codificacao <= 2) {
        cout << "\n--- Forma de onda do sinal ---" << endl;
        construir_sinal(trem_recebido, codificacao);
    }

    // ============ RECEPCAO ============
    exibir_separador("RECEPCAO");

    // Decodificacao
    vector<char> quadro_recebido;
    if (codificacao == 3) {
        quadro_recebido = decodificar_4b5b(trem_recebido);
    } else {
        quadro_recebido = camada_fisica_receptora(trem_recebido, codificacao);
    }
    exibir_bits("6. Apos decodificacao " + nomes_cod[codificacao], quadro_recebido);

    // Remover enquadramento
    if (usar_enquadramento == 1) {
        quadro_recebido = remover_enquadramento(quadro_recebido);
        exibir_bits("7. Apos remover enquadramento", quadro_recebido);
    }

    // Deteccao de erro (receptor)
    if (deteccao > 0) {
        string nomes_det[] = {"", "Paridade", "CRC-8", "Hamming(7,4)"};
        switch (deteccao) {
            case 1: {
                vector<char> dados;
                verificar_paridade(quadro_recebido, dados);
                quadro_recebido = dados;
                break;
            }
            case 2: {
                vector<char> dados;
                verificar_crc8(quadro_recebido, dados);
                quadro_recebido = dados;
                break;
            }
            case 3: {
                quadro_recebido = decodificar_hamming(quadro_recebido);
                break;
            }
        }
        exibir_bits("8. Apos verificacao " + nomes_det[deteccao], quadro_recebido);
    }

    // Recepcao
    string mensagem_recebida = camada_de_aplicacao_receptora(quadro_recebido);

    exibir_separador("RESULTADO");
    cout << "Mensagem original:  " << mensagem_enviada << endl;
    cout << "Mensagem recebida:  " << mensagem_recebida << endl;
    cout << "Mensagens iguais:   " << (mensagem_enviada == mensagem_recebida ? "SIM" : "NAO") << endl;

    // Metricas
    exibir_metricas(codificacao, tamanho_original, tamanho_codificado);
}

int main(void) {
    int opcao;

    do {
        cout << "\n========== SIMULADOR DE CAMADA FISICA ==========" << endl;
        cout << "[1] Executar simulacao" << endl;
        cout << "[0] Sair" << endl;
        cout << "Escolha: ";
        cin >> opcao;
        cin.ignore();

        if (opcao == 1) {
            executar_simulacao();
        }
    } while (opcao != 0);

    return 0;
}
