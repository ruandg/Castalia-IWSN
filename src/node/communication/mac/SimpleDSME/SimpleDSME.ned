//****************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2010                         *
//*  Developed at the ATP lab, Networked Systems research theme              *
//*  Author(s): Yuriy Tselishchev                                            *
//*  This file is distributed under the terms in the attached LICENSE file.  *
//*  If you do not find this file, copies can be found by writing to:        *
//*                                                                          *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
//*      Attention:  License Inquiry.                                        *
//*                                                                          *  
//****************************************************************************/

package node.communication.mac.SimpleDSME;

simple SimpleDSME like node.communication.mac.iMac {
 parameters:
	bool collectTraceInfo = default (false);
	int macMaxPacketSize = default (0);		//BypassMac does not limit packet size
	int macPacketOverhead = default (8);	//only 2 fields are added in simple MAC frame, 8 bytes in total
	int macBufferSize = default (1);		//BypassMac does not use buffers
	string allocatedSlots = default ("");	
	int numberOfAttempts = default (1);
	int superFrameOrder = default (3);
	int multiSuperFrameOrder = default (3);
	int beaconOrder = default (3);
	bool isCoordinator = default (false);
	int channelDiversity = default (0); //0 -> Frequency Hopping 1 -> Channel Adaptation 2 -> H-DSME
	string nodeIdSlot = default ("1111111111111111");
	string gackSlots = default ("");
	double prrThreshold = default(0.5);
	

 gates:
	output toNetworkModule;
	output toRadioModule;
	input fromNetworkModule;
	input fromRadioModule;
	input fromCommModuleResourceMgr;
}

