#include "../include/MontagemGrafico.hpp"
#include <string>

// Construir Sinal

// Função responsável por criar uma moldura para encapsular o gráfico
string construir_moldura(string linha) {
    int tamanho_moldura = linha.length();
    string moldura = "|";
    for (int i = 0; i < tamanho_moldura - 2; i++) {
        moldura += "-";
    }
    moldura += "|";

    return moldura;
}

// Função que monta o gráfico do sinal binário
void construir_sinal_binario(vector<char> quadro) {
    int largura_desenho_em_espaco;
    int maxima_largura_desenho_em_espaco = 40;
    int numero_de_espacos_para_desenhar_bit = 4;

    // Estrutura que limita a representação de 4 caracteres.
    int diferenca = ((quadro.size() * numero_de_espacos_para_desenhar_bit) - maxima_largura_desenho_em_espaco);
    if (diferenca > 0) {
        largura_desenho_em_espaco = maxima_largura_desenho_em_espaco;
    } else {
        largura_desenho_em_espaco = quadro.size();
    }

    // Imprimindo o sinal completo
    for (int i = 0; i < quadro.size(); i++) {
        cout << quadro[i];
    }

    cout << "\n";

    string segunda_linha, terceira_linha, quarta_linha, quinta_linha;
    segunda_linha = terceira_linha = quarta_linha = quinta_linha = "|";

    char ultimo_bit_desenhado = 'i';

    // Construção do Gráfico
    for (int i = 0; i < largura_desenho_em_espaco; i++) {
        if (quadro[i] == '1' && ultimo_bit_desenhado == 'i') {
            segunda_linha += "***";
            terceira_linha += "   ";
            quarta_linha += "   ";
            quinta_linha += "   ";
            ultimo_bit_desenhado = '1';
        } else if (quadro[i] == '0' && ultimo_bit_desenhado == 'i') {
            segunda_linha += "   ";
            terceira_linha += "   ";
            quarta_linha += "   ";
            quinta_linha += "***";
            ultimo_bit_desenhado = '0';
        } else if (quadro[i] == '1' && quadro[i] != ultimo_bit_desenhado) {
            segunda_linha += "****";
            terceira_linha += "*   ";
            quarta_linha += "*   ";
            quinta_linha += "*   ";
            ultimo_bit_desenhado = '1';
        } else if (quadro[i] == '1' && quadro[i] == ultimo_bit_desenhado) {
            segunda_linha += "***";
            terceira_linha += "   ";
            quarta_linha += "   ";
            quinta_linha += "   ";
            ultimo_bit_desenhado = '1';
        } else if (quadro[i] == '0' && quadro[i] != ultimo_bit_desenhado) {
            segunda_linha += "*   ";
            terceira_linha += "*   ";
            quarta_linha += "*   ";
            quinta_linha += "****";
            ultimo_bit_desenhado = '0';
        } else if (quadro[i] == '0' && quadro[i] == ultimo_bit_desenhado) {
            segunda_linha += "   ";
            terceira_linha += "   ";
            quarta_linha += "   ";
            quinta_linha += "***";
            ultimo_bit_desenhado = '0';
        }
    }

    string moldura = construir_moldura(segunda_linha);

    segunda_linha += "|";
    terceira_linha += "|";
    quarta_linha += "|";
    quinta_linha += "|";

    // Imprimindo gráfico no terminal
    cout << moldura << endl;
    cout << segunda_linha << endl;
    cout << terceira_linha << endl;
    cout << quarta_linha << endl;
    cout << quinta_linha << endl;
    cout << moldura << endl;
}

// Função que monta o gráfico do sinal manchester
void construir_sinal_manchester(vector<char> quadro) {
    int largura_desenho_em_espaco;
    int maxima_largura_desenho_em_espaco = 40;

    // Estrutura que limita a representação de 4 caracteres.
    int diferenca = (quadro.size() - maxima_largura_desenho_em_espaco);
    if (diferenca > 0) {
        largura_desenho_em_espaco = maxima_largura_desenho_em_espaco;
    } else {
        largura_desenho_em_espaco = quadro.size();
    }

    string segunda_linha = "|";
    string terceira_linha = "|";
    string quarta_linha = "|";
    string quinta_linha = "|";

    // Imprimindo o sinal completo
    for (int i = 0; i < quadro.size(); i++) {
        cout << quadro[i];
    }

    cout << "\n";

    // Construção do Gráfico
    for (int i = 0; i < largura_desenho_em_espaco; i += 2) {
        string bit_codificado;
        for (int j = i; (j < (i + 2)); j++) {
            bit_codificado += quadro[j];
        }
        terceira_linha += "  *  ";
        quarta_linha += "  *  ";
        if (bit_codificado == "01") {
            segunda_linha += "  ***";
            quinta_linha += "***  ";
        } else {
            segunda_linha += "***  ";
            quinta_linha += "  ***";
        }
    }
    segunda_linha += "|";
    terceira_linha += "|";
    quarta_linha += "|";
    quinta_linha += "|";

    string moldura = construir_moldura(quinta_linha);

    // Imprimindo gráfico no terminal
    cout << moldura << endl;
    cout << segunda_linha << endl;
    cout << terceira_linha << endl;
    cout << quarta_linha << endl;
    cout << quinta_linha << endl;
    cout << moldura << endl;
}

// Função que monta o gráfico do sinal bipolar
void construir_sinal_bipolar(vector<char> quadro) {
    int largura_desenho_em_espaco;
    int maxima_largura_desenho_em_espaco = 40;
    int quantidade_de_traco = 0;

    // Estrutura que limita a representação de 4 caracteres.
    for (int i = 0; i < quadro.size(); i++) {
        if (quadro[i] == '-') {
            quantidade_de_traco += 1;
        }
    }

    int diferenca = (((quadro.size() - quantidade_de_traco)) - maxima_largura_desenho_em_espaco);
    if (diferenca > 0) {
        largura_desenho_em_espaco = maxima_largura_desenho_em_espaco;
    } else {
        largura_desenho_em_espaco = quadro.size();
    }

    string segunda_linha, terceira_linha, quarta_linha, quinta_linha, sexta_linha, setima_linha, oitava_linha;
    segunda_linha = terceira_linha = quarta_linha = quinta_linha = sexta_linha = setima_linha = oitava_linha = "|";

    // Imprimindo o sinal completo
    for (int i = 0; i < quadro.size(); i++) {
        cout << quadro[i];
    }

    cout << "\n";

    // Construção do Gráfico
    for (int i = 0; i < largura_desenho_em_espaco; i++) {
        if (quadro[i] == '-') {
            continue;
        }
        terceira_linha += "   ";
        quarta_linha   += "   ";
        sexta_linha    += "   ";
        setima_linha   += "   ";
        if (quadro[i] == '0') {
            segunda_linha += "   ";
            quinta_linha  += "***";
            oitava_linha  += "   ";
        } else if (quadro[i] == '1' && quadro[i - 1] != '-') {
            segunda_linha += "***";
            quinta_linha  += "   ";
            oitava_linha  += "   ";
        } else if (quadro[i] == '1' && quadro[i - 1] == '-') {
            segunda_linha += "   ";
            quinta_linha  += "   ";
            oitava_linha  += "***";
        }
        
        if (i < largura_desenho_em_espaco - 1 && quadro[i] != '-') {
            if (quadro[i] == '0' && quadro[i + 1] == '0') {
                continue;
            } else if (quadro[i] == '1' && quadro[i + 1] != '0') {
                segunda_linha  += "*";
                terceira_linha += "*";
                quarta_linha   += "*";
                quinta_linha   += "*";
                sexta_linha    += "*";
                setima_linha   += "*";
                oitava_linha   += "*";
            } else if (quadro[i - 1] != '-' && quadro[i + 1] != '-') {
                segunda_linha  += "*";
                terceira_linha += "*";
                quarta_linha   += "*";
                quinta_linha   += "*";
                sexta_linha    += " ";
                setima_linha   += " ";
                oitava_linha   += " ";
            } else {
                segunda_linha  += " ";
                terceira_linha += " ";
                quarta_linha   += " ";
                quinta_linha   += "*";
                sexta_linha    += "*";
                setima_linha   += "*";
                oitava_linha   += "*";
            }
        }
    }
        segunda_linha += "|";
        terceira_linha += "|";
        quarta_linha += "|";
        quinta_linha += "|";
        sexta_linha += "|";
        setima_linha += "|";
        oitava_linha += "|";

        string moldura = construir_moldura(quinta_linha);

        // Imprimindo gráfico no terminal
        cout << moldura << endl;
        cout << segunda_linha << endl;
        cout << terceira_linha << endl;
        cout << quarta_linha << endl;
        cout << quinta_linha << endl;
        cout << sexta_linha << endl;
        cout << setima_linha << endl;
        cout << oitava_linha << endl;
        cout << moldura << endl;
}

// Função que administra a construção do gráfico de acordo com a codificação desejada.
void construir_sinal(vector<char> quadro, int tipo_de_codificacao) {
    switch (tipo_de_codificacao)
    {
    case 0: // Sinal Binário
        construir_sinal_binario(quadro);
        break;
    case 1: // Sinal Manchester
        construir_sinal_manchester(quadro);
        break;
    case 2: // Sinal Bipolar
        construir_sinal_bipolar(quadro);
        break;
    }
}