#include <ilcplex/ilocplex.h>
#include <vector>

ILOSTLBEGIN;

int main() {
    IloEnv env;
    IloModel model(env);

    int n = 3; 
    int m = 4;

    double f[n] = {100, 150, 120}; 

    vector<vector<double>> c = {
        {20, 104, 11, 325},
        {28, 104, 325, 8},
        {325, 5, 5, 96}
    };

    IloIntVarArray y(env, n, 0, 1);

    IloArray <IloIntVarArray> x(env, n);
    for(int i=0; i<n; ++i)
        x[i] = IloIntVarArray(env, m, 0, 1);

    IloExpr somatorioInstalacao(env);
    for(int i=0; i<n; ++i)
        somatorioInstalacao += f[i] * y[i];

    IloExpr somatorioAtendimento(env);
    for(int i=0; i<n; ++i)
        for(int j=0; j<m; ++j)
            somatorioAtendimento += c[i][j] * x[i][j];

    IloObjective obj = IloMinimize(env, somatorioInstalacao + somatorioAtendimento);
    model.add(obj);


    for (int j = 0; j < m; ++j){
        IloExpr somatorioRes(env);
        for (int i = 0; i < n; ++i){
            somatorioRes += x[i][j];
        }
        model.add(somatorioRes == 1);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            model.add(x[i][j] <= y[i]);
        }
    }

    IloCplex cplex(model);
    IloBool result = cplex.solve();
  
    result ? printf("Problem solved!\n"): printf("Problem not solved\n");
    
    printf("Optimal value: %f\n", cplex.getObjValue());

    for (int i = 0; i < n; ++i) {
        printf("Deposito %d instalado\n", i);
        printf("Clientes atendidos: ");
        for (int j = 0; j < m; ++j) {
            if(cplex.getValue(x[i][j]) > 0){
                printf(" %d", j);
            }
        }
        printf("\n");
    }
}