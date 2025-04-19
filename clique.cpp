#include <ilcplex/ilocplex.h>
#include <vector>

ILOSTLBEGIN;

int main() {
    IloEnv env;
    IloModel model(env);

    int n = 10;

    IloIntVarArray x(env, n, 0, 1);

    std::vector<std::vector<int>> g(n);

    std::vector<std::pair<int, int>> edges = {
        {0,1}, {0,2}, {0,3}, {1,3}, {2,5}, {2,3}, {5,6}, {5,7},
        {6,3}, {6,9}, {1,4}, {3,4}, {9,7}, {8,9}, {6,7}, {3,6}
    };
    
    for (auto [u, v] : edges) {
        g[u].push_back(v);
        g[v].push_back(u);
    }


    IloExpr somatorioObj(env);
    for(int v=0; v<n; ++v)
        somatorioObj += x[v];

    IloObjective obj = IloMaximize(env, somatorioObj);
    
    model.add(obj);

    for(int v = 0; v < n; ++v) {
        for(int u = v+1; u < n; ++u) {
            bool adjacente = false;
            for(int w : g[v]) {
                if(w == u){ 
                    adjacente = true; 
                    break; 
                }
            }
            if (!adjacente) {
                model.add(x[v] + x[u] <= 1);
            }
        }
    }

    IloCplex cplex(model);

    IloBool result = cplex.solve();

    result ? printf("Problem solved!\n"): printf("Problem not solved\n");
    
    printf("Optimal value: %.2f\n", cplex.getObjValue());

    printf("Vertices na clique: ");
    for (int v = 0; v < n; ++v) {
        if(cplex.getValue(x[v]) > 0){
            printf("%d ", v);
        }
    }
    printf("\n");

}