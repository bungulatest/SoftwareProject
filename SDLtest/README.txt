##########################################################################################

README file for the CatAndMouse project (2015)
Partners: Ilay Luz & May Michelson

##########################################################################################

Our design choices:
	We chose to stay with the MVP design, with a few adjustments:
	1. We changed the guis array to be a global pointer array, so that every screen could access it,
	   and will allocate new memory for the models only if it hasn't been done before.
	2. We changed the return and parameter types of the function pointers in the GUI struct from void*
	   into the actual struct types we created. We did it because we used the same structs for every screen,
	   Model struct for the models, Widget struct for the view hierarchy, and LogicEvent struct for the logical events.
	3. We changed the event loop of MVP: if the event queue is empty, we created a "NO_EVENT" type of logical event,
	   to make it possible to pause machine-machine games.
	
	We chose to create a separate file for each GUI - each holds the 4 functions that define the GUI struct:
	start, viewTranslateEvent, presenterHandleEvent, and stop.
	The reasoning behind this is that we wanted it to be easy to add further screens, and to be able to test parts
	of the game before all the screens are finished.
	We avoided code duplication by creating functions that serve multiple screens, and calling them from the relevant function of the screen.
						 
	We created a Widget struct defined in Widget.h, and every widget is of that type. There is a general constructor for a widget that sets every field,
	and a constructor for every one of the 4 types that calls the general constructor with it's relevant field. For example, a Panel widget doesn't need a text
	string field, so the createPanel function calls the createWidget function with a NULL in the appropriate place.
						 
	The widgets are defined in a tree: every widget has a children list, the root of the tree is the window widget which holds a pointer to SDL_Surface that was created once,
	panels are internal nodes in the tree, and buttons and labels are leafs.
	There are recursive functions that draw the widget tree by calling the draw function of each widget.
	The reasoning behind this is making it easy to add widget types, by making generic functions that use function pointers of the widget struct.
						 
	

##########################################################################################

List of modules:
	main -				  We included the required files (ListUtils, MiniMax, CatAndMouse), and the console mode handler.
						  In order to implement the alpha-beta minimax function without changing the MiniMax.h header file,
						  we added a MiniMaxWithAlphaBeta.h file that declares the alpha-beta function, and implemented it in
						  MiniMax.c .
	ModelInfrastructure - It includes all the structs that the Model struct uses and their constructors and destructors, the Logical Event struct and it's constructor, and the GUI factory.
						  The GUI factory is a function that receives a stateId for the relevant screen, and allocates and initialized the GUI struct
						  for that screen, including the function pointers.
						  The gui.h header defines the GUI struct, and an enum that defines the StateId.
						  
						  The Model struct includes all the information that is contained in the model part of MVP.
						  It contains 3 fields that are relevant to every screen:
						  1. stateIdModel - the stateId of the screen
						  2. prevModel - holds a pointer to the previous model, used for knowing where to go when clicking "back"
						  3. markedButton - the ID of the current marked button (explained below in "Screens")
						  
						  and also 2 structs that are not used in all the screens (they are initialized to NULL if they are unused):
						  The GameConfig structs holds that is relevant when configuring the game - when choosing the type and skill of the players,
						  and choosing the world number.
						  
						  The World struct holds data about the active game itself - the game board, the current player, player positions, information
						  about the status of the game and etc. The world builder screens also use this struct for building the game board and choosing the first player.
						  
						  The LogicEvent struct holds all the information that is needed for handling a logical event:
						  The type of the logical event( mark a button, select a button, move a player to a position, etc).
						  The ID of the current marked button (so that the presenterHandleEvent could know which button to unmark)
						  The ID of the button to be marked
						  The position or direction to move the current player to.
						  
						  
						  
	ViewInfrastrucutre - It includes the definitions of the 4 types of widgets that were defined in the project instructions.
						 We created a separate file for each of the 4 types, and a Widget.c file that includes general functions for the widgets
						 , and constructors and destructors ( as described above in design).
						 
						 To make it easy to change text on buttons and labels, we used a bitmap font: we used several pictures of the ascii characters,
						 each picture in a different letter size of font. When displaying a text on the screen, instead of using a bmp file with the exact text,
						 we simply extract the characters from the bitmap font file, and blit them to the surface. By doing it this way, we didn't need to create
						 dozens of bmp files for the project, just a few for different screens.
						 To do this, we defined a BitmapFont struct in BitmapFont.h, that is initialized once for every bitmap font file. These structs are global pointer variables,
						 so that every screen could use an initialized font without reinitializing it itself.
						 
						 There are 3 different states for every button - marked, regular, and disabled. So we created 3 directories for the pictures.
						 When we mark, unmark, disable, or enable a button, we change the image by simply changing the the directory from which we read the file.
						 
	Screens - 			It includes a separate file for each screen, each screen is initialized by a start function. The start function calls an initialize window function,
						that creates the view. The start function also initializes the model (or updates the model if it was already initialized).
						
						In order to prevent code duplication, we merged some of the similar screens to one file. For example, ChooseSkill.c is used for both the cat skill and mouse skill window,
						SelectWorld.c is used for the load, save, and edit windows.
						
						In addition to the 4 functions that define a screen and the initialize window function, there are helper functions in each screen, that update the view according to the model -
						they accept the view and the model as parameters. The GameWindow.c file holds such helper functions for the UI elements in the game play window and world builder window.
						
	services - 			The services are functions that accept as parameter the model or parts of the model (never the view), and are used by the presenters.
						We did this because this functions need to be modular, and uncoupled from the view (for example, the console mode uses the Evaluation Service without needing a view at all).
						
						The Evaluation service calculates the evaluation of a world (described below).
						The Move Service handles the game logic - advancing players on the game board, getting the machine move by calling MiniMax, etc.
						The World File Service handles reading and writing to the world files.

##########################################################################################

Utility evaluation function:
	high level description - calculate the distance between the cat and the mouse (taking into consideration obstacles like walls), calculate the distance betwen the mouse and the cheese,
							 and add the distances multiplied by constant factors:
							 Evaluation = A*mouseToCheese + B*catToMouse
							 We also check if the game is won, and if the cat won, the evaluation is MIN_EVALUATION which represents the lowest number.
							 If the mouse won, the evaluation is MAX_EVALUATION (the largest number).
							 If the game ended with a draw, the evaluation is 0.
							 
							 
							 The distance is computed by a BFS on the game board.
							 
	reasoning -				 We defined the mouse to be the max player, and the cat min player, so the A factor is negative and B is positive, because the mouse wants to be close the cheese,
							 and far from the cat.
							 The evaluation function is symmetric, because the game is zero sum - what is good for the mouse is bad for the cat and vice versa.
							 We chose the weights by trial and error.
							 
	running time - 			 The running time of BFS is O(|V| + |E|), with |E| being the number of edges, and |V| the number of nodes.
							 In our case, the grid is a square grid, so the number of nodes is n^2, and the number of edges is O(n^2),
							 so the total running time of the BFS calculations is O(n^2).
							 
							 Our evaluation function does a constant number of BFS searches, plus constant work, so the running time is O(n^2).

##########################################################################################
