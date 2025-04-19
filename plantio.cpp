/*
  Problema da dieta

  Uma cooperativa agrícola opera 3 fazendas. A produção total de cada fazenda
  depende da área disponível para o plantio e da água para irrigação. A cooperativa
  procura diversificar sua produção e vai plantar este ano 3 culturas em cada
  fazenda: milho, arroz e feijão. Cada cultura demanda uma certa quantidade de
  água. São estabelecidos limites de área plantada de cada cultura. Para evitar
  concorrência entre os cooperado, acordou-se que a proporção de área cultivada
  seja a mesma para cada fazenda. Determinar a área plantada de cada cultura em
  cada fazenda de modo a otimizar o lucro da cooperativa.

  Fazenda | Área (acres) | Água (litros)
  1       | 400          | 1800
  2       | 650          | 2200
  3       | 350          | 950

  Cultura | Área máx | Água (litros por área) | Lucro (por área)
  milho   | 660      | 5,5                    | 5000
  arroz   | 880      | 4                      | 4000
  feijão  | 400      | 3,5                    | 1800

  
*/

#include <ilcplex/ilocplex.h>
#include <vector>

ILOSTLBEGIN;

const std::vector<std::string> LABELS = {
  "Milho",
  "Arroz",
  "Feijao"
};

int main() {
  IloEnv env;
  IloModel model(env);
  
  IloArray <IloIntVarArray> x(env, 3);

  for(unsigned i=0; i < 3; i++) {
    x[i] = IloIntVarArray(env, 3, 0, IloInfinity);
  }

  IloExpr obj_expr(env);
  obj_expr += 5000 * (x[0][0] + x[1][0] + x[2][0]);
  obj_expr += 4000 * (x[0][1] + x[1][1] + x[2][1]);
  obj_expr += 1800 * (x[0][2] + x[1][2] + x[2][2]);

  IloObjective obj = IloMaximize(env, obj_expr);
  model.add(obj);

  IloExpr max_area_farm1(env);
  max_area_farm1 += (x[0][0] + x[0][1] + x[0][2]);
  model.add(IloRange(env, 0, max_area_farm1, 400));

  IloExpr max_area_farm2(env);
  max_area_farm2 += (x[1][0] + x[1][1] + x[1][2]);
  model.add(IloRange(env, 0, max_area_farm2, 650));

  IloExpr max_area_farm3(env);
  max_area_farm3 += (x[2][0] + x[2][1] + x[2][2]);
  model.add(IloRange(env, 0, max_area_farm3, 350));

  IloExpr max_area_corn(env);
  max_area_corn += (x[0][0] + x[1][0] + x[2][0]);
  model.add(IloRange(env, 0, max_area_corn, 660));

  IloExpr max_area_rice(env);
  max_area_rice += (x[0][1] + x[1][1] + x[2][1]);
  model.add(IloRange(env, 0, max_area_rice, 880));

  IloExpr max_area_bean(env);
  max_area_bean += (x[0][2] + x[1][2] + x[2][2]);
  model.add(IloRange(env, 0, max_area_bean, 400));
  
  IloExpr max_liters_water1(env);
  max_liters_water1 += (5.5 * x[0][0] + 4 * x[0][1] + 3.5 * x[0][2]);
  model.add(IloRange(env, 0, max_liters_water1, 1800));

  IloExpr max_liters_water2(env);
  max_liters_water2 += (5.5 * x[1][0] + 4 * x[1][1] + 3.5 * x[1][2]);
  model.add(IloRange(env, 0, max_liters_water2, 2200));

  IloExpr max_liters_water3(env);
  max_liters_water3 += (5.5 * x[2][0] + 4 * x[2][1] + 3.5 * x[2][2]);
  model.add(IloRange(env, 0, max_liters_water3, 950));

  // Proporção
  model.add(((x[0][0] + x[0][1] + x[0][2]) / 400) == ((x[1][0] + x[1][1] + x[1][2]) / 650));
  model.add((x[1][0] + x[1][1] + x[1][2]) / 650 == (x[2][0] + x[2][1] + x[2][2]) / 350);

  IloCplex cplex(model);
  IloBool result = cplex.solve();

  result ? printf("Problem solved!\n"): printf("Problem not solved\n");
  
  printf("Optimal value: %f\n", cplex.getObjValue());

  for (unsigned i=0; i < 3; i++) {
    printf("Fazenda %d:\n", i + 1);
    for (unsigned j=0; j < 3; j++) {
      printf("  %s: %f\n", LABELS[j].c_str(), cplex.getValue(x[i][j]));
    }
  }
  

  return 0;
}