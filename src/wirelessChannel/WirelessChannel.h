/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *  
 ****************************************************************************/

#ifndef _WIRELESSCHANNEL_H
#define _WIRELESSCHANNEL_H

#include "WirelessChannelMessages_m.h"
#include "WirelessChannelTemporal.h"
#include "VirtualMobilityManager.h"
#include "CastaliaModule.h"

#include <list>

#include "stdlib.h"
#include "math.h"
#include "time.h"
#include "Radio.h"
#include <itpp/itbase.h>
#include <itpp/comm/channel.h>
#include <random>
#include <cstdlib>
#include <chrono>
#include <string.h>
#include <iostream>
#include <complex>

using namespace std;
using namespace itpp;

#define CHANGE_PARAMETERS 99

class PathLossElement {
 public:
	int cellID;
	float avgPathLoss[16]; //now it is different avgPathLoss for each channel
	float lastObservedDiffFromAvgPathLoss;
	simtime_t lastObservationTime;
	int channel;

	PathLossElement(int c, float PL) {
		cellID = c;
		for(int i = 0; i < 16; i++) //Initializing all the channels with the same value
			avgPathLoss[i] = PL;
		lastObservedDiffFromAvgPathLoss = PL;
		lastObservationTime = 0.0;
	};

	~PathLossElement() {
	};
};

class WirelessChannel: public CastaliaModule {
 private:

	/*--- variables corresponding to .ned file's parameters ---*/
	int numOfNodes;

	double xFieldSize;
	double yFieldSize;
	double zFieldSize;
	double xCellSize;
	double yCellSize;
	double zCellSize;

	// variables corresponding to Wireless Channel module parameters
	double pathLossExponent;	// the path loss exponent
	double noiseFloor;			// in dBm
	double PLd0;				// Power loss at a reference distance d0 (in dBm)
	double d0;					// reference distance (in meters)
	double sigma;				// std of a zero-mean Gaussian RV
	double bidirectionalSigma;	// std of a zero-mean Gaussian RV

	const char *pathLossMapFile;
	const char *temporalModelParametersFile;
	double signalDeliveryThreshold;
	bool onlyStaticNodes;
	double receiverSensitivity;
	double maxTxPower;			// this is derived, by reading all the Tx power levels

	/*--- other class member variables ---*/
	int numOfXCells, numOfYCells, numOfZCells;
	int numOfSpaceCells;
	int xIndexIncrement, yIndexIncrement, zIndexIncrement;

	list <PathLossElement*>*pathLoss;		// an array of lists (numOfSpaceCels long)
											// holding info on path loss. Element i of the
											// array is a list elements that describe which
											// cells are affected (and how) when a
											// node in cell i transmits.

	list <int>*nodesAffectedByTransmitter;	// an array of lists (numOfNodes long). The list
											// at array element i holds the node IDs that are
											// affected when node i transmits.

	list <int>*cellOccupation;				// an array of lists (numOfSpaceCels long) that
											// tells us which nodes are in cell i.

	NodeLocation_type *nodeLocation;		// an array (numOfNodes long) that gives the
											// location for each node.

	bool temporalModelDefined;
	channelTemporalModel *temporalModel;
	
	/*--- variables corresponding to the new channel model---*/
	double *varrice; 
	double *meanrice;
	
	double meanTimeChange;
	double probabilityChange;
	simtime_t timeAnt, currentTime;
	double k, k_sigma;
	
	int seed;
	
	std::mt19937 gen;
	
 protected:
	virtual void initialize(int);
	virtual void handleMessage(cMessage * msg);
	virtual void finishSpecific();

	void readIniFileParameters(void);
	void parsePathLossMap(void);
	int parseInt(const char *, int *);
	int parseFloat(const char *, float *);
	void printRxSignalTable(void);
	void updatePathLossElement(int, int, float);
	float calculateProb(float, int);

	int numInitStages() const;
    
    bool updateChannel(double);
    void updatePathLossElement2(int, int, float, int);
	void generatePowerFading(int,int);
};

#endif				//_WIRELESSCHANNEL_H
