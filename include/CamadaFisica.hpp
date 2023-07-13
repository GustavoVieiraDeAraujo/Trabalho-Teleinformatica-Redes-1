#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;

#define limpar system("clear");

// Suporte

string converter_mensagem_em_bit(string mensagem);

// Transmissão

string aplicacao_transmissora(void);
vector<char> camada_de_aplicacao_transmissora(string mensagem);
vector<char> camada_fisica_transmissora(vector<char> quadro, int tipo_de_codificacao);
vector<char> camada_fisica_transmissora_codificacao_bipolar(vector<char> quadro);
vector<char> camada_fisica_transmissora_codificacao_binaria(vector<char> quadro);
vector<char> camada_fisica_transmissora_codificacao_manchester(vector<char> quadro);

// Comunicação

vector<char> meio_de_comunicacao(vector<char> fluxo_bruto_de_bits);

// Recepçao

vector<char> camada_fisica_receptora(vector<char> quadro, int tipo_de_codificacao);
string camada_de_aplicacao_receptora(vector<char> quadro);
vector<char> camada_fisica_receptora_decodificacao_bipolar(vector<char> quadro);
vector<char> camada_fisica_receptora_decodificacao_binaria(vector<char> quadro);
vector<char> camada_fisica_receptora_decodificacao_manchester(vector<char> quadro);
void aplicacao_receptora(string mensagem);