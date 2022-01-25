import ctypes
#from ctypes import CDLL ## for 64bit code
#from ctypes import WinDLL
from ctypes import *

import os
## python 64bit load this
os.add_dll_directory("D:\github-SDK\SharedLibrary\Windows\X64\Release")
#os.environ["PATH"] += ";D:\github-SDK\windows\Dll\X64"
## python 32bit load this
#os.add_dll_directory("D:\github-SDK\windows\Dll\Win32")
#os.environ["PATH"] += ";D:\github-SDK\windows\Dll\Win32"
#print ('path: ', os.environ["PATH"])


## load library   __stdcall using  windll
mdll = ctypes.WinDLL("VDSO.dll")

############################ Initialization/Finished Dll ##############################
## init Dll
fInitDll = mdll.InitDll
fInitDll.restype = ctypes.c_int
## finish Dll
fFinishDll = mdll.FinishDll  
fFinishDll.restype = ctypes.c_int
############################ Initialization/Finished Dll ##############################

########################### Device ##############################
## get equipment information id0
fGetOnlyId0 = mdll.GetOnlyId0
fGetOnlyId0.restype = ctypes.c_uint
## get equipment information id1
fGetOnlyId1 = mdll.GetOnlyId1
fGetOnlyId1.restype = ctypes.c_uint
## reset device
fResetDevice = mdll.ResetDevice
fResetDevice.restype = ctypes.c_int
############################ Device ##############################

############################ USB status ##############################
fSetDevNoticeCallBack = mdll.SetDevNoticeCallBack
#fSetDevNoticeCallBack.restype = ctypes.void

## check instrument connection
fIsDevAvailable = mdll.IsDevAvailable
fIsDevAvailable.restype = ctypes.c_int
############################ USB status ##############################

############################ Oscilloscope ##############################
## capture range set
fSetOscChannelRange = mdll.SetOscChannelRange
fSetOscChannelRange.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
fSetOscChannelRange.restype = ctypes.c_int

## get coupling mode
fGetAcDc = mdll.GetAcDc
fGetAcDc.argtypes = [ctypes.c_uint]
fGetAcDc.restype = ctypes.c_int

## get sample num
fGetOscSupportSampleNum = mdll.GetOscSupportSampleNum
fGetOscSupportSampleNum.restype = ctypes.c_int

## get support sample num
fGetOscSupportSamples = mdll.GetOscSupportSamples
fGetOscSupportSamples.restype = ctypes.c_int

## set sample
fSetOscSupportSamples = mdll.SetOscSample
fSetOscSupportSamples.restype = ctypes.c_uint
############################ Oscilloscope ##############################

############################ Capture ##############################
## get Memory Length
fGetMemoryLength = mdll.GetMemoryLength
fGetMemoryLength.restype = ctypes.c_uint

## Capture
fCapture = mdll.Capture
fCapture.restype = ctypes.c_int
############################ Capture ##############################

############################ Data Ready ##############################
fSetDataReadyCallBack = mdll.SetDataReadyCallBack
############################ Data Ready ##############################

############################ Read Data ##############################
fReadVoltageDatas = mdll.ReadVoltageDatas
fReadVoltageDatas.restype = ctypes.c_uint
############################ Read Data ##############################





## init Dll
print ('## Init Dll: ', fInitDll())

##Data Revice
@ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p)
def DevDataReadyCallBack_func(p):
    print('## DevDataReadyCallBack_func', p)
    
    totallength = fGetMemoryLength()*1024;
    arraytypedouble = ctypes.c_double * totallength
    datas = arraytypedouble()
    num = fReadVoltageDatas(ctypes.c_char(0), datas, totallength);
    minv = datas[0];
    maxv = datas[0];
    for index in range(num):
        if(datas[index]<minv):
            minv = datas[index];
        if(datas[index]>maxv):
            maxv = datas[index];
    print('## fReadVoltageDatas',num)
    print(' minv',minv)
    print(' maxv',maxv)

    #Next Capture
    length = fGetMemoryLength();
    fCapture(length);
    return 0
    
##USB status
@ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p)
def DevNoticeAddCallBack_func(p):
    print('DevNoticeAddCallBack_func', p)
    
    ## test not used
    print ('## connection ok:', fIsDevAvailable())
    
    ## onyid
    print ('## ID0: ', fGetOnlyId0())
    print ('## ID1: ', fGetOnlyId1())

    ## acdc
    print ('## ch0 coupling type ', fGetAcDc(0))
    print ('## ch1 coupling type ', fGetAcDc(1))
    
    ## set range to +/- 5000mV
    print ('## ch0 range set: ', fSetOscChannelRange(0, -5000, 5000))
    print ('## ch1 range set: ', fSetOscChannelRange(1, -5000, 5000))
    
    ## Sample
    samplenum = fGetOscSupportSampleNum();
    print ('## support sample number: ', samplenum)
    arraytype = ctypes.c_uint * samplenum
    samples = arraytype()
    fGetOscSupportSamples(samples, samplenum)
    for s in samples: 
        print(s)
        
    fSetOscSupportSamples(samples[samplenum-1]);
    
    length = fGetMemoryLength();
    print ('## MemoryLength ', length*1024)
    
    para = ctypes.c_void_p(2000)
    fSetDataReadyCallBack(para, DevDataReadyCallBack_func);
    fCapture(length);
    
    return 0

@ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p)
def DevNoticeRemoveCallBack_func(p):
    print('## DevNoticeRemoveCallBack_func', p)
    ## finish Dll
    print ('## Finish Dll: ', fFinishDll())
    return 0

#1000 is just test
para = ctypes.c_void_p(1000)
fSetDevNoticeCallBack(para, DevNoticeAddCallBack_func, DevNoticeRemoveCallBack_func)









