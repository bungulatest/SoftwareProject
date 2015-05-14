#include "ConsoleMode.h"
#include "../ModelInfrastructure/Model.h"
#include "../Services/EvaluationService.h"
#include "../Services/WorldFileService.h"
#include <stdio.h>

int startConsole(Animal animal) {
	WorldData* worldData;
	World* world;
	int evaluation;
	int exit = 0;
	
	worldData = createEmptyWorldData();


	while (!exit) {
		if (createWorldDataFromFile(stdin, worldData) != 0) {
			return 1;
		}


		world = createEmptyWorld();
		convertWorldDataToWorld(world, worldData);

		freeWorldData(worldData);

		evaluation = evaluateBoard(world);

		// the mouse is defined to be MAX player, and the evaluation function is symmetric,
		// so the evaluation of the cat is negative of the mouse.
		if (animal == CAT) {
			evaluation *= -1;
		}

		printf("%d \n", evaluation);
		freeWorld(world);
	}

	return 0;

	
	
}