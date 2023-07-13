#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;

#define limpar system("clear");

// Construir Sinal

void construir_sinal(vector<char> quadro, int tipo_de_codificacao);
void construir_sinal_binario(vector<char> quadro);
void construir_sinal_bipolar(vector<char> quadro);
void construir_sinal_manchester(vector<char> quadro);

string construir_moldura(string linha);