/*
  /* Problema de Escalonamento de Horários das Enfermeiras
  
  Em um hospital, deseja-se planejar o horário de enfermeiras no turno da noite. A
  demanda de enfermeiras no turno da noite no dia i é um número inteiro di , com
  i = 1 . . . 7. Cada enfermeira trabalha 5 dias consecutivos e descansa por três dias
  seguintes. O objetivo consiste em minimizar o número de enfermeiras contratadas.
*/


#include <ilcplex/ilocplex.h>
#include <vector>

ILOSTLBEGIN;

const std::vector<int> DEMANDAS = {
  5, 6, 4, 7, 3, 4, 6
};

int main() {
  IloEnv env;
  IloModel model(env);
  
  int days = DEMANDAS.size();

  IloIntVarArray x(env, days, 0, IloInfinity);


  IloExpr obj_expr(env);
  for (int i=0; i < days; i++) {
    obj_expr += x[i];
  }

  // Função Objetivo: Minimizar o número de enfermeiras contratadas
  IloObjective obj = IloMinimize(env, obj_expr);
  model.add(obj);
  

  // Restrição: Número de enfermeiras trabalhando no dia i deve atender a demanda;
  for (int i=0; i < days; i++) {
    IloExpr working(env);
    // Verifica o dia atual e os 4 dias anteriores
    printf("working: %d \n", i);
    for (int j=0; j < 5; j++) {
        working += x[(i-j + days) % days];
    };
   
    model.add(working >= DEMANDAS[i]);
  }

  IloCplex cplex(model);
  IloBool result = cplex.solve();

  if (!result) {
    printf("No solution found\n");
    return 1;
  }

  printf("Problem solved!\n");
  printf("Número de enfermeiras contratadas: %f\n", cplex.getObjValue());
  
  for(int i = 0; i < days; i++) {
    printf("Enfermeiras que começam no dia %d: %f \n", i + 1, cplex.getValue(x[i]));
  }

  return 0;
}