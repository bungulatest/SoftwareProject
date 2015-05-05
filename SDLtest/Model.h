#ifndef Model_h__
#define Model_h__

typedef enum {
	WALL,
	EMPTY
} BoardPoint;

typedef enum {
	CAT,
	MOUSE
} Animal;

typedef enum {
	HUMAN,
	MACHINE
} Player;

typedef enum {
	LOAD,
	EDIT,
	SAVE
} WorldSelectionWindow;

typedef struct world {
	char** gameBoard;
	int totalTurns;
	int currTurn;
	Animal firstAnimal;
	int mouseXPos;
	int mouseYPos;
	int catXPos;
	int catYPos;
	int cheeseXPos;
	int cheeseYPos;
} World;

typedef struct model {
	int stateIdModel; //can't be state id type to avoid circular dependency
	World* world;
	struct model* prevModel;
	int markedButton;
	Player currPlayer;
	Animal currAnimal;
	int catSkill; //skill 0 is human.
	int mouseSkill;
	int isPaused;
	int worldIndex;
} Model;

typedef enum {
	MARK_BUTTON,
	SELECT_BUTTON,
	MOVE_PLAYER_TO_POINT, // by mouse
	MOVE_PLAYER_TO_DIRECTION, // by keyboard
	QUIT_EVENT,
	NO_EVENT
} EventType;

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
} Direction;

typedef struct logicevent {
	EventType type;
	int markedId; // the Id of the current marked button
	int buttonId; // the Id of the chosen button (marked or selected)
	int row, col; // row and column of mouse click in game
	Direction direction;


} LogicEvent;

Model* createModel(int stateId, Model* prevModel, int markedButton, Player currPlayer, Animal currAnimal, int catSkill, int mouseSkill, int isPaused, int worldIndex);
void freeModel(Model* model);
LogicEvent* createLogicEvent(EventType type, int markedButton, int buttonId, int row, int col, Direction direction);


#endif // Model_h__
