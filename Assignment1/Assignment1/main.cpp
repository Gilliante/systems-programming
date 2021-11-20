#include <iostream>

int main()
{
	int numbers[10];
	int input;
	int length = sizeof(numbers)/sizeof(*numbers);

	std::cout << "Please input " << length << " numbers\n";

	//Populate array from user
	for (int i = 0; i < length; i++)
	{
		std::cin >> input;
		numbers[i] = input;
	}

	//print array before sort
	std::cout << "Numbers before sort:";
	for (int i = 0; i < length; i++)
	{
		std::cout << " " << numbers[i];
	}
	std::cout << "\n";

	//Sort the array
	for (int i = 1; i < length; i++)
	{
		int temp = numbers[i];
		int j = i - 1;
		while (j >= 0 && numbers[j] > temp)
		{
			numbers[j + 1] = numbers[j];
			j--;
		}
		numbers[j + 1] = temp;
	}

	//print array after sort
	std::cout << "Numbers after sort:";
	for (int i = 0; i < length; i++)
	{
		std::cout << " " << numbers[i];
	}
	std::cout << "\n";

	system("pause");
}

/*import java.util.Scanner;


public class InsertionSort
{
	public static void main(String[] args)
	{
		Scanner input = new Scanner(System.in);

		int[] numbers = new int[10];

		//Please note that arrays in C++ are not objects
		System.out.println("Please input " + numbers.length + " numbers\n");

		//Populate array from user
		for(int i=0; i < numbers.length; i++)
		{
			numbers[i] = input.nextInt();
		}

		//print array before sort
		System.out.print("Numbers before sort:");
		for(int i=0; i < numbers.length; i++)
		{
			System.out.print(" " + numbers[i]);
		}
		System.out.println();

		//Sort the array
		for(int i=1; i < numbers.length; i++)
		{
			int temp = numbers[i];
			int j = i-1;
			while(j >= 0 && numbers[j] > temp)
			{
				numbers[j+1] = numbers[j];
				j--;
			}
			numbers[j+1] = temp;
		}

		//print array after sort
		System.out.print("Numbers after sort:");
		for(int i=0; i < numbers.length; i++)
		{
			System.out.print(" " + numbers[i]);
		}
		System.out.println();
	}

}
*/