#include <iostream>


namespace Ackermann{
	int exp(int x, int power){
		int result = 1;
			while(power > 0){
				if(power % 2 == 1){
					result *= x;
				}
				x *= x;
				power /= 2;
			}
		return result;		
	}
	
	int tetration(int x, int tetr){
		    if(x == 0 && tetr == 0){
		   		throw std::invalid_argument("0^0 is undefined.");
		   	}else if(x == 0){
		   		return 0;
		   	}else if(tetr == 0){
		   		return 1;
		   	}else if(tetr == 1){
		   		return x;
		   	}
		   	return exp(x, tetration(x, tetr - 1));
	}
	
	int A(int m, int n, size_t& calls){
		++calls;
		if(m < 0 or n < 0){
			return -1;
		}
		if(m == 0){
			return n + 1;
		}else if(n == 0){
			return A(m - 1, 1, calls);
		}else if(m == 4){
			return tetration(m, n + 3) - 3;
		}else{
			return A(m - 1, A(m, n - 1, calls), calls);
		}
	}


}

void rec_test(size_t& calls){
	++calls;
	if(!calls % 10000000){
		std::cout << calls << '\n';
	}
	rec_test(calls);
}

int main(){
	//size_t calls = 0;
	//rec_test(calls);
	std::cout << "Welcome to Wilhelm Ackermann's function executor\nPlease, provide the m n integers or exit with return\n";
	std::cout << "You can try A(4, n) as well;)\n";
	int m, n;
	while(std::cin >> m >> n){
		size_t calls = 0;
		std::cout << "Value for A(" << m << ", " << n <<  ") = " << Ackermann::A(m, n, calls) << " with " << calls << " calls" << '\n';
		std::cout << "Please, provide the value or exit with return\n";
	}
}

