#include "Gradation.h"

using namespace ofxDome;

QuarterSphereGradation::QuarterSphereGradation(ofPtr<QuarterSphereMesh> mesh, float width)
: mesh(mesh), halfWidth(width * 0.5f) {
	generate();
}

QuarterSphereGradation::QuarterSphereGradation()
: halfWidth(0.0f) {
	
}
	
void QuarterSphereGradation::generate() {
	assert(mesh != NULL);
	
	vbo.clear();
	
	std::vector<ofVec2f> verts;
	verts.reserve((mesh->getVerticalDivision() + 1) * 2 + 4);
	std::vector<ofFloatColor> colors;
	colors.reserve((mesh->getVerticalDivision() + 1) * 2 + 4);
	std::vector<ofIndexType> indices;
	int trisNum = mesh->getVerticalDivision() * 4 + mesh->getVerticalDivision() * 2 + 3;
	indices.reserve(trisNum * 3);
	
	for (int i = mesh->getVerticalDivision() - 1; i >= 0; i--) {
		const MeshLine& line = mesh->getNthHorizontalLine(i);
		float w = 1.0f / line.vertIndices.size();
		verts.push_back(line.getInterpolatedScreenPosition(w * halfWidth));
		colors.push_back(ofFloatColor::white);
		verts.push_back(line.getInterpolatedScreenPosition(- w * halfWidth));
		colors.push_back(ofFloatColor::black);
	}
	
	int ceilIndex = verts.size();
	{
		const MeshLine& centerLine = mesh->getNthVerticalLine((mesh->getHorizontalDivision() - 2) / 2);
		float w = 1.0f / centerLine.vertIndices.size();
		verts.push_back(centerLine.getInterpolatedScreenPosition(0.0f/*- w * halfWidth*/));
		colors.push_back(ofFloatColor::black);
		verts.push_back(centerLine.getInterpolatedScreenPosition(w * halfWidth));
		colors.push_back(ofFloatColor::white);
	}
	
	for (int i = 0; i < mesh->getVerticalDivision(); i++) {
		const MeshLine& line = mesh->getNthHorizontalLine(i);
		float w = 1.0f / line.vertIndices.size();
		verts.push_back(line.getInterpolatedScreenPosition(1.0f - w * halfWidth));
		colors.push_back(ofFloatColor::white);
		verts.push_back(line.getInterpolatedScreenPosition(1.0f/* + w * halfWidth*/));
		colors.push_back(ofFloatColor::black);
	}
	
	// create triangle strip
	for (int i = 0; i < verts.size() - 2; i++) {
		indices.push_back(i);
		indices.push_back(i+1);
		indices.push_back(i+2);
	}
	
	int lefttopIndex = verts.size();
	verts.push_back(ofVec2f(0.0f, 0.0f));
	colors.push_back(ofFloatColor::black);

	int righttopIndex = verts.size();
	verts.push_back(ofVec2f(1.0f, 0.0f));
	colors.push_back(ofFloatColor::black);
	
	int leftbottomIndex = verts.size();
	verts.push_back(ofVec2f(0.0f, 1.0f));
	colors.push_back(ofFloatColor::black);
	
	int rightbottomIndex = verts.size();
	verts.push_back(ofVec2f(1.0f, 1.0f));
	colors.push_back(ofFloatColor::black);
	
	for (int i = 0; i < mesh->getVerticalDivision(); i += 2) {
		indices.push_back(lefttopIndex);
		indices.push_back(i);
		indices.push_back(i + 2);
		
		indices.push_back(righttopIndex);
		indices.push_back(ceilIndex + i);
		indices.push_back(ceilIndex + i + 2);
	}
	
	indices.push_back(ceilIndex);
	indices.push_back(lefttopIndex);
	indices.push_back(righttopIndex);
	
	indices.push_back(0);
	indices.push_back(lefttopIndex);
	indices.push_back(leftbottomIndex);
	
	indices.push_back(mesh->getVerticalDivision() + ceilIndex);
	indices.push_back(righttopIndex);
	indices.push_back(rightbottomIndex);
	
	vbo.setVertexData(verts.data(), verts.size(), GL_DYNAMIC_DRAW);
	vbo.setColorData(colors.data(), colors.size() * 4, GL_DYNAMIC_DRAW);
	vbo.setIndexData(indices.data(), indices.size(), GL_DYNAMIC_DRAW);
}

void QuarterSphereGradation::setMesh(ofPtr<QuarterSphereMesh> mesh) {
	this->mesh = mesh;
}

void QuarterSphereGradation::setWidth(float width) {
	halfWidth = width * 0.5f;
}

void QuarterSphereGradation::draw() {
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	vbo.drawElements(GL_TRIANGLES, vbo.getNumIndices());
	ofPopStyle();
}