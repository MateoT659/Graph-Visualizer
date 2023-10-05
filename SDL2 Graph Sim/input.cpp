#include "Includes.h"

void createObject(SDL_Event*);
void deleteObject();
void parseKey(SDL_Event*);
void dragNode(SDL_Event*);
void openNodeMenu(SDL_Event*);
void openEdgeMenu(SDL_Event*);

void parseEvent(SDL_Event* event) {
	Vec2 mousePos(0, 0);
	switch (event->type) {
	case SDL_MOUSEMOTION:
		mousePos = getMousePos();
		ghost->setPos(mousePos);

		if (sidebar->isTouched(mousePos)) {
			for (int i = 0; i < icons.size(); i++) {
				if (!icons[i]->containsPoint(mousePos) && icons[i]->isHovered()) {
					icons[i]->setHover(false);
				}
				else if (icons[i]->containsPoint(mousePos) && !icons[i]->isHovered()) {
					icons[i]->setHover(true);
				}
			}
		}

		break;

	case SDL_MOUSEBUTTONDOWN:
		switch (event->button.button) {

		case SDL_BUTTON_LEFT: 
			
			mousePos = getMousePos();
			
			if (sidebar->isTouched(mousePos)){
				for (int i = 0; i < icons.size()-3; i++) {
					if (icons[i]->containsPoint(mousePos)) {
						icons[selectedInd]->toggleSelected();
						icons[i]->toggleSelected();
						selectedInd = i;
					}
				}
				if (icons[icons.size() - 3]->containsPoint(mousePos)) {
					saveFile();
				}
				else if (icons[icons.size() - 2]->containsPoint(mousePos)) {
					saveFile(); //save as eventually
				}
				else if (icons[icons.size() - 1]->containsPoint(mousePos)) {
					openFile();
				}
				else if (icons[0]->containsPoint(mousePos)) {
					openNodeMenu(event);
				}
				else if (icons[1]->containsPoint(mousePos)) {
					openEdgeMenu(event);
				}
			}
			else {
				createObject(event);
			}
			break;

		case SDL_BUTTON_RIGHT:
			deleteObject();
			break;

		case SDL_BUTTON_MIDDLE:
			dragNode(event);
			break;
		}
		break;

	case SDL_KEYDOWN:
		parseKey(event);
		ghost->setColor(currentColor);
		break;
		
	case SDL_MOUSEWHEEL:
		if (event->wheel.y > 0) {
			ghost->setRadius((int)(ghost->getRadius() *6 / 5.));
		}
		else if (event->wheel.y < 0) {
			ghost->setRadius((int)(ghost->getRadius() * 5 / 6.));
		}
	}
}

void openNodeMenu(SDL_Event* event) {
	SDL_Rect menuBG = { 0, 0, (int)(63 * (int)nodeIcons.size()), 63 };
	SDL_Color color = { 64, 64, 64, 255 };
	Vec2 mousePos(0, 0);

	while (!SDL_PollEvent(event));

	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if(SDL_PollEvent(event)) {
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)) {
				for (int i = 0; i < nodeIcons.size(); i++) {
					if (!nodeIcons[i]->containsPoint(mousePos) && nodeIcons[i]->isHovered()) {
						nodeIcons[i]->setHover(false);
					}
					else if (nodeIcons[i]->containsPoint(mousePos) && !nodeIcons[i]->isHovered()) {
						nodeIcons[i]->setHover(true);
					}
				}

			}

			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < nodeIcons.size(); i++) {
				nodeIcons[i]->render();
			}
			SDL_RenderPresent(renderer);

		}
	}
	mousePos = getMousePos();
	int i;
	for (i = 0; i < nodeIcons.size() && !nodeIcons[i]->containsPoint(mousePos); i++);
	//int i has the index
	if (i < nodeIcons.size()) {
		ghost->setType((NodeType)i);
		updateIcons();
	}

}

void openEdgeMenu(SDL_Event* event) {
	SDL_Rect menuBG = { 0, 63, (int)(63 * (int)edgeIcons.size()), 63 };
	SDL_Color color = { 64, 64, 64, 255 };
	Vec2 mousePos(0, 0);

	while (!SDL_PollEvent(event));

	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if (SDL_PollEvent(event)) {
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)){
				for (int i = 0; i < edgeIcons.size(); i++) {
					if (!edgeIcons[i]->containsPoint(mousePos) && edgeIcons[i]->isHovered()) {
						edgeIcons[i]->setHover(false);
					}
					else if (edgeIcons[i]->containsPoint(mousePos) && !edgeIcons[i]->isHovered()) {
						edgeIcons[i]->setHover(true);
					}
				}

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
	//int i has the index
	if (i < edgeIcons.size()) {
		edgeType = (EdgeType)i;
		updateIcons();
	}

}

void createObject(SDL_Event* event) {
	// if initial click is not touching a node, creates another node
	// else, waits for the left button to release. if the release is also on a node,
	// creates an edge between those two nodes 
	
	Vec2 mousePos = getMousePos();
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

void deleteObject() {
	// deletes any nodes or edges touched
	Vec2 mousePos = getMousePos();

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
		saveFile();
		break;
	case SDLK_o:
		openFile();
		break;
	}
}

void dragNode(SDL_Event* event) {

	Vec2 mousePos = getMousePos();
	
	GraphNode* toMove = nullptr;
	std::vector<GraphEdge*> toUpdate;

	//finds node to be moved
	for (int i = 0; i < nodes.size() && toMove == nullptr; i++) {
		if (nodes[i]->containsPoint(mousePos)) {
			toMove = nodes[i];
		}
	}

	if (toMove != nullptr) {
		// finds connected edges
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i]->containsNode(toMove)) {
				toUpdate.push_back(edges[i]);
			}
		}

		// waits for middleclick to release
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_MIDDLE)){
			if (SDL_PollEvent(event)) {
				SDL_GetMouseState(toMove->getXaddr(), toMove->getYaddr());
				renderU(false);
			}
		}

		SDL_GetMouseState(ghost->getXaddr(), ghost->getYaddr());
		// updates any connected nodes
		for (int i = 0; i < toUpdate.size(); i++) {
			toUpdate[i]->update();
		}
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