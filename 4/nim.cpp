#include <iostream>
#include <vector>


int turn = 0;
std::vector <int> piles{3, 4, 5};

bool check_turn(int pile, int take_stones){
	return piles[pile] != 0;
}

bool check_win(){
	for(const auto& pile: piles){
		if(pile != 0){
			return false;
		}
	}
	
	return true;
}

int main(){
	std::cout << "Welcome to Nim game!";
	for(;;){
		int pile, take_stones;
		std::cout << "Piles:\n";
		for(const auto& pile: piles){
			for(int i = 0; i < pile; ++i){
				std::cout << '#';
			}
			std::cout << '\n';
		}
		std::cout << "It's player " << turn << " turn, please write the pile number (0-" << piles.size() - 1 << ") and number of stones to take. If you take more than there is - you just empty the pile.\n";
		std::cin >> pile >> take_stones;
		if(check_turn(pile, take_stones)){
			piles[pile] -= take_stones;
			if(check_win()){
				std::cout << "Player " << turn << " won. Congrats!\n";
				return 0;
			}
		}else{
			std::cout << "Invalid turn!!! Retake your turn!\n";
			continue;
		}

		turn = (turn + 1) % 2;
	}
}