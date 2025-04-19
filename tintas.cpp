
/*
  Uma empresa de tintas produz 2 tipos de tinta: a de secagem rápida (SR) e a de
  secagem normal (SN). Ambas usam os mesmos componente, variando apenas a
  proporção. Os componentes são: componente de secagem (SEC) e o componente
  de cor (COR). A empresa tem a disposição para a compra duas misturas para
  compor as tintas:

  – Solução A (SolA) com 30% de SEC e 70% de COR a R$ 1,5 o litro
  – Solução B (Solb) com 60% de SEC e 40% de COR a R$ 1,0 o litro.

  Além disso, a empresa tem a disposição também:
  – Comp. SEC a R$ 4,0 o litro
  – Comp. COR a R$ 6,0 o litro

  Sabendo que para compor SR temos que ter no mínimo 25% de SEC e no mínimo
  50% de COR. Já para SN temos que ter no mínimo de 20% de SER e 50% de COR.
  Qual a quantidade de produtos (SolA, SolB, SEC, COR) a serem comprados para se
  produzir 1000 litro de SR e 250 litros de SN com menor custo?
*/

#include <ilcplex/ilocplex.h>
#include <vector>

ILOSTLBEGIN;

enum TINTAS {
  SR,
  SN
};

const std::vector
<std::string> TINTAS_NOMES = {
  "Secagem Rápida",
  "Secagem Normal",
};

enum COMPONENTES {
  SOL_A,
  SOL_B,
  SEC,
  COR
};


const std::vector<std::string> COMPONENTES_NOMES = {
  "Solução A",
  "Solução B",
  "Componente Secante",
  "Componente Cor"
};

int main() {
  IloEnv env;
  IloModel model(env);
  
  IloArray <IloIntVarArray> x(env, 2);

  for(unsigned i=0; i < 2; i++) {
    x[i] = IloIntVarArray(env, 4, 0, IloInfinity);
  }

  IloExpr obj_expr(env);
  obj_expr += 1.5 * (x[SR][SOL_A] + x[SN][SOL_A]);
  obj_expr += 1 * (x[SR][SOL_B] + x[SN][SOL_B]);
  obj_expr += 4 * (x[SR][SEC] + x[SN][SEC]);
  obj_expr += 6 * (x[SR][COR] + x[SN][COR]);

  IloObjective obj = IloMinimize(env, obj_expr);
  model.add(obj);

  // Qtd SR = 1000
  model.add((x[SR][SOL_A] + x[SR][SOL_B] + x[SR][COR] + x[SR][SEC]) == 1000);

  // Qtd SN == 250;
  model.add((x[SN][SOL_A] + x[SN][SOL_B] + x[SN][COR] + x[SN][SEC]) == 250);

  // Qtd Min Sec
  model.add(( 0.3 * x[SR][SOL_A] + 0.6 * x[SR][SOL_B] + x[SR][SEC]) >= 250);
  model.add(( 0.3 * x[SN][SOL_A] + 0.6 * x[SN][SOL_B] + x[SN][SEC]) >= 50);

  // Qtd Min Cor
  model.add(( 0.7 * x[SR][SOL_A] + 0.4 * x[SR][SOL_B] + x[SR][COR]) >= 500);
  model.add(( 0.7 * x[SN][SOL_A] + 0.4 * x[SN][SOL_B] + x[SN][COR]) >= 125);


  IloCplex cplex(model);
  IloBool result = cplex.solve();

  if (!result) {
    printf("No solution found\n");
    return 1;
  }

  printf("Problem solved!\n");
  
  printf("Lucro máximo: %f\n", cplex.getObjValue());

  for (unsigned i=0; i < 2; i++) {
    printf("Tinta %s:\n", TINTAS_NOMES[i].c_str());
    for (unsigned j=0; j < 4; j++) {
      printf("  %s: %f\n", COMPONENTES_NOMES[j].c_str(), cplex.getValue(x[i][j]));
    }
  }
  

  return 0;
}