/*
– Uma fábrica de latinhas possui 4 padrões de impressão em folhas de metal
(existem 2 tipos de folhas demetal diferentes).
– A fábrica possui 200 folhas de metal de tam 1 e 90 de tam 2.
– Cada latinhas é vendida a 50 u.
– Cada corpo não utilizado possui um custo de estocagem de 50 u e cada tampa 
custa 3 u.
– Quantas impressões de cada padrão devem ser feitas para maximizar o lucro?
*/

#include <ilcplex/ilocplex.h>
#include <vector>

ILOSTLBEGIN;

int main() {
    IloEnv env;
    IloModel model(env);

    IloIntVarArray x(env, 4, 0, IloIntMax);
    IloIntVar y(env, 0, IloIntMax);

    // Função Objetivo: Maximiza o lucro
    IloObjective obj = IloMaximize(env, 50 * y - 5 * (x[0] + 2 * x[1] + 4 * x[3] - y) - 3 * (7 * x[0] + 3 * x[1] + 9 * x[2] + 4 * x[3] - 2 * y));

    model.add(obj);

    // Restrições
    model.add(x[0] + x[2] + x[3] <= 200);

    model.add(x[1] <= 90);

    // Restrição de tempo
    // model.add(2 * x[0] + 3 * x[1] + 2 * x[2] + x[3] <= 10);

    model.add(y <= x[0] + 2 * x[1] + 4 * x[3]);

    model.add(y <= (7 * x[0] + 3 * x[1] + 9 * x[2] + 4 * x[3])/2);

    IloCplex cplex(model);
    IloBool result = cplex.solve();
  
    result ? printf("Problem solved!\n"): printf("Problem not solved\n");
    
    printf("Optimal value: %.0f\n", cplex.getObjValue());
    printf("x1 = %.0f, x2 = %.0f, x3 = %.0f, x4 = %.0f\n", cplex.getValue(x[0]), cplex.getValue(x[1]), cplex.getValue(x[2]), cplex.getValue(x[3]));
    printf("y = %.0f\n", cplex.getValue(y));

}