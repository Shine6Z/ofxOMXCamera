#include "ofxOMXVideoGrabber.h"

#pragma mark SETUP
ofxOMXVideoGrabber::ofxOMXVideoGrabber()
{
    resetValues();
	updateFrameCounter = 0;
	frameCounter = 0;
	hasNewFrame = false;
    camera = NULL;
	pixelsRequested = false;
	ofAddListener(ofEvents().update, this, &ofxOMXVideoGrabber::onUpdate);    
}

void ofxOMXVideoGrabber::reset()
{
    resetValues();
    //settings.resetValues();
    applyAllSettings();
}

void ofxOMXVideoGrabber::setup(ofxOMXCameraSettings& settings_)
{
    settings = settings_;
    ofLogNotice(__func__) << settings.toJSON().dump();
    ofLogNotice(__func__) << "settings: " << settings.toString();
    
    
    
    if(settings.drawRectangle.isZero())
    {
        settings.drawRectangle.set(0, 0, settings.width, settings.height);
    }
    
    
    if(settings.enableTexture)
    {
        displayController.generateEGLImage(settings.width, settings.height);
        if(settings.enablePixels)
        {
            pixelsRequested = true;
        }
    }
    if(engine.isOpen)
    {
        engine.close();
        //ofSleepMillis(2000);
    }
    bool didOpen = engine.setup(&settings, this, displayController.eglImage);
    if(!didOpen)
    {
        ofLogError(__func__) << "COULD NOT OPEN ENGINE";
    }
    
}

void ofxOMXVideoGrabber::onVideoEngineStart()
{
    ofLogVerbose(__func__) << endl;

    OMX_ERRORTYPE error = OMX_ErrorNone;
    camera = engine.camera;
    resetValues();
    
    //checkBurstMode();
    error = applyExposure();
    OMX_TRACE(error);
    
    //checkFlickerCancellation();
    
    applyAllSettings();
    
    if(settings.enableTexture)
    {
        displayController.setup(&settings);
    }else
    {
        displayController.setup(&settings, engine.render);

    }
    
    ofAddListener(ofEvents().update, this, &ofxOMXVideoGrabber::onUpdate); 
    engine.isOpen = true;
    
    
}

void ofxOMXVideoGrabber::onVideoEngineClose()
{
    ofLogVerbose(__func__) << endl;
}

void ofxOMXVideoGrabber::onRecordingComplete(string filePath)
{
    if(settings.videoGrabberListener)
    {
        settings.videoGrabberListener->onRecordingComplete(filePath);
    }else
    {
        ofLogWarning(__func__) << "RECEIVED " << filePath << " BUT NO LISTENER SET";
    }
}

bool ofxOMXVideoGrabber::isReady()
{
    return engine.isOpen;
}

#pragma mark UPDATE
void ofxOMXVideoGrabber::onUpdate(ofEventArgs & args)
{
    
    if(!engine.isOpen)
    {
        //ofLogError() << "ENGINE CLOSED";
        return;
    }
    if (settings.enableTexture) 
    {
        frameCounter = engine.getFrameCounter();
        if (frameCounter > updateFrameCounter) 
        {
            updateFrameCounter = frameCounter;
            hasNewFrame = true;
            
        }else
        {
            hasNewFrame = false;
        }
        if (hasNewFrame) 
        {
            displayController.updateTexture(pixelsRequested);
        }
    }
	//ofLogVerbose() << "hasNewFrame: " << hasNewFrame;
}



#pragma mark GETTERS

bool ofxOMXVideoGrabber::isFrameNew()
{
    if(!settings.enableTexture)
    {
        return  true;
    }
    return hasNewFrame;
}

bool ofxOMXVideoGrabber::isTextureEnabled()
{
    return settings.enableTexture;
}

int ofxOMXVideoGrabber::getWidth()
{
	return settings.width;
}

int ofxOMXVideoGrabber::getHeight()
{
	return settings.height;
}

int ofxOMXVideoGrabber::getFrameRate()
{
	return settings.framerate;
}

#pragma mark PIXELS/TEXTURE
GLuint ofxOMXVideoGrabber::getTextureID()
{
	return displayController.textureID;
}

void ofxOMXVideoGrabber::enablePixels()
{
    if(settings.enableTexture)
    {
        pixelsRequested = true;
    }
}

void ofxOMXVideoGrabber::disablePixels()
{
    if(settings.enableTexture)
    {
        pixelsRequested = false;
    }
}

unsigned char * ofxOMXVideoGrabber::getRawPixels()
{
    return displayController.pixels;
}

ofPixels& ofxOMXVideoGrabber::getPixels()
{
    return displayController.of_pixels;
}

ofTexture& ofxOMXVideoGrabber::getTextureReference()
{
	return displayController.texture;
}

#pragma mark RECORDING


bool ofxOMXVideoGrabber::isRecording()
{
    return engine.isRecording;
}

void ofxOMXVideoGrabber::startRecording()
{
    engine.startRecording();
}

void ofxOMXVideoGrabber::stopRecording()
{
	engine.stopRecording();
}

#pragma mark DRAW
void ofxOMXVideoGrabber::setDisplayAlpha(int alpha)
{
    displayController.setDisplayAlpha(alpha);
}

void ofxOMXVideoGrabber::setDisplayLayer(int layer)
{
    displayController.setDisplayLayer(layer);
}

void ofxOMXVideoGrabber::setDisplayRotation(int rotationDegrees)
{
    displayController.setDisplayRotation(rotationDegrees);
}

void ofxOMXVideoGrabber::setDisplayDrawRectangle(ofRectangle drawRectangle)
{
    displayController.setDisplayDrawRectangle(drawRectangle);

}

void ofxOMXVideoGrabber::setDisplayCropRectangle(ofRectangle cropRectangle)
{
    displayController.setDisplayCropRectangle(cropRectangle);

}

void ofxOMXVideoGrabber::setDisplayMirror(bool doMirror)
{
    displayController.setDisplayMirror(doMirror);
}



void ofxOMXVideoGrabber::draw(ofRectangle& rectangle)
{
    displayController.draw(rectangle.x, rectangle.y, rectangle.width, rectangle.height);
}

void ofxOMXVideoGrabber::draw(int x, int y)
{
    displayController.draw(x, y);  
}

void ofxOMXVideoGrabber::draw(int x, int y, int width, int height)
{
    displayController.draw(x, y, width, height);
}

void ofxOMXVideoGrabber::draw()
{
    displayController.draw();  
}


#pragma mark EXIT

void ofxOMXVideoGrabber::close()
{
    
    cout << "ofxOMXVideoGrabber::close" << endl;
    ofRemoveListener(ofEvents().update, this, &ofxOMXVideoGrabber::onUpdate);
    engine.close(); 
    cout << "~ofxOMXVideoGrabber::close END" << endl;
}

ofxOMXVideoGrabber::~ofxOMXVideoGrabber()
{
    cout << "~ofxOMXVideoGrabber" << endl;
    close();
}


