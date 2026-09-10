#ifndef GCANVAS_H_
#define GCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gModel.h"
#include "gMesh.h"
#include "gMorphingMesh.h"
#include "gCamera.h"
#include "gLight.h"

#include <vector>

class gCanvas : public gBaseCanvas {
public:
	gCanvas(gApp* root);
	virtual ~gCanvas();

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void charPressed(unsigned int codepoint);

	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseScrolled(int x, int y);
	void mouseEntered();
	void mouseExited();

	void windowResized(int w, int h);
	void showNotify();
	void hideNotify();

private:
	void updateMouthVisual(float openness);

	gApp* root;

	gModel goldfishmodel;
	gMesh opentargetmesh;
	gMorphingMesh fishmorph;
	gLight spotlight;

	gMesh mouthvisual;
	std::vector<gVertex> mouthvertices;
	std::vector<gIndex> mouthindices;

	gCamera camera;
	gLight keylight;
	gLight filllight;

	int targetid;
	int currentframe;
	int framecount;
	int updatetick;

	bool opening;
	bool playing;
	bool dragging;

	int lastmousex;
	int lastmousey;

	// Mouse ile yapýlan toplam dönüþleri tutar.
	float panangle;
	float tiltangle;
};

#endif
