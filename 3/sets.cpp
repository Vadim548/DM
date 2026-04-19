#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>

using SetStr = std::set<std::string>;

SetStr readSet(){
    SetStr s;
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::string word;
    while(iss >> word){
        s.insert(word);
    }
    return s;
}

void printSet(const SetStr& s){
    std::cout << "{";
    for(const auto& elem: s){
        std::cout << elem << ", ";
    }
    std::cout << "}";
}

SetStr setUnion(SetStr a, const SetStr& b){
    for(const auto& elem: b){
        a.insert(elem);
    }
    return a;
}

SetStr setIntersection(SetStr a, const SetStr& b){
    SetStr result;
    for(const auto& elem: b){
        if(a.find(elem) != a.end()){
            result.insert(elem);
        }
    }
    return result;
}

SetStr setDifference(SetStr a, const SetStr& b){
    for(const auto& elem: b){
        a.erase(elem);
    }
    return a;
}

SetStr absoluteComplement(const SetStr& a, const SetStr& u){
    return setDifference(u, a);
}

void cartesianProduct(const SetStr& a, const SetStr& b){
    std::cout << '{';
    for(const auto& x: a){
        for(const auto& y: b){
            std::cout << "(" << x << ", " << y << "), ";
        }
    }
    std::cout << '}';
}

std::vector<SetStr> powerSet(const SetStr& s){
    std::vector<SetStr> result;
    std::vector<std::string> elements(s.begin(), s.end());
    int n = elements.size();
    int powerSetSize = 1 << n;

    for(int mask = 0; mask < powerSetSize; ++mask){
        SetStr subset;
        for(int i = 0; i < n; ++i){
            if(mask &(1 << i)){
                subset.insert(elements[i]);
            }
        }
        result.push_back(subset);
    }
    return result;
}


void printPowerSet(const std::vector<SetStr>& ps){
    std::cout << "{";
    for(const auto& s: ps){
        printSet(s);
        std::cout << ", ";
    }
    std::cout << "}" << std::endl;
}

int main(){
    std::cout << "Set A(space-separated, return to finish): ";
    SetStr A = readSet();

    std::cout << "Set B(space-separated, return to finish): ";
    SetStr B = readSet();

    std::cout << "Set universal set(space-separated, return to finish): ";
    SetStr U = readSet();

    std::cout << "\nA: ";
    printSet(A);
    
    std::cout << "\nB: ";
    printSet(B);
    
    std::cout << "\nU: ";
    printSet(U);

    std::cout << "\nUnion: ";
    printSet(setUnion(A, B));

    std::cout << "\nIntersection: ";
    printSet(setIntersection(A, B));

    std::cout << "\nA \\ B: ";
    printSet(setDifference(A, B));

    std::cout << "\nB \\ A: ";
    printSet(setDifference(B, A));

    std::cout << "\nAbsolute complement of A: ";
    printSet(absoluteComplement(A, U));

    auto powerset = powerSet(A);
    std::cout << "\nPower set of A: ";
    printPowerSet(powerset);

    return 0;
}