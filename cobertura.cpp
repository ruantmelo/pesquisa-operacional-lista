/*
  O governo planeja construir escolas de modo a satisfazer a demanda em uma
  cidade. Ainda não se sabe quantas escolas são necessárias, mas a lei demanda que
  todo bairro deve ter uma escolas ou estar próximo de uma. Em que bairros dever
  ser construídas escolas de modo a minimizar o número de escolas construídas.
*/

#include <ilcplex/ilocplex.h>
#include <vector>

ILOSTLBEGIN;

const std::vector<std::pair<int, int>> EDGES = {
  {0, 1}, {0, 2}, {0, 3}, {0, 7}, {1, 3}, {2, 3},
  {3, 4}, {3, 5}, {3, 6}, {5, 8}, {6, 7},
};

int main() {
  IloEnv env;
  IloModel model(env);

  int n = 9;
  std::vector<std::vector<int>> adj(n, std::vector<int>(n, 0));

  for (auto e : EDGES) {
    adj[e.first][e.second] = 1;
    adj[e.second][e.first] = 1;
  }

  IloBoolVarArray x(env, n);

  IloExpr obj_expr(env);
  for (int i=0; i < n; i++) {
    obj_expr += x[i];
  }

  IloObjective obj = IloMinimize(env, obj_expr);
  model.add(obj);

  // Restrição de cobertura
  for (int i = 0; i < n; i++) {
    IloExpr coverage(env);
    coverage += x[i];
    for (int j = 0; j < n; j++) {
      if (adj[i][j] == 1) {
        coverage += x[j];
      }
    }
    model.add(coverage >= 1);
  }


  IloCplex cplex(model);
  IloBool result = cplex.solve();

  result ? printf("Problem solved!\n"): printf("Problem not solved\n");
  
  printf("Optimal value: %f\n", cplex.getObjValue());

  for (int i = 0; i < n; i++) {
    if ((int) cplex.getValue(x[i])){
      printf("Escola no bairro %d \n", i);
    }
  }

  return 0;
}