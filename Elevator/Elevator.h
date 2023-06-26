#pragma once
#include <vector>
#include <iostream>
#include <SDL.h>
#include "Window.h"
#include <chrono>

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
	chrono::steady_clock::time_point timer;
	const int MAX_TIME_SPENT_ON_EMPTY_FLOOR = 5;
	int timeSpentOnEmptyFloor = 0;

	Elevator()
	{
		elevatorCarRect = startingRect;
		timer = chrono::steady_clock::now();
	};

public:

	SDL_Rect elevatorCarRect;
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
		auto now = chrono::steady_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(now - timer);

		if (duration.count() < MOVE_DURATION_MS)
		{
			return false;
		}
		else
		{
			timer = chrono::steady_clock::now();
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
		}
	}

	void render()
	{
		// ELEVATOR SHAFT
		SDL_Rect shaftRect = startingRect;
		shaftRect.h = NUMBER_OF_FLOORS * elevatorCarRect.h + 2;
		shaftRect = Window::generatePaddingRect(shaftRect, -1, -1);
		Window::drawRect(shaftRect, 0, 255, 0);
		// ELEVATOR CAR
		elevatorCarRect.y = startingRect.y + elevatorCarRect.h * (NUMBER_OF_FLOORS - currentFloor - 1);
		Window::drawRect(elevatorCarRect, 0, 0, 255);
		// PEOPLE
		renderPeopleWaiting();
		renderPeopleInElevator(elevatorCarRect, peopleInElevator.size());
		// GENERAL INFO LOG
		renderInfo();
	}

private:

	void renderPeopleWaiting()
	{
		const int ELEVATOR_MARGIN_X = 10;
		const int PERSON_MARGIN_X = 1;
		const int PERSON_SIZE = 30;

		vector<int> peopleOnTheFloor(Elevator::get().NUMBER_OF_FLOORS, 0);
		for (int i = 0; i < peopleWaiting.size(); i++)
		{
			Person& person = peopleWaiting[i];
			peopleOnTheFloor[person.srcFloor]++;
			person.rect.w = PERSON_SIZE;
			person.rect.h = PERSON_SIZE;
			person.rect.y = startingRect.y + (NUMBER_OF_FLOORS - peopleWaiting[i].srcFloor) * elevatorCarRect.h - person.rect.h;
			person.rect.x = startingRect.x + startingRect.w + ELEVATOR_MARGIN_X + (person.rect.w + PERSON_MARGIN_X) * peopleOnTheFloor[peopleWaiting[i].srcFloor];
			Window::drawCenteredTextInRect(person.rect, to_string(peopleWaiting[i].dstFloor), 255, 0, 0);
		}
	}

	void renderPeopleInElevator(SDL_Rect elevatorCar, int n)
	{
		if (n == 0)
		{
			return;
		}
		elevatorCar = Window::generatePaddingRect(elevatorCar, 2, 2);

		// calculations to ajust person size (to fit an number of people)
		int rectWidth = elevatorCar.w / ceil(sqrt(n));
		int rectHeight = elevatorCar.h / ceil(sqrt(n));
		// min to prevent person from taking all the space
		rectWidth = min(rectWidth, elevatorCar.w / 3);
		rectHeight = min(rectHeight, elevatorCar.h / 3);

		int rectsPerRow = elevatorCar.w / rectWidth;
		int rectsPerCol = elevatorCar.h / rectHeight;

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
				int rectX = elevatorCar.x + (j * rectWidth);
				int rectY = elevatorCar.y + (i * rectHeight);
				person.rect = { rectX, rectY, rectWidth, rectHeight };
				Window::drawCenteredTextInRect(person.rect, to_string(person.dstFloor), 255, 0, 0);
				k++;
			}
		}
	}

	void renderInfo()
	{
		Window::drawRect(infoRect, 0, 0, 255);

		string weightInfo = "WEIGHT: " + to_string(peopleInElevator.size() * Person::WEIGHT);
		SDL_Point centeredWeightInfo = Window::Text::getCenteredTextPoint(infoRect, weightInfo);
		Window::Text::drawString(centeredWeightInfo.x, centeredWeightInfo.y - Window::Text::LETTER_SIZE, weightInfo);

		string floorInfo = "FLOOR: " + to_string(currentFloor);
		SDL_Point centeredFloorInfo = Window::Text::getCenteredTextPoint(infoRect, floorInfo);
		Window::Text::drawString(centeredFloorInfo.x, centeredWeightInfo.y + Window::Text::LETTER_SIZE, floorInfo);
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
				i--; // after deleting element we need to back up 1 element
			}
		}
	}

	void moveNextFloor()
	{
		currentFloor += isGoingUp ? 1 : -1;
	}

};

