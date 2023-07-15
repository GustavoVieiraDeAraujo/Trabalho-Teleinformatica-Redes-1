#include "../include/Melhorias.hpp"

// ========================== 4B/5B ==========================

static map<string, string> tabela_4b5b = {
    {"0000", "11110"}, {"0001", "01001"}, {"0010", "10100"}, {"0011", "10101"},
    {"0100", "01010"}, {"0101", "01011"}, {"0110", "01110"}, {"0111", "01111"},
    {"1000", "10010"}, {"1001", "10011"}, {"1010", "10110"}, {"1011", "10111"},
    {"1100", "11010"}, {"1101", "11011"}, {"1110", "11100"}, {"1111", "11101"}
};

static map<string, string> tabela_5b4b;

static void inicializar_tabela_inversa() {
    if (tabela_5b4b.empty()) {
        for (auto& par : tabela_4b5b) {
            tabela_5b4b[par.second] = par.first;
        }
    }
}

vector<char> codificar_4b5b(vector<char> quadro) {
    vector<char> codificado;

    // Padding para multiplo de 4
    while (quadro.size() % 4 != 0) {
        quadro.push_back('0');
    }

    for (size_t i = 0; i < quadro.size(); i += 4) {
        string nibble(quadro.begin() + i, quadro.begin() + i + 4);
        string codigo = tabela_4b5b[nibble];
        for (char c : codigo) {
            codificado.push_back(c);
        }
    }

    return codificado;
}

vector<char> decodificar_4b5b(vector<char> quadro) {
    inicializar_tabela_inversa();
    vector<char> decodificado;

    for (size_t i = 0; i + 4 < quadro.size(); i += 5) {
        string codigo(quadro.begin() + i, quadro.begin() + i + 5);
        if (tabela_5b4b.count(codigo)) {
            string nibble = tabela_5b4b[codigo];
            for (char c : nibble) {
                decodificado.push_back(c);
            }
        }
    }

    return decodificado;
}


// ========================== Simulacao de Ruido ==========================

vector<char> meio_de_comunicacao_com_ruido(vector<char> fluxo, double taxa_erro) {
    vector<char> fluxo_com_ruido;
    int erros_inseridos = 0;

    for (size_t i = 0; i < fluxo.size(); i++) {
        char bit = fluxo[i];
        double aleatorio = (double)rand() / RAND_MAX;
        if (aleatorio < taxa_erro && (bit == '0' || bit == '1')) {
            bit = (bit == '0') ? '1' : '0';
            erros_inseridos++;
        }
        fluxo_com_ruido.push_back(bit);
    }

    if (erros_inseridos > 0) {
        cout << "[RUIDO] " << erros_inseridos << " bit(s) invertido(s) durante a transmissao." << endl;
    } else {
        cout << "[RUIDO] Nenhum erro introduzido." << endl;
    }

    return fluxo_com_ruido;
}


// ========================== Bit de Paridade ==========================

vector<char> adicionar_paridade(vector<char> quadro) {
    vector<char> com_paridade;

    for (size_t i = 0; i < quadro.size(); i += 8) {
        int contagem_uns = 0;
        for (size_t j = i; j < i + 8 && j < quadro.size(); j++) {
            com_paridade.push_back(quadro[j]);
            if (quadro[j] == '1') contagem_uns++;
        }
        com_paridade.push_back((contagem_uns % 2 == 0) ? '0' : '1');
    }

    return com_paridade;
}

bool verificar_paridade(vector<char> quadro, vector<char>& dados_sem_paridade) {
    bool valido = true;
    dados_sem_paridade.clear();
    int byte_num = 0;

    for (size_t i = 0; i + 8 < quadro.size(); i += 9) {
        int contagem_uns = 0;
        byte_num++;
        for (size_t j = i; j < i + 8; j++) {
            dados_sem_paridade.push_back(quadro[j]);
            if (quadro[j] == '1') contagem_uns++;
        }
        char paridade_esperada = (contagem_uns % 2 == 0) ? '0' : '1';
        if (quadro[i + 8] != paridade_esperada) {
            cout << "[PARIDADE] Erro detectado no byte " << byte_num << "!" << endl;
            valido = false;
        }
    }

    if (valido) {
        cout << "[PARIDADE] Nenhum erro detectado." << endl;
    }

    return valido;
}


// ========================== CRC-8 ==========================

static vector<char> calcular_resto_crc(vector<char> dados) {
    // Polinomio CRC-8: x^8 + x^2 + x + 1 = 100000111
    string polinomio = "100000111";
    int grau = polinomio.size() - 1;

    // Copia dos dados com 8 zeros no final
    vector<char> dividendo(dados.begin(), dados.end());
    for (int i = 0; i < grau; i++) {
        dividendo.push_back('0');
    }

    // Divisao por XOR
    for (size_t i = 0; i <= dividendo.size() - polinomio.size(); i++) {
        if (dividendo[i] == '1') {
            for (size_t j = 0; j < polinomio.size(); j++) {
                dividendo[i + j] = ((dividendo[i + j] - '0') ^ (polinomio[j] - '0')) + '0';
            }
        }
    }

    // Resto sao os ultimos 8 bits
    vector<char> resto(dividendo.end() - grau, dividendo.end());
    return resto;
}

vector<char> adicionar_crc8(vector<char> quadro) {
    vector<char> crc = calcular_resto_crc(quadro);
    vector<char> com_crc(quadro.begin(), quadro.end());
    for (char c : crc) {
        com_crc.push_back(c);
    }
    return com_crc;
}

bool verificar_crc8(vector<char> quadro, vector<char>& dados_sem_crc) {
    vector<char> resto = calcular_resto_crc(quadro);
    bool valido = true;

    for (size_t i = 0; i < resto.size(); i++) {
        if (resto[i] != '0') {
            valido = false;
            break;
        }
    }

    dados_sem_crc.assign(quadro.begin(), quadro.end() - 8);

    if (valido) {
        cout << "[CRC-8] Nenhum erro detectado." << endl;
    } else {
        cout << "[CRC-8] Erro detectado no quadro!" << endl;
    }

    return valido;
}


// ========================== Hamming(7,4) ==========================

vector<char> codificar_hamming(vector<char> quadro) {
    vector<char> codificado;

    // Padding para multiplo de 4
    while (quadro.size() % 4 != 0) {
        quadro.push_back('0');
    }

    for (size_t i = 0; i < quadro.size(); i += 4) {
        int d1 = quadro[i] - '0';
        int d2 = quadro[i + 1] - '0';
        int d3 = quadro[i + 2] - '0';
        int d4 = quadro[i + 3] - '0';

        int p1 = d1 ^ d2 ^ d4;
        int p2 = d1 ^ d3 ^ d4;
        int p3 = d2 ^ d3 ^ d4;

        // Ordem: p1 p2 d1 p3 d2 d3 d4
        codificado.push_back(p1 + '0');
        codificado.push_back(p2 + '0');
        codificado.push_back(d1 + '0');
        codificado.push_back(p3 + '0');
        codificado.push_back(d2 + '0');
        codificado.push_back(d3 + '0');
        codificado.push_back(d4 + '0');
    }

    return codificado;
}

vector<char> decodificar_hamming(vector<char> quadro) {
    vector<char> decodificado;
    int erros_corrigidos = 0;

    for (size_t i = 0; i + 6 < quadro.size(); i += 7) {
        int b[7];
        for (int j = 0; j < 7; j++) {
            b[j] = quadro[i + j] - '0';
        }

        // Sindrome
        int s1 = b[0] ^ b[2] ^ b[4] ^ b[6];
        int s2 = b[1] ^ b[2] ^ b[5] ^ b[6];
        int s3 = b[3] ^ b[4] ^ b[5] ^ b[6];

        int posicao_erro = s1 * 1 + s2 * 2 + s3 * 4;

        if (posicao_erro != 0) {
            b[posicao_erro - 1] ^= 1;
            erros_corrigidos++;
        }

        // Extrair dados: posicoes 3, 5, 6, 7 (indices 2, 4, 5, 6)
        decodificado.push_back(b[2] + '0');
        decodificado.push_back(b[4] + '0');
        decodificado.push_back(b[5] + '0');
        decodificado.push_back(b[6] + '0');
    }

    if (erros_corrigidos > 0) {
        cout << "[HAMMING] " << erros_corrigidos << " erro(s) corrigido(s) automaticamente." << endl;
    } else {
        cout << "[HAMMING] Nenhum erro detectado." << endl;
    }

    return decodificado;
}


// ========================== Enquadramento com Bit Stuffing ==========================

vector<char> aplicar_enquadramento(vector<char> quadro) {
    vector<char> enquadrado;
    string flag = "01111110";

    // Flag de inicio
    for (char c : flag) enquadrado.push_back(c);

    // Bit stuffing: inserir 0 apos 5 uns consecutivos
    int consecutivos = 0;
    for (size_t i = 0; i < quadro.size(); i++) {
        enquadrado.push_back(quadro[i]);
        if (quadro[i] == '1') {
            consecutivos++;
            if (consecutivos == 5) {
                enquadrado.push_back('0');
                consecutivos = 0;
            }
        } else {
            consecutivos = 0;
        }
    }

    // Flag de fim
    for (char c : flag) enquadrado.push_back(c);

    return enquadrado;
}

vector<char> remover_enquadramento(vector<char> quadro) {
    vector<char> dados;

    // Remover flags (8 bits no inicio e 8 no fim)
    if (quadro.size() < 16) return dados;

    // Remover bit stuffing
    int consecutivos = 0;
    for (size_t i = 8; i < quadro.size() - 8; i++) {
        dados.push_back(quadro[i]);
        if (quadro[i] == '1') {
            consecutivos++;
            if (consecutivos == 5) {
                i++; // Pula o 0 inserido
                consecutivos = 0;
            }
        } else {
            consecutivos = 0;
        }
    }

    return dados;
}


// ========================== Metricas ==========================

void exibir_metricas(int tipo_codificacao, int tamanho_original, int tamanho_codificado) {
    cout << "\n------ METRICAS ------" << endl;
    cout << "Bits originais:   " << tamanho_original << endl;
    cout << "Bits codificados: " << tamanho_codificado << endl;

    double overhead = ((double)(tamanho_codificado - tamanho_original) / tamanho_original) * 100.0;
    double eficiencia = ((double)tamanho_original / tamanho_codificado) * 100.0;

    cout << "Overhead:         " << overhead << "%" << endl;
    cout << "Eficiencia:       " << eficiencia << "%" << endl;

    string nomes[] = {"Binaria", "Manchester", "Bipolar", "4B/5B"};
    if (tipo_codificacao >= 0 && tipo_codificacao <= 3) {
        cout << "Codificacao:      " << nomes[tipo_codificacao] << endl;
    }

    cout << "Sincronizacao:    ";
    switch (tipo_codificacao) {
        case 0: cout << "Nao" << endl; break;
        case 1: cout << "Sim (clock embutido)" << endl; break;
        case 2: cout << "Parcial" << endl; break;
        case 3: cout << "Sim (transicoes garantidas)" << endl; break;
    }

    cout << "Componente DC:    ";
    switch (tipo_codificacao) {
        case 0: cout << "Sim" << endl; break;
        case 1: cout << "Nao" << endl; break;
        case 2: cout << "Nao" << endl; break;
        case 3: cout << "Depende da codificacao de linha" << endl; break;
    }
    cout << "----------------------" << endl;
}
