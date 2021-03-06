﻿#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->ico_sphere = ofIcoSpherePrimitive(65, 2);
	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	this->noise_seed = ofRandom(1000);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh.clear();
	this->frame.clear();

	auto triangle_list = this->ico_sphere.getMesh().getUniqueFaces();

	for (int x = -200; x <= 200; x += 200) {

		for (int y = -200; y <= 200; y += 200) {

			glm::vec3 location = glm::vec3(x, y, 0);
			for (auto& triangle : triangle_list) {

				auto mesh_index = this->mesh.getNumVertices();
				auto frame_index = this->frame.getNumVertices();
				glm::vec3 avg = location + (triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
				auto noise_value = ofNoise(glm::vec4(avg * 0.015, this->noise_seed));
				auto noise_radius = 0;

				if (noise_value < 0.75) {

					noise_radius = 65;
				}
				else {

					noise_radius = ofMap(noise_value, 0.75, 1.0, 65, 120);
				}

				this->mesh.addVertex(location + glm::normalize(triangle.getVertex(0)) * noise_radius);
				this->mesh.addVertex(location + glm::normalize(triangle.getVertex(1)) * noise_radius);
				this->mesh.addVertex(location + glm::normalize(triangle.getVertex(2)) * noise_radius);

				this->mesh.addIndex(mesh_index + 0); this->mesh.addIndex(mesh_index + 1); this->mesh.addIndex(mesh_index + 2);

				avg = (this->mesh.getVertex(mesh_index + 0) + this->mesh.getVertex(mesh_index + 1) + this->mesh.getVertex(mesh_index + 2)) / 3;

				this->frame.addVertex(avg);
				this->frame.addVertex(location);

				this->frame.addIndex(frame_index + 0); this->frame.addIndex(frame_index + 1);
			}
		}
	}

	if (ofGetFrameNum() % 90 < 60) {

		this->noise_seed += ofMap(ofGetFrameNum() % 90, 0, 60, 0.05, 0);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(239);
	this->mesh.draw();

	ofSetColor(39);
	this->mesh.drawWireframe();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}