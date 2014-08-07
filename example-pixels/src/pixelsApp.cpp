#include "pixelsApp.h"

//Pixel access is not implemented in the player
//so here is how to do it yourself

//it should be implemented along with an "isFrameNew" but that isn't working yet

//--------------------------------------------------------------
void pixelsApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_SILENT);
	ofSetVerticalSync(false);
	
	doDrawInfo	= true;
		
	consoleListener.setup(this);
	
	omxCameraSettings.width = 1280;
	omxCameraSettings.height = 720;
	omxCameraSettings.framerate = 30;
	omxCameraSettings.isUsingTexture = true;
	
	videoGrabber.setup(omxCameraSettings);
	
	filterCollection.setup();

	doPixels = true;
	if (doPixels) 
	{
		videoGrabber.enablePixels();
		videoTexture.allocate(omxCameraSettings.width, omxCameraSettings.height, GL_RGBA);
	}

	
		
}	

//--------------------------------------------------------------
void pixelsApp::update()
{
	if (!doPixels)
	{
		return;
	}
	
	videoTexture.loadData(videoGrabber.getPixels(), omxCameraSettings.width, omxCameraSettings.height, GL_RGBA);


}


//--------------------------------------------------------------
void pixelsApp::draw(){
	
	videoGrabber.draw();
	if(doPixels)
	{
		videoTexture.draw(0, 0, omxCameraSettings.width/2, omxCameraSettings.height/2);
	}

	stringstream info;
	info << "APP FPS: " << ofGetFrameRate() << "\n";
	info << "Camera Resolution: " << videoGrabber.getWidth() << "x" << videoGrabber.getHeight()	<< " @ "<< videoGrabber.getFrameRate() <<"FPS"<< "\n";
	info << "CURRENT FILTER: " << filterCollection.getCurrentFilterName() << "\n";
	info << "PIXELS ENABLED: " << doPixels << "\n";
	//info <<	filterCollection.filterList << "\n";
	
	info << "\n";
	info << "Press e to Increment filter" << "\n";
	info << "Press p to Toggle pixel processing" << "\n";
	info << "Press g to Toggle info" << "\n";
	
	if (doDrawInfo) 
	{
		ofDrawBitmapStringHighlight(info.str(), 100, 100, ofColor::black, ofColor::yellow);
	}
	
	//
}

//--------------------------------------------------------------
void pixelsApp::keyPressed  (int key)
{
	ofLogVerbose(__func__) << key;
	
	if (key == 'e')
	{
		videoGrabber.applyImageFilter(filterCollection.getNextFilter());
	}
	if (key == 'g')
	{
		doDrawInfo = !doDrawInfo;
	}
	
	if (key == 'p')
	{
		doPixels = !doPixels;
		if (!doPixels) 
		{
			videoGrabber.disablePixels();
		}else
		{
			videoGrabber.enablePixels();
		}
	}
}

void pixelsApp::onCharacterReceived(SSHKeyListenerEventData& e)
{
	keyPressed((int)e.character);
}
