#include <iostream>
#include <random>

int generateRandomNumber(int min, int max)
{
	static std::random_device randomDevice;
	static std::mt19937 engine{randomDevice()};
	std::uniform_int_distribution<int> distribution(min, max);
	
	return distribution(engine);
}

int main()
{
	std::cout << "Input difficulty: ";
	
	int difficultyLevel{0};
	std::cin >> difficultyLevel;

	const int biggestPossibleNumber {difficultyLevel * 100};
	int number = generateRandomNumber(0, biggestPossibleNumber);

	int userNumber{0};

	int numberOfGuesses{1};

	while (true)
	{		
		std::cout << "Your input : ";
		std::cin >> userNumber;

		if (number != userNumber)
		{
			std::cout << "You failed\n"; 		
		}	
		else
		{
			break;	
		}

		if (number > userNumber)
		{
			std::cout << "You should input bigger number\n";		
		}
		else
		{
			std::cout << "You should input smaller number\n";		
		} 
		
		numberOfGuesses++;	
	}
 	
	std::cout << "You win!\n";
	std::cout << "Number of inputs : "<< numberOfGuesses << "\n";
}
