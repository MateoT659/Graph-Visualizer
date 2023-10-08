#include "Includes.h"

void createObject(SDL_Event*, Vec2 mousePos);
void deleteObject(Vec2 mousePos);
void parseKey(SDL_Event*);
void dragNode(SDL_Event*, Uint8 button, Vec2 mousePos);
void openNodeMenu(SDL_Event*);
void openEdgeMenu(SDL_Event*);
void updateHoverStatus(Vec2 mousePos, std::vector<Icon*> iconVec);
void parseMenuClick(Vec2 mousePos, SDL_Event * event); 
void adjustGhostSize(SDL_Event* event);

void parseEvent(SDL_Event* event) {
	Vec2 mousePos = getMousePos();
	switch (event->type) {
	case SDL_MOUSEMOTION:
		
		ghost->setPos(mousePos);

		if (sidebar->isTouched(mousePos)) {
			updateHoverStatus(mousePos, icons);
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		
		switch (event->button.button) {
		case SDL_BUTTON_LEFT: 
			
			if (sidebar->isTouched(mousePos)){
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
					createObject(event, mousePos);
					break;
				case 2:
					dragNode(event, SDL_BUTTON_LEFT, mousePos);
					break;
				case 3:
					deleteObject(mousePos);
					break;
				}
			}
			break;

		case SDL_BUTTON_RIGHT:
			deleteObject(mousePos);
			break;

		case SDL_BUTTON_MIDDLE:
			dragNode(event, SDL_BUTTON_MIDDLE, mousePos);
			break;
		}
		break;

	case SDL_KEYDOWN:
		parseKey(event);
		ghost->setColor(currentColor);
		break;
		
	case SDL_MOUSEWHEEL:
		if (selectedInd <= 1) {
			adjustGhostSize(event);
		}
	}
}

void openNodeMenu(SDL_Event* event) {
	
	SDL_Rect menuBG = { 0, 0, (int)(63 * (int)nodeIcons.size()), 63 };
	SDL_Color menuColor = { 64, 64, 64, 255 };
	Vec2 mousePos(0, 0);

	render(false);
	drawFilledRectangle(menuBG, menuColor);
	for (int i = 0; i < nodeIcons.size(); i++) {
		nodeIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));

	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if(SDL_PollEvent(event)) {
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
	SDL_Color color = { 64, 64, 64, 255 };
	Vec2 mousePos(0, 0);

	render(false);
	drawFilledRectangle(menuBG, color);
	for (int i = 0; i < edgeIcons.size(); i++) {
		edgeIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));

	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if (SDL_PollEvent(event)) {
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

	if (n1 != -1) {
		renderU(false);
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)){
			SDL_PollEvent(event);
		}

		mousePos = getMousePos();
		ghost->setPos(mousePos);

		for (int i = 0; i < nodes.size() && n2 == -1; i++) {
			if (nodes[i]->containsPoint(mousePos)) {
				n2 = i;
			}
		}

		if (n2 != -1 && n2 != n1) {
			edges.push_back(new GraphEdge(nodes[n1], nodes[n2], currentColor, edgeType));
		}
	}
	else {
		nodes.push_back(ghost->copy());
	}
}
void deleteObject(Vec2 mousePos) {
	// deletes any nodes or edges touched

	for (int i = 0; i < nodes.size();) {
		if (nodes[i]->containsPoint(mousePos)) {
			for (int j = 0; j < edges.size();) {
				if (edges[j]->containsNode(nodes[i])) {
					GraphEdge *toDelete = edges[j];
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

	for (int i = 0; i < edges.size();) {
		if (edges[i]->isTouched(mousePos)) {
			GraphEdge* toDelete = edges[i];
			edges.erase(edges.begin() + i);
			delete toDelete;
		}
		else {
			i++;
		}
	}
}
void dragNode(SDL_Event* event, Uint8 button, Vec2 mousePos) {
	
	GraphNode* toMove = nullptr;

	//finds node to be moved
	for (int i = 0; i < nodes.size() && toMove == nullptr; i++) {
		if (nodes[i]->containsPoint(mousePos)) {
			toMove = nodes[i];
		}
	}

	if (toMove == nullptr) return;

	std::vector<GraphEdge*> toUpdate;

	// finds connected edges
	for (int i = 0; i < edges.size(); i++) {
		if (edges[i]->containsNode(toMove)) {
			toUpdate.push_back(edges[i]);
		}
	}

	// waits for click to release
	while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)){
		if (SDL_PollEvent(event)) {
			SDL_GetMouseState(toMove->getXaddr(), toMove->getYaddr());
			for (int i = 0; i < toUpdate.size(); i++) {
				toUpdate[i]->update();
			}
			renderU(false);
		}
	}
	SDL_GetMouseState(ghost->getXaddr(), ghost->getYaddr());

	// updates any connected nodes
	for (int i = 0; i < toUpdate.size(); i++) {
		toUpdate[i]->update();
	}
}

void parseKey(SDL_Event* event) {

	switch (event->key.keysym.sym) {
	case SDLK_ESCAPE:
		running = false;
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
		currentColor = PURPLE;
		break;
	case SDLK_0:
		currentColor = AMETHYST;
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
		if (selectedInd <= 1) {
			ghost->setType((NodeType)(((int)ghost->getType() + 1) % nodeTypeTotal));
			updateIcons();
		}
		else {
			icons[selectedInd]->toggleSelected();
			selectedInd = 1;
			icons[1]->toggleSelected();
		}
		break;
	case SDLK_s:
		saveAsFile();
		break;
	case SDLK_o:
		openFile();
		break;
	}
}

Vec2 getMousePos() {
	Vec2 position;
	SDL_GetMouseState(&position.x, &position.y);
	return position;
}

bool rectIsTouched(SDL_Rect rect, int x, int y) {
	return (x >= rect.x) && (x <= rect.x + rect.w) && (y >= rect.y) && (y <= rect.y + rect.h);
}
bool rectIsTouched(SDL_Rect rect, Vec2 pos) {
	return rectIsTouched(rect, pos.x, pos.y);
}

void updateHoverStatus(Vec2 mousePos, std::vector<Icon*> iconVec) {
	for (int i = 0; i < iconVec.size(); i++) {
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

	if (clickedInd < icons.size() - 3 && clickedInd != selectedInd) {
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
	default:
		if (clickedInd == icons.size() - 3)
			saveFile();
		else if (clickedInd == icons.size() - 2) {
			saveAsFile();
		}
		else if (clickedInd == icons.size() - 1) {
			openFile();
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