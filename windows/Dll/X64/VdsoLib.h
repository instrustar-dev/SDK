#ifndef VDSOLIB_H
#define VDSOLIB_H

#include <Windows.h>


#ifdef DLL_IMPORT
#define DLL_API extern "C" __declspec(dllimport)
#else
#define DLL_API extern "C" __declspec(dllexport)
#endif

/*************************************************
　　V20191217
*************************************************/

//////////////////////////////////////////////////////////////////////Initialization/Finished Dll//////////////////////////////////////////////////////////////////
/*************************************************
　　Description   Dll initialization
    Input:       -
　　Output:      Init Status
　　             Return value 1 Success
　　	                      0 Failed
*************************************************/
DLL_API int WINAPI InitDll();

/*************************************************
　　Description   Dll finished
    Input:       -
　　Output:      -Finished Status
　　              Return value 1 Success
　　						   0 Failed
*************************************************/
DLL_API int WINAPI FinishDll();
//////////////////////////////////////////////////////////////////////Initialization/Finished Dll//////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////Device/////////////////////////////////////////////////////////////////////////
/*************************************************
　　Description  This routines return only id(0-31)
	Input:       -
	Output:      -only id(0-31)
	　　         Return only id(0-31)
*************************************************/
DLL_API unsigned int WINAPI GetOnlyId0();

/*************************************************
　　Description  This routines return only id(32-63)
	Input:       -
	Output:      -only id(32-63)
  　　			Return only id(32-63)
*************************************************/
DLL_API unsigned int WINAPI GetOnlyId1();

/*************************************************
　　Description   Reset Device
	Input:       -
  　Output:      -Reset Status
　　             Return value 1 Success
　　	                      0 Failed
   *************************************************/
DLL_API int WINAPI ResetDevice();
///////////////////////////////////////////////////////////////////////////Device/////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////USB status/////////////////////////////////////////////////////////////////////////
/**********************************callback functio****************************************
	 Description    This routines sets the callback function of equipment status changed. 
     Input:			ppara			the parameter of the callback function
　　				addcallback		a pointer to a function with the following prototype:
										void AddCallBack( void * ppara)
					rmvcallback		a pointer to a function with the following prototype:
										Void RemoveCallBack( void * ppara)
     Output      
*******************************************************************************************/
typedef void (CALLBACK *AddCallBack)(void* ppara);
typedef void (CALLBACK *RemoveCallBack)(void* ppara);
DLL_API void WINAPI SetDevNoticeCallBack(void* ppara, AddCallBack addcallback, RemoveCallBack rmvcallback);

/**************************************Event*************************************************
	Description  This routines set the event handle, these will be set, when equipment status changed. 
    Input:       addevent   the event handle
				 rmvevent   the event handle
	Output  
*******************************************************************************************/
DLL_API void WINAPI SetDevNoticeEvent(HANDLE addevent, HANDLE rmvevent);

/**************************************Event*************************************************
	Description  This routines return the device is available or not.
　　Input:      -
　　Output     Return value 1 available
　　						0 not available
*******************************************************************************************/
DLL_API int WINAPI IsDevAvailable();
///////////////////////////////////////////////////////////////////////////USB status/////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////Oscilloscope///////////////////////////////////////////////////////////////////////////
/**************************************Capture range*************************************************
	Description  This routines set the range of input signal.
　　Input:      channel     the set channel 
　　                                    0  channel 1
　　                                    1  channel 2
               minmv     the minimum voltage of the input signal (mV)
               maxmv     the maximum voltage of the input signal (mV)
　　Output     Return value 1 Success
　　	                    0 Failed
*************************************************************************************************/
DLL_API int WINAPI SetOscChannelRange(int channel, int minmv, int maxmv);

/**************************************Support Sample Number********************************************
	Description  This routines get the number of samples that the equipment support.
　　Input:		-               
	Output		Return value	the sample number
*************************************************************************************************/
DLL_API int WINAPI GetOscSupportSampleNum();

/**************************************Support Sample********************************************
	Description  This routines get support samples of equipment.
　　Input:		sample			the array store the support samples of the equipment
				maxnum			the length of the array               
	Output		Return value	the sample number of array stored
*************************************************************************************************/
DLL_API int WINAPI GetOscSupportSamples(unsigned int* sample, int maxnum);

/**************************************Sample*****************************************************
	Description  This routines get the sample.
　　Input:      
　　Output      Return value sample
*************************************************************************************************/
DLL_API unsigned int WINAPI GetOscSample();
/**************************************Sample*****************************************************
	Description  This routines set the sample.
　　Input:      sample       the set sample
　　Output      Return value 0 Failed
							 other value new sample					 
*************************************************************************************************/
DLL_API unsigned int WINAPI SetOscSample(unsigned int sample);


/**************************************Hardware Trigger********************************************
	Description  This routines get the equipment support hardware trigger or not .
　　Input:      -
　　Output     Return value 1 support hardware trigger
							0 not support hardware trigger
	購噐乾窟�歳菻�譜姥屶隔啣周乾窟
****************************************Hardware Trigger*******************************************/
DLL_API int WINAPI IsSupportHardTrigger();


#define TRIGGER_MODE_AUTO 0
#define TRIGGER_MODE_LIANXU 1
/**************************************Trigger Mode***********************************************
    Description  This routines get the trigger mode.
　　Input:      -
　　Output     Return value TRIGGER_MODE_AUTO
                            TRIGGER_MODE_LIANXU
*************************************************************************************************/
DLL_API unsigned int WINAPI GetTriggerMode();
/**************************************Trigger Mode***********************************************
    Description  This routines set the trigger mode.
　　Input:      mode  TRIGGER_MODE_AUTO
　　                  TRIGGER_MODE_LIANXU
    Output
*************************************************************************************************/
DLL_API void WINAPI SetTriggerMode(unsigned int mode);


#define TRIGGER_STYLE_NONE 0x0000    		//not trigger
#define TRIGGER_STYLE_RISE_EDGE 0x0001		//Rising edge
#define TRIGGER_STYLE_FALL_EDGE 0x0002 		//Falling edge
#define TRIGGER_STYLE_EDGE 0x0004 			//Edge
#define TRIGGER_STYLE_P_MORE 0x0008 		//Positive Pulse width(>)
#define TRIGGER_STYLE_P_LESS 0x0010 		//Positive Pulse width(>)
#define TRIGGER_STYLE_P      0x0020 		//Positive Pulse width(<>)
#define TRIGGER_STYLE_N_MORE 0x0040 		//Negative Pulse width(>)
#define TRIGGER_STYLE_N_LESS 0x0080 		//Negative Pulse width(>)
#define TRIGGER_STYLE_N      0x0100  		//Negative Pulse width(<>)
/**************************************Trigger Style***********************************************
	Description  This routines get the trigger style.
　　Input:      -
　　Output     Return value TRIGGER_STYLE_NONE
							TRIGGER_STYLE_RISE_EDGE
							TRIGGER_STYLE_FALL_EDGE
							TRIGGER_STYLE_EDGE
							TRIGGER_STYLE_P_MORE
							TRIGGER_STYLE_P_LESS 		
							TRIGGER_STYLE_P
							TRIGGER_STYLE_N_MORE
							TRIGGER_STYLE_N_LESS
							TRIGGER_STYLE_N
*************************************************************************************************/
DLL_API unsigned int WINAPI GetTriggerStyle();
/**************************************Trigger Style***********************************************
	Description  This routines set the trigger style.
　　Input:       style	TRIGGER_STYLE_NONE
						TRIGGER_STYLE_RISE_EDGE
						TRIGGER_STYLE_FALL_EDGE
						TRIGGER_STYLE_EDGE
						TRIGGER_STYLE_P_MORE
						TRIGGER_STYLE_P_LESS 		
						TRIGGER_STYLE_P
						TRIGGER_STYLE_N_MORE
						TRIGGER_STYLE_N_LESS
						TRIGGER_STYLE_N
　　Output     -
*************************************************************************************************/
DLL_API void WINAPI SetTriggerStyle(unsigned int style);


/**************************************Pulse Width***************************************************
	Description  This routines get the min time of pulse width.
　　Input:      -
　　Output     Return min time value of pulse width(ns)
*******************************************************************************************************/
DLL_API int WINAPI GetTriggerPulseWidthNsMin();
/**************************************Pulse Width***************************************************
	Description  This routines get the max time of pulse width.
　　Input:      -
　　Output     Return max time value of pulse width(ns)
*******************************************************************************************************/
DLL_API int WINAPI GetTriggerPulseWidthNsMax();
/**************************************Pulse Width***************************************************
	Description  This routines get the down time of pulse width.
　　Input:      -
　　Output     Return down time value of pulse width(ns)
*******************************************************************************************************/
DLL_API int WINAPI GetTriggerPulseWidthDownNs();
/**************************************Pulse Width***************************************************
	Description  This routines set the down time of pulse width.
　　Input:      down time value of pulse width(ns)
　　Output     - 
*******************************************************************************************************/
DLL_API int WINAPI GetTriggerPulseWidthUpNs();
/**************************************Pulse Width***************************************************
	Description  This routines set the up time of pulse width.
　　Input:      up time value of pulse width(ns)
　　Output     _ 
*******************************************************************************************************/
DLL_API void WINAPI SetTriggerPulseWidthNs(int down_ns, int up_ns);


/**************************************Trigger Source***********************************************
	Description  This routines get the trigger source.
　　Input:      -
　　Output     Return value  0 :channel 1
　　                         1 :channel 2
*************************************************************************************************/
DLL_API unsigned int WINAPI GetTriggerSource();
/**************************************Trigger Source***********************************************
	Description  This routines set the trigger source.
    Input:      source  0 :channel 1
　　                    1 :channel 2
　　Output      -
*************************************************************************************************/
DLL_API void WINAPI SetTriggerSource(unsigned int source); 


/**************************************Trigger level***********************************************
    Description  This routines get the trigger level.
　　Input:      -
　　Output     Return value  level (mV)
*************************************************************************************************/
DLL_API int WINAPI GetTriggerLevel();
/**************************************Trigger level***********************************************
    Description  This routines set the trigger level.
　　Input:      level (mV)
    Output     -
*************************************************************************************************/
DLL_API void WINAPI SetTriggerLevel(int level);


/**************************************Trigger Sense********************************************
	Description  This routines get the equipment support trigger sense or not .
　　Input:      -
　　Output     Return value 1 support
							0 not support
****************************************Trigger Sense*******************************************/
DLL_API int WINAPI IsSupportTriggerSense();
/**************************************Trigger Sense***********************************************
    Description  This routines get the trigger Sense.
　　Input:      -
　　Output     Return value  Sense (0-1 div)
*************************************************************************************************/
DLL_API double WINAPI GetTriggerSenseDiv();
/**************************************Trigger Sense***********************************************
    Description  This routines set the trigger Sense.
　　Input:      Sense (0-1div)
    Output     -
*************************************************************************************************/
DLL_API void WINAPI SetTriggerSenseDiv(double sense);


//
/**************************************Pre-trigger Percent 圓乾窟曳箭********************************************
	Description  This routines get the equipment support Pre-trigger Percent or not .
　　Input:      -
　　Output     Return value 1 support
							0 not support
****************************************Trigger Sense*******************************************/
DLL_API bool WINAPI IsSupportPreTriggerPercent();
/**************************************Pre-trigger Percent***********************************************
    Description  This routines get the Pre-trigger Percent.
　　Input:      -
　　Output     Return value Percent (5-95)
*************************************************************************************************/
DLL_API int WINAPI GetPreTriggerPercent();
/**************************************Pre-trigger Percent***********************************************
    Description  This routines set the Pre-trigger Percent.
　　Input:      Percent (5-95)
    Output     -
*************************************************************************************************/
DLL_API void WINAPI SetPreTriggerPercent(int front);


/**************************************Trigger Force********************************************
	Description  This routines get the equipment support trigger force or not .
　　Input:      -
　　Output     Return value 1 support
							0 not support
****************************************Trigger Force*******************************************/
DLL_API int WINAPI IsSupportTriggerForce();
/**************************************Trigger Force***********************************************
    Description  This routines force capture once
　　Input:      
    Output     -
*************************************************************************************************/
DLL_API void WINAPI TriggerForce();


/********************************************Support AC/DC****************************************
	Description  This routines get the device support AC/DC switch or not.
　　Input:      -
　　Output     Return value  0 :support AC/DC switch
							 1 :not support AC/DC switch
*************************************************************************************************/
DLL_API int WINAPI IsSupportAcDc();

/********************************************Set AC/DC****************************************
	Description  This routines set the device AC coupling.
　　Input:      1 : set AC coupling
　　            0 : set DC coupling
	Output     -
*************************************************************************************************/
DLL_API void WINAPI SetAcDc(unsigned int chn, int ac);

/********************************************Get AC/DC****************************************
	Description  This routines get the device AC coupling.
　　Input:      -
　　Output     Return value  1 : AC coupling
　　                         0 : DC coupling
*************************************************************************************************/
DLL_API int WINAPI GetAcDc(unsigned int chn);


/**************************************Roll Mode********************************************
	Description  This routines get the equipment support roll mode or not .
　　Input:      -
　　Output     Return value 1 support roll mode
							0 not support roll mode
	購噐Roll Mode�歳菻�譜姥屶隔Roll Mode
******************************************************************************************/
DLL_API int WINAPI IsSupportRollMode();

/**************************************Set Roll Mode********************************************
	Description  This routines enable or disenable the equipment into roll mode.
　　Input:      -
　　Output     Return value 1 success
							0 failed
******************************************************************************************/
DLL_API int WINAPI SetRollMode(unsigned int en);


/******************************************Capture***********************************************
	Description    This routines get memory depth of equipment (KB). 
    Input:         -
    Output         memory depth of equipment
*************************************************************************************************/
DLL_API unsigned int WINAPI GetMemoryLength();

/******************************************Capture***********************************************
	Description  This routines set the capture length and start capture.
　　Input:      length  capture length(KB)
				force_length 1: force usint the length, no longer limits the maximum collection 1 seconds
    Output     Return value  the real capture length(KB)
*************************************************************************************************/
DLL_API int WINAPI Capture(int length, char force_length);


//******************************************Data Ready***********************************************
/******************************************************************************************
	Description    This routines sets the callback function of capture complete. 
    Input:       ppara        the parameter of the callback function
　　             datacallback  a pointer to a function with the following prototype:
                                 void DataReadyCallBack ( void * ppara)
    Output       -
******************************************************************************************/
typedef void (CALLBACK *DataReadyCallBack)(void* ppara);
DLL_API void WINAPI SetDataReadyCallBack(void* ppara, DataReadyCallBack datacallback);

/******************************************************************************************
	Description  This routines set the event handle, these will be set, when capture complete                   
    Input:       dataevent   the event handle
    Output      -
******************************************************************************************/
DLL_API void WINAPI SetDataReadyEvent(HANDLE dataevent);

/******************************************************************************************
	Description  This routines return the capture is complete or not.
　　Input:      -
　　Output     Return value 1 complete 
　　						0 not complete
******************************************************************************************/
DLL_API int WINAPI IsDataReady();
//******************************************Data Ready***********************************************


//******************************************Read Data***********************************************
/******************************************************************************************
	Description  This routines read the voltage datas. (V)
    Input:      channel     read channel  0 :channel 1
　　                                   1 :channel 2
　　           buffer      the buffer to store voltage datas   
　　           length      the buffer length
	Output     Return value the read length
******************************************************************************************/
DLL_API unsigned int WINAPI ReadVoltageDatas(char channel, double* buffer, unsigned int length);

/******************************************************************************************
Description  This routines return the voltage datas is out range or not.
	Input:     channel     read channel	0 :channel 1
　　									1 :channel 2
	Output     Return value 0 :not out range
　　						1 :out range
 ******************************************************************************************/
DLL_API int WINAPI IsVoltageDatasOutRange(char channel);

//******************************************Read Data***********************************************

///////////////////////////////////////////////////////////////////////////Oscilloscope///////////////////////////////////////////////////////////////////////////


//******************************************************************DDS**********************************************************************************************
/******************************************************************************************
	Description  This routines get support dds or not 
　　Input:      -
	Output     Return value support dds or not 
******************************************************************************************/
DLL_API int WINAPI IsSupportDDSDevice();

#define BX_SINE 0x00           //Sine
#define BX_SQUARE 0x01         //Square
#define BX_TRIANGULAR 0x02     //Triangular
#define BX_UP_SAWTOOTH 0x03    //Up Sawtooth
#define BX_DOWN_SAWTOOTH 0x04  //Down Sawtooth
/******************************************************************************************
　　Description  This routines get support wave styles
　　Input:     style         array to store support wave styles
　　Output     Return value  if style==NULL return number of support wave styles
                             else store the styles to array, and return number of wave style
******************************************************************************************/
DLL_API int WINAPI GetDDSSupportBoxingStyle(int* style);
/******************************************************************************************
　　Description  This routines set wave style
　　Input:      boxing   BX_SINE 0x00           //Sine
　　                    BX_SQUARE 0x01         //Square
　　                    BX_TRIANGULAR 0x02     //Triangular
　　                    BX_UP_SAWTOOTH 0x03    //Up Sawtooth
　　                    BX_DOWN_SAWTOOTH 0x04  //Down Sawtooth
　　Output:     -
******************************************************************************************/
DLL_API void WINAPI SetDDSBoxingStyle(unsigned int boxing);

/******************************************************************************************
　　Description  This routines set frequence
　　Input:       pinlv frequence
　　Output:     -
******************************************************************************************/
DLL_API void WINAPI SetDDSPinlv(unsigned int pinlv);

/******************************************************************************************
　　Description  This routines set duty cycle
　　Input:       cycle  duty cycle
	Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSDutyCycle(int cycle);

/******************************************************************************************
　　Description  This routines enable dds output or not
　　Input:       enable 1 enable 
　　                  0 not enable
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI DDSOutputEnable(int enable);

/******************************************************************************************
	Description  This routines get dds output enable or not 
　　Input:      -
　　Output     Return value dds enable or not
******************************************************************************************/
DLL_API int WINAPI IsDDSOutputEnable();


/******************************************************************************************
	Description  This routines get dds output voltage is support software control
　　Input:      -
　　Output     Return value support or not
******************************************************************************************/
DLL_API int WINAPI IsDDSSupportSoftwareControlZoomBias();

/*******************************************************************************************
	Description  This routines get the resistance min value of DDS Bias range.
　　Input:      
　　Output      Return value 0 Failed
							 other value minimum resistance 
********************************************************************************************/
DLL_API int WINAPI GetDDSBiasResistanceRangeMin();

/*******************************************************************************************
	Description  This routines get the resistance max value of DDS Bias range.
　　Input:
　　Output      Return value 0 Failed
							 other value maximum resistance
********************************************************************************************/
DLL_API int WINAPI GetDDSBiasResistanceRangeMax();

/*******************************************************************************************
	Description  This routines set the resistance value of DDS Bias.
　　Input:		value resistance
　　Output      
********************************************************************************************/
DLL_API void WINAPI SetDDSBiasResistance(int Resistance);

/*******************************************************************************************
	Description  This routines get the resistance value of DDS Bias.
　　Input:
　　Output      Return value 0 Failed
							 other value resistance
********************************************************************************************/
DLL_API int WINAPI GetDDSBiasResistance();

/*******************************************************************************************
	Description  This routines get the resistance min value of DDS Zoom range.
　　Input:
　　Output      Return value 0 Failed
							 other value minimum resistance
********************************************************************************************/
DLL_API int WINAPI GetDDSZoomResistanceRangeMin();
/*******************************************************************************************
	Description  This routines get the resistance max value of DDS Zoom range.
　　Input:
　　Output      Return value 0 Failed
							 other value maximum resistance
********************************************************************************************/
DLL_API int WINAPI GetDDSZoomResistanceRangeMax();

/*******************************************************************************************
	Description  This routines set the resistance value of DDS Zoom.
　　Input:		value resistance
　　Output      
********************************************************************************************/
DLL_API void WINAPI SetDDSZoomResistance(int Resistance);

/*******************************************************************************************
	Description  This routines get the resistance value of DDS Zoom.
　　Input:
　　Output      Return value 0 Failed
							 other value resistance
********************************************************************************************/
DLL_API int WINAPI GetDDSZoomResistance();

//******************************************************************DDS**********************************************************************************************

//******************************************************************IO**********************************************************************************************
/******************************************************************************************
	Description  This routines get support IO ctrl or not
　　Input:      -
	Output     Return value support io ctrl or not
******************************************************************************************/
DLL_API int WINAPI IsSupportIODevice();

/**************************************Support IO Num********************************************
	Description  This routines get support io nums of equipment.
	Output		Return value	the sample number of io nums
*************************************************************************************************/
DLL_API int WINAPI GetSupportIoNumber();

/**********************************callback function****************************************
	 Description    This routines sets the callback function of read io status.
	 Input:			ppara			the parameter of the callback function
　　				callback		a pointer to a function with the following prototype:
										void IOReadStateCallBack(unsigned char channel, unsigned char state)
	 Output
*******************************************************************************************/
typedef void (CALLBACK* IOReadStateCallBack)(void* ppara, unsigned char channel, char state);
DLL_API void WINAPI SetIOReadStateCallBack(void* ppara, IOReadStateCallBack callback);

/******************************************************************************************
	Description  This routines set the event handle, these will be set, when read io status complete
	Input:       dataevent   the event handle
	Output      -
******************************************************************************************/
DLL_API void WINAPI SetIOReadStateReadyEvent(HANDLE dataevent);

/******************************************************************************************
	Description  This routines return the read io status is complete or not.
　　Input:      -
　　Output     Return value 1 complete
　　						0 not complete
******************************************************************************************/
DLL_API int WINAPI IsIOReadStateReady();

/******************************************************************************************
　　Description  This routines set io in or out
　　Input:       channel  channel number
				 in 0 
				 out 1
	Output:      -
******************************************************************************************/
DLL_API void WINAPI SetIOInOut(unsigned char channel, unsigned char inout);

/******************************************************************************************
　　Description  This routines set io state
　　Input:       channel  channel number
				 state 0 or 1
	Output:      -
******************************************************************************************/
DLL_API void WINAPI SetIOState(unsigned char channel, unsigned char state);

/******************************************************************************************
　　Description  This routines get io state start 
　　Input:       channel  channel number
	Output:      -
******************************************************************************************/
DLL_API void WINAPI ReadIOState(unsigned char channel);

/******************************************************************************************
　　Description  This routines get io state of using ReadIOState to read
　　Input:       channel  channel number
	Output:      -1 not success
				 0 state
				 1 state
******************************************************************************************/
DLL_API char WINAPI GetIOState(unsigned char channel);


//******************************************************************IO**********************************************************************************************

#endif