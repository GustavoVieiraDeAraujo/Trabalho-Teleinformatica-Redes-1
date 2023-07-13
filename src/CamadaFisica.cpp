#include "../include/CamadaFisica.hpp"
#include "../include/MontagemGrafico.hpp"

#include <string>

// Suporte

// Função com o objetivo de converter uma string em uma sequência de bits
string converter_mensagem_em_bit(string mensagem) {
    string quadro;
    for (char caracter : mensagem) {
        int caracter_em_decimal = static_cast<unsigned char>(caracter); // TTransformação da mensagem em um quadro.
        for (int i = 7; i >= 0; --i) {
            quadro += ((caracter_em_decimal & (1 << i)) ? '1' : '0');
        }
    }
    return quadro;
}


// Transmissão

// Função para receber a mensagem da simulação, enviada pelo usuário via terminal
string aplicacao_transmissora(void) {
    string mensagem;
    cout << "Digite uma mensagem:" << endl;
    getline(cin, mensagem); // Mensagem a a ser encaminhada
    cout << "A mensagem e: ";
    cout << mensagem << endl;

    return mensagem;
}

// Função cujo o objetivo é transformar a mensagem :string em um quadro :vector<char>, convertendo os caracteres em bytes, seguindo o padrão ASCII e concatenando os bytes no quadro.
vector<char> camada_de_aplicacao_transmissora(string mensagem) {
    vector<char> quadro;

    for (char byte : converter_mensagem_em_bit(mensagem)) {
        quadro.push_back(byte);
    }

    return quadro;
}


// Iniciando a Codificação, com base no padrão desejado
vector<char> camada_fisica_transmissora(vector<char> quadro, int tipo_de_codificacao) {
    vector<char> fluxo_bruto_de_bits;

    switch (tipo_de_codificacao) {
    case 0: // Codificação Binária
        fluxo_bruto_de_bits = camada_fisica_transmissora_codificacao_binaria(quadro);
        break;
    case 1: // Codificação Manchester
        fluxo_bruto_de_bits = camada_fisica_transmissora_codificacao_manchester(quadro);
        break;
    case 2: // Codificação Bipolar
        fluxo_bruto_de_bits = camada_fisica_transmissora_codificacao_bipolar(quadro);
        break;
    }

    return fluxo_bruto_de_bits; // Quadro de bits codificado.
}


// Transformação em Sinal Binário
vector<char> camada_fisica_transmissora_codificacao_binaria(vector<char> fluxo_bruto_de_bits) {
    vector<char> fluxo_bruto_de_bits_codificado;

    for (char bit_fluxo : fluxo_bruto_de_bits) {
        fluxo_bruto_de_bits_codificado.push_back(bit_fluxo); // Considera-se o mesmo fluxo do quadro bruto.
    }

    return fluxo_bruto_de_bits_codificado; // Sinal Binário codificado.
}


// Transformação em Sinal Manchester
vector<char> camada_fisica_transmissora_codificacao_manchester(vector<char> fluxo_bruto_de_bits) {
    vector<char> fluxo_bruto_de_bits_codificado;

    for (char bit : fluxo_bruto_de_bits) {
        if (bit == '0') { // Transforma o sinal de baixo nível em sinal de subida de nível
            fluxo_bruto_de_bits_codificado.push_back('0');
            fluxo_bruto_de_bits_codificado.push_back('1');
        }
        else { // Transforma o sinal de alto nível em sinal de queda de nível
            fluxo_bruto_de_bits_codificado.push_back('1');
            fluxo_bruto_de_bits_codificado.push_back('0');
        }
    }

    return fluxo_bruto_de_bits_codificado; // Sinal Manchester codificado.
}


// Transformação em Sinal Bipolar
vector<char> camada_fisica_transmissora_codificacao_bipolar(vector<char> fluxo_bruto_de_bits) {
    string qual_foi_ultimo_bit = "-1";
    string construir_quadro_codificado;

    for (char bit : fluxo_bruto_de_bits) {
        if (bit == '0') { // Não altera
            construir_quadro_codificado += "0";
        }
        else { // Muda sempre intercalando um sinal positivo e um sinal negativo.
            construir_quadro_codificado += ((qual_foi_ultimo_bit == "1") ? "-1" : "1");
            qual_foi_ultimo_bit = ((qual_foi_ultimo_bit == "1") ? "-1" : "1");
        }
    }

    vector<char> fluxo_bruto_de_bits_codificado(construir_quadro_codificado.begin(), construir_quadro_codificado.end()); // Transformação do quadro representado em string em um quadro representado em vetor de caractéres.

    return fluxo_bruto_de_bits_codificado; // Sinal bipolar codificado.
}


// Comunicação

// Transformação de um trem de bit transmissor para um trem de bit receptor.
vector<char> meio_de_comunicacao(vector<char> fluxo_bruto_de_bits) {
    vector<char> fluxo_bruto_de_bits_ponto_a, fluxo_bruto_de_bits_ponto_b;
    fluxo_bruto_de_bits_ponto_a = fluxo_bruto_de_bits; // Trem de bit receptor

    // Ambos os vetores possuem o mesmo valor, esta etapa tem como objivo representar um meio de comunicação entre o dispositivo que está transmitindo e o dispositivo que está recebendo
    for (char bit : fluxo_bruto_de_bits_ponto_a) { 
        fluxo_bruto_de_bits_ponto_b.push_back(bit);
    }
    
    return fluxo_bruto_de_bits_ponto_b; // Trem de bit transmissor
}


// Recepção

// Iniciando a Decodificação, com base no padrão desejado
vector<char> camada_fisica_receptora(vector<char> quadro, int tipo_de_decodificacao) {
    vector<char> fluxo_bruto_de_bits;

    switch (tipo_de_decodificacao) {
    case 0: // Decodificação Binária
        fluxo_bruto_de_bits = camada_fisica_receptora_decodificacao_binaria(quadro);
        break;
    case 1: // Decodificação Manchester
        fluxo_bruto_de_bits = camada_fisica_receptora_decodificacao_manchester(quadro);
        break;
    case 2: // Decodificação Bipolar
        fluxo_bruto_de_bits = camada_fisica_receptora_decodificacao_bipolar(quadro);
        break;
    }

    return fluxo_bruto_de_bits; // Quadro de bits decodificado.
}


// Decodificação Sinal Binário
vector<char> camada_fisica_receptora_decodificacao_binaria(vector<char> quadro) {
    string construir_quadro_decodificado;

    for (int i = 0; i < quadro.size(); i++) {
        construir_quadro_decodificado += quadro[i]; // Considera-se o mesmo fluxo do quadro codificado.
    }

    vector<char> fluxo_bruto_de_bits_decodificado(construir_quadro_decodificado.begin(), construir_quadro_decodificado.end());
    return fluxo_bruto_de_bits_decodificado; // Sinal Binário decodificado.
}


// Decodificação Sinal Manchester
vector<char> camada_fisica_receptora_decodificacao_manchester(vector<char> quadro) {
    string construir_quadro_decodificado;

    for (int i = 0; i < quadro.size(); i += 2) {
        string clock;
        for (int j = i; (j < (i + 2)); j++) {
            clock += quadro[j];
        }
        if (clock == "01") { // Transforma o sinal de subida de nível em sinal de baixo nível
            construir_quadro_decodificado += "0";
        } else { // Transforma o sinal de queda de nível em sinal de alto nível
            construir_quadro_decodificado += "1";
        }
    }

    vector<char> fluxo_bruto_de_bits_decodificado(construir_quadro_decodificado.begin(), construir_quadro_decodificado.end());

    return fluxo_bruto_de_bits_decodificado; // Sinal Manchester decodificado.
}


// Decodificação Sinal Bipolar
vector<char> camada_fisica_receptora_decodificacao_bipolar(vector<char> quadro) {
    string construir_quadro_decodificado;

    for (int i = 0; i < quadro.size(); i++) {
        if (quadro[i] == '0') { // Não altera
            construir_quadro_decodificado += '0';

        } else if (quadro[i] == '-') { // Recebe o módulo do valor para retirada de sinal.
            construir_quadro_decodificado += '1';
            i += 1;

        } else {
            construir_quadro_decodificado += '1';
        }
    }

    vector<char> fluxo_bruto_de_bits_decodificado(construir_quadro_decodificado.begin(), construir_quadro_decodificado.end());

   
    return fluxo_bruto_de_bits_decodificado; // Sinal bipolar decodificado.
}


// Função cujo o objetivo é transformar um quadro :vector<char> em uma mensagem :string, convertendo os bytes em caracteres, seguindo o padrão ASCII e concatenando os carateres na mensagem.
string camada_de_aplicacao_receptora(vector<char> quadro) {
    string mensagem;

    for (int i = 0; i < quadro.size(); i += 8) {
        string byte;
        for (int j = i; (j < (i + 8)); j++) {
            byte += quadro[j];
        }

        int valor = stoi(byte, nullptr, 2);
        char caractere = static_cast<char>(valor);
        mensagem += caractere;
    }

    return mensagem;
}


// Função para imprime no terminal a mensagem da simulação, enviada pelo usuário.
void aplicacao_receptora(string mensagem) {
    cout << "A mensagem recebida foi: " << mensagem << endl; // Mensagem recebida
}
