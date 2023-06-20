#pragma once
#include <vector>
#include <iostream>
#include "GameObject.h"
#include <thread>
#include <map>

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
	SDL_Rect rect;

	bool isGoingUp()
	{
		return srcFloor < dstFloor;
	}
	bool isGoingDown()
	{
		return srcFloor > dstFloor;
	}

};



class Elevator
{

	const int MOVE_DURATION_MS = 1000;
	chrono::steady_clock::time_point timerStart;
	const int MAX_TIME_SPENT_ON_EMPTY_FLOOR = 5;
	int timeSpentOnEmptyFloor = 0;

	Elevator()
	{
		rect = startingRect;
		timerStart = chrono::steady_clock::now();
	};

public:

	SDL_Rect rect;
	const int MAX_WEIGHT = 600;
	const int NUMBER_OF_FLOORS = 6;
	bool isGoingUp = true;

	int currentFloor = 0;
	vector<Person> peopleInElevator;
	vector<Person> peopleWaiting;
	const SDL_Rect startingRect = { 300,80,100,100 };
	const SDL_Rect infoRect = { 0,0,200,100 };


	Elevator(Elevator const&) = delete;
	void operator=(Elevator const&) = delete;

	static Elevator& get()
	{
		static Elevator instance;
		return instance;
	}

	bool isTimeToUpdate()
	{
		auto stop = chrono::steady_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(stop - timerStart);
		if (duration.count() < MOVE_DURATION_MS)
		{
			return false;
		}
		else
		{
			timerStart = chrono::steady_clock::now();
			return true;
		}
	}

	void update()
	{
		if (!isTimeToUpdate())
		{
			return;
		}

		if (peopleInElevator.empty())
		{
			if (peopleWaiting.empty())
			{
				// wait 5s and go down
				if (timeSpentOnEmptyFloor < MAX_TIME_SPENT_ON_EMPTY_FLOOR)
				{
					timeSpentOnEmptyFloor++;
				}
				else
				{
					timeSpentOnEmptyFloor = 0;
					currentFloor = 0;
				}
			}
			else
			{
				timeSpentOnEmptyFloor = 0;
				Person firstPerson = peopleWaiting[0];
				if (firstPerson.srcFloor == currentFloor)
				{
					isGoingUp = firstPerson.isGoingUp();
				}
				else
				{
					isGoingUp = firstPerson.srcFloor > currentFloor;
				}
				pickUpPeopleInTheSameDirection();
				moveNextFloor();
			}
		}
		else
		{
			dropOffPeople();
			pickUpPeopleInTheSameDirection();
			if (!peopleInElevator.empty())
			{
				moveNextFloor();
			}
			else
			{
				return;
			}
		}
		//system("cls");
		//print();
		//cin.get();
	}

	void render()
	{
		// ELEVATOR SHAFT
		SDL_Rect shaftRect = startingRect;
		shaftRect.h = NUMBER_OF_FLOORS * rect.h + 2;
		shaftRect = Window::generatePaddingRect(shaftRect, -1, -1);
		Window::drawRect(shaftRect, 0, 255, 0);
		// ELEVATOR CAR
		rect.y = startingRect.y + rect.h * (NUMBER_OF_FLOORS - currentFloor - 1);
		Window::drawRect(rect, 0, 0, 255);
		// PEOPLE
		renderPeopleWaiting();
		renderPeopleInElevator(rect, peopleInElevator.size());
		// GENERAL INFO LOG
		renderInfo();
	}

private:

	void renderPeopleWaiting()
	{
		SDL_Rect personRect = { startingRect.x + startingRect.w + 10, startingRect.y, 30,30 };
		map<int, int> offset;
		for (int i = 0; i < peopleWaiting.size(); i++)
		{
			offset[peopleWaiting[i].srcFloor]++;
			personRect.y = startingRect.y + (NUMBER_OF_FLOORS - peopleWaiting[i].srcFloor) * rect.h - personRect.h;
			personRect.x = startingRect.x + startingRect.w + 10 + (personRect.w + 1) * offset[peopleWaiting[i].srcFloor];
			Window::drawRect(personRect, 255, 0, 0);
			SDL_Point centred = Window::Text::getCenteredTextPoint(personRect, to_string(peopleWaiting[i].dstFloor));
			Window::Text::drawString(centred.x, centred.y, to_string(peopleWaiting[i].dstFloor).c_str());
		}
	}

	void renderPeopleInElevator(SDL_Rect square, int n)
	{
		// collision when wants to render and update at the same time
		if (n == 0)
		{
			return;
		}
		square = Window::generatePaddingRect(square, 2, 2);

		int rectWidth = square.w / ceil(sqrt(n));
		int rectHeight = square.h / ceil(sqrt(n));
		// min to prevent squares from taking all the space
		rectWidth = min(rectWidth, square.w / 3);
		rectHeight = min(rectHeight, square.h / 3);

		int rectsPerRow = square.w / rectWidth;
		int rectsPerCol = square.h / rectHeight;

		int k = 0;
		for (int i = 0; i < rectsPerCol; i++)
		{
			for (int j = 0; j < rectsPerRow; j++)
			{
				if (k > n - 1)
				{
					return;
				}
				Person& person = peopleInElevator[k];
				int rectX = square.x + (j * rectWidth);
				int rectY = square.y + (i * rectHeight);
				person.rect = { rectX, rectY, rectWidth, rectHeight };
				Window::drawRect(person.rect, 255, 0, 0);
				SDL_Point centered = Window::Text::getCenteredTextPoint(person.rect, to_string(person.dstFloor));
				Window::Text::drawString(centered.x, centered.y, to_string(person.dstFloor).c_str());
				k++;
			}
		}
	}

	void renderInfo()
	{
		Window::drawRect(infoRect, 0, 0, 255);

		string weightInfo = "WEIGHT: " + to_string(peopleInElevator.size() * Person::WEIGHT);
		SDL_Point centeredWeightInfo = Window::Text::getCenteredTextPoint(infoRect, weightInfo);
		Window::Text::drawString(centeredWeightInfo.x, centeredWeightInfo.y - Window::Text::LETTER_SIZE, weightInfo.c_str());

		string floorInfo = "FLOOR: " + to_string(currentFloor);
		SDL_Point centeredFloorInfo = Window::Text::getCenteredTextPoint(infoRect, floorInfo);
		Window::Text::drawString(centeredFloorInfo.x, centeredWeightInfo.y + Window::Text::LETTER_SIZE, floorInfo.c_str());
	}



	void printDebug()
	{
		for (int i = NUMBER_OF_FLOORS - 1; i >= 0; i--)
		{
			string shaft = string() + "| " + ((i == currentFloor) ? "@" : " ") + " |";
			cout << i << shaft;
			printDebugPersonsOnFloor(i);
			cout << '\n';
		}
		cout << "-------\n";
		cout << "CF: " << currentFloor << endl;
		for (const Person& person : peopleInElevator)
			cout << person.ID << ", ";
		cout << "\nPE^ , PWv\n";
		for (const Person& person : peopleWaiting)
			cout << person.ID << ", \n";
	}

	void printDebugPersonsOnFloor(int floor)
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
				if ((peopleInElevator.size() + 1) * person.WEIGHT <= MAX_WEIGHT)
				{
					enterElevator(person, i);
					i--; // after deleting element we need to back up 1 element
				}
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

