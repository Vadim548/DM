#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <algorithm>
#include <iomanip> 
#include <chrono>
#include <random>
#include <cmath>


struct solution_result{
    std::string name;
    double time_ms;
    int max_cut;
    std::vector<char> solution;
};

int compute_solution_cost(int graph[], char* solution, int N){
    int result = 0;
    for(size_t i = 0; i < N; ++i){
        for(size_t j = i + 1; j < N; ++j){
            result += int(solution[i] != solution[j]) * graph[i * N + j];
        }
    }
    return result;
}

int bruteforce_solver(int graph[], char* solution, int N){
    int best_cut = 0;
    int best_solution_mask = 0;

    for (int solution_mask = 0; solution_mask < (1 << N); ++solution_mask){
        int current_cost = 0;
        for(int i = 0; i < N; ++i){
            for(int j = i + 1; j < N; ++j){
                if(((solution_mask >> i) & 1) != ((solution_mask >> j) & 1)){
                    current_cost += graph[i * N + j];
                }
            }
        }
        if (current_cost > best_cut){
            best_cut = current_cost;
            best_solution_mask = solution_mask;
        }
    }

    for(int i = 0; i < N; ++i){
        solution[i] = (best_solution_mask >> i) & 1;
    }

    return best_cut;

}

int goemans_williamson_solver(int* graph, char* solution, int N){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    double vectors[N][N];
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            vectors[i][j] = 0.0;
        }
    }

    for(int i = 0; i < N; ++i){
        double norm = 0.0;
        for(int j = 0; j < N; ++j){
            vectors[i][j] = dis(gen);
            norm += vectors[i][j] * vectors[i][j];
        }
        norm = std::sqrt(norm);
        for(int j = 0; j < N; ++j){
            vectors[i][j] /= norm;
        }
    }

    for(size_t i = 0; i < N; ++i){
        solution[i] = 0;
    }

    for (int i = 0; i < N; ++i) {
        double r = dis(gen);
        double proj = 0.0;
        for (int j = 0; j < N; ++j) {
            proj += vectors[i][j] * r;
        }
        solution[i] = (proj >= 0);
    }

    return compute_solution_cost(graph, solution, N);
}


int greedy_by_node_solver(int* graph, char* solution, int N){
    for (int i = 0; i < N; ++i) {
        solution[i] = -1;
    }

    std::vector<std::pair<int, int>> node_weights;
    for (int i = 0; i < N; ++i) {
        int total_weight = 0;
        for (int j = 0; j < N; ++j) {
            total_weight += graph[i * N + j];
        }
        node_weights.emplace_back(total_weight, i);
    }

    std::sort(node_weights.rbegin(), node_weights.rend()); //find top nodes

    if (!node_weights.empty()) {
        int first_node = node_weights[0].second;
        solution[first_node] = 1; //just some starting color for most influential node
    }

    //give them colors one-by-one
    for (const auto& [weight, node] : node_weights) {
        // std::cout << node << ' ' << weight << ' ' << (int)solution[node] <<  '\n';
        

        //not sure here... In this example it was worse than without, 
        //but seems incorrect: because -1 and 1 - (-1) would make 2
        if (solution[node] == -1) continue;
        
        // Not yet colored - it is not connected directly to "larger" nodes.
        //This means they are connected to more "smaller" nodes 
        //But assigning color randomly can worsen the later connections, mb skip better
        //Yeah, so later it would be passed by the "smaller" nodes and will be colored as well

        //find the most influential neighbour
        //does not work, because the neighbour himself can be not colored yet
        // int best_neighbor = -1;
        // int best_edge_weight = -1;
        // for (int neighbor = 0; neighbor < N; ++neighbor) {
        //     //check not to collide with itself and that neighbour does not have the color yet
        //     if (node == neighbor || solution[neighbor] != -1) continue;
        //     if (graph[node * N + neighbor] > best_edge_weight) {
        //         best_edge_weight = graph[node * N + neighbor];
        //         best_neighbor = neighbor;
        //     }
        // }
        // // std::cout << "best_neighbor: " << best_neighbor << '\n'; 
        // //color the most influential neighbour with the opposite color of yours 
        // if(best_neighbor != -1){
        //     solution[best_neighbor] = 1 - solution[node];
        // }
        
        for (int neighbor = 0; neighbor < N; ++neighbor) {
            if (neighbor == node || graph[node * N + neighbor] == 0) continue;
            solution[neighbor] = 1 - solution[node];
        }

        //not needed, because will color all neighbours
        //or not, not colored only
        // //so we scan all the neighbours, sort and take the best available (colored) - 
        // //like reverse to classic paint your neighbour
        // std::vector<std::pair<int, int>> neighbors;
        // for(int neighbor = 0; neighbor < N; ++neighbor){
        //     if (node == neighbor) continue;
        //     neighbors.emplace_back(graph[node * N + neighbor], neighbor);
        // }
        // std::sort(neighbors.rbegin(), neighbors.rend());

        
        // for(const auto& [edge_weight, neighbor]: neighbors){
        //     if(solution[neighbor] != -1){
        //         solution[node] = 1 - solution[neighbor];
        //         break;
        //     }
        // }
    }

    //if some nodes are left - they were not connected anywhere
    for(const auto& [weight, node] : node_weights){
        if (solution[node] == -1){
            solution[node] = 0;
        }
    }
    return compute_solution_cost(graph, solution, N);
}

float net_weight_of_biggest_neighbours = 2.0 / 3.0;
int greedy_by_node_plus_genetic_solver(int* graph, char* solution, int N){
    for (int i = 0; i < N; ++i) {
        solution[i] = -1;
    }

    std::vector<std::pair<int, int>> node_weights;
    for (int i = 0; i < N; ++i) {
        int total_weight = 0;
        for (int j = 0; j < N; ++j) {
            total_weight += graph[i * N + j];
        }
        node_weights.emplace_back(total_weight, i);
    }

    std::sort(node_weights.rbegin(), node_weights.rend()); //find top nodes

    if (!node_weights.empty()) {
        int first_node = node_weights[0].second;
        solution[first_node] = 1; //just some starting color for most influential node
    }

    for (const auto& [weight, node] : node_weights) {
        // not needed, because will color all neighbours
        // or not, not colored only
        //so we scan all the neighbours, sort and take the best available (colored) - 
        //like reverse to classic paint your neighbour
        std::vector<std::pair<int, int>> neighbors;
        int all_neighbours_weight = 0;
        int first_neighbours_weight = 0;
        for(int neighbor = 0; neighbor < N; ++neighbor){
            if (node == neighbor || graph[node * N + neighbor] == 0) continue;
            neighbors.emplace_back(graph[node * N + neighbor], neighbor);
            all_neighbours_weight += graph[node * N + neighbor];
        }
        std::sort(neighbors.rbegin(), neighbors.rend());


        
        for(const auto& [edge_weight, neighbor]: neighbors){
            if(solution[neighbor] == -1){
                solution[neighbor] = 1 - solution[node];
            }
            if(solution[neighbor] == 1 - solution[node]){
                first_neighbours_weight += graph[node * N + neighbor];
            }
            if(float(first_neighbours_weight) / all_neighbours_weight > net_weight_of_biggest_neighbours){
                break;
            }
        }
    }
        //now every marked node is part of some cluster
        //and we genetically iterate over nodes not in cluster. Well, they can also be mutated with much less chance




    

    return compute_solution_cost(graph, solution, N);
}

int main() {
    std::ifstream fin("input");

    if(!fin){
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    std::vector<std::string> vertexNames;
    std::map<std::string, int> vertexToIndex;

    std::string line;
    std::getline(fin, line);

    while(std::getline(fin, line) && line != "E"){
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        line = line.substr(1, line.size() - 2);
        vertexNames.push_back(line);
    }

    for(size_t i = 0; i < vertexNames.size(); ++i){
        vertexToIndex[vertexNames[i]] = i;
    }

    int N = vertexNames.size();
    int graph[N][N] = {0};

    for(size_t i = 0; i < N; ++i){
        for(size_t j = 0; j < N; ++j){
            graph[i][j] = 0;
        }
    }

    while(std::getline(fin, line)){
        if(line.empty()){
            continue;
        }

        //   (uN)-(vN)-cost
        size_t u_start = line.find('(') + 1;
        size_t u_end = line.find(')');
        size_t v_start = line.find('(', u_end + 1) + 1;
        size_t v_end = line.find(')', v_start);
        size_t cost_start = line.find('-', v_end + 1) + 1;

        std::string uN = line.substr(u_start, u_end - u_start);
        std::string vN = line.substr(v_start, v_end - v_start);
        int cost = std::stoi(line.substr(cost_start));
        
        int u_index = vertexToIndex[uN];
        int v_index = vertexToIndex[vN];
        graph[u_index][v_index] = cost;
        graph[v_index][u_index] = cost;
        // std::cout << u_index << ' ' << u_index << ' ' << uN << ' ' << vN << ' ' << cost << '\n';
    }

    for(size_t i = 0; i < N; ++i){
        for(size_t j = 0; j < N; ++j){
            std::cout << std::setw(6) << graph[i][j];
        }
        std::cout << "\n";
    }

    char solution[N];
    for(size_t i = 0; i < N; ++i){
        solution[i] = 0;
    }


    // char solution[N] = {1, 1, 0, 0, 0};

    // std::cout << "Solving with BruteForce... Please, wait...";
    // int max_cut = goemans_williamson_solver(&graph[0][0], solution, N);

    // std::cout << "Solution is:\n";
    // std::cout << "Index - Color - Name\n";

    // for(size_t i = 0; i < N; ++i){
    //     std::cout << i << " - " << int(solution[i]) << " - " << vertexNames[i] << '\n';
    // }
    // std::cout << "With cut score: " << max_cut << "\n";
    
    std::vector<solution_result> results;

    {
        char solution[N];
        auto start = std::chrono::high_resolution_clock::now();
        int max_cut = bruteforce_solver(&graph[0][0], solution, N);
        auto end = std::chrono::high_resolution_clock::now();
        double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
        results.push_back({"BruteForce", time_ms, max_cut, std::vector<char>(solution, solution + N)});
    }

    {
        char solution[N];
        auto start = std::chrono::high_resolution_clock::now();
        int max_cut = goemans_williamson_solver(&graph[0][0], solution, N);
        auto end = std::chrono::high_resolution_clock::now();
        double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
        results.push_back({"Goemans-Williamson", time_ms, max_cut, std::vector<char>(solution, solution + N)});
    }

    {
        char solution[N];
        auto start = std::chrono::high_resolution_clock::now();
        int max_cut = greedy_by_node_solver(&graph[0][0], solution, N);
        auto end = std::chrono::high_resolution_clock::now();
        double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
        results.push_back({"Greedy-by-node", time_ms, max_cut, std::vector<char>(solution, solution + N)});
    }

    {
        char solution[N];
        auto start = std::chrono::high_resolution_clock::now();
        int max_cut = greedy_by_node_plus_genetic_solver(&graph[0][0], solution, N);
        auto end = std::chrono::high_resolution_clock::now();
        double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
        results.push_back({"Greedy-by-node+genetic", time_ms, max_cut, std::vector<char>(solution, solution + N)});
    }

    std::cout << "\nResults:\n";
    std::cout << std::setw(20) << "Solver Name" << " | "
              << std::setw(12) << "Time (ms)" << " | "
              << std::setw(12) << "Max-Cut" << " | "
              << "Solution\n";
    std::cout << std::string(70, '-') << "\n";

    for (const auto& res : results) {
        std::cout << std::setw(20) << res.name << " | "
                  << std::setw(12) << std::fixed << std::setprecision(3) << res.time_ms << " | "
                  << std::setw(12) << res.max_cut << " | ";
        for (char val : res.solution) {
            std::cout << int(val) << " ";
        }
        std::cout << "\n";
    }



    return 0;
}
