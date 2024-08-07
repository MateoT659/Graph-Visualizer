#include "Includes.h"

void createObject(SDL_Event*, Vec2 mousePos);
void deleteObject(Vec2 mousePos, SDL_Event* event, Uint8 button);
void parseKey(SDL_Event*);
bool dragNode(SDL_Event*, Uint8 button, Vec2 mousePos, bool pan);
void copyObject(SDL_Event*, Uint8 button, Vec2 mousePos);
void openNodeMenu(SDL_Event*);
void openEdgeMenu(SDL_Event*);
void updateHoverStatus(Vec2 mousePos, std::vector<Icon*> iconVec);
void parseMenuClick(Vec2 mousePos, SDL_Event * event); 
void adjustGhostSize(SDL_Event* event);
bool checkSwitchEdges(Vec2 mousePos);
void createText(SDL_Event*, Vec2 mousePos);
void createNodeText(SDL_Event*, Vec2 mousePos);
void deleteText(SDL_Event* event, Vec2 mousePos, Uint8 button);
bool moveText(SDL_Event* event, Vec2 mousePos, Uint8 button);
void getColorEyedropper(Vec2 mousePos);
void setColorFill(Vec2 mousePos);
void newFile();
void placeGate(SDL_Event*, Vec2 mousePos);


void parseEvent(SDL_Event* event) {
	Vec2 mousePos = getMousePos();
	switch (event->type) {
	case SDL_QUIT:
		running = false;
		saveFile();
		break;
	case SDL_MOUSEMOTION:

		ghost->setPos(mousePos);
		ghostGate->setPos(mousePos);

		if (sidebar->isTouched(mousePos)) {
			updateHoverStatus(mousePos, icons);
			if (rectIsTouched(colorBox, mousePos)) {
				currentToolTip = "Color Picker";
			}
		}


		break;

	case SDL_MOUSEBUTTONDOWN:
		switch (event->button.button) {
		case SDL_BUTTON_LEFT:

			if (sidebar->isTouched(mousePos)) {
				if (rectIsTouched(colorBox, mousePos)) {
					openColorPicker();
				}
				else {
					parseMenuClick(mousePos, event);
				}
			}
			else {
				switch (selectedInd) {
				case 0:
				case 1:
					if (!checkSwitchEdges(mousePos)) {
						SetCursor(LoadCursor(NULL, IDC_CROSS));
						createObject(event, mousePos);
					}
					break;
				case 2:
					switch (selectedEditTool) {
					case 0:
						deleteObject(mousePos, event, SDL_BUTTON_LEFT);
						break;
					case 1:
						if (!checkSwitchEdges(mousePos)) {
							SetCursor(LoadCursor(NULL, IDC_SIZEALL));
							if (!moveText(event, mousePos, SDL_BUTTON_LEFT))
								dragNode(event, SDL_BUTTON_LEFT, mousePos, false);
						}
						break;
					case 2:
						copyObject(event, SDL_BUTTON_LEFT, mousePos);
						break;
					}
					break;
				case 3:
					switch (selectedColorTool) {
					case 0:
						setColorFill(mousePos);
						break;
					case 1:
						getColorEyedropper(mousePos);
						break;
					}
					break;
				case 4:
					switch (selectedTextTool) {
					case 0:
						SetCursor(LoadCursor(NULL, IDC_IBEAM));
						createText(event, mousePos);
						break;
					case 1:
						//node char (create node text)

						SetCursor(LoadCursor(NULL, IDC_IBEAM));
						createNodeText(event, mousePos);
						break;
					case 2:
						deleteText(event, mousePos, SDL_BUTTON_LEFT);
						break;
					case 3:
						if (!checkSwitchEdges(mousePos))
							SetCursor(LoadCursor(NULL, IDC_SIZEALL));
							if (!moveText(event, mousePos, SDL_BUTTON_LEFT))
								dragNode(event, SDL_BUTTON_LEFT, mousePos, false);
						break;
					}
					break;
				case 5:
					SetCursor(LoadCursor(NULL, IDC_CROSS));
					placeGate(event, mousePos);
					break;
				}
			}
			break;

		case SDL_BUTTON_RIGHT:
			switch (selectedInd) {
			case 4:
				deleteText(event, mousePos, SDL_BUTTON_RIGHT);
				break;
			default:
				deleteObject(mousePos, event, SDL_BUTTON_RIGHT);
				break;
			}
			break;
		case SDL_BUTTON_MIDDLE:
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			if (!checkSwitchEdges(mousePos))
				if (!moveText(event, mousePos, SDL_BUTTON_MIDDLE))
					dragNode(event, SDL_BUTTON_MIDDLE, mousePos, true);
			break;
		}
		break;
	case SDL_KEYDOWN:
		parseKey(event);
		ghost->setColor(currentColor);
		ghostGate->setColor(currentColor);
		break;

	case SDL_MOUSEWHEEL:
		if (selectedInd <= 1) {
			adjustGhostSize(event);
		}
		break;
	case SDL_WINDOWEVENT:
		SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
		bool resize = false;
		if (SCREEN_HEIGHT < 680) {
			SCREEN_HEIGHT = 680;
			resize = true;
		}
		if (SCREEN_WIDTH < 200) {
			SCREEN_WIDTH = 200;
			resize = true;
		}

		if (resize) SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		updateUIOnMove();
		break; 
	}
}


void createText(SDL_Event* event, Vec2 mousePos) {
	//NEXT figure out text input, use mouse wheel to change size. clicking colorbox changes color using picker. end on ENTER or click off of box. 	
	Textbox* editing = nullptr;
	std::string text;
	bool quit = false;
	int ind = -1;
	for (int i = 0; i < textboxes.size() && editing == nullptr; i++) {
		if (textboxes[i]->containsPoint(mousePos)) {
			editing = textboxes[i];
			text = editing->getText();
			ind = i;
		}
	}

	if (editing == nullptr) {
		editing = new Textbox(" ", mousePos, 30, currentColor);
	}

	if (ind == -1) {
		textboxes.push_back(editing);
		ind = (int)textboxes.size() - 1;
	}
	
	editing->setEditState(true);

	SDL_StartTextInput();

	while (!quit) {
		while (SDL_PollEvent(event)) {
			Vec2 mousePos = getMousePos();
			switch (event->type) {
			case SDL_KEYDOWN:
				if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
				}
				else if (event->key.keysym.sym == SDLK_BACKSPACE && text.length() > 0){
					text.pop_back();
					editing->setText(text);
				}
				break;
			case SDL_TEXTINPUT:
				text += event->text.text;
				editing->setText(text);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event->button.button == SDL_BUTTON_LEFT && (editing->containsPoint(mousePos) || rectIsTouched(colorBox, mousePos))) {
					openColorPicker();
					editing->setColor(currentColor);
				}
				else {
					quit = true;
				}
				break;
			case SDL_MOUSEWHEEL:
				if (event->wheel.y > 0) {
					editing->setHeight(editing->getHeight()+2);
				}
				else if (event->wheel.y < 0) {
					editing->setHeight(editing->getHeight() - 2);
				}
			}
			if (text == " ") text = "";
			renderU(false);
		}
	}

	SDL_StopTextInput();
	
	editing->setEditState(false);
	if (text == "") {
		delete editing;
		textboxes.erase(textboxes.begin() + ind);
	}
}

void createNodeText(SDL_Event* event, Vec2 mousePos)
{
	//NEXT figure out text input, use mouse wheel to change size. clicking colorbox changes color using picker. end on ENTER or click off of box. 	
	NodeText* editing = nullptr;
	std::string text;
	bool quit = false;
	int ind = -1;
	for (int i = 0; i < nodetexts.size() && editing == nullptr; i++) {
		if (nodetexts[i]->isTouched(mousePos)) {
			editing = nodetexts[i];
			text = editing->getText();
			ind = i;
		}
	}

	if (editing == nullptr) {
		for (int i = 0; i < nodes.size() && editing == nullptr; i++) {
			if (nodes[i]->containsPoint(mousePos)) {
				editing = new NodeText(nodes[i], " ");
			}
		}
	}
	if (editing == nullptr) return;

	if (ind == -1) {
		nodetexts.push_back(editing);
		ind = (int)nodetexts.size() - 1;
	}

	editing->setState(true);

	SDL_StartTextInput();

	while (!quit) {
		while (SDL_PollEvent(event)) {
			Vec2 mousePos = getMousePos();
			switch (event->type) {
			case SDL_KEYDOWN:
				if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
				}
				else if (event->key.keysym.sym == SDLK_BACKSPACE && text.length() > 0) {
					text.pop_back();
					editing->setText(text);
				}
				break;
			case SDL_TEXTINPUT:
				text += event->text.text;
				editing->setText(text);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event->button.button == SDL_BUTTON_LEFT && (editing->isTouched(mousePos))) {}
				else {
					quit = true;
				}
				break;
			}
			if (text == " ") text = "";
			else if (text.size() > 2) text.resize(2);
			renderU(false);
		}
	}

	SDL_StopTextInput();

	editing->setState(false);
	if (text == "") {
		delete editing;
		nodetexts.erase(nodetexts.begin() + ind);
	}
}

void deleteText(SDL_Event* event, Vec2 mousePos, Uint8 button) {
	while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
		if (SDL_PollEvent(event)) {
			mousePos = getMousePos();
			
			for (int i = 0; i < textboxes.size();) {
				if (textboxes[i]->containsPoint(mousePos)) {
					delete textboxes[i];
					textboxes.erase(textboxes.begin() + i);
				}
				else {
					i++;
				}
			}

			for (int i = 0; i < nodetexts.size();) {
				if (nodetexts[i]->isTouched(mousePos)) {
					delete nodetexts[i];
					nodetexts.erase(nodetexts.begin() + i);
				}
				else {
					i++;
				}
			}
			renderU(false);
		}
	}
}

bool moveText(SDL_Event* event, Vec2 mousePos, Uint8 button) {
	Textbox* toMove = nullptr;
	for (int i = 0; i < textboxes.size() && toMove == nullptr; i++) {
		if (textboxes[i]->containsPoint(mousePos)) {
			toMove = textboxes[i];
		}
	}
	if (toMove == nullptr) return false;
	Vec2 mousePos2;
	while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
		if (SDL_PollEvent(event)) {
			mousePos2 = getMousePos();
			toMove->translateBy(mousePos2 - mousePos);
			mousePos = getMousePos();
			renderU(false);
		}
	}
	return true;
}

void getColorEyedropper(Vec2 mousePos)
{
	SDL_Color color = currentColor;
	bool colorFound = false;
	for (int i = 0; !colorFound && i < nodes.size(); i++) {
		if (nodes[i]->containsPoint(mousePos)){
			color = nodes[i]->getColor();
			colorFound = true;
		}
	}
	
	for (int i = 0; !colorFound && i < edges.size(); i++) {
		if (edges[i]->isTouched(mousePos)){
			color = edges[i]->getColor();
			colorFound = true;
		}
	}

	for (int i = 0; !colorFound && i < freeEdges.size(); i++) {
		if (freeEdges[i]->isTouched(mousePos)){
			color = freeEdges[i]->getColor();
			colorFound = true;
		}
	}

	for (int i = 0; !colorFound && i < textboxes.size(); i++) {
		if (textboxes[i]->containsPoint(mousePos)){
			color = textboxes[i]->getColor();
			colorFound = true;
		}
	}

	for (int i = 0; !colorFound && i < gates.size(); i++) {
		if (gates[i]->isTouched(mousePos)) {
			color = gates[i]->getColor();
			colorFound = true;
		}
	}

	if (!colorFound) {
		//CHANGE TO BACKGROUND COLOR
		color = bgColor;
	}

	currentColor = color;
	ghost->setColor(currentColor);
	ghostGate->setColor(currentColor);
	adjustCustomColors();
}

void setColorFill(Vec2 mousePos)
{
	bool colorFound = false;
	for (int i = 0; !colorFound && i < nodes.size(); i++) {
		if (nodes[i]->containsPoint(mousePos)) {
			nodes[i]->setColor(currentColor);
			for (int j = 0; j < nodetexts.size(); j++) {
				if (nodetexts[j]->getNode() == nodes[i]){
					nodetexts[j]->updateColor();
				}
			}
			colorFound = true;
		}
	}

	for (int i = 0; !colorFound && i < edges.size(); i++) {
		if (edges[i]->isTouched(mousePos)) {
			edges[i]->setColor(currentColor);
			colorFound = true;
		}
	}

	for (int i = 0; !colorFound && i < freeEdges.size(); i++) {
		if (freeEdges[i]->isTouched(mousePos)) {
			freeEdges[i]->setColor(currentColor);
			colorFound = true;
		}
	}

	for (int i = 0; !colorFound && i < textboxes.size(); i++) {
		if (textboxes[i]->containsPoint(mousePos)) {
			textboxes[i]->setColor(currentColor);
			colorFound = true;
		}
	}

	for (int i = 0; !colorFound && i < gates.size(); i++) {
		if (gates[i]->isTouched(mousePos)) {
			gates[i]->setColor(currentColor);
			colorFound = true;
		}
	}

	if (!colorFound) {
		//CHANGE TO BACKGROUND COLOR
		bgColor = currentColor;
	}
}

void newFile()
{
	saveFile();

	//delete all things
	for (GraphNode* node : nodes) {
		delete node;
	}
	nodes.clear();
	for (GraphEdge* edge : edges) {
		delete edge;
	}
	edges.clear();
	for (FreeEdge* edge : freeEdges) {
		delete edge;
	}
	freeEdges.clear();
	for (Textbox* text : textboxes) {
		delete text;
	}
	for (NodeText* nt : nodetexts) {
		delete nt;
	}
	for (Gate* gate : gates) {
		delete gate;
	}
	nodetexts.clear();
	textboxes.clear();
	switches.clear();
	fswitches.clear();
	gates.clear();
	bgColor = BLACK;
	currentColor = WHITE;
	ghost->setColor(currentColor);
	currentFilepath = "None";
}

void placeGate(SDL_Event* event, Vec2 mousePos)
{
	int n1 = -1, n2 = -1;

	for (int i = 0; i < nodes.size() && n1 == -1; i++) {
		if (nodes[i]->containsPoint(mousePos)) {
			n1 = i;
		}
	}

	for (int i = 0; i < gates.size(); i++) {
		int j = gates[i]->isMemberTouched(mousePos);
		if (j>= 0) {
			n1 = (int)nodes.size() + i * 3 + j;
		}
	}

	if (n1 != -1) {
		renderU(false);
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)) {
			SDL_PollEvent(event);
		}

		mousePos = getMousePos();
		ghost->setPos(mousePos);

		for (int i = 0; i < nodes.size() && n2 == -1; i++) {
			if (nodes[i]->containsPoint(mousePos)) {
				n2 = i;
			}
		}

		for (int i = 0; i < gates.size(); i++) {
			int j = gates[i]->isMemberTouched(mousePos);
			if (j >= 0) {
				n2 = (int)nodes.size() + i * 3 + j;
			}
		}

		if (n2 != -1 && n2 != n1) {
			if (n1 < nodes.size() && n2 < nodes.size()) {
				edges.push_back(new GraphEdge(nodes[n1], nodes[n2], currentColor, edgeType));
			}
			else if (n1 < nodes.size() && n2 >= nodes.size()) {
				edges.push_back(new GraphEdge(nodes[n1], gates[(n2 - (int)nodes.size())/3]->getNode((n2-(int)nodes.size())%3), currentColor, edgeType));
			}
			else if (n1 >= nodes.size() && n2 < nodes.size()) {
				edges.push_back(new GraphEdge(gates[(n1 - (int)nodes.size()) / 3]->getNode((n1 - (int)nodes.size()) % 3), nodes[n2], currentColor, edgeType));
			}
			else if (n1 >= nodes.size() && n2 >= nodes.size()) {
				edges.push_back(new GraphEdge(gates[(n1 - (int)nodes.size()) / 3]->getNode((n1 - (int)nodes.size()) % 3), gates[(n2 - (int)nodes.size()) / 3]->getNode((n2 - (int)nodes.size()) % 3), currentColor, edgeType));
			}
			if (edgeType == Switch) {
					switches.insert(edges[edges.size() - 1]);
				}
		}
	}
	else {
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)) {
			SDL_PollEvent(event);
		}
		Vec2 mousepos2 = getMousePos();

		if (ghostGate->isTouched(mousepos2)) {
			gates.push_back(ghostGate->copy());
		}
		else {
			freeEdges.push_back(new FreeEdge(mousePos, mousepos2, currentColor, edgeType));
			if (edgeType == Switch) {
				fswitches.insert(freeEdges[freeEdges.size() - 1]);
			}
		}
		ghostGate->setPos(mousepos2);
	}
}

bool checkSwitchEdges(Vec2 mousePos) {
	bool ret = false;
	for (GraphEdge *edge : switches) {
		if (edge->isSwitchTouched(mousePos)) {
			edge->toggleSwitch();
			ret = true;
		}
	}
	for (FreeEdge* edge : fswitches) {
		if (edge->isSwitchTouched(mousePos)) {
			edge->toggleSwitch();
			ret = true;
		}
	}
	return ret;
}

void openNodeMenu(SDL_Event* event) {
	
	SDL_Rect menuBG = { 0, 0, (int)(63 * (int)nodeTypeTotal), 63 };
	SDL_Color menuColor = { 80, 80, 80, 255 };
	Vec2 mousePos = getMousePos();

	render(false);
	drawFilledRectangle(menuBG, menuColor);
	for (int i = 0; i < nodeIcons.size(); i++) {
		nodeIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));
	bool toolTipRendered = false;
	long lastRenderMilli = 0;
	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if(SDL_PollEvent(event)) {
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)) {
				updateHoverStatus(mousePos, nodeIcons);
			}

			//rendering plus the extra menu
			render(false);
			drawFilledRectangle(menuBG, menuColor);
			for (int i = 0; i < nodeIcons.size(); i++) {
				nodeIcons[i]->render();
			}
			SDL_RenderPresent(renderer);
			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli > 500) {
			render(false);
			drawFilledRectangle(menuBG, menuColor);
			for (int i = 0; i < nodeIcons.size(); i++) {
				nodeIcons[i]->render();
			}
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	mousePos = getMousePos();
	int i;
	for (i = 0; i < nodeIcons.size() && !nodeIcons[i]->containsPoint(mousePos); i++);
	if (i < nodeIcons.size()) {
		ghost->setType((NodeType)i);
		updateIcons();
	}

}
void openEdgeMenu(SDL_Event* event) {
	SDL_Rect menuBG = { 0, 63, (int)(63 * (int)edgeIcons.size()), 63 };
	SDL_Color color = { 80, 80, 80, 255 };
	Vec2 mousePos = getMousePos();

	render(false);
	drawFilledRectangle(menuBG, color);
	for (int i = 0; i < edgeIcons.size(); i++) {
		edgeIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));
	bool toolTipRendered = false;
	long lastRenderMilli = 0;
	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if (SDL_PollEvent(event)) {
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)){
				updateHoverStatus(mousePos, edgeIcons);
			}

			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < edgeIcons.size(); i++) {
				edgeIcons[i]->render();
			}
			SDL_RenderPresent(renderer);

			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli > 500) {
			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < edgeIcons.size(); i++) {
				edgeIcons[i]->render();
			}
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	mousePos = getMousePos();
	int i;
	for (i = 0; i < edgeIcons.size() && !edgeIcons[i]->containsPoint(mousePos); i++);
	if (i < edgeIcons.size()) {
		edgeType = (EdgeType)i;
		updateIcons();
	}

}
void openEditMenu(SDL_Event* event) {
	SDL_Rect menuBG = { 0, 63*2, (int)(63 * (int)editIcons.size()), 63 };
	SDL_Color color = { 80, 80, 80, 255 };
	Vec2 mousePos = getMousePos();

	render(false);
	drawFilledRectangle(menuBG, color);
	for (int i = 0; i < editIcons.size(); i++) {
		editIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));
	bool toolTipRendered = false;
	long lastRenderMilli = 0;
	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if (SDL_PollEvent(event)) {
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)) {
				updateHoverStatus(mousePos, editIcons);
			}

			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < editIcons.size(); i++) {
				editIcons[i]->render();
			}
			SDL_RenderPresent(renderer);

			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli > 500) {
			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < editIcons.size(); i++) {
				editIcons[i]->render();
			}
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	mousePos = getMousePos();
	int i;
	for (i = 0; i < editIcons.size() && !editIcons[i]->containsPoint(mousePos); i++);
	if (i < editIcons.size()) {
		selectedEditTool = i;
		updateIcons();
	}

}

void openColorMenu(SDL_Event* event) {
	SDL_Rect menuBG = { 0, 63 * 3, (int)(63 * (int)colorIcons.size()), 63 };
	SDL_Color color = { 80, 80, 80, 255 };
	Vec2 mousePos = getMousePos();

	render(false);
	drawFilledRectangle(menuBG, color);
	for (int i = 0; i < colorIcons.size(); i++) {
		colorIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));
	bool toolTipRendered = false;
	long lastRenderMilli = 0;
	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if (SDL_PollEvent(event)) {
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)) {
				updateHoverStatus(mousePos, colorIcons);
			}

			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < colorIcons.size(); i++) {
				colorIcons[i]->render();
			}
			SDL_RenderPresent(renderer);

			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli > 500) {
			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < colorIcons.size(); i++) {
				colorIcons[i]->render();
			}
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	mousePos = getMousePos();
	int i;
	for (i = 0; i < colorIcons.size() && !colorIcons[i]->containsPoint(mousePos); i++);
	if (i < colorIcons.size()) {
		selectedColorTool = i;
		updateIcons();
	}

}

void openTextMenu(SDL_Event* event) {
	SDL_Rect menuBG = { 0, 63*4, (int)(63 * (int)textIcons.size()), 63 };
	SDL_Color color = { 80, 80, 80, 255 };
	Vec2 mousePos = getMousePos();

	render(false);
	drawFilledRectangle(menuBG, color);
	for (int i = 0; i < textIcons.size(); i++) {
		textIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));
	bool toolTipRendered = false;
	long lastRenderMilli = 0;
	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if (SDL_PollEvent(event)) {
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)) {
				updateHoverStatus(mousePos, textIcons);
			}

			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < textIcons.size(); i++) {
				textIcons[i]->render();
			}
			SDL_RenderPresent(renderer);

			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli > 500) {
			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < textIcons.size(); i++) {
				textIcons[i]->render();
			}
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	mousePos = getMousePos();
	int i;
	for (i = 0; i < textIcons.size() && !textIcons[i]->containsPoint(mousePos); i++);
	if (i < textIcons.size()) {
		selectedTextTool = i;
		updateIcons();
	}

}
void openGateMenu(SDL_Event* event) {
	SDL_Rect menuBG = { 0, 63*5, (int)(63 * (int)gateIcons.size()), 63 };
	SDL_Color color = { 80, 80, 80, 255 };
	Vec2 mousePos = getMousePos();

	render(false);
	drawFilledRectangle(menuBG, color);
	for (int i = 0; i < gateIcons.size(); i++) {
		gateIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));
	bool toolTipRendered = false;
	long lastRenderMilli = 0;
	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if (SDL_PollEvent(event)) {
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)) {
				updateHoverStatus(mousePos, gateIcons);
			}

			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < gateIcons.size(); i++) {
				gateIcons[i]->render();
			}
			SDL_RenderPresent(renderer);
			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli > 500) {
			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < gateIcons.size(); i++) {
				gateIcons[i]->render();
			}
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}
	mousePos = getMousePos();
	int i;
	for (i = 0; i < gateIcons.size() && !gateIcons[i]->containsPoint(mousePos); i++);
	if (i < gateIcons.size()) {
		delete ghostGate;
		if (i >= 2) {
			ghostGate = new BinaryGate(mousePos, currentColor, (bGateType)i);
		}
		else {
			ghostGate = new UnaryGate(mousePos, currentColor, (uGateType)i);
		}
		updateIcons();
	}
}


void createObject(SDL_Event* event, Vec2 mousePos) {
	// if initial click is not touching a node, creates another node
	// else, waits for the left button to release. if the release is also on a node,
	// creates an edge between those two nodes 
	int n1 = -1, n2 = -1;

	for (int i = 0; i < nodes.size() && n1 == -1; i++) {
		if (nodes[i]->containsPoint(mousePos)) {
			n1 = i;
		}
	}

	for (int i = 0; i < gates.size(); i++) {
		int j = gates[i]->isMemberTouched(mousePos);
		if (j >= 0) {
			n1 = (int)nodes.size() + i * 3 + j;
		}
	}

	if (n1 != -1) {
		renderU(false);
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)) {
			SDL_PollEvent(event);
		}

		mousePos = getMousePos();
		ghost->setPos(mousePos);

		for (int i = 0; i < nodes.size() && n2 == -1; i++) {
			if (nodes[i]->containsPoint(mousePos)) {
				n2 = i;
			}
		}

		for (int i = 0; i < gates.size(); i++) {
			int j = gates[i]->isMemberTouched(mousePos);
			if (j >= 0) {
				n2 = (int)nodes.size() + i * 3 + j;
			}
		}

		if (n2 != -1 && n2 != n1) {
			if (n1 < nodes.size() && n2 < nodes.size()) {
				edges.push_back(new GraphEdge(nodes[n1], nodes[n2], currentColor, edgeType));
			}
			else if (n1 < nodes.size() && n2 >= nodes.size()) {
				edges.push_back(new GraphEdge(nodes[n1], gates[(n2 - (int)nodes.size()) / 3]->getNode((n2 - (int)nodes.size()) % 3), currentColor, edgeType));
			}
			else if (n1 >= nodes.size() && n2 < nodes.size()) {
				edges.push_back(new GraphEdge(gates[(n1 - (int)nodes.size()) / 3]->getNode((n1 - (int)nodes.size()) % 3), nodes[n2], currentColor, edgeType));
			}
			else if (n1 >= nodes.size() && n2 >= nodes.size()) {
				edges.push_back(new GraphEdge(gates[(n1 - (int)nodes.size()) / 3]->getNode((n1 - (int)nodes.size()) % 3), gates[(n2 - (int)nodes.size()) / 3]->getNode((n2 - (int)nodes.size()) % 3), currentColor, edgeType));
			}
			if (edgeType == Switch) {
				switches.insert(edges[edges.size() - 1]);
			}
		}
		else {
			GraphNode* addNode = ghost->copy();
			if (n1 < nodes.size()) {
				edges.push_back(new GraphEdge(nodes[n1], addNode, currentColor, edgeType));
			}
			else {
				edges.push_back(new GraphEdge(gates[(n1 - (int)nodes.size()) / 3]->getNode((n1 - (int)nodes.size()) % 3), addNode, currentColor, edgeType));
			}
			if (edgeType == Switch) {
				switches.insert(edges[edges.size() - 1]);
			}
			nodes.push_back(addNode);

		}
	}
	else {
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)) {
			SDL_PollEvent(event);
		}
		Vec2 mousepos2 = getMousePos();
		
		if (ghost->containsPoint(mousepos2)){ 
			nodes.push_back(ghost->copy());
		}
		else {
			freeEdges.push_back(new FreeEdge(mousePos, mousepos2, currentColor, edgeType));
			if (edgeType == Switch) {
				fswitches.insert(freeEdges[freeEdges.size() - 1]);
			}
		}
		ghost->setPos(mousepos2);
	}
}
void deleteObject(Vec2 mousePos, SDL_Event* event, Uint8 button) {
	// deletes any nodes or edges touched
	while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
		if (SDL_PollEvent(event)) {
			mousePos = getMousePos();
			for (int i = 0; i < nodes.size();) {
				if (nodes[i]->containsPoint(mousePos)) {
					for (int j = 0; j < edges.size();) {
						if (edges[j]->containsNode(nodes[i])) {
							GraphEdge* toDelete = edges[j];
							if (toDelete->getType() == Switch) {
								switches.erase(toDelete);
							}
							edges.erase((edges.begin() + j));
							delete toDelete;
						}
						else {
							j++;
						}
					}
					for (int j = 0; j < nodetexts.size();) {
						if (nodetexts[j]->getNode() == nodes[i]) {
							delete nodetexts[j];
							nodetexts.erase(nodetexts.begin() + j);
						}
						else {
							j++;
						}
					}
					GraphNode* toDelete = nodes[i];
					nodes.erase(nodes.begin() + i);
					delete toDelete;
					
				}
				else {
					i++;
				}
			}

			for (int i = 0; i < gates.size();) {
				if (gates[i]->isTouched(mousePos)) {
					for (int j = 0; j < edges.size();) {
						if (edges[j]->containsGate(gates[i])) {
							if (edges[j]->getType() == Switch) {
								switches.erase(edges[j]);
							}
							delete edges[j];
							edges.erase(edges.begin() + j);

						}
						else {
							j++;
						}
					}
					delete gates[i];
					gates.erase(gates.begin() + i);
				}
				else {
					i++;
				}
			}

			for (int i = 0; i < edges.size();) {
				if (edges[i]->isTouched(mousePos)) {
					GraphEdge* toDelete = edges[i];
					if (toDelete->getType() == Switch) {
						switches.erase(toDelete);
					}
					edges.erase(edges.begin() + i);
					delete toDelete;
				}
				else {
					i++;
				}
			}
			for (int i = 0; i < freeEdges.size();) {
				if (freeEdges[i]->isTouched(mousePos)) {
					FreeEdge* toDelete = freeEdges[i];
					if (toDelete->getType() == Switch) {
						fswitches.erase(toDelete);
					}
					freeEdges.erase(freeEdges.begin() + i);
					delete toDelete;
				}
				else {
					i++;
				}
				for (int i = 0; i < textboxes.size();) {
					if (textboxes[i]->containsPoint(mousePos)) {
						delete textboxes[i];
						textboxes.erase(textboxes.begin() + i);
					}
					else {
						i++;
					}
				}
			}
			renderU(false);
		}
	}ghost->setPos(mousePos);
}
bool dragNode(SDL_Event* event, Uint8 button, Vec2 mousePos, bool pan) {

	GraphNode* toMove = nullptr;
	Vec2 mousePos2;
	//finds node to be moved
	for (int i = 0; i < nodes.size() && toMove == nullptr; i++) {
		if (nodes[i]->containsPoint(mousePos)) {
			toMove = nodes[i];
		}
	}

	if (toMove == nullptr) {
		FreeEdge* freeMove = nullptr;
		for (int i = 0; i < freeEdges.size() && freeMove == nullptr; i++) {
			if (freeEdges[i]->isTouched(mousePos)) {
				freeMove = freeEdges[i];
			}
		}

		if (freeMove == nullptr) {
			
			Gate* gateMove = nullptr;
			for (int i = 0; i < gates.size() && gateMove == nullptr; i++) {
				if (gates[i]->isTouched(mousePos)) {
					gateMove = gates[i];
				}
			}
			if (gateMove == nullptr) {
				if (pan == true) {
					while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
						if (SDL_PollEvent(event)) {
							mousePos2 = getMousePos();
							for (int i = 0; i < nodes.size(); i++) {
								nodes[i]->translateBy(mousePos2 - mousePos);
							}
							for (int i = 0; i < gates.size(); i++) {
								gates[i]->translateBy(mousePos2 - mousePos);
							}
							for (int i = 0; i < edges.size(); i++) {
								edges[i]->update();
							}
							for (int i = 0; i < freeEdges.size(); i++) {
								freeEdges[i]->translateBy(mousePos2 - mousePos);
								freeEdges[i]->update();
							}
							for (int i = 0; i < nodetexts.size(); i++) {
								nodetexts[i]->updatePos();
							}
							for (int i = 0; i < textboxes.size(); i++) {
								textboxes[i]->translateBy(mousePos2 - mousePos);
							}
							mousePos = getMousePos();
							renderU(false);
							SDL_RenderPresent(renderer);
						}
					}
					return true;
				}

				//make box and do box copy (do this part after adding free edges to box selection)
				//make the box
				SDL_Rect selectBox = { mousePos.x, mousePos.y, 0, 0 };
				while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
					if (SDL_PollEvent(event)) {
						mousePos2 = getMousePos();
						selectBox.w = mousePos2.x - mousePos.x;
						selectBox.h = mousePos2.y - mousePos.y;
						renderObjects(false);
						drawFilledRectangle(selectBox, { 137, 207, 240 , 150 });
						renderInterface();
						SDL_RenderPresent(renderer);
					}
				}

				//next: include free edges in the box selection

				std::unordered_set<GraphNode*> touched;
				std::unordered_set<NodeText*> textTouched;
				std::unordered_set<FreeEdge*> ftouched;
				std::unordered_set<Gate*> gtouched;

				for (int i = 0; i < nodes.size(); i++) {
					if (rectIsTouched(selectBox, nodes[i]->getPos())) {
						touched.insert(nodes[i]);
						for (int j = 0; j < nodetexts.size(); j++) {
							if (nodetexts[j]->getNode() == nodes[i]) {
								textTouched.insert(nodetexts[j]);
							}
						}
					}
				}
				for (int i = 0; i < freeEdges.size(); i++) {
					if (lineIntersectsRect(selectBox, freeEdges[i]->getFrom(), freeEdges[i]->getTo())) {
						ftouched.insert(freeEdges[i]);
					}
				}
				for (int i = 0; i < gates.size(); i++) {
					if (rectIsTouched(selectBox, gates[i]->getPos())) {
						gtouched.insert(gates[i]);
					}
				}
				if (touched.size() == 0 && ftouched.size() == 0 && gtouched.size() == 0) {
					return false;
				}
				while (event->type != SDL_MOUSEBUTTONDOWN) {
					SDL_PollEvent(event);
				}
				mousePos = getMousePos();
				bool isNodeTouched = false;
				for (GraphNode* node : touched) {
					isNodeTouched = isNodeTouched || node->containsPoint(mousePos);
				}
				for (FreeEdge* edge : ftouched) {
					isNodeTouched = isNodeTouched || edge->isTouched(mousePos);
				}
				for (Gate* gate : gtouched) {
					isNodeTouched = isNodeTouched || gate->isTouched(mousePos);
				}
				if (!isNodeTouched) {
					return false;
				}

				mousePos2 = getMousePos();
				switch (event->button.button) {

				case SDL_BUTTON_LEFT:

					mousePos2 = getMousePos();
					
					while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)) {
						if (SDL_PollEvent(event)) {
							mousePos2 = getMousePos();
							for (GraphNode* node : touched) {
								node->setPos(node->getPos() + mousePos2 - mousePos);
							}
							for (NodeText* nt : textTouched) {
								nt->updatePos();
							}
							for (GraphEdge* edge : edges) {
								edge->update();
							}
							for (FreeEdge* edge : ftouched) {
								edge->translateBy(mousePos2 - mousePos);
							}
							for (Gate* gate : gtouched) {
								gate->translateBy(mousePos2 - mousePos);
							}
							renderU(false);
							mousePos = getMousePos();
						}
					}
					break;

				case SDL_BUTTON_RIGHT:
					//delete all nodes in the square
					for (int i = 0; i < nodes.size();) {
						if (touched.find(nodes[i]) != touched.end()) {
							for (int j = 0; j < edges.size();) {
								if (edges[j]->containsNode(nodes[i])) {
									GraphEdge* toDelete = edges[j];
									if (toDelete->getType() == Switch) {
										switches.erase(toDelete);
									}
									edges.erase((edges.begin() + j));
									delete toDelete;
								}
								else {
									j++;
								}
							}
							GraphNode* toDelete = nodes[i];
							nodes.erase(nodes.begin() + i);
							delete toDelete;
						}
						else {
							i++;
						}
					}
					for (int i = 0; i < gates.size();) {
						if (gtouched.find(gates[i]) != gtouched.end()) {
							for (int j = 0; j < edges.size();) {
								if (edges[j]->containsGate(gates[i])) {
									delete edges[j];
									edges.erase(edges.begin() + j);
								}
								else {
									j++;
								}
							}
							delete(gates[i]);
							gates.erase(gates.begin() + i);
						}
						else {
							i++;
						}
					}
					for (int i = 0; i < nodetexts.size();) {
						if (textTouched.find(nodetexts[i]) != textTouched.end()) {
							delete nodetexts[i];
							nodetexts.erase(nodetexts.begin() + i);
						}
						else {
							i++;
						}
					}
					for (int i = 0; i < freeEdges.size();) {
						if (ftouched.find(freeEdges[i]) != ftouched.end()) {
							FreeEdge* toDelete = freeEdges[i];
							if (toDelete->getType() == Switch) {
								fswitches.erase(toDelete);
							}
							freeEdges.erase((freeEdges.begin() + i));
							delete toDelete;
						}
						else {
							i++;
						}
					}
					break;
				case SDL_BUTTON_MIDDLE:
					button = event->button.button;
					std::vector<GraphNode*> copies;
					std::vector<Gate*> gCopies;
					std::vector<GraphEdge*> eCopies;
					std::vector<FreeEdge*> fCopies;
					std::vector<NodeText*> tCopies;
					std::vector<GraphNode*> touchedVec(touched.begin(), touched.end());
					std::vector<Gate*> gtouchedVec(gtouched.begin(), gtouched.end());
					for (int i = 0; i < touchedVec.size(); i++) {
						copies.push_back(touchedVec[i]->copy());
						for (NodeText* nt : textTouched) {
							if (nt->getNode() == touchedVec[i]) {
								nodetexts.push_back(new NodeText(copies[copies.size() - 1], nt->getText()));
								tCopies.push_back(nodetexts[nodetexts.size() - 1]);
							}
						}
					}
					for (int i = 0; i < gtouchedVec.size(); i++) {
						gCopies.push_back(gtouchedVec[i]->copy());
					}
					for (int i = 0; i < touchedVec.size(); i++) {
						for (int j = 0; j < edges.size(); j++) {
							if (edges[j]->isFrom(touchedVec[i])) {
								for (int k = 0; k < copies.size(); k++) {
									if (edges[j]->isTo(touchedVec[k])) {
										eCopies.push_back(new GraphEdge(copies[i], copies[k], edges[j]->getColor(), edges[j]->getType()));
										if (edges[j]->getType() == Switch) {
											switches.insert(eCopies[eCopies.size() - 1]);
											if (edges[j]->isSwitchedOn()) {
												eCopies[eCopies.size() - 1]->toggleSwitch();
											}
										}
									}
								}
								for (int k = 0; k < gCopies.size(); k++) {
									if (edges[j]->isTo(gtouchedVec[k])) {
										eCopies.push_back(new GraphEdge(copies[i], gCopies[k]->getNode(gtouchedVec[k]->getNodeInd(edges[j]->getNode2())), edges[j]->getColor(), edges[j]->getType()));
										if (edges[j]->getType() == Switch) {
											switches.insert(eCopies[eCopies.size() - 1]);
											if (edges[j]->isSwitchedOn()) {
												eCopies[eCopies.size() - 1]->toggleSwitch();
											}
										}
									}
								}
							}
						}
					}
					for (int i = 0; i < gtouchedVec.size(); i++) {
						for (int j = 0; j < edges.size(); j++) {
							if (edges[j]->isFrom(gtouchedVec[i])) {
								for (int k = 0; k < copies.size(); k++) {
									if (edges[j]->isTo(touchedVec[k])) {
										eCopies.push_back(new GraphEdge(gCopies[i]->getNode(gtouchedVec[i]->getNodeInd(edges[j]->getNode1())), copies[k], edges[j]->getColor(), edges[j]->getType()));
										if (edges[j]->getType() == Switch) {
											switches.insert(eCopies[eCopies.size() - 1]);
											if (edges[j]->isSwitchedOn()) {
												eCopies[eCopies.size() - 1]->toggleSwitch();
											}
										}
									}
								}
								for (int k = 0; k < gCopies.size(); k++) {
									if (edges[j]->isTo(gtouchedVec[k])) {
										eCopies.push_back(new GraphEdge(gCopies[i]->getNode(gtouchedVec[i]->getNodeInd(edges[j]->getNode1())), gCopies[k]->getNode(gtouchedVec[k]->getNodeInd(edges[j]->getNode2())), edges[j]->getColor(), edges[j]->getType()));
										if (edges[j]->getType() == Switch) {
											switches.insert(eCopies[eCopies.size() - 1]);
											if (edges[j]->isSwitchedOn()) {
												eCopies[eCopies.size() - 1]->toggleSwitch();
											}
										}
									}
								}
							}
						}
					}
					for (FreeEdge* edge : ftouched) {
						fCopies.push_back(edge->copy());
						if (edge->getType() == Switch) {
							fswitches.insert(fCopies[fCopies.size() - 1]);
						}
					}
					for (GraphNode* node : copies) nodes.push_back(node);
					for (GraphEdge* edge : eCopies) edges.push_back(edge);
					for (FreeEdge* edge : fCopies) freeEdges.push_back(edge);
					for (Gate* gate : gCopies) gates.push_back(gate);
					while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_MIDDLE)) {
						if (SDL_PollEvent(event)) {
							mousePos2 = getMousePos();
							for (GraphNode* node : copies) {
								node->setPos(node->getPos() + mousePos2 - mousePos);
							}
							for (NodeText* nt : tCopies) {
								nt->updatePos();
							}
							for (GraphEdge* edge : eCopies) {
								edge->update();
							}
							for (FreeEdge* edge : fCopies) {
								edge->translateBy(mousePos2 - mousePos);
							}
							for (Gate* gate : gCopies) {
								gate->translateBy(mousePos2 - mousePos);
							}
							renderU(false);
							mousePos = getMousePos();
						}
					}
					break;
				}
			}
			else {
				mousePos2 = getMousePos();

				std::vector<GraphEdge*> touched;
				for (int i = 0; i < edges.size(); i++) {
					if (edges[i]->containsGate(gateMove)) {
						touched.push_back(edges[i]);
					}
				}

				while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
					if (SDL_PollEvent(event)) {
						mousePos = getMousePos();
						gateMove->translateBy(mousePos - mousePos2);

						for (int i = 0; i < touched.size(); i++) {
							touched[i]->update();
						}

						renderU(false);
						mousePos2 = getMousePos();

					}
				}
			}
		}
		else {
			Vec2 mousePos2 = getMousePos();
			while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
				if (SDL_PollEvent(event)) {
					mousePos = getMousePos();
					freeMove->translateBy(mousePos - mousePos2);

					renderU(false);
					mousePos2 = getMousePos();

				}
			}
			SDL_GetMouseState(ghost->getXaddr(), ghost->getYaddr());
		}
		//if you press right click (ON A NODE), delete all nodes in the box and connected edges.
		//if you press left click (ON A NODE), move nodes and connected edges
		//if you press middle click (ON A NODE), copy nodes and connected edges and then move their copies
		//then, find the edges connected to those nodes(or i could update all of the edges every time might be better)
		
	}
	else {
		//move the node
		std::vector<GraphEdge*> toUpdate;
		NodeText* updateT = nullptr;
		// finds connected edges
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i]->containsNode(toMove)) {
				toUpdate.push_back(edges[i]);
			}
		}

		for (int i = 0; i < nodetexts.size() && updateT == nullptr; i++) {
			if (nodetexts[i]->getNode() == toMove) {
				updateT = nodetexts[i];
			}
		}

		// waits for click to release
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
			if (SDL_PollEvent(event)) {
				SDL_GetMouseState(toMove->getXaddr(), toMove->getYaddr());
				for (int i = 0; i < toUpdate.size(); i++) {
					toUpdate[i]->update();
				}
				if (updateT != nullptr) updateT->updatePos();
				renderU(false);
			}
		}

		// updates any connected nodes
		for (int i = 0; i < toUpdate.size(); i++) {
			toUpdate[i]->update();
		}
	}
	ghost->setPos(getMousePos());
	return true;
}
void copyObject(SDL_Event* event, Uint8 button, Vec2 mousePos)
{
	GraphNode *nToCopy = nullptr;
	for (int i = 0; i < nodes.size() && nToCopy == nullptr; i++) {
		if (nodes[i]->containsPoint(mousePos)) {
			nToCopy = nodes[i];
		}
	}

	if (nToCopy != nullptr) {
		//copy the node and move it around
		GraphNode* copy = nToCopy->copy();
		NodeText* textcopy = nullptr;
		for (int i = 0; i < nodetexts.size() && textcopy == nullptr; i++) {
			if (nodetexts[i]->getNode() == nToCopy) {
				textcopy = new NodeText(copy, nodetexts[i]->getText());
				nodetexts.push_back(textcopy);
			}
		}

		nodes.push_back(copy);
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
			if (SDL_PollEvent(event)) {
				SDL_GetMouseState(copy->getXaddr(), copy->getYaddr());
				if (textcopy != nullptr) textcopy->updatePos();
				renderU(false);
			}
		}
		ghost->setPos(getMousePos());
	}
	else {
		FreeEdge* fToCopy = nullptr;
		for (int i = 0; i < freeEdges.size() && fToCopy == nullptr; i++) {
			if (freeEdges[i]->isTouched(mousePos)) {
				fToCopy = freeEdges[i];
			}
		}
		if (fToCopy != nullptr) {
			//copy the edge and move it around
			FreeEdge* copy = fToCopy->copy();
			if (copy->getType() == Switch) {
				fswitches.insert(copy);
			}
			Vec2 mousePos2 = getMousePos();
			freeEdges.push_back(copy);
			while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
				if (SDL_PollEvent(event)) {
					mousePos = getMousePos();
					copy->translateBy(mousePos - mousePos2);
					renderU(false);
					mousePos2 = getMousePos();
				}
			}
			ghost->setPos(getMousePos());
		}
		else {
			Gate* gateToCopy = nullptr;
			for (int i = 0; i < gates.size() && gateToCopy == nullptr; i++) {
				if (gates[i]->isTouched(mousePos)) {
					gateToCopy = gates[i];
				}

			}
			if (gateToCopy != nullptr) {
				Gate* copy = gateToCopy->copy();
				Vec2 mousePos2 = getMousePos();
				gates.push_back(copy);
				while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
					if (SDL_PollEvent(event)) {
						mousePos = getMousePos();
						copy->translateBy(mousePos - mousePos2);
						renderU(false);
						mousePos2 = getMousePos();
					}
				}
			}
			else {
				//make box and do box copy (do this part after adding free edges to box selection)
				//make the box
				Vec2 mousePos2;
				SDL_Rect selectBox = { mousePos.x, mousePos.y, 0, 0 };
				while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
					if (SDL_PollEvent(event)) {
						mousePos2 = getMousePos();
						selectBox.w = mousePos2.x - mousePos.x;
						selectBox.h = mousePos2.y - mousePos.y;
						renderObjects(false);
						drawFilledRectangle(selectBox, { 137, 207, 240 , 150 });
						renderInterface();
						SDL_RenderPresent(renderer);
					}
				}

				//next: include free edges in the box selection

				std::unordered_set<GraphNode*> touched;
				std::unordered_set<NodeText*> textTouched;
				std::unordered_set<FreeEdge*> ftouched;
				std::unordered_set<Gate*> gtouched;

				for (int i = 0; i < nodes.size(); i++) {
					if (rectIsTouched(selectBox, nodes[i]->getPos())) {
						touched.insert(nodes[i]);
						for (int j = 0; j < nodetexts.size(); j++) {
							if (nodetexts[j]->getNode() == nodes[i]) {
								textTouched.insert(nodetexts[j]);
							}
						}
					}
				}
				for (int i = 0; i < freeEdges.size(); i++) {
					if (lineIntersectsRect(selectBox, freeEdges[i]->getFrom(), freeEdges[i]->getTo())) {
						ftouched.insert(freeEdges[i]);
					}
				}
				for (int i = 0; i < gates.size(); i++) {
					if (rectIsTouched(selectBox, gates[i]->getPos())) {
						gtouched.insert(gates[i]);
					}
				}
				if (touched.size() == 0 && ftouched.size() == 0 && gtouched.size() == 0) {
					return;
				}
				while (event->type != SDL_MOUSEBUTTONDOWN) {
					SDL_PollEvent(event);
				}
				mousePos = getMousePos();
				bool isNodeTouched = false;
				for (GraphNode* node : touched) {
					isNodeTouched = isNodeTouched || node->containsPoint(mousePos);
				}
				for (FreeEdge* edge : ftouched) {
					isNodeTouched = isNodeTouched || edge->isTouched(mousePos);
				}
				for (Gate* gate : gtouched) {
					isNodeTouched = isNodeTouched || gate->isTouched(mousePos);
				}
				if (!isNodeTouched) {
					return;
				}

				switch (event->button.button) {

				case SDL_BUTTON_RIGHT:
					//delete all nodes in the square
					for (int i = 0; i < nodes.size();) {
						if (touched.find(nodes[i]) != touched.end()) {
							for (int j = 0; j < edges.size();) {
								if (edges[j]->containsNode(nodes[i])) {
									GraphEdge* toDelete = edges[j];
									if (toDelete->getType() == Switch) {
										switches.erase(toDelete);
									}
									edges.erase((edges.begin() + j));
									delete toDelete;
								}
								else {
									j++;
								}
							}
							GraphNode* toDelete = nodes[i];
							nodes.erase(nodes.begin() + i);
							delete toDelete;
						}
						else {
							i++;
						}
					}
					for (int i = 0; i < gates.size();) {
						if (gtouched.find(gates[i]) != gtouched.end()) {
							for (int j = 0; j < edges.size();) {
								if (edges[j]->containsGate(gates[i])) {
									delete edges[j];
									edges.erase(edges.begin() + j);
								}
								else {
									j++;
								}
							}
							delete(gates[i]);
							gates.erase(gates.begin() + i);
						}
						else {
							i++;
						}
					}
					for (int i = 0; i < nodetexts.size();) {
						if (textTouched.find(nodetexts[i]) != textTouched.end()) {
							delete nodetexts[i];
							nodetexts.erase(nodetexts.begin() + i);
						}
						else {
							i++;
						}
					}
					for (int i = 0; i < freeEdges.size();) {
						if (ftouched.find(freeEdges[i]) != ftouched.end()) {
							FreeEdge* toDelete = freeEdges[i];
							if (toDelete->getType() == Switch) {
								fswitches.erase(toDelete);
							}
							freeEdges.erase((freeEdges.begin() + i));
							delete toDelete;
						}
						else {
							i++;
						}
					}
					break;
				case SDL_BUTTON_LEFT:
				case SDL_BUTTON_MIDDLE:
					button = event->button.button;
					std::vector<GraphNode*> copies;
					std::vector<Gate*> gCopies;
					std::vector<GraphEdge*> eCopies;
					std::vector<FreeEdge*> fCopies;
					std::vector<NodeText*> tCopies;
					std::vector<GraphNode*> touchedVec(touched.begin(), touched.end());
					std::vector<Gate*> gtouchedVec(gtouched.begin(), gtouched.end());
					for (int i = 0; i < touchedVec.size(); i++) {
						copies.push_back(touchedVec[i]->copy());
						for (NodeText* nt : textTouched) {
							if (nt->getNode() == touchedVec[i]) {
								nodetexts.push_back(new NodeText(copies[copies.size() - 1], nt->getText()));
								tCopies.push_back(nodetexts[nodetexts.size() - 1]);
							}
						}
					}
					for (int i = 0; i < gtouchedVec.size(); i++) {
						gCopies.push_back(gtouchedVec[i]->copy());
					}
					for (int i = 0; i < touchedVec.size(); i++) {
						for (int j = 0; j < edges.size(); j++) {
							if (edges[j]->isFrom(touchedVec[i])) {
								for (int k = 0; k < copies.size(); k++) {
									if (edges[j]->isTo(touchedVec[k])) {
										eCopies.push_back(new GraphEdge(copies[i], copies[k], edges[j]->getColor(), edges[j]->getType()));
										if (edges[j]->getType() == Switch) {
											switches.insert(eCopies[eCopies.size() - 1]);
											if (edges[j]->isSwitchedOn()) {
												eCopies[eCopies.size() - 1]->toggleSwitch();
											}
										}
									}
								}
								for (int k = 0; k < gCopies.size(); k++) {
									if (edges[j]->isTo(gtouchedVec[k])) {
										eCopies.push_back(new GraphEdge(copies[i], gCopies[k]->getNode(gtouchedVec[k]->getNodeInd(edges[j]->getNode2())), edges[j]->getColor(), edges[j]->getType()));
										if (edges[j]->getType() == Switch) {
											switches.insert(eCopies[eCopies.size() - 1]);
											if (edges[j]->isSwitchedOn()) {
												eCopies[eCopies.size() - 1]->toggleSwitch();
											}
										}
									}
								}
							}
						}
					}
					for (int i = 0; i < gtouchedVec.size(); i++) {
						for (int j = 0; j < edges.size(); j++) {
							if (edges[j]->isFrom(gtouchedVec[i])) {
								for (int k = 0; k < copies.size(); k++) {
									if (edges[j]->isTo(touchedVec[k])) {
										eCopies.push_back(new GraphEdge(gCopies[i]->getNode(gtouchedVec[i]->getNodeInd(edges[j]->getNode1())), copies[k], edges[j]->getColor(), edges[j]->getType()));
										if (edges[j]->getType() == Switch) {
											switches.insert(eCopies[eCopies.size() - 1]);
											if (edges[j]->isSwitchedOn()) {
												eCopies[eCopies.size() - 1]->toggleSwitch();
											}
										}
									}
								}
								for (int k = 0; k < gCopies.size(); k++) {
									if (edges[j]->isTo(gtouchedVec[k])) {
										eCopies.push_back(new GraphEdge(gCopies[i]->getNode(gtouchedVec[i]->getNodeInd(edges[j]->getNode1())), gCopies[k]->getNode(gtouchedVec[k]->getNodeInd(edges[j]->getNode2())), edges[j]->getColor(), edges[j]->getType()));
										if (edges[j]->getType() == Switch) {
											switches.insert(eCopies[eCopies.size() - 1]);
											if (edges[j]->isSwitchedOn()) {
												eCopies[eCopies.size() - 1]->toggleSwitch();
											}
										}
									}
								}
							}
						}
					}
					for (FreeEdge* edge : ftouched) {
						fCopies.push_back(edge->copy());
						if (edge->getType() == Switch) {
							fswitches.insert(fCopies[fCopies.size() - 1]);
						}
					}
					for (GraphNode* node : copies) nodes.push_back(node);
					for (GraphEdge* edge : eCopies) edges.push_back(edge);
					for (FreeEdge* edge : fCopies) freeEdges.push_back(edge);
					for (Gate* gate : gCopies) gates.push_back(gate);
					while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
						if (SDL_PollEvent(event)) {
							mousePos2 = getMousePos();
							for (GraphNode* node : copies) {
								node->setPos(node->getPos() + mousePos2 - mousePos);
							}
							for (NodeText* nt : tCopies) {
								nt->updatePos();
							}
							for (GraphEdge* edge : eCopies) {
								edge->update();
							}
							for (FreeEdge* edge : fCopies) {
								edge->translateBy(mousePos2 - mousePos);
							}
							for (Gate* gate : gCopies) {
								gate->translateBy(mousePos2 - mousePos);
							}
							renderU(false);
							mousePos = getMousePos();
						}
					}
					break;
				}
			}
		}
	}
}

void parseKey(SDL_Event* event) {

	switch (event->key.keysym.sym) {
	case SDLK_ESCAPE:
		running = false;
		saveFile();
		break;
	case SDLK_1:
		currentColor = WHITE;
		break;
	case SDLK_2:
		currentColor = PINK;
		break;
	case SDLK_3:
		currentColor = RED;
		break;
	case SDLK_4:
		currentColor = ORANGE;
		break;
	case SDLK_5:
		currentColor = YELLOW;
		break;
	case SDLK_6:
		currentColor = GREEN;
		break;
	case SDLK_7:
		currentColor = CYAN;
		break;
	case SDLK_8:
		currentColor = BLUE;
		break;
	case SDLK_9:
		currentColor = AMETHYST;
		break;
	case SDLK_0:
		currentColor = BLACK;
		break;
	case SDLK_e:
		if (selectedInd <= 1) {
			edgeType = (EdgeType)(((int)edgeType + 1) % edgeTypeTotal);
			updateIcons();
		}
		else {
			icons[selectedInd]->toggleSelected();
			selectedInd = 1;
			icons[1]->toggleSelected();
		}
		break;
	case SDLK_n:
		if (SDL_GetModState() & KMOD_CTRL) {
			newFile();
		}
		else if (selectedInd <= 1) {
			ghost->setType((NodeType)(((int)ghost->getType() + 1) % nodeTypeTotal));
			updateIcons();
		}
		else {
			icons[selectedInd]->toggleSelected();
			selectedInd = 1;
			icons[1]->toggleSelected();
		}
		break;
	case SDLK_t:
		if (selectedInd != 4) {
			icons[selectedInd]->toggleSelected();
			selectedInd = 4;
			icons[4]->toggleSelected();
		}
		else {
			selectedTextTool = (selectedTextTool + 1) % textIcons.size();
			updateIcons();
		}
		break;
	case SDLK_g:
		if (selectedInd != 5) {
			icons[selectedInd]->toggleSelected();
			selectedInd = 5;
			icons[5]->toggleSelected();
		}
		else {
			int newtype = (ghostGate->getType() + 1) % gateIcons.size();
			delete ghostGate;
			if (newtype < 2) {
				ghostGate = new UnaryGate(getMousePos(), currentColor, (uGateType)newtype);
			}
			else {
				ghostGate = new BinaryGate(getMousePos(), currentColor, (bGateType)newtype);
			}
			updateIcons();
		}
		break;
	case SDLK_s:
		if (SDL_GetModState() & KMOD_CTRL) {
			saveFile();
		}
		break;
	case SDLK_o:
		if (SDL_GetModState() & KMOD_CTRL) {
			openFile();
		}
		break;
	case SDLK_c:
		icons[selectedInd]->toggleSelected();
		selectedInd = 2;

		selectedEditTool = 2;
		updateIcons();
		icons[2]->toggleSelected();
		break;
	case SDLK_m:
		icons[selectedInd]->toggleSelected();
		selectedInd = 2;
		selectedEditTool = 1;
		updateIcons();
		icons[2]->toggleSelected();
		break;
	case SDLK_d:
		icons[selectedInd]->toggleSelected();
		selectedInd = 2;
		selectedEditTool = 0;
		updateIcons();
		icons[2]->toggleSelected();
		break;
	case SDLK_f:
		icons[selectedInd]->toggleSelected();
		selectedInd = 3;
		selectedColorTool = 0;
		updateIcons();
		icons[3]->toggleSelected();
		break;
	case SDLK_i:
		icons[selectedInd]->toggleSelected();
		selectedInd = 3;
		selectedColorTool = 1;
		updateIcons();
		icons[3]->toggleSelected();
		break;
	case SDLK_RIGHT:
		SCREEN_WIDTH += 100;
		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	case SDLK_LEFT:
		SCREEN_WIDTH -= 100;
		if (SCREEN_WIDTH < 200) {
			SCREEN_WIDTH = 200;
		}
		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	case SDLK_UP:
		SCREEN_HEIGHT -= 100;
		if (SCREEN_HEIGHT < 680) {
			SCREEN_HEIGHT = 680;
		}
		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	case SDLK_DOWN:
		SCREEN_HEIGHT += 100;
		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	}
}

Vec2 getMousePos() {
	Vec2 position;
	SDL_GetMouseState(&position.x, &position.y);
	return position;
}

bool lineIntersectsRect(SDL_Rect rect, Vec2 p1, Vec2 p2) {
	if (rect.w < 0) {
		rect.x += rect.w;
		rect.w *= -1;
	}
	if (rect.h < 0) {
		rect.y += rect.h;
		rect.h *= -1;
	}
	Vec2 topLeft(rect.x, rect.y);
	Vec2 width(rect.w, 0);
	Vec2 height(0, rect.h);

	return (rectIsTouched(rect, p1) && rectIsTouched(rect, p2))
		|| (lineIntersectsLine(p1, p2, topLeft, topLeft + width))
		|| (lineIntersectsLine(p1, p2, topLeft, topLeft + height))
		|| (lineIntersectsLine(p1, p2, topLeft + height, topLeft + height + width))
		|| (lineIntersectsLine(p1, p2, topLeft + width, topLeft + height + width));
}


bool lineIntersectsLine(Vec2 v1, Vec2 v2, Vec2 v3, Vec2 v4) {
	double den = ((v4.y - v3.y) * (v2.x - v1.x) - (v4.x - v3.x) * (v2.y - v1.y));
	if (den == 0) return false;
	
	double uA = ((v4.x - v3.x) * (v1.y - v3.y) - (v4.y - v3.y) * (v1.x - v3.x)) / den;
	
	double uB = ((v2.x - v1.x) * (v1.y - v3.y) - (v2.y - v1.y) * (v1.x - v3.x)) / den;

	return uA > 0 && uA < 1 && uB > 0 && uB < 1;
}

bool rectIsTouched(SDL_Rect rect, int x, int y) {
	if (rect.w < 0) {
		rect.x += rect.w;
		rect.w *= -1;
	}
	if (rect.h < 0) {
		rect.y += rect.h;
		rect.h *= -1;
	}

	return (x >= rect.x) && (x <= rect.x + rect.w) && (y >= rect.y) && (y <= rect.y + rect.h);
}
bool rectIsTouched(SDL_Rect rect, Vec2 pos) {
	return rectIsTouched(rect, pos.x, pos.y);
}

void updateHoverStatus(Vec2 mousePos, std::vector<Icon*> iconVec) {
	for (int i = 0; i < iconVec.size(); i++) {
		if (iconVec[i]->isHovered()) currentToolTip = iconVec[i]->getMessage();
		if (!iconVec[i]->containsPoint(mousePos) && iconVec[i]->isHovered()) {
			iconVec[i]->setHover(false);
			
		}
		else if (iconVec[i]->containsPoint(mousePos) && !iconVec[i]->isHovered()) {
			iconVec[i]->setHover(true);
		}
	}

}

void parseMenuClick(Vec2 mousePos, SDL_Event *event)
{
	int clickedInd = -1;

	for (int i = 0; i < icons.size() && clickedInd < 0; i++) {
		if (icons[i]->containsPoint(mousePos)) {
			clickedInd = i;
		}
	}

	if (clickedInd < icons.size() - 4 && clickedInd != selectedInd) {
		icons[selectedInd]->toggleSelected();
		icons[clickedInd]->toggleSelected();
		selectedInd = clickedInd;
	}

	switch (clickedInd) {
	case -1:
		break;
	case 0:
		openNodeMenu(event);
		break;
	case 1:
		openEdgeMenu(event);
		break;
	case 2:
		openEditMenu(event);
		break;
	case 3:
		openColorMenu(event);
		break;
	case 4:
		openTextMenu(event);
		break;
	case 5:
		openGateMenu(event);
	default:
		if (clickedInd == icons.size() - 3)
			saveFile();
		else if (clickedInd == icons.size() - 2) {
			saveAsFile();
		}
		else if (clickedInd == icons.size() - 1) {
			openFile();
		}
		else if (clickedInd == icons.size() - 4) {
			newFile();
		}
	}
}

void adjustGhostSize(SDL_Event* event)
{
	if (event->wheel.y > 0) {
		ghost->setRadius((int)(ghost->getRadius() * 6 / 5.));
	}
	else if (event->wheel.y < 0) {
		ghost->setRadius((int)(ghost->getRadius() * 5 / 6.));
	}
}