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
			//add node or edge
			createObject(event);
			break;
		
		case SDL_BUTTON_RIGHT:
			//remove node or object
			deleteObject();
			break;
		
		case SDL_BUTTON_MIDDLE:
			//drag node
			dragNode(event);
			break;
		}

		break;

	case SDL_KEYDOWN:
		//exit program or change color
		parseKey(event);
		break;
	}
}

void createObject(SDL_Event* event) {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int n1 = -1, n2 = -1;
	/* if initial click is not touching a node, create another node
	else, wait for the left button to release. if the release is also on a node,
	create an edge between those two nodes */

	/**/for (int i = 0; i < nodes.size() && n1 == -1; i++) {
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
			edges.push_back(new GraphEdge(nodes[n1], nodes[n2], currentColor));
		}

	}
	else {
		nodes.push_back(new GraphNode(mouseX, mouseY, 15, currentColor));
	}
}

void deleteObject() {
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
	switch (event->key.keysym.sym) {
	case SDLK_SPACE:
		running = false; //exit
		break;
	case SDLK_1:
		currentColor = WHITE;
		break;
	case SDLK_2:
		currentColor = RED;
		break;
	case SDLK_3:
		currentColor = GREEN;
		break;
	case SDLK_4:
		currentColor = BLUE;
		break;
	case SDLK_5:
		currentColor = PURPLE;
		break;
	}
}

void dragNode(SDL_Event* event) {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	GraphNode* toMove = nullptr;
	for (int i = 0; i < nodes.size() && toMove == nullptr; i++) {
		if (nodes[i]->cotainsPoint(mouseX, mouseY)) {
			toMove = nodes[i];
		}
	}

	if (toMove != nullptr) {

	}

}