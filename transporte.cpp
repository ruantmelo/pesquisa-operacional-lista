/*
  – Os depósitos possuem demandas que precisam ser atendidas.
  – As fábricas possuem ofertas para atender os depósitos
  – Cada arco possui um custo de transporte cij entre a fábrica i e o depósito j .
  – Queremos atender as demandas dos depósitos com um custo mínimo de
  transporte, respeitando as restrições.

  Fab-Dep | Depósito 1 | Depósito 2 | Depósito 3
  Fáb 1   | 8          | 5          | 6
  Fáb 2   | 15         | 10         | 12
  Fáb 3   | 3          | 9          | 10
  
  Oferta das Fábricas:
  - Fábrica 1: 120 unidades
  - Fábrica 2: 80 unidades
  - Fábrica 3: 80 unidades

  Demanda dos Depósitos:
  - Depósito 1: 150 unidades
  - Depósito 2: 70 unidades
  - Depósito 3: 60 unidades
*/

#include <ilcplex/ilocplex.h>
#include <vector>

ILOSTLBEGIN;

const std::vector<std::vector<int>> CUSTOS = {
  { 8, 5, 6},
  { 15, 10, 12 },
  { 3, 9, 10 },
};

const std::vector<int> CAPACIDADES = {
  120, 80, 80
};

const std::vector<int> DEMANDAS = {
  150, 70, 60
};

int main() {
  IloEnv env;
  IloModel model(env);
  
  IloArray <IloIntVarArray> x(env, 3);

  for(unsigned i=0; i < 3; i++) {
    x[i] = IloIntVarArray(env, 3, 0, IloInfinity);
  }

  // Função Objetivo: Minimizar o custo de transporte
  IloExpr obj_expr(env);
  for(unsigned i=0; i < 3; i++) {
    for(unsigned j=0; j < 3; j++) {
      obj_expr += CUSTOS[i][j] * x[i][j];
    }
  }

  IloObjective obj = IloMinimize(env, obj_expr);
  model.add(obj);


  // Restrição: Capacidade das fábricas
  for(unsigned i=0; i < 3; i++) {
    model.add((x[i][0] + x[i][1] + x[i][2]) <= CAPACIDADES[i]);
  }

  // Restrição: Demanda dos depósitos
  for(unsigned i=0; i < 3; i++) {
    model.add((x[0][i] + x[1][i] + x[2][i]) == DEMANDAS[i]);
  }

  IloCplex cplex(model);
  IloBool result = cplex.solve();

  if (cplex.solve()) {
    printf("Problem solved!\n");
    printf("Custo mínimo: %f\n", cplex.getObjValue());
  } else {
    printf("Problem not solved\n");
  }
  
  return 0;
}