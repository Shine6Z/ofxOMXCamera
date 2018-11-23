/*
 *  BaseEngine.h
 *
 *  Created by jason van cleave on 2/10/14.
 *  Copyright 2014 jasonvancleave.com. All rights reserved.
 *
 */

#pragma once

#include "OMX_Maps.h"
#include "ofAppEGLWindow.h"
#include "ofxOMXCameraSettings.h"
#include "DisplayController.h"

class VideoEngineListener
{
public:
    virtual void onRecordingComplete(string filePath)=0;
    virtual void onVideoEngineStart()=0;
    virtual void onVideoEngineClose()=0;

    
};

class VideoEngine
{
public:
	VideoEngine();
    ~VideoEngine();

	bool setup(ofxOMXCameraSettings*, VideoEngineListener*, EGLImageKHR eglImage_=NULL);
    int getFrameCounter();
    
    void startRecording();
    void stopRecording();
	OMX_HANDLETYPE camera;
	bool isOpen;
    bool isRecording;
    VideoEngineListener* listener;

    int frameCounter;
    void close();
    OMX_ERRORTYPE onCameraEventParamOrConfigChanged();
    
    OMX_ERRORTYPE onImageFXEventParamOrConfigChanged();
    OMX_HANDLETYPE render;

protected:
	
    OMX_BUFFERHEADERTYPE* eglBuffer;

    ofxOMXCameraSettings* settings;
	
	OMX_HANDLETYPE splitter;
	OMX_HANDLETYPE encoder;
    OMX_HANDLETYPE imageFX;
    OMX_HANDLETYPE nullSink;

    EGLImageKHR eglImage;

	bool didWriteFile;
	
	int recordingBitRate;
	
	bool stopRequested;
	bool isStopping;
	
	void writeFile();
	
	ofBuffer recordingFileBuffer;
	OMX_BUFFERHEADERTYPE* encoderOutputBuffer;
	
	int recordedFrameCounter;
	


    static OMX_ERRORTYPE textureRenderFillBufferDone( OMX_HANDLETYPE, OMX_PTR, OMX_BUFFERHEADERTYPE*);

    
    static OMX_ERRORTYPE encoderFillBufferDone(OMX_HANDLETYPE encoder, OMX_PTR engine, OMX_BUFFERHEADERTYPE* encoderOutputBuffer);
    static OMX_ERRORTYPE cameraEventHandlerCallback(OMX_HANDLETYPE camera, OMX_PTR videoModeEngine_, OMX_EVENTTYPE eEvent, OMX_U32 nData1, OMX_U32 nData2, OMX_PTR pEventData);

    static OMX_ERRORTYPE nullEmptyBufferDone(OMX_HANDLETYPE, OMX_PTR, OMX_BUFFERHEADERTYPE*){return OMX_ErrorNone;};
    static OMX_ERRORTYPE nullFillBufferDone(OMX_HANDLETYPE, OMX_PTR, OMX_BUFFERHEADERTYPE*){return OMX_ErrorNone;};
    static OMX_ERRORTYPE nullEventHandler(OMX_HANDLETYPE, OMX_PTR, OMX_EVENTTYPE, OMX_U32, OMX_U32, OMX_PTR){return OMX_ErrorNone;};

    static OMX_ERRORTYPE imageFXEventHandlerCallback(OMX_HANDLETYPE camera, OMX_PTR videoModeEngine_, OMX_EVENTTYPE eEvent, OMX_U32 nData1, OMX_U32 nData2, OMX_PTR pEventData);

    OMX_STRING renderType; 
    int renderInputPort;
    
	
};
