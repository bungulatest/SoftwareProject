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

typedef enum {
	NEW_WORLD_BUILDER_WINDOW,
	EDIT_WORLD_BUILDER_WINDOW
} WorldBuilderWindow;

typedef enum {
	NO_WINNER,
	DRAW,
	MOUSE_WON,
	CAT_WON
} GameStatus;


typedef struct gameconfig {
	int catSkill; //skill 0 is human.
	int mouseSkill;
	int worldIndex;
} GameConfig;

typedef enum {
	VALID,
	CAT_IS_MISSING,
	MOUSE_IS_MISSING,
	CHEESE_IS_MISSING
} GameValidity;

typedef struct world {
	GameConfig* gameconfig;
	char** gameBoard;
	Animal currAnimal;
	int totalTurns;
	int currTurn;
	GameStatus gameStatus;
	Animal firstAnimal;
	int mouseXPos;
	int mouseYPos;
	int catXPos;
	int catYPos;
	int cheeseXPos;
	int cheeseYPos;
	int isGameOver;
	int isPaused;
	GameValidity gameValidity;
} World;

typedef struct model {
	int stateIdModel; //can't be state id type to avoid circular dependency
	World* world;
	GameConfig* gameConfig;
	struct model* prevModel;
	int markedButton;

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
	RIGHT,
	INVALID
} Direction;

typedef struct logicevent {
	EventType type;
	int markedId; // the Id of the current marked button
	int buttonId; // the Id of the chosen button (marked or selected)
	int row, col; // row and column of mouse click in game
	Direction direction;


} LogicEvent;

Model* createModel(int stateId, Model* prevModel, int markedButton);
GameConfig* createGameConfig(int catSkill, int mouseSkill, int worldIndex);
void freeModel(Model* model);
LogicEvent* createLogicEvent(EventType type, int markedButton, int buttonId, int row, int col, Direction direction);


#endif // Model_h__
