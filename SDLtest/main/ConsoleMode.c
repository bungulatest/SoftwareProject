#include "ConsoleMode.h"
#include "../ModelInfrastructure/Model.h"
#include "../Services/EvaluationService.h"
#include "../Services/WorldFileService.h"
#include "../ViewInfrastructure/BitmapFont.h"
#include <stdio.h>
#include <stdlib.h>

int startConsole(Animal animal) {
	WorldData* worldData;
	World* world;
	int evaluation;
	int stop = 0;
	
	


	while (!stop) {
		worldData = createEmptyWorldData();

		if (createWorldDataFromFile(stdin, worldData) != 0) {
			perror("Error: could not parse world from input\n");
			exit(1);
		}

		if (worldData->totalTurns == 0) {
			freeWorldData(worldData);
			exit(0); // user pressed 'q': we want to exit
		}

		world = createEmptyWorld();
		convertWorldDataToWorld(world, worldData);

		freeWorldData(worldData);

		evaluation = evaluateBoard((void*) world);

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