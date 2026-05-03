#include <iostream>
#include <quadmath.h>
#include <chrono>
#include <stdexcept>

__float128 e = expq((__float128)1.0);
__float128 pi = acosq((__float128)-1.0);
    
__float128 pown(__float128 x, int power){
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
	
__float128 stierling(long long n){
    if(n < 0){
    	throw std::runtime_error("Less than 0");
    }
    if(n == 0 || n == 1){
    	return 1.0Q;
    }
	
    
	return sqrtq(pi * 2.0Q * n) * powq(n / e, n);
}

__float128 stierling_powq(long long n){
    if(n < 0){
    	throw std::runtime_error("Less than 0");
    }
    if(n == 0 || n == 1){
    	return 1.0Q;
    }
	
    
	return sqrtq(pi * 2.0Q * n) * powq(n / e, n);
}

int main(){
	long double n;
	std::cout << "Input n: ";
	std::cin >> n; 

	auto t0 = std::chrono::steady_clock::now();
	__float128 res_0 = stierling_powq(n);
	auto t1 = std::chrono::steady_clock::now();
    long long dt0 = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();

	t0 = std::chrono::steady_clock::now();
	__float128 res_1 = stierling(n);
	t1 = std::chrono::steady_clock::now();
    long long dt1 = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();

	std::cout << "The result with lib powq is approximately: " << (long double) res_0 << " in " << dt0 << " nanoseconds;" << '\n';
	std::cout << "The result with my  pown is approximately: " << (long double) res_1 << " in " << dt1 << " nanoseconds;" << '\n';

}
