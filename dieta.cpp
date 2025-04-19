/*
  Problema da dieta

  Para o bom funcionamento do organismo, uma pessoa precisa ingerir uma
  quantidade mínima de certas vitaminas, presentes em 6 diferentes ingredientes
  que deverão ser combinados em um composto alimentar de custo mínimo.
  
                 vitaminas              Quant min
                1   2   3   4   5   6
    A           1   0   2   2   1   2         9
    C           0   1   3   1   3   2        19
    Preço    35  30  60  50  27  22

  */

#include <ilcplex/ilocplex.h>

int main() {
  IloEnv env;
  IloModel model (env);
  IloFloatVarArray x(env, 6, 0, IloInfinity);
  
  // Objetivo: Minimizar o valor gasto
  IloObjective obj = IloMinimize(env, 35 * x[0] + 30 * x[1] + 60 * x[2] + 50 * x[3] + 27 * x[4] + 22 * x[5]);
  model.add(obj);

  // Restrição: Quantidade de vitamina A
  IloExpr expr_vit_a(env);
  expr_vit_a += 1 * x[0] + 0 * x[1] + 2 * x[2] + 2 * x[3] + 1 * x[4] + 2 * x[5];
  model.add(IloRange(env, 9, expr_vit_a, IloInfinity));

  // Restrição: Quantidade de vitamina C
  IloExpr expr_vit_c(env);
  expr_vit_c += 0 * x[0] + 1 * x[1] + 3 * x[2] + 1 * x[3] + 3 * x[4] + 2 * x[5];
  model.add(IloRange(env, 19, expr_vit_c, IloInfinity));


  IloCplex cplex(model);
  
  IloBool result = cplex.solve();

  if (!result) {
    printf("No solution found\n");
    return 1;
  }

  printf("Problem solved!\n");

  printf("Optimal value: %f\n", cplex.getObjValue());

  for (int i = 0; i < 6; i++) {
    printf("Ingrediente %d: %f\n", i, cplex.getValue(x[i]));
  }

  printf("Vitamina A: %f\n", cplex.getValue(x[0]) * 1 + cplex.getValue(x[1]) * 0 + cplex.getValue(x[2]) * 2 + cplex.getValue(x[3]) * 2 + cplex.getValue(x[4]) * 1 + cplex.getValue(x[5]) * 2);
  printf("Vitamina C: %f\n", cplex.getValue(x[0]) * 0 + cplex.getValue(x[1]) * 1 + cplex.getValue(x[2]) * 3 + cplex.getValue(x[3]) * 1 + cplex.getValue(x[4]) * 3 + cplex.getValue(x[5]) * 2);
  printf("Custo: %f\n", cplex.getValue(x[0]) * 35 + cplex.getValue(x[1]) * 30 + cplex.getValue(x[2]) * 60 + cplex.getValue(x[3]) * 50 + cplex.getValue(x[4]) * 27 + cplex.getValue(x[5]) * 22);


  return 0;
}