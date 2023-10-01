#include "GraphSimConst.h"
#include "GraphNode.h"
#include "GraphEdge.h"

void createObject(SDL_Event*);
void deleteObject();
void parseKey(SDL_Event*);
void dragNode(SDL_Event*);

void parseEvent(SDL_Event* event) {
	switch (event->type) {
	case SDL_MOUSEMOTION:
		ghost->setPos(getMousePos());
		break;

	case SDL_MOUSEBUTTONDOWN:
		switch (event->button.button) {

		case SDL_BUTTON_LEFT:
			createObject(event);
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
		render(false);
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
	case SDLK_d:
		switch (edgeType) {
		case Directed:
			edgeType = None;
			break;
		default:
			edgeType = Directed;
			break;

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
				render(false);
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