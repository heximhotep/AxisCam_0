#include "ofApp.h"

//SendControl: Handles digest authentification with the Axis server
void SendControl(std::string suff)
{
	std::stringstream reqStream;
	reqStream << CONTROL_URI << suff;
	//_controller.getAsync(reqStream.str());
	Poco::Net::HTTPClientSession session(CONTROL_URL);

	Poco::Net::HTTPRequest request(
		"GET",
		reqStream.str(),
		Poco::Net::HTTPMessage::HTTP_1_1
	);
	session.sendRequest(request);

	Poco::Net::HTTPResponse response;
	session.receiveResponse(response);

	response.write(std::cout);

	Poco::Net::HTTPCredentials creds(USERNAME, PASSWORD);
	creds.authenticate(request, response);

	session.sendRequest(request);

	std::istream& bodyStream = session.receiveResponse(response);
	response.write(std::cout);
}

void ControlParams::setPan(const float v)
{
	_pan = CLAMP(v, PAN_MIN, PAN_MAX);
}

void ControlParams::setTilt(const float v)
{
	_tilt = CLAMP(v, TILT_MIN, TILT_MAX);
}

void ControlParams::setZoom(const int v)
{ 
	_zoom = CLAMP(v, ZOOM_MIN, ZOOM_MAX); 
}

void IPCameraDef::setTilt(const float deg)
{
	if (deg > -15 && deg < 85)
	{
		SendControl("tilt=" + ofToString(deg));
	}
}

void IPCameraDef::setPan(const float deg)
{
	if (deg > -150 && deg < 150)
	{
		SendControl("pan=" + ofToString(deg));
	}
}

void IPCameraDef::setZoom(const int zoom)
{
	if (zoom > 200 && zoom < 2800)
	{
		SendControl("zoom=" + ofToString(zoom));
	}
}

void IPCameraDef::setCookie(const std::string& key, const std::string& value)
{
	cookies.add(key, value);
}

void IPCameraDef::eraseCookie(const std::string& key)
{
	cookies.erase(key);
}

std::string IPCameraDef::getCookie(const std::string& key) const
{
	return cookies.get(key);
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(30);
	loadCamera();

	//set initial values for (pan, tilt, zoom) respectively
	ctrl = ControlParams(0, 0, 500);

	grabber = std::make_shared<Video::IPVideoGrabber>();

	grabber->setUsername(camera.getUsername());
	grabber->setPassword(camera.getPassword());
	grabber->setCameraName(camera.getName());
	grabber->setURI(STREAM_URL);
	grabber->connect();
}

void ofApp::keyPressed(int key)
{
	bool tiltChanged = false;
	bool panChanged = false;
	bool zoomChanged = false;
	switch (key)
	{
	case('w'):
		ctrl.setTilt(ctrl.getTilt() + 5.0f);
		tiltChanged = true;
		break;
	case('a'):
		ctrl.setPan(ctrl.getPan() + 5.0f);
		panChanged = true;
		break;
	case('s'):
		ctrl.setTilt(ctrl.getTilt() - 5.0f);
		tiltChanged = true;
		break;
	case('d'):
		ctrl.setPan(ctrl.getPan() - 5.0f);
		panChanged = true;
		break;
	case('e'):
		ctrl.setZoom(ctrl.getZoom() + 10);
		zoomChanged = true;
		break;
	case('q'):
		ctrl.setZoom(ctrl.getZoom() - 10);
		zoomChanged = true;
		break;
	}
	if (tiltChanged)
		camera.setTilt(ctrl.getTilt());
	if (panChanged)
		camera.setPan(ctrl.getPan());
	if (zoomChanged)
		camera.setZoom(ctrl.getZoom());
}

void ofApp::update()
{
	grabber->update();
}

void ofApp::draw()
{
	ofBackground(0, 0, 0);
	ofSetHexColor(0xffffff);

	int w = ofGetWidth();
	int h = ofGetHeight();

	float kbps = 0, fps = 0;

	ofSetColor(255, 255, 255, 255);
	grabber->draw(0, 0, w, h);
	//draw info
	ofEnableAlphaBlending();
	ofSetColor(0, 80);
	ofDrawRectangle(5, 5, w - 10, h - 10);
	kbps = grabber->getBitRate() / 1000.0f;
	fps = grabber->getFrameRate();

	std::stringstream ss;

	ss << "            NAME: " << grabber->getCameraName() << std::endl;
	ss << "            HOST: " << grabber->getHost() << std::endl;
	ss << "             FPS: " << ofToString(fps) << std::endl;
	ss << "            Kb/S: " << ofToString(kbps) << std::endl;
	ss << "          Failed: " << (grabber->hasConnectionFailed() ? "YES" : "NO");

	ofSetColor(255);
	ofDrawBitmapString(ss.str(), 10, 10 + 5);
	ofDisableAlphaBlending();
}

void ofApp::loadCamera()
{

	std::string username(USERNAME);
	std::string password(PASSWORD);
	std::string url(STREAM_URL);
	std::string name(NAME);

	ofLog(OF_LOG_NOTICE, "-----Loading Stream-----");
	IPCameraDef::AuthType authType = IPCameraDef::AuthType::NONE;

	camera = IPCameraDef("", url, "", "", authType);

	ofLogNotice() << "STREAM LOADED" << std::endl;
}
