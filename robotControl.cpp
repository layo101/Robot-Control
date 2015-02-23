#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

int main()
{
	//This program controls a sorting robot
	//This program keeps track of which way the robot is facing
	//The robot grasps a bottle or fragment, lifts it, then
	//rotates to the appropriate conveyor(or the waste container)
	//and lowers the bottle and releases it.
	//after releasing the bottle(or fragment) the robot rotates back
	//(oposite direction) to the incoming conveyor belt
	//This program prompts the user for an input file
	//In the input file are the number of Sensor readings
	//On the left of the input document is the weight sensor reading
	//On the right of the input document is the color sensor reading
	//Creation date: 2014-10-30
	//Last modified date:2014-11-04
	//Author: Layomi Dele-Dare

	// declare and initialize constants
	const int max_retries = 5;
	const int minNumPairs = 1;
	const int maxNumPairs = 10000;
	const int minSensorReading = 0;
	const int maxSensorReading = 255;
	const double minSpringLength = 10.00;
	const double maxSpringLength = 15.00;
	const double minIncreaseInLength = 0.0;
	const double maxIncreaseInLength = 5.0;
	const double minWeight = 0.0;
	const double maxWeight = 500.00;
	const double coeff = 12.0;
	const int minClearBottle = 240;
	const int maxClearBottle = 255;
	const int minGreenBottle = 150;
	const int maxGreenBottle = 175;
	const int minBrownBottle = 20;
	const int maxBrownBottle = 90;
	const int minSmallBottleWeight = 15;
	const int maxSmallBottleWeight = 21;
	const int minMediumBottleWeight = 35;
	const int maxMediumBottleWeight = 41;
	const int minLargeBottleWeight = 53;
	const int maxLargeBottleWeight = 59;


	// declare and initialize variables
	double springLength = 0.0;
	double increaseInLength = 0.0;
	double weight = 0.0;
	char inputName[80] = { '\0' };
	char outputName[80] = { '\0' };
	int retries = 0;
	int numPairs = 0;
	int outputValues = 0;
	int count = 0;
	int i = 0;
	int nextValue1 = 0;
	int nextValue2 = 0;
	int numBrownBottles = 0;
	int numClearBottles = 0;
	int numGreenBottles = 0;
	double totalWeight = 0;
	bool conveyorStop = false;
	double convert = 0.001;
	double conversion = 0.0;
	double totalGlassWaste = 0.0;
	ifstream myInputStream;
	ofstream myOutputStream;

	//request input filename 
	//open the file and check it was opened correctly
	//if all 6 tries fail, an error message would be printed
	do{
		cout << "Please enter the name of the input file containing sensor readings:" << endl;
		cin >> inputName;
		myInputStream.open(inputName);
		if (myInputStream.fail())
		{
			cerr << "The input file " << inputName << " could not be opened" << endl;
		}
		if (retries == max_retries)
		{
			cout << "You exceeded maximum number of tries allowed for entering the input fileName" << endl;
			return 1;
		}
		retries++;
	} while (myInputStream.fail());

	//request output filename
	//open the file and check it was opened correctly
	//if all 6 tries fail, an error message would be printed
	do{
		cout << "Please enter the name of the output file containing simulation results:" << endl;
		cin >> outputName;
		myOutputStream.open(outputName);
		if (myOutputStream.fail())
		{
			cerr << "The output file " << outputName << " could not be opened" << endl;
		}
		if (retries == max_retries)
		{
			cerr << "You exceeded the maximum number of tries allowed for entering the output filename" << endl;
			return 2;
		}
		retries++;
	} while (myOutputStream.fail());
	
	//This condition in the first if below is true if
	//data could not be read. The condition in the nested if 
	//is true if the end of the file is reached
	//NOTE: EOF is one possible	reason that the data could not be read
	//The outer if is entered if data could not be read
	//The inner if is entered if an attempt to read past the end of the
	//file has been made

	if (!(myInputStream >> numPairs))
	{

		if (myInputStream.eof())
		{
			cerr << "Data file is empty" << endl;
			//close the file before leaving the program
			//(for all possible ways of leaving the program)
			myInputStream.close();
			return 3;
		}
		else
		{
			cerr << "Unable to read the number of sensor readings" << endl;
			myInputStream.close();
			return 4;
		}
	}

	if (numPairs <= minNumPairs || numPairs > maxNumPairs)
	{
		cerr << "The number of sensor readings is out of range" << endl;
		myInputStream.close();
		return 5;
	}
	for (count = 1; count <= numPairs; count++)
	{
		if (!(myInputStream >> nextValue1))
		{

			if (myInputStream.eof())
			{
				//if the first value is not read, we are at the end of the data in the file
				//or we have encountered incorrect data that is not part of an integer
				cerr << "No more data, simulation completed early at line ";
				cerr << count + 1 << " of input" << endl;
				break;
			}
			else
			{
				//We should have more data, so print an error message
				//Then exit the loop
				cerr << "The number read on line ";
				cerr << count + 1 << " of your input file is not an integer" << endl;
				cerr << "Please correct your data file and then rerun" << endl;
				return 6;
			}
		}
		else if (!(myInputStream >> nextValue2))
		{
			if (myInputStream.eof())
			{
				//if the second value of the pair is not read, we are at the end of the data in the file
				//or we have encountered incorrect data that is not part of an integer
				cerr << "No light sensor measurement to match final weight measurement" << endl;
				break;
			}
			else
			{
				//We should have more data, so print an error message
				//Then exit the loop
				cerr << "The number read on line ";
				cerr << count + 1 << " of your input file is not an integer" << endl;
				cerr << "Please correct your data file and then rerun" << endl;
				return 7;
			}
		}
		//if the sensor reading is in range
		if ((nextValue1 >= minSensorReading && nextValue1 <= maxSensorReading)
			&& (nextValue2 >= minSensorReading && nextValue2 <= maxSensorReading))
		{
			//convert the integer to the weight of the bottle or the fragment of the bottle in grams
			increaseInLength = (nextValue1 * maxIncreaseInLength) / maxSensorReading;
			weight = coeff * increaseInLength;
			
			//rotate 90 degrees when the bottle is clear glass
			if ((weight >= minSmallBottleWeight && weight <= maxSmallBottleWeight) && (nextValue2 >= minClearBottle && nextValue2 <= maxClearBottle)){
				numClearBottles++;

				//prints to the screen
				cout << "The robot has rotated 90 degrees to face the clear glass conveyor belt" << endl;
				cout << "The robot has placed " << numClearBottles << " clear bottles on the conveyor belt" << endl;
				cout << "The robot rotates -90 degrees" << endl;

				//prints to the output file
				myOutputStream << "The robot has rotated 90 degees to face the clear glass conveyor belt" << endl;
				myOutputStream << "The robot has placed " << numClearBottles << " clear bottles on the conveyor belt" << endl;
				myOutputStream << "The robot rotates -90 degrees" << endl;
			}
			// rotates 150 degrees when the bottle is green glass
			else if ((weight >= minMediumBottleWeight && weight <= maxMediumBottleWeight) && (nextValue2 >= minGreenBottle && nextValue2 <= maxGreenBottle)){
				numGreenBottles++;

				//prints to the screen
				cout << "The robot has rotated 150 degrees to face the green glass conveyor belt" << endl;
				cout << "The robot has placed " << numGreenBottles << " green bottles on the conveyor belt" << endl;
				cout << "The robot rotates -150 degrees" << endl;

				//prints to the output file
				myOutputStream << "The robot has rotated 150 degrees to face the green glass conveyor belt" << endl;
				myOutputStream << "The robot has placed " << numGreenBottles << " green bottles on the conveyor belt" << endl;
				myOutputStream << "The robot rotates -150 degrees" << endl;
			}
			//rotates 210 degrees when the bottle is brown glass
			else if ((weight >= minLargeBottleWeight && weight <= maxLargeBottleWeight) && (nextValue2 >= minBrownBottle && nextValue2 <= maxBrownBottle)){
				numBrownBottles++;
				//prints to the screen
				cout << "The robot has rotated 210 degrees to face the brown glass conveyor belt" << endl;
				cout << "The robot has placed " << numBrownBottles << " brown bottles on the conveyor belt" << endl;
				cout << "The robot rotates -210 degrees" << endl;

				//prints to output file
				myOutputStream << "The robot has rotated 210 degrees to face the brown glass conveyor belt" << endl;
				myOutputStream << "The robot has placed " << numBrownBottles << " brown bottles on the conveyor belt" << endl;
				myOutputStream << "The robot rotates -210 degrees" << endl;
			}
			else{
				//Rotates 270 degrees when a bottle fragment is detected or when the color of the bottle or fragment is not clear, green or brown
				//accumulates the sum of glass weight
				totalWeight += weight;
				totalGlassWaste += weight;
				cout << "The robot has rotated 270 degrees to face the waste container" << endl;
				//Sets the total weight to the current weight when the total weight is greater than the max weight
				if (totalWeight > maxWeight){
					//prints to the screen
					cout << "A new waste container is needed" << endl;
					cout << "Stop the incoming conveyor belt" << endl;
					cout << "A new waste container is ready for use" << endl;

					//prints to output
					myOutputStream << "A new waste container is needed" << endl;
					myOutputStream << "Stop the incoming conveyor belt" << endl;
					myOutputStream << "A new waste container is ready for use" << endl;

					totalWeight = weight;
					conveyorStop = true;
				}

				cout << "The waste container now contains " << setw(8) << fixed << setprecision(3) << totalWeight << " grams of glass" << endl;
				cout << "The robot rotates -270 degrees" << endl;

				myOutputStream << "The waste container now contains " << setw(8) << fixed << setprecision(3) << totalWeight << " grams of glass" << endl;
				myOutputStream << "The robot rotates -270 degrees" << endl;
				if (conveyorStop == true)
				{
					cout << "Start the incoming conveyor belt" << endl;
					myOutputStream << "Start the incoming conveyor belt" << endl;
				}
			}
		}
		else
		{
			//print an error message for any sensor readings that are out of range
			cerr << "Sensor reading out of range, ignoring line ";
			cerr << count + 1 << " of input file" << endl;
		}
	}
	//Calculate and convert total glass weight from grams to kilograms
	conversion = totalGlassWaste * convert;
	cout << endl << endl << endl;
	myOutputStream << endl << endl << endl;

	//prints the results to the screen
	cout << "The robot processed " << numClearBottles << " clear bottles" << endl;
	cout << "The robot processed " << numGreenBottles << " green bottles" << endl;
	cout << "The robot processed " << numBrownBottles << " brown bottles" << endl;
	cout << "The robot processed " << setw(12) << fixed << setprecision(4) << conversion << " kg of waste glass" << endl;

	//prints the results to the output
	myOutputStream << "The robot processed " << numClearBottles << " clear bottles" << endl;
	myOutputStream << "The robot processed " << numGreenBottles << " green bottles" << endl;
	myOutputStream << "The robot processed " << numBrownBottles << " brown bottles" << endl;
	myOutputStream << "The robot processed " << setw(12) << fixed << setprecision(4) << conversion << " kg of waste glass" << endl;

	//Close the file when leaving the program
	myInputStream.close();
	myOutputStream.close();


	return 0;
}