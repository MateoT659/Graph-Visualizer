#include "Includes.h"

void createObject(SDL_Event*, Vec2 mousePos);
void deleteObject(Vec2 mousePos, SDL_Event* event, Uint8 button);
void parseKey(SDL_Event*);
void dragNode(SDL_Event*, Uint8 button, Vec2 mousePos);
void copyObject(SDL_Event*, Uint8 button, Vec2 mousePos);
void openNodeMenu(SDL_Event*);
void openEdgeMenu(SDL_Event*);
void updateHoverStatus(Vec2 mousePos, std::vector<Icon*> iconVec);
void parseMenuClick(Vec2 mousePos, SDL_Event * event); 
void adjustGhostSize(SDL_Event* event);
bool checkSwitchEdges(Vec2 mousePos);
void createText(SDL_Event*, Vec2 mousePos);
void deleteText(SDL_Event* event, Vec2 mousePos, Uint8 button);
void moveText(SDL_Event* event, Vec2 mousePos, Uint8 button);

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
					if(!checkSwitchEdges(mousePos))
						createObject(event, mousePos);
					break;
				case 2:
					if (!checkSwitchEdges(mousePos))
						dragNode(event, SDL_BUTTON_LEFT, mousePos);
					break;
				case 3:
					deleteObject(mousePos, event, SDL_BUTTON_LEFT);
					break;
				case 4:
					copyObject(event, SDL_BUTTON_LEFT, mousePos);
					break;
				case 5:
					createText(event, mousePos);
					break;
				}
			}
			break;

		case SDL_BUTTON_RIGHT:
			switch (selectedInd) {
			case 5:
				deleteText(event, mousePos, SDL_BUTTON_RIGHT);
				break;
			default:
				deleteObject(mousePos, event, SDL_BUTTON_RIGHT);
				break;
			}
			break;
		case SDL_BUTTON_MIDDLE:
			switch (selectedInd) {
			case 5:
				moveText(event, mousePos, SDL_BUTTON_MIDDLE);
				break;
			default:
				if (!checkSwitchEdges(mousePos))
					dragNode(event, SDL_BUTTON_MIDDLE, mousePos);
				break;
			}
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

void createText(SDL_Event* event, Vec2 mousePos) {
	//NEXT figure out text input, use mouse wheel to change size. clicking colorbox changes color using picker. end on ENTER or click off of box. 	
	Textbox* editing = nullptr;
	std::string text;
	bool quit = false;
	bool inArray = false;
	for (int i = 0; i < textboxes.size() && editing == nullptr; i++) {
		if (textboxes[i]->containsPoint(mousePos)) {
			editing = textboxes[i];
			text = editing->getText();
			inArray = true;
		}
	}

	if (editing == nullptr) {
		editing = new Textbox(" ", mousePos, 30, currentColor);
	}
	
	editing->setEditState(true);

	if(!inArray) textboxes.push_back(editing);

	SDL_StartTextInput();

	while (!quit) {
		while (SDL_PollEvent(event)) {
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
				Vec2 mousePos = getMousePos();
				if (event->button.button == SDL_BUTTON_LEFT && (editing->containsPoint(mousePos) || rectIsTouched(colorBox, mousePos))) {
					openColorPicker();
					editing->setColor(currentColor);
				}
				else {
					quit = true;
				}
				break;
			}
			if (text == " ") text = "";
			renderU(false);
		}
	}

	SDL_StopTextInput();
	
	editing->setEditState(false);
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
			renderU(false);
		}
	}
}

void moveText(SDL_Event* event, Vec2 mousePos, Uint8 button) {
	Textbox* toMove = nullptr;
	for (int i = 0; i < textboxes.size() && toMove == nullptr; i++) {
		if (textboxes[i]->containsPoint(mousePos)) {
			toMove = textboxes[i];
		}
	}
	if (toMove == nullptr) return;
	Vec2 mousePos2;
	while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
		if (SDL_PollEvent(event)) {
			mousePos2 = getMousePos();
			toMove->translateBy(mousePos2 - mousePos);
			mousePos = getMousePos();
			renderU(false);
		}
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
			}
			renderU(false);
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

	if (toMove == nullptr) {
		FreeEdge* freeMove = nullptr;
		for (int i = 0; i < freeEdges.size() && freeMove == nullptr; i++) {
			if (freeEdges[i]->isTouched(mousePos)) {
				freeMove = freeEdges[i];
			}
		}

		if (freeMove == nullptr) {
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
			std::unordered_set<FreeEdge*> ftouched;

			for (int i = 0; i < nodes.size(); i++) {
				if (rectIsTouched(selectBox, nodes[i]->getPos())) {
					touched.insert(nodes[i]);
				}
			}
			for (int i = 0; i < freeEdges.size(); i++) {
				if (lineIntersectsRect(selectBox, freeEdges[i]->getFrom(), freeEdges[i]->getTo())) {
					ftouched.insert(freeEdges[i]);
				}
			}
			if (touched.size() == 0 && ftouched.size() == 0) {
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
			if (!isNodeTouched) {
				return;
			}

			switch (event->button.button) {
			case SDL_BUTTON_LEFT:
				//find all nodes in square. move them all by a set amount
				while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)) {
					if (SDL_PollEvent(event)) {
						mousePos2 = getMousePos();
						for (GraphNode* node : touched) {
							node->setPos(node->getPos() + mousePos2 - mousePos);
						}
						for (GraphEdge* edge : edges) {
							edge->update();
						}
						for (FreeEdge* edge : ftouched) {
							edge->translateBy(mousePos2 - mousePos);
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
				std::vector<GraphNode*> copies;
				std::vector<GraphEdge*> eCopies;
				std::vector<FreeEdge*> fCopies;
				std::vector<GraphNode*> touchedVec(touched.begin(), touched.end());
				for (int i = 0; i < touchedVec.size(); i++) {
					copies.push_back(touchedVec[i]->copy());
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
				while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_MIDDLE)) {
					if (SDL_PollEvent(event)) {
						mousePos2 = getMousePos();
						for (GraphNode* node : copies) {
							node->setPos(node->getPos() + mousePos2 - mousePos);
						}
						for (GraphEdge* edge : eCopies) {
							edge->update();
						}
						for (FreeEdge* edge : fCopies) {
							edge->translateBy(mousePos2 - mousePos);
						}
						renderU(false);
						mousePos = getMousePos();
					}
				}
				break;
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

		// finds connected edges
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i]->containsNode(toMove)) {
				toUpdate.push_back(edges[i]);
			}
		}

		// waits for click to release
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
			if (SDL_PollEvent(event)) {
				SDL_GetMouseState(toMove->getXaddr(), toMove->getYaddr());
				for (int i = 0; i < toUpdate.size(); i++) {
					toUpdate[i]->update();
				}
				renderU(false);
			}
		}

		// updates any connected nodes
		for (int i = 0; i < toUpdate.size(); i++) {
			toUpdate[i]->update();
		}
	}
	ghost->setPos(getMousePos());
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
		nodes.push_back(copy);
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
			if (SDL_PollEvent(event)) {
				SDL_GetMouseState(copy->getXaddr(), copy->getYaddr());
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
			std::unordered_set<FreeEdge*> ftouched;

			for (int i = 0; i < nodes.size(); i++) {
				if (rectIsTouched(selectBox, nodes[i]->getPos())) {
					touched.insert(nodes[i]);
				}
			}
			for (int i = 0; i < freeEdges.size(); i++) {
				if (lineIntersectsRect(selectBox, freeEdges[i]->getFrom(), freeEdges[i]->getTo())) {
					ftouched.insert(freeEdges[i]);
				}
			}
			if (touched.size() == 0 && ftouched.size() == 0) {
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
				std::vector<GraphEdge*> eCopies;
				std::vector<FreeEdge*> fCopies;
				std::vector<GraphNode*> touchedVec(touched.begin(), touched.end());
				for (int i = 0; i < touchedVec.size(); i++) {
					copies.push_back(touchedVec[i]->copy());
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
				while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
					if (SDL_PollEvent(event)) {
						mousePos2 = getMousePos();
						for (GraphNode* node : copies) {
							node->setPos(node->getPos() + mousePos2 - mousePos);
						}
						for (GraphEdge* edge : eCopies) {
							edge->update();
						}
						for (FreeEdge* edge : fCopies) {
							edge->translateBy(mousePos2 - mousePos);
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

void parseKey(SDL_Event* event) {

	switch (event->key.keysym.sym) {
	case SDLK_ESCAPE:
		running = false;
		//saveFile();
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
	case SDLK_t:
		icons[selectedInd]->toggleSelected();
		selectedInd = 5;
		icons[5]->toggleSelected();
		break;
	case SDLK_c:
		icons[selectedInd]->toggleSelected();
		selectedInd = 4;
		icons[4]->toggleSelected();
	case SDLK_m:
		icons[selectedInd]->toggleSelected();
		selectedInd = 2;
		icons[2]->toggleSelected();
		break;
	case SDLK_d:
		icons[selectedInd]->toggleSelected();
		selectedInd = 3;
		icons[3]->toggleSelected();
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
	if (den == 0) return true;
	
	double uA = ((v4.x - v3.x) * (v1.y - v3.y) - (v4.y - v3.y) * (v1.x - v3.x)) / den;
	
	double uB = ((v2.x - v1.x) * (v1.y - v3.y) - (v2.y - v1.y) * (v1.x - v3.x)) / den;

	return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
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