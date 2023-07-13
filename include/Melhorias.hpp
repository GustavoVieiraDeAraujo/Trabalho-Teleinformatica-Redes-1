#ifndef MELHORIAS_HPP
#define MELHORIAS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>

using namespace std;

// 4B/5B
vector<char> codificar_4b5b(vector<char> quadro);
vector<char> decodificar_4b5b(vector<char> quadro);

// Simulacao de Ruido
vector<char> meio_de_comunicacao_com_ruido(vector<char> fluxo, double taxa_erro);

// Bit de Paridade
vector<char> adicionar_paridade(vector<char> quadro);
bool verificar_paridade(vector<char> quadro, vector<char>& dados_sem_paridade);

// CRC-8
vector<char> adicionar_crc8(vector<char> quadro);
bool verificar_crc8(vector<char> quadro, vector<char>& dados_sem_crc);

// Hamming(7,4)
vector<char> codificar_hamming(vector<char> quadro);
vector<char> decodificar_hamming(vector<char> quadro);

// Enquadramento com Bit Stuffing
vector<char> aplicar_enquadramento(vector<char> quadro);
vector<char> remover_enquadramento(vector<char> quadro);

// Metricas
void exibir_metricas(int tipo_codificacao, int tamanho_original, int tamanho_codificado);

#endif
