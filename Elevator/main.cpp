#include "SDL.h"
#include "SDL_image.h"
#include "string"
#include "Window.h"
#include "Game.h"

#include <queue>

using namespace std;

class Person
{

public:

	static const int WEIGHT;
	static int ID_counter;
	Person(int srcFloor, int dstFloor)
		: srcFloor(srcFloor), dstFloor(dstFloor), ID(ID_counter)
	{
		ID_counter++;
	}

	int ID;
	int srcFloor;
	int dstFloor;


	bool isGoingUp()
	{
		return srcFloor < dstFloor;
	}
	bool isGoingDown()
	{
		return srcFloor > dstFloor;
	}
	bool operator==(const Person& other)
	{
		return ID == other.ID;
	}

};

class Elevator
{
	
public:

	const int MAX_WEIGHT = 600;
	const int NUMBER_OF_FLOORS = 6;
	bool isGoingUp = true;

	int currentFloor = 0;
	vector<Person> peopleInElevator;
	vector<Person> peopleWaiting;

	void print()
	{
		for (int i = NUMBER_OF_FLOORS - 1; i >= 0; i--)
		{
			string shaft = string() + "| " + ((i == currentFloor) ? "@" : " ") + " |";
			cout << i << shaft;
			printPersonsOnFloor(i);
			cout << '\n';
		}
		cout << "-------\n";
		cout << "CF: " << currentFloor << endl;
		for (auto it : peopleInElevator)
			cout << it.ID << ", "; 
		cout << "\nPE^ , PWv\n";
		for (auto it : peopleWaiting)
			cout << it.ID << ", ";
	}

	void printPersonsOnFloor(int floor)
	{
		for (Person person : peopleWaiting)
		{
			if (person.srcFloor == floor)
			{
				if (person.isGoingUp())
				{
					cout << "^";
				}
				else
				{
					cout << "v";
				}
			}
		}
	}

	void run()
	{

		while (true)
		{
			if (peopleInElevator.empty())
			{
				if (peopleWaiting.empty())
				{
					// wait 5s and go down
					currentFloor = 0;
				}
				else
				{
					Person firstPerson = peopleWaiting[0];

					if (currentFloor == firstPerson.srcFloor)
					{
						enterElevator(firstPerson, 0);
					}
					isGoingUp = firstPerson.isGoingUp();
					pickUpPeopleInTheSameDirection();
					moveNextFloor();
				}
			}
			else
			{
				dropOffPeople();
				pickUpPeopleInTheSameDirection();
				if (peopleInElevator.empty() && peopleWaiting.empty())
				{
					continue;
				}
				else if(peopleInElevator.empty() && !peopleWaiting.empty())
				{
					continue;
				}
				else
				{
					moveNextFloor();
				}
			}
			system("cls");
			print();
			cin.get();
		}
	}

	void enterElevator(Person person, int index)
	{
		peopleWaiting.erase(peopleWaiting.begin() + index);
		peopleInElevator.push_back(person);
	}
	void pickUpPeopleInTheSameDirection()
	{
		for (int i = 0; i < peopleWaiting.size(); i++)
		{
			Person person = peopleWaiting[i];
			if (person.srcFloor == currentFloor && person.isGoingUp() == this->isGoingUp)
			{
				enterElevator(person, i);
				i--; // after deleting element we need to back up 1 element
			}
		}
	}
	void dropOffPeople()
	{
		for (int i = 0; i < peopleInElevator.size(); i++)
		{
			if (peopleInElevator[i].dstFloor == currentFloor)
			{
				peopleInElevator.erase(peopleInElevator.begin() + i);
				i--;
			}
		}
	}

	void moveNextFloor()
	{
		currentFloor += isGoingUp ? 1 : -1;
	}
			
};

const int Person::WEIGHT = 70;
int Person::ID_counter = 0;


int main(int argc, char* args[])
{
	//Game game("title", Window::createRect(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Window::WIDTH, Window::HEIGHT), false);
	//while (game.isRunning)
	//{
	//	game.handleEvents();
	//	game.update();
	//	game.render();
	//}
	//game.clean();



	Elevator elevator;
	//elevator.peopleWaiting.push_back(Person(1, 2));
	//elevator.peopleWaiting.push_back(Person(1, 0));
	elevator.peopleWaiting.push_back(Person(1, 2));
	elevator.peopleWaiting.push_back(Person(3, 1));
	elevator.peopleWaiting.push_back(Person(2, 5));
	elevator.peopleWaiting.push_back(Person(2, 5));
	//cout << elevator.isSomeoneWaitingToGoDown() << endl;
	elevator.print();
	cin.get();
	//elevator.print();
	elevator.run();

	return 0;
}