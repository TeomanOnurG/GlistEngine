/*
 * gCanvas.cpp
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */


#include "gCanvas.h"


gCanvas::gCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}


gCanvas::~gCanvas() {
}


void gCanvas::setup() {
	bothAngle = 0.0f;
	pictureAngle = 0.0f;
	maskAngle = 0.0f;
	freeImageAngle = 0.0f;
	freeMaskAngle = 0.0f;

	image.loadImage("colours5.png");
	mask.loadImage("starmask.png");
	engineLogo.loadImage("glistengine_logo.png");

	imageMaskNoRotation.loadImage("colours5.png");
	imageMaskNoRotation.loadMaskImage("starmask.png");
	imageMaskNoRotation.setMaskMode(gTexture::MASKMODE_BOTH);

	imageBoth.loadImage("colours5.png");
	imageBoth.loadMaskImage("starmask.png");
	imageBoth.setMaskMode(gTexture::MASKMODE_BOTH);

	imagePicture.loadImage("colours5.png");
	imagePicture.loadMaskImage("starmask.png");
	imagePicture.setMaskMode(gTexture::MASKMODE_PICTURE);

	imageMask.loadImage("colours5.png");
	imageMask.loadMaskImage("starmask.png");
	imageMask.setMaskMode(gTexture::MASKMODE_MASK);

	imageFree.loadImage("colours5.png");
	imageFree.loadMaskImage("starmask.png");
	imageFree.setMaskMode(gTexture::MASKMODE_BOTH_FREE_ROTATE);

	font.loadFont("FreeSans.ttf", 16);
}


void gCanvas::update() {
	bothAngle += 1.0f;
	pictureAngle += 1.5f;
	maskAngle += 1.5f;
	freeImageAngle += 3.0f;
	freeMaskAngle += 1.0f;

	if(bothAngle >= 360.0f) bothAngle -= 360.0f;
	if(pictureAngle >= 360.0f) pictureAngle -= 360.0f;
	if(maskAngle >= 360.0f) maskAngle -= 360.0f;
	if(freeImageAngle >= 360.0f) freeImageAngle -= 360.0f;
	if(freeMaskAngle >= 360.0f) freeMaskAngle -= 360.0f;
}


void gCanvas::draw() {
	int size = 180;
	int gap = 55;
	int textGap = 28;

	int topY = 70;
	int bottomY = 370;

	int topWidth = size * 4 + gap * 3;
	int topStartX = (getWidth() - topWidth) / 2;

	int x1 = topStartX;
	int x2 = x1 + size + gap;
	int x3 = x2 + size + gap;
	int x4 = x3 + size + gap;

	image.draw(
		x1,
		topY,
		size,
		size
	);

	mask.draw(
		x2,
		topY,
		size,
		size
	);

	imageMaskNoRotation.setImageRotation(0.0f);
	imageMaskNoRotation.setMaskRotation(0.0f);
	imageMaskNoRotation.draw(
		x3,
		topY,
		size,
		size
	);

	imageBoth.setImageRotation(0.0f);
	imageBoth.setMaskRotation(0.0f);
	imageBoth.draw(
		x4,
		topY,
		size,
		size,
		size / 2,
		size / 2,
		bothAngle
	);

	font.drawText(
		"Image Only",
		x1 + 35,
		topY + size + textGap
	);

	font.drawText(
		"Mask Only",
		x2 + 45,
		topY + size + textGap
	);

	font.drawText(
		"Image + Mask",
		x3 + 30,
		topY + size + textGap
	);

	font.drawText(
		"Rotate Together",
		x4 + 20,
		topY + size + textGap
	);

	int bottomWidth = size * 3 + gap * 2;
	int bottomStartX = (getWidth() - bottomWidth) / 2;

	int x5 = bottomStartX;
	int x6 = x5 + size + gap;
	int x7 = x6 + size + gap;

	imagePicture.setImageRotation(pictureAngle);
	imagePicture.setMaskRotation(0.0f);
	imagePicture.draw(
		x5,
		bottomY,
		size,
		size,
		size / 2,
		size / 2,
		0.0f
	);

	imageMask.setImageRotation(0.0f);
	imageMask.setMaskRotation(maskAngle);
	imageMask.draw(
		x6,
		bottomY,
		size,
		size,
		size / 2,
		size / 2,
		0.0f
	);

	imageFree.setImageRotation(freeImageAngle);
	imageFree.setMaskRotation(freeMaskAngle);
	imageFree.draw(
		x7,
		bottomY,
		size,
		size,
		size / 2,
		size / 2,
		0.0f
	);

	font.drawText(
		"Image Rotates",
		x5 + 30,
		bottomY + size + textGap
	);

	font.drawText(
		"Mask Rotates",
		x6 + 30,
		bottomY + size + textGap
	);

	font.drawText(
		"Free Rotate",
		x7 + 40,
		bottomY + size + textGap
	);
}


void gCanvas::keyPressed(int key) {
}


void gCanvas::keyReleased(int key) {
}


void gCanvas::charPressed(unsigned int codepoint) {
}


void gCanvas::mouseMoved(int x, int y) {
}


void gCanvas::mouseDragged(int x, int y, int button) {
}


void gCanvas::mousePressed(int x, int y, int button) {
}


void gCanvas::mouseReleased(int x, int y, int button) {
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
