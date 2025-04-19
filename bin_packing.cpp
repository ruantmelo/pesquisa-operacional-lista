/*
  Algoritmo de Bin Packing usando GRASP

  Representação da solução: 
  - Vetor de inteiros representando os volumes dos itens
  Função de Avaliação:
  - Número de bins utilizados através da estratégia First Fit
  - Cada bin tem uma capacidade máxima
  Estratégia de busca local: 
    - Vizinhança gerada com a permutação de dois itens 
    - K vizinhos gerados e avaliados
    - Melhor entre os K vizinhos é escolhido
    - Busca local é repetida até o critério de parada ser atingido
  Critério de Parada:
  - Tempo máximo de execução


  Execução: ./{arquivo_binario} {tempo_maximo_execucao}  < {entrada}
  Entrada:
  - Primeira linha: capacidade do bin
  - Segunda linha: número de itens
  - N-próximas linhas: volume dos itens
*/ 


#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include <thread>
#include <chrono>
#include <future>
#include <stdexcept>

#include <atomic>

void print_items(std::vector<int> items);
std::vector<int> permute(std::vector<int> initial);
std::vector<int> bin_packing_ff(std::vector<int> items, int capacity);
size_t gen_random_index(std::vector<int> items);
std::vector<std::vector<int>> fitness_first_fit(std::vector<int> items, int capacity);

std::atomic<bool> stop_execution(false);

std::vector<int> current;

int main(int argc, char* argv[]) { 

  std::cout << "Algoritmo de Bin Packing com busca local" << std::endl;
  if(argc < 2) {
    std::cout << "Invalid Args: Definição do tempo máximo de execução não realizada" << std::endl;
    std::cout << "Uso: " << argv[0] << " <tempo_maximo_execucao (s)>" << std::endl;
    return 1;
  }
  
  int time_limit = std::stoi(argv[1]); 

  int capacity, n;
 
  std::cin >> capacity; 
  std::cin >> n;

  std::vector<int> items(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> items[i];
  }

  current = permute(items);

  std::future<std::vector<int>> expected_result = std::async(std::launch::async, bin_packing_ff, items, capacity);

  try {
    auto status = expected_result.wait_for(std::chrono::seconds(time_limit));

    if (status == std::future_status::ready) {
      std::cout << "Resultado encontrado antes do tempo limite!" << std::endl;
     
    } else {
      stop_execution = true; 
      std::cout << "Tempo limite excedido!" << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << "Exceção: " << e.what() << std::endl;
  }

  std::vector<std::vector<int>> bins = fitness_first_fit(current, capacity);
  int qtd_bins = bins.size();
  
  for (int i = 0; i < bins.size(); ++i) {
    std::cout << "Bin " << i + 1 << ": ";
    print_items(bins[i]);
  }

  std::cout << "Número de bins utilizadas " << qtd_bins << std::endl;


  return 0;
}

std::vector<int> bin_packing_ff(std::vector<int> items, int capacity) {
  int n = items.size(); 

  int best_fitness = fitness_first_fit(items, capacity).size();
  
  while (!stop_execution) {
    int k = std::min(100, n); 

    std::vector<std::vector<int>> neighbors(k);
    for (int i = 0; i < k; ++i) {
      int pos_a = gen_random_index(items);
      int pos_b = gen_random_index(items);
      while (pos_a == pos_b) {
        pos_b = gen_random_index(items);
      }
      std::vector<int> neighbor = items;
      std::swap(neighbor[pos_a], neighbor[pos_b]);
      neighbors[i] = neighbor;
    }

    for (const auto& neighbor : neighbors) {
      int current_fit = fitness_first_fit(neighbor, capacity).size();

      if (current_fit < best_fitness) {
        current = neighbor;
        best_fitness = current_fit;
      }
    }
  }

  return current;
}

std::vector<std::vector<int>> fitness_first_fit(std::vector<int> items, int capacity) {
  std::vector<std::vector<int>> bins;
  int bin_count = 0;

  for (int i = 0; i < items.size(); ++i) {
    bool placed = false;

    for (int j = 0; j < bins.size(); ++j) {
      int bin_sum = std::accumulate(bins[j].begin(), bins[j].end(), 0);
      if (bin_sum + items[i] <= capacity) {
        bins[j].push_back(items[i]);
        placed = true;
        break;
      }
    }

    if (!placed) {
      bins.push_back({items[i]});
      bin_count++;
    }
  }

  return bins;
}

size_t gen_random_index(std::vector<int> items) {

   std::random_device rd;
   std::mt19937 gen(rd());
 
   std::uniform_int_distribution<> distrib(0, static_cast<int>(items.size() - 1));
 
   // Gera um índice aleatório
   size_t random_index = distrib(gen);

   return random_index;
}

void print_items(std::vector<int> items) {
 
  for (size_t i = 0; i < items.size(); ++i) {
    std::cout << items[i];
    if (i < items.size() - 1) {
      std::cout << ", ";
    }
  }

  std::cout << '\n';
}


std::vector<int> permute(std::vector<int> initial) {
  std::vector<int> permutation = initial;

  std::random_device rd;
  std::mt19937 g(rd());

  shuffle(permutation.begin(), permutation.end(), g);

  return permutation;
}