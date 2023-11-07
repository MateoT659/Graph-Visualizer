#include "Includes.h"

GraphNode* ghost;
Gate* ghostGate;
std::vector<GraphNode*> nodes;
std::vector<GraphEdge*> edges;
std::vector<FreeEdge*> freeEdges;
std::vector<Textbox*> textboxes;
std::vector<NodeText*> nodetexts;
std::unordered_set<GraphEdge*> switches;
std::unordered_set<FreeEdge*> fswitches;
std::vector<Gate*> gates;
bool running;
EdgeType edgeType; //node type is just ghost's type
SDL_Window* window;
SDL_Surface* windowSurface;
Image *sidebar;
SDL_Renderer* renderer;
SDL_Rect colorBox;
SDL_Color currentColor;
SDL_Color bgColor;
TTF_Font* font;
SDL_Texture *hoveredTexture;
SDL_Texture* selectedTexture;
std::vector<Icon*> icons;
int selectedInd;
std::vector<Icon*> nodeIcons;
std::vector<Icon*> edgeIcons;
std::vector<Icon*> textIcons;
int selectedTextTool;
std::vector<Icon*>gateIcons;
std::string currentToolTip;
Textbox* tip;

/*
* 
* done :) time to export
* 
*/

void renderToolTip() {
	
	tip->setText(currentToolTip);
	Vec2 mousepos = getMousePos();
	if (mousepos.y + 30 >= SCREEN_HEIGHT) {
		tip->setCornerPos(mousepos - Vec2(0, 20));
	}
	else {
		tip->setCornerPos(mousepos + Vec2(10, 10));
	}
	tip->renderBoundingBox();
	tip->render();
}

int main( int argc, char* args[] )
{
	SDL_Event event;
	running = true;
	
	
	
	initSDL();
	initTextures();
	initFiles();

	tip = new Textbox(" ", getMousePos(), 20, { 100, 100, 100, 255 });
	bool toolTipRendered = false;
	long lastRenderMilli = 0;

	while (running) {
		//input section
		while (SDL_PollEvent(&event)){
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			parseEvent(&event);

			renderU(true);
			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli>500) {
			render(false);
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	SDL_DestroyWindow( window );
	SDL_Quit();
	return 0;
}

//inits

void initSDL() {
	//initiates SDL, and creates/launches the window


	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "SDL_Init error: " << SDL_GetError << std::endl;
		exit(1);
	}
	
	window = SDL_CreateWindow("Graph Diagram Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	int imgFlags = IMG_INIT_PNG;

	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image init error: " << IMG_GetError() << "\n";
		exit(1);
	}

	if (TTF_Init() == -1) {
		std::cout << "SDL_font init error: " << TTF_GetError() << "\n";
		exit(1);
	}

	currentToolTip = "";

	 windowSurface = SDL_GetWindowSurface(window);

	 currentColor = WHITE;
	 bgColor = BLACK;
	 colorBox = { 8, SCREEN_HEIGHT - 53, 45, 45 };

	 edgeType = None;
	 ghost = new GraphNode(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 15, currentColor, Filled);
	 ghost->toggleGhost();
	 ghostGate = new UnaryGate(Vec2(), currentColor, IDENTITY);

	clearScreen(BLACK);

}

void initTextures() {
	font = TTF_OpenFont("Assets/font.ttf", 255);
	if (font == NULL)
	{
		std::cout << "Failed to load font! Error: " << TTF_GetError() << "\n";
	}

	sidebar = new Image(0, 0, 61, 1000, "Assets/Sidebar.png");

	//icon textures
	hoveredTexture = loadTexture("Assets/IconHover.png");
	selectedTexture = loadTexture("Assets/IconSelected.png");

	//node type icons
	nodeIcons.push_back(new Icon(0, 0, "Assets/IconFilledCircle.png", "Circle Node"));
	nodeIcons.push_back(new Icon(63, 0, "Assets/IconOpenCircle.png", "Open Circle"));
	nodeIcons.push_back(new Icon(63 * 2, 0, "Assets/IconCrossedCircle.png", "Crossed Circle"));
	nodeIcons.push_back(new Icon(63 * 3, 0, "Assets/IconFilledSquare.png", "Square Node"));
	nodeIcons.push_back(new Icon(63 * 4, 0, "Assets/IconOpenSquare.png", "Open Square"));
	nodeIcons.push_back(new Icon(63 * 5, 0, "Assets/IconCrossedSquare.png", "Crossed Square"));

	//edge type icons
	edgeIcons.push_back(new Icon(0, 63, "Assets/IconEdge.png", "Line"));
	edgeIcons.push_back(new Icon(63, 63, "Assets/IconArrow.png", "Arrow"));
	edgeIcons.push_back(new Icon(63 * 2, 63, "Assets/IconResistEdge.png", "Resistance Edge"));
	edgeIcons.push_back(new Icon(63 * 3, 63, "Assets/IconDashedLine.png", "Dashed Line"));
	edgeIcons.push_back(new Icon(63 * 4, 63, "Assets/IconDottedLine.png", "Dotted Line"));
	edgeIcons.push_back(new Icon(63 * 5, 63, "Assets/IconSwitch.png", "Circuit Breaker"));
	edgeIcons.push_back(new Icon(63 * 6, 63, "Assets/IconVertL.png", "Vertical Angle-Edge"));
	edgeIcons.push_back(new Icon(63 * 7, 63, "Assets/IconHorizL.png", "Horizontal Angle-Edge"));


	selectedTextTool = 0;
	textIcons.push_back(new Icon(0, 63 * 5, "Assets/IconText.png", "Textbox"));
	textIcons.push_back(new Icon(63, 63 * 5, "Assets/IconNodeText.png", "In-Node Label"));
	textIcons.push_back(new Icon(63 * 2, 63 * 5, "Assets/IconEraseText.png", "Text Eraser"));
	textIcons.push_back(new Icon(63 * 3, 63 * 5, "Assets/IconMoveText.png", "Text Movement"));


	gateIcons.push_back(new Icon(0, 63 * 8, "Assets/IconIdentity.png", "Identity Gate"));
	gateIcons.push_back(new Icon(63, 63 * 8, "Assets/IconNot.png", "NOT Gate"));
	gateIcons.push_back(new Icon(63*2, 63 * 8, "Assets/IconAnd.png", "AND Gate"));
	gateIcons.push_back(new Icon(63*3, 63 * 8, "Assets/IconNand.png", "NAND Gate"));
	gateIcons.push_back(new Icon(63*4, 63 * 8, "Assets/IconOr.png", "OR Gate"));
	gateIcons.push_back(new Icon(63*5, 63 * 8, "Assets/IconNor.png", "NOR Gate"));
	gateIcons.push_back(new Icon(63*6, 63 * 8, "Assets/IconXor.png", "XOR Gate"));
	gateIcons.push_back(new Icon(63*7, 63 * 8, "Assets/IconXnor.png", "XNOR Gate"));
	
	selectedInd = 0;

	//sidebar icons
	icons.push_back(new Icon(0, 0, "Assets/IconFilledCircle.png", "Node Placement Tools (N)"));
	icons[0]->toggleSelected();
	icons.push_back(new Icon(0, 63, "Assets/IconEdge.png", "Edge Connection Tools (E)"));
	icons[1]->linkTo(icons[0]);
	icons.push_back(new Icon(0, 63 * 2, "Assets/IconMove.png", "Node Movement Tool (M)"));
	icons.push_back(new Icon(0, 63 * 3, "Assets/IconEraser.png", "Eraser Tool (D)"));
	icons.push_back(new Icon(0, 63 * 4, "Assets/IconCopy.png", "Copy Tool (C)"));
	icons.push_back(new Icon(0, 63 * 5, "Assets/IconText.png", "Text Tools (T)"));
	icons.push_back(new Icon(0, 63 * 6, "Assets/IconFillTool.png", "Fill Bucket Tool (F)"));
	icons.push_back(new Icon(0, 63 * 7, "Assets/IconEyedropper.png", "Eyedropper Tool (I)"));
	icons.push_back(new Icon(0, 63 * 8, "Assets/IconIdentity.png", "Logic Gate Tools (G)"));
	icons.push_back(new Icon(0, SCREEN_HEIGHT - 120 - 63 * 3, "Assets/IconNewFile.png", "New File"));
	icons.push_back(new Icon(0, SCREEN_HEIGHT - 120 - 63 * 2, "Assets/IconSave.png", "Save Current File"));
	icons.push_back(new Icon(0, SCREEN_HEIGHT - 120 - 63, "Assets/IconSaveAs.png", "Save As"));
	icons.push_back(new Icon(0, SCREEN_HEIGHT - 120, "Assets/IconOpen.png", "Open File"));
}