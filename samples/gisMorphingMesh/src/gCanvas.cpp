#include "gCanvas.h"

#include <cmath>
#include <vector>

gCanvas::gCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}

gCanvas::~gCanvas() {
}

void gCanvas::updateMouthVisual(float openness) {
	const int segmentcount = 20;

	const float centerx = 6.4f;
	const float centery = -0.22f;
	const float centerz = 0.26f;

	const float radiusx = 0.70f;
	const float radiusz = 0.02f + openness * 0.18f;

	mouthvertices.clear();

	gVertex center;
	center.position = glm::vec3(centerx, centery, centerz);
	center.normal = glm::vec3(0.0f, -1.0f, 0.0f);
	mouthvertices.push_back(center);

	for(int i = 0; i < segmentcount; i++) {
		float angle = 6.2831853f * static_cast<float>(i) / segmentcount;

		gVertex vertex;
		vertex.position = glm::vec3(
				centerx + std::cos(angle) * radiusx,
				centery,
				centerz + std::sin(angle) * radiusz
		);

		vertex.normal = glm::vec3(0.0f, -1.0f, 0.0f);
		mouthvertices.push_back(vertex);
	}

	mouthvisual.setVertices(mouthvertices, mouthindices);
	mouthvisual.setDrawMode(gMesh::DRAWMODE_TRIANGLES);
}

void gCanvas::setup() {
	goldfishmodel.loadModelWithOriginalVertices(
			"Goldfish/13001_Ryukin_Goldfish_v1_L3.obj"
	);

	if(goldfishmodel.getMeshNum() == 0) {
		gLoge("FishMorph") << "Goldfish model could not be loaded";
		return;
	}

	gMesh* basemesh = goldfishmodel.getMeshPtr(0);

	if(basemesh == nullptr) {
		gLoge("FishMorph") << "Goldfish base mesh is nullptr";
		return;
	}

	basemesh->getMaterial()->setAmbientColor(25, 25, 25);
	basemesh->getMaterial()->setDiffuseColor(145, 125, 100);
	basemesh->getMaterial()->setSpecularColor(70, 70, 70);
	basemesh->getMaterial()->setShininess(10.0f);

	std::vector<gVertex> targetvertices = basemesh->getVertices();
	std::vector<gIndex> targetindices = basemesh->getIndices();

	for(std::size_t i = 0; i < targetvertices.size(); i++) {
		glm::vec3 position = targetvertices[i].position;

		float frontweight = glm::clamp(
				(position.x - 4.25f) / 2.20f,
				0.0f,
				1.0f
		);

		float sideweight = glm::clamp(
				1.0f - (position.y / 2.10f) * (position.y / 2.10f),
				0.0f,
				1.0f
		);

		float lowerlip = glm::clamp(
				(-position.z + 0.18f) / 1.05f,
				0.0f,
				1.0f
		);

		float upperlip = glm::clamp(
				(position.z + 0.12f) / 0.90f,
				0.0f,
				1.0f
		);

		float mouthweight = frontweight * sideweight;

		// Target mesh: ayný balýðýn az açýk aðýz hâli.
		position.z -= 0.35f * mouthweight * lowerlip;
		position.z += 0.08f * mouthweight * upperlip;
		position.x += 0.05f * mouthweight * lowerlip;

		targetvertices[i].position = position;
	}

	opentargetmesh.setVertices(targetvertices, targetindices);
	opentargetmesh.setDrawMode(basemesh->getDrawMode());

	fishmorph.setBaseMesh(basemesh);
	targetid = fishmorph.addTargetMesh(&opentargetmesh);

	framecount = 12;
	currentframe = 0;
	updatetick = 0;

	opening = true;
	playing = true;
	dragging = false;

	lastmousex = 0;
	lastmousey = 0;

	panangle = 1.36f;
	tiltangle = -1.61f;

	fishmorph.setCurrentTargetMeshId(targetid);
	fishmorph.setFrameCount(targetid, framecount);
	fishmorph.setCurrentFrameId(currentframe);
	fishmorph.interpolate(true);

	mouthindices.clear();

	for(int i = 0; i < 20; i++) {
		mouthindices.push_back(0);
		mouthindices.push_back(i + 1);
		mouthindices.push_back((i + 1) % 20 + 1);
	}

	updateMouthVisual(0.0f);

	mouthvisual.getMaterial()->setAmbientColor(4, 2, 2);
	mouthvisual.getMaterial()->setDiffuseColor(18, 4, 3);
	mouthvisual.getMaterial()->setSpecularColor(45, 18, 15);
	mouthvisual.getMaterial()->setShininess(10.0f);

	fishmorph.scale(0.30f);
	fishmorph.rotateDeg(-90.0f, 1.0f, 0.0f, 0.0f);
	fishmorph.rotateDeg(-11.0f, 0.0f, 1.0f, 0.0f);
	fishmorph.rotateDeg(-6.0f, 0.0f, 0.0f, 1.0f);

	mouthvisual.scale(0.30f);
	mouthvisual.rotateDeg(-90.0f, 1.0f, 0.0f, 0.0f);
	mouthvisual.rotateDeg(-11.0f, 0.0f, 1.0f, 0.0f);
	mouthvisual.rotateDeg(-6.0f, 0.0f, 0.0f, 1.0f);

	camera.setPosition(0.0f, 0.0f, 6.2f);

	camera.rotateAroundDeg(
			59.0f,
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
	);

	camera.rotateAroundDeg(
			-24.15f,
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
	);

	camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

	keylight.setPosition(3.0f, 3.0f, 5.0f);
	keylight.setAmbientColor(18, 18, 18);
	keylight.setDiffuseColor(120, 105, 85);
	keylight.setSpecularColor(90, 90, 90);

	filllight.setPosition(-3.0f, 1.0f, 3.0f);
	filllight.setAmbientColor(7, 9, 14);
	filllight.setDiffuseColor(45, 60, 85);
	filllight.setSpecularColor(45, 55, 70);

	spotlight.setType(gLight::LIGHTTYPE_SPOT);
	spotlight.setPosition(0.0f, 3.0f, 5.0f);
	spotlight.rotateDeg(-31.0f, 1.0f, 0.0f, 0.0f);

	spotlight.setAmbientColor(3, 3, 3);
	spotlight.setDiffuseColor(255, 210, 170);
	spotlight.setSpecularColor(255, 230, 210);

	spotlight.setSpotCutOffAngle(18.0f);
	spotlight.setSpotCutOffSpread(12.0f);
}

void gCanvas::update() {
	if(!playing) {
		return;
	}

	updatetick++;

	if(updatetick < 5) {
		return;
	}

	updatetick = 0;

	if(opening) {
		if(currentframe < framecount - 1) {
			currentframe++;
		} else {
			opening = false;
		}
	} else {
		if(currentframe > 0) {
			currentframe--;
		} else {
			opening = true;
		}
	}

	fishmorph.setCurrentFrameId(currentframe);
	fishmorph.interpolate(true);

	updateMouthVisual(
			static_cast<float>(currentframe)
			/ static_cast<float>(framecount - 1)
	);
}

void gCanvas::draw() {
	camera.begin();

	enableDepthTest();

	keylight.enable();
	filllight.enable();
	spotlight.enable();

	fishmorph.draw();

	filllight.disable();
	keylight.disable();
	spotlight.disable();

	disableDepthTest();
	mouthvisual.draw();

	camera.end();
}

void gCanvas::keyPressed(int key) {
	if(key == G_KEY_SPACE) {
		playing = !playing;
	}

	if(key == G_KEY_P) {
		gLogi("FishMorph") << "CAMERA PAN: " << panangle
				<< " | CAMERA TILT: " << tiltangle;
	}
}

void gCanvas::keyReleased(int key) {
}

void gCanvas::charPressed(unsigned int codepoint) {
}

void gCanvas::mouseMoved(int x, int y) {
}

void gCanvas::mouseDragged(int x, int y, int button) {
	if(!dragging) {
		return;
	}

	float pandifference = (x - lastmousex) * 0.01f;
	float tiltdifference = (y - lastmousey) * 0.01f;

	panangle += pandifference;
	tiltangle += tiltdifference;

	camera.rotateAroundDeg(
			pandifference * 25.0f,
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
	);

	camera.rotateAroundDeg(
			tiltdifference * 15.0f,
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
	);

	camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

	lastmousex = x;
	lastmousey = y;
}

void gCanvas::mousePressed(int x, int y, int button) {
	dragging = true;
	lastmousex = x;
	lastmousey = y;
}

void gCanvas::mouseReleased(int x, int y, int button) {
	dragging = false;
}

void gCanvas::mouseScrolled(int x, int y) {
}

void gCanvas::mouseEntered() {
}

void gCanvas::mouseExited() {
}

void gCanvas::windowResized(int w, int h) {
}

void gCanvas::showNotify() {
}

void gCanvas::hideNotify() {
}
