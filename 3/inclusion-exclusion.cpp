#include <iostream>
using namespace std;

int main() {
    int A, B, C;
    int AB, AC, BC;
    int ABC;

    cout << "Enter |A|: ";
    cin >> A;
    cout << "Enter |B|: ";
    cin >> B;
    cout << "Enter |C|: ";
    cin >> C;

    cout << "Enter |A ∩ B|: ";
    cin >> AB;
    cout << "Enter |A ∩ C|: ";
    cin >> AC;
    cout << "Enter |B ∩ C|: ";
    cin >> BC;

    cout << "Enter |A ∩ B ∩ C|: ";
    cin >> ABC;

    int unionSize = A + B + C - AB - AC - BC + ABC;
    cout << "Cardinality of A ∪ B ∪ C: " << unionSize << endl;

    return 0;
}