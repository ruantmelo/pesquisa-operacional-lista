#include <ilcplex/ilocplex.h>
#include <vector>

ILOSTLBEGIN;

int main() {
    IloEnv env;
    IloModel model(env);

    int n = 5; 

    std::vector<std::vector<int>> g(n);

    g[0].push_back(1);
    g[1].push_back(0);

    g[0].push_back(2);
    g[2].push_back(0);

    g[0].push_back(3);
    g[3].push_back(0);

    g[0].push_back(4);
    g[4].push_back(0);

    g[1].push_back(2);
    g[2].push_back(1);

    g[1].push_back(3);
    g[3].push_back(1);

    g[1].push_back(4);
    g[4].push_back(1);

    g[2].push_back(3);
    g[3].push_back(2);

    g[2].push_back(4);
    g[4].push_back(2);

    g[3].push_back(4);
    g[4].push_back(3);


    IloArray<IloIntVarArray> x(env, n);
    for(int i = 0; i < n; ++i) {
        x[i] = IloIntVarArray(env, n, 0, 1);
    }

    IloIntVarArray z(env, n, 0, 1);

    IloExpr somatorioObj(env);
    for(int k=0; k<n; ++k)
        somatorioObj += z[k];

    IloObjective obj = IloMinimize(env, somatorioObj);
    model.add(obj);

    for(int i = 0; i < n; ++i) {
        IloExpr somatorioAtribuiFreq(env);
        for(int k = 0; k < n; ++k)
            somatorioAtribuiFreq += x[i][k];
        model.add(somatorioAtribuiFreq == 1);
    }

    for(int i = 0; i < n; ++i) {
        for(int k = 0; k < n; ++k) {
            model.add(x[i][k] <= z[k]);
        }
    }

    for(int i = 0; i < n; ++i) {
        for(int j : g[i]) {
            for(int k = 0; k < n; ++k) {
                model.add(x[i][k] + x[j][k] <= 1);
            }
        }
    }

    IloCplex cplex(model);
    IloBool result = cplex.solve();
  
    result ? printf("Problem solved!\n"): printf("Problem not solved\n");
    
    printf("Optimal value: %f\n", cplex.getObjValue());

    for(int i = 0; i < n; ++i) {
        for(int k = 0; k < n; ++k) {
            if(cplex.getValue(x[i][k]) > 0){
                printf("Antena %d: Frequencia %d\n", i, k);
            }
        }
    }
}