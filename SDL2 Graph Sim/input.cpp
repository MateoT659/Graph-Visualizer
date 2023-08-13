#include "GraphSimConst.h"
#include "GraphNode.h"
#include "GraphEdge.h"

void createObject(SDL_Event*);
void deleteObject();
void parseKey(SDL_Event*);
void dragNode(SDL_Event*);

void parseEvent(SDL_Event* event) {
	switch (event->type) {

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
		break;
	}
}

void createObject(SDL_Event* event) {
	// if initial click is not touching a node, creates another node
	// else, waits for the left button to release. if the release is also on a node,
	// creates an edge between those two nodes 
	
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int n1 = -1, n2 = -1;

	for (int i = 0; i < nodes.size() && n1 == -1; i++) {
		if (nodes[i]->cotainsPoint(mouseX, mouseY)) {
			n1 = i;
		}
	}

	if (n1 != -1) {
		
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)){
			SDL_PollEvent(event);
		}
		SDL_GetMouseState(&mouseX, &mouseY);

		for (int i = 0; i < nodes.size() && n2 == -1; i++) {
			if (nodes[i]->cotainsPoint(mouseX, mouseY)) {
				n2 = i;
			}
		}

		if (n2 != -1 && n2 != n1) {
			edges.push_back(new GraphEdge(nodes[n1], nodes[n2], currentColor, diGraph));
		}
	}
	else {
		nodes.push_back(new GraphNode(mouseX, mouseY, 15, currentColor));
	}
}

void deleteObject() {
	// deletes any nodes or edges touched
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	for (int i = 0; i < nodes.size();) {
		if (nodes[i]->cotainsPoint(mouseX, mouseY)) {
			for (int j = 0; j < edges.size();) {
				if (edges[j]->hasNode(nodes[i])) {
					edges.erase((edges.begin() + j));
				}
				else {
					j++;
				}
			}
			nodes.erase(nodes.begin() + i);
		}
		else {
			i++;
		}
	}

	for (int i = 0; i < edges.size();) {
		if (edges[i]->isTouched(mouseX, mouseY)) {
			edges.erase(edges.begin() + i);
		}
		else {
			i++;
		}
	}
}

void parseKey(SDL_Event* event) {
	// exits program on spacebar, or sets color on nums 1-5

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
		diGraph = !diGraph;
		break;
	}
}

void dragNode(SDL_Event* event) {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	
	GraphNode* toMove = nullptr;
	std::vector<GraphEdge*> toUpdate;

	//finds node to be moved
	for (int i = 0; i < nodes.size() && toMove == nullptr; i++) {
		if (nodes[i]->cotainsPoint(mouseX, mouseY)) {
			toMove = nodes[i];
		}
	}

	if (toMove != nullptr) {
		// finds connected edges
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i]->hasNode(toMove)) {
				toUpdate.push_back(edges[i]);
			}
		}

		// waits for middleclick to release
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_MIDDLE)){
			if (SDL_PollEvent(event)) {
				SDL_GetMouseState(toMove->getXaddr(), toMove->getYaddr());
				
				render();
			}
		}

		// updates any connected nodes
		for (int i = 0; i < toUpdate.size(); i++) {
			toUpdate[i]->update();
		}
	}
}