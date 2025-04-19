/*
  – Um produtor de gás natural s precisa enviar a maior quantidade de gás para a
  fábrica t através de dutos.
  – Cada duto ij é direcionado (o gás passa apenas em uma direção) e possui uma
  capacidade associada.
*/

#include <ilcplex/ilocplex.h>
#include <vector>

ILOSTLBEGIN;

const std::vector<std::vector<int>> GRAFO = {
  {0, 10, 5, 0},
  {0, 0, 15, 8},
  {0, 0, 0, 10},
  {0, 0, 0, 0}
};

int main() {
  IloEnv env;
  IloModel model(env);

  int n = GRAFO.size(), s = 0, d = 3;
  
  // Criação da matriz de variáveis com a limitação da capacidade
  IloArray<IloNumVarArray> x(env, n);
  for (int i = 0; i < n; ++i) {
      x[i] = IloNumVarArray(env, n);
      for (int j = 0; j < n; ++j) 
        x[i][j] = IloIntVar(env, 0, GRAFO[i][j]);      
  }

  // Função Objetivo: Maximizar o fluxo de saída de s
  IloExpr obj_expr(env);
    for (unsigned i = 0; i < 4; i++) {
      obj_expr += x[0][i];
    }
  
  IloObjective obj = IloMaximize(env, obj_expr);
  model.add(obj);


  // Restrição de capacidade dos dutos que não sejam s e d
  for (int i = 0; i < n; i++) {
    if (i == s || i == d) continue;
    IloExpr fluxo_entrada(env);
    IloExpr fluxo_saida(env);
    for (int j = 0; j < 4; j++) {
      fluxo_entrada += x[j][i];
      fluxo_saida += x[i][j];
    }
    model.add(fluxo_entrada == fluxo_saida);
  }

  IloCplex cplex(model);
  IloBool result = cplex.solve();

  if (result) {
    printf("Problem solved!\n");
    printf("Optimal value: %f\n", cplex.getObjValue());
  } else {
    printf("Problem not solved\n");
  }

  env.end();
  return 0;
}