#pragma once

//CAMERA CREDENTIALS//
#define USERNAME ""
#define PASSWORD ""
#define STREAM_URL ""
#define CONTROL_URL ""
#define NAME ""
#define CONTROL_URI ""
//////////////////////

//CAMERA CONTROL LIMITS//
#define PAN_MAX 150
#define PAN_MIN -150
#define TILT_MAX 85
#define TILT_MIN -15
#define ZOOM_MAX 2800
#define ZOOM_MIN 200
///////////////////////

#include "ofMain.h"
#include "ofURLFileLoader.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "IPVideoGrabber.h"

void SendControl(std::string suff);

//ControlParams: basically just a depot for keeping track of pan, tilt and zoom and modifying them
//               within the given bounds
class ControlParams
{
public:
	ControlParams(const float pan, const float tilt, const int zoom) :
		_pan(pan),
		_tilt(tilt),
		_zoom(zoom)
	{
	}
	ControlParams()
	{
	}
	float getPan() { return _pan; }
	float getTilt() { return _tilt; }
	int getZoom() { return _zoom; }

	void setPan(float v); 
	void setTilt(float v); 
	void setZoom(int v); 

private:
	float _pan, _tilt;
	int _zoom;
};

//IPCameraDef: control class for the camera. Basically a direct port from cbaker's IPVideoGrabber example
class IPCameraDef
{
public:
	enum AuthType {
		NONE,
		BASIC,
		COOKIE
	};

	IPCameraDef()
	{
	}

	IPCameraDef(const std::string& url) : _url(url)
	{
	}

	IPCameraDef(const std::string& name,
		const std::string& url,
		const std::string& username,
		const std::string& password,
		const AuthType authType) :
		_name(name),
		_url(url),
		_username(username),
		_authType(authType)
	{
	}

	void setTilt(const float degs);
	void setPan(const float degs);
	void setZoom(const int zoom);

	void setName(const std::string& name) { _name = name; }
	std::string getName() const {
		return _name;
	}

	void setURL(const std::string& url) { _url = url; }

	void setUsername(const std::string& username) {
		_username = username;
	}

	std::string getUsername() const {
		return _username;
	}

	void setPassword(const std::string& password) {
		_password = password;
	}

	std::string getPassword() {
		return _password;
	}

	void setAuthType(AuthType authType) {
		_authType = authType;
	}

	AuthType getAuthType() const {
		return _authType;
	}

	//cookies do not currently work
	void setCookie(const std::string& key, const std::string& value);
	void eraseCookie(const std::string& key);
	std::string getCookie(const std::string& key) const;

private:
	std::string _name;
	std::string _url;
	std::string _username;
	std::string _password;
	AuthType _authType = NONE;
	Poco::Net::NameValueCollection cookies;
};

using namespace ofx;

class ofApp : public ofBaseApp{
	
	ControlParams ctrl;
	std::shared_ptr<Video::IPVideoGrabber> grabber;
	IPCameraDef camera;
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void loadCamera();
		
		
};
