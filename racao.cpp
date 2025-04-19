/*
  Problema da Ração

  Uma empresa de alimentos caninos produz 2 tipos de ração:
  – All Mega Giga Suprema (AMGS) e Ração das Estrelas (RE).
  – Para a sua manufatura são utilizadas cereais e carne.
  – O AMGS utiliza 5 kg de cereais e 1 kg de carne e o RE utiliza 4 kg de carne e 2 kg
  de cereais.
  – O preço do AMGS é 20 reais e o do RE é 30 reais
  – 1 kg de carne custa 4 reais e 1 kg de cereal custa 1 reais.
  – Existe a disposição 10.000 kg de carne e 30.000 kg de cereais.
  – Como deve ser a produção da empresa para ganhar o máximo de dinheiro?
*/


#include <ilcplex/ilocplex.h>

ILOSTLBEGIN;

int main() {
  IloEnv env;

  IloModel model(env);

  IloNumVar qtd_amgs (env, 0, IloInfinity, ILOFLOAT);
  IloNumVar qtd_re (env, 0, IloInfinity, ILOFLOAT);

  // Função Objetivo: Maximizar o lucro
  IloObjective obj = IloMaximize(env, qtd_amgs * 11 + qtd_re * 12);

  model.add(obj);

  // Restrições: Quantidade de cereal e carne utilizados
  model.add(qtd_amgs * 1 + qtd_re * 4 <= 10000);
  model.add(qtd_amgs * 5 + qtd_re * 2 <= 30000);

  IloCplex cplex(model);

  IloBool result = cplex.solve();

  result ? printf("Problem solved!\n"): printf("Problem not solved\n");

  if (!result) {
    printf("No solution found\n");
    return 1;
  } 

  printf("Optimal value: %f\n", cplex.getObjValue());
  printf("Qtd AMGS: %f\n", cplex.getValue(qtd_amgs));
  printf("Qtd RE: %f\n", cplex.getValue(qtd_re));
  printf("Qtd Carne: %f\n", cplex.getValue(qtd_amgs) + cplex.getValue(qtd_re) * 4);
  printf("Qtd Cereal: %f\n", cplex.getValue(qtd_amgs) * 5 + cplex.getValue(qtd_re) * 2);
  printf("Custo: %f\n", cplex.getValue(qtd_amgs) * 5 + cplex.getValue(qtd_re) * 1 + cplex.getValue(qtd_amgs) * 1 + cplex.getValue(qtd_re) * 4);
  printf("Receita: %f\n", cplex.getValue(qtd_amgs) * 20 + cplex.getValue(qtd_re) * 30);
  printf("Objetivo: %f\n", cplex.getValue(qtd_amgs) * 5 + cplex.getValue(qtd_re) * 4);
  

  return 0;
}