CXX = g++
CXXFLAGS = -std=c++11 -Wall
SRC = src/Simulador.cpp src/CamadaFisica.cpp src/MontagemGrafico.cpp src/Melhorias.cpp
BIN = executar

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)
