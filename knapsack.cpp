#include <ilcplex/ilocplex.h>
#include <vector>

ILOSTLBEGIN;

int main() {
    IloEnv env;
    IloModel model(env);

    int n = 20;
    int W = 878;
    
    vector<double> w = {92, 4, 43, 83, 84, 68, 92, 82, 6, 44, 32, 18, 56, 83, 25, 96, 70, 48, 14, 58};
    vector<double> v = {44, 46, 90, 72, 91, 40, 75, 35, 8, 54, 78, 40, 77, 15, 61, 17, 75, 29, 75, 63};   
    
    IloIntVarArray x(env, n, 0, 1);

    IloExpr somatorioObj(env);
    for(int i=0; i<n; ++i)
        somatorioObj += v[i]*x[i];

    IloObjective obj = IloMaximize(env, somatorioObj);

    model.add(obj);

    IloExpr somatorioRes(env);
    for(int i=0; i<n; ++i)
        somatorioRes += w[i]*x[i];

    model.add(somatorioRes <= W);

    IloCplex cplex(model);

    IloBool result = cplex.solve();

    result ? printf("Problem solved!\n"): printf("Problem not solved\n");
 
    printf("Optimal value: %.2f\n", cplex.getObjValue());

    printf("Itens\n");

    float pesoTotal = 0.0;
    for (int i = 0; i < n; ++i) {
        if(cplex.getValue(x[i]) > 0){
            printf("Item %d - valor: %.2f, peso: %.2f\n", i, v[i], w[i]);
            pesoTotal += w[i];
        }
    }

}