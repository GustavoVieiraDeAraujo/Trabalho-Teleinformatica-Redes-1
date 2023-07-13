#include "../include/MontagemGrafico.hpp"
#include <string>
#include <algorithm>

static const int MAX_BITS_EXIBIDOS = 32;

// Exibe regua com valores dos bits originais acima da onda
static void exibir_regua(vector<char> quadro, int largura_bit, int num_bits) {
    cout << "  Bits: ";
    for (int i = 0; i < num_bits; i++) {
        cout << quadro[i];
        for (int j = 1; j < largura_bit; j++) cout << " ";
    }
    cout << endl;
}

// Sinal Binario (NRZ) — duas linhas: HIGH e LOW
void construir_sinal_binario(vector<char> quadro) {
    int num_bits = min((int)quadro.size(), MAX_BITS_EXIBIDOS);
    int largura = 4;

    exibir_regua(quadro, largura, num_bits);

    string linha_high = "   H  ";
    string linha_low  = "   L  ";

    char anterior = '0';

    for (int i = 0; i < num_bits; i++) {
        if (quadro[i] == '1') {
            if (anterior == '0') {
                linha_high += "┌";
                linha_low  += "┘";
            } else {
                linha_high += "─";
                linha_low  += " ";
            }
            for (int j = 1; j < largura; j++) {
                linha_high += "─";
                linha_low  += " ";
            }
        } else {
            if (anterior == '1') {
                linha_high += "┐";
                linha_low  += "└";
            } else {
                linha_high += " ";
                linha_low  += "─";
            }
            for (int j = 1; j < largura; j++) {
                linha_high += " ";
                linha_low  += "─";
            }
        }
        anterior = quadro[i];
    }

    cout << linha_high << endl;
    cout << linha_low << endl;

    if ((int)quadro.size() > MAX_BITS_EXIBIDOS) {
        cout << "  ... (" << quadro.size() - MAX_BITS_EXIBIDOS << " bits restantes omitidos)" << endl;
    }
}

// Sinal Manchester — transicao no meio de cada bit
void construir_sinal_manchester(vector<char> quadro) {
    int num_pares = min((int)quadro.size() / 2, MAX_BITS_EXIBIDOS);
    int largura = 3;

    // Regua com bits originais decodificados
    cout << "  Bits: ";
    for (int i = 0; i < num_pares; i++) {
        string par;
        par += quadro[i * 2];
        par += quadro[i * 2 + 1];
        char bit_original = (par == "01") ? '0' : '1';
        cout << bit_original;
        for (int j = 1; j < largura * 2; j++) cout << " ";
    }
    cout << endl;

    string linha_high = "   H  ";
    string linha_low  = "   L  ";

    for (int i = 0; i < num_pares; i++) {
        string par;
        par += quadro[i * 2];
        par += quadro[i * 2 + 1];

        if (par == "01") {
            // 0: baixo -> alto (subida)
            for (int j = 0; j < largura; j++) {
                linha_high += " ";
                linha_low  += "─";
            }
            linha_high += "┌";
            linha_low  += "┘";
            for (int j = 1; j < largura; j++) {
                linha_high += "─";
                linha_low  += " ";
            }
        } else {
            // 1: alto -> baixo (descida)
            for (int j = 0; j < largura; j++) {
                linha_high += "─";
                linha_low  += " ";
            }
            linha_high += "┐";
            linha_low  += "└";
            for (int j = 1; j < largura; j++) {
                linha_high += " ";
                linha_low  += "─";
            }
        }
    }

    cout << linha_high << endl;
    cout << linha_low << endl;

    if ((int)quadro.size() / 2 > MAX_BITS_EXIBIDOS) {
        cout << "  ... (" << quadro.size() / 2 - MAX_BITS_EXIBIDOS << " bits restantes omitidos)" << endl;
    }
}

// Sinal Bipolar (AMI) — tres niveis: +1, 0, -1
void construir_sinal_bipolar(vector<char> quadro) {
    // Contar bits reais (sem '-')
    int bits_reais = 0;
    for (size_t i = 0; i < quadro.size(); i++) {
        if (quadro[i] != '-') bits_reais++;
    }
    int num_bits = min(bits_reais, MAX_BITS_EXIBIDOS);
    int largura = 4;

    // Reconstruir bits originais para a regua
    cout << "  Bits: ";
    int contagem = 0;
    for (size_t i = 0; i < quadro.size() && contagem < num_bits; i++) {
        if (quadro[i] == '-') continue;
        if (quadro[i] == '0') {
            cout << "0";
        } else {
            cout << "1";
        }
        for (int j = 1; j < largura; j++) cout << " ";
        contagem++;
    }
    cout << endl;

    string linha_pos  = "  +1  ";
    string linha_zero = "   0  ";
    string linha_neg  = "  -1  ";

    contagem = 0;
    char anterior_nivel = '0'; // 0 = zero, p = positivo, n = negativo

    for (size_t i = 0; i < quadro.size() && contagem < num_bits; i++) {
        if (quadro[i] == '-') continue;

        char nivel;
        if (quadro[i] == '0') {
            nivel = '0';
        } else if (i > 0 && quadro[i - 1] == '-') {
            nivel = 'n';
        } else {
            nivel = 'p';
        }

        if (nivel == 'p') {
            if (anterior_nivel == '0') {
                linha_pos  += "┌";
                linha_zero += "┘";
                linha_neg  += " ";
            } else if (anterior_nivel == 'n') {
                linha_pos  += "┌";
                linha_zero += "│";
                linha_neg  += "┘";
            } else {
                linha_pos  += "─";
                linha_zero += " ";
                linha_neg  += " ";
            }
            for (int j = 1; j < largura; j++) {
                linha_pos  += "─";
                linha_zero += " ";
                linha_neg  += " ";
            }
        } else if (nivel == 'n') {
            if (anterior_nivel == '0') {
                linha_pos  += " ";
                linha_zero += "┐";
                linha_neg  += "└";
            } else if (anterior_nivel == 'p') {
                linha_pos  += "┐";
                linha_zero += "│";
                linha_neg  += "└";
            } else {
                linha_pos  += " ";
                linha_zero += " ";
                linha_neg  += "─";
            }
            for (int j = 1; j < largura; j++) {
                linha_pos  += " ";
                linha_zero += " ";
                linha_neg  += "─";
            }
        } else {
            if (anterior_nivel == 'p') {
                linha_pos  += "┐";
                linha_zero += "└";
                linha_neg  += " ";
            } else if (anterior_nivel == 'n') {
                linha_pos  += " ";
                linha_zero += "┌";
                linha_neg  += "┘";
            } else {
                linha_pos  += " ";
                linha_zero += "─";
                linha_neg  += " ";
            }
            for (int j = 1; j < largura; j++) {
                linha_pos  += " ";
                linha_zero += "─";
                linha_neg  += " ";
            }
        }

        anterior_nivel = nivel;
        contagem++;
    }

    cout << linha_pos << endl;
    cout << linha_zero << endl;
    cout << linha_neg << endl;

    if (bits_reais > MAX_BITS_EXIBIDOS) {
        cout << "  ... (" << bits_reais - MAX_BITS_EXIBIDOS << " bits restantes omitidos)" << endl;
    }
}

void construir_sinal(vector<char> quadro, int tipo_de_codificacao) {
    cout << endl;
    switch (tipo_de_codificacao) {
        case 0: construir_sinal_binario(quadro); break;
        case 1: construir_sinal_manchester(quadro); break;
        case 2: construir_sinal_bipolar(quadro); break;
    }
    cout << endl;
}
