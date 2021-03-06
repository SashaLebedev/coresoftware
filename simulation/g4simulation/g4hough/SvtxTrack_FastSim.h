/*
 * SvtxTrack_FastSim.h
 *
 *  Created on: Jul 28, 2016
 *      Author: yuhw
 */

#ifndef __SVTXTRACK_FAST_SIM_H__
#define __SVTXTRACK_FAST_SIM_H__

#include "SvtxTrack_v1.h"

class SvtxTrack_FastSim : public SvtxTrack_v1 {

public:
	SvtxTrack_FastSim();
	virtual ~SvtxTrack_FastSim();

	unsigned int get_truth_track_id() const {
		return _truth_track_id;
	}

	void set_truth_track_id(unsigned int truthTrackId) {
		_truth_track_id = truthTrackId;
	}

	// The "standard PHObject response" functions...
	void identify(std::ostream &os=std::cout) const;
	void Reset() {*this = SvtxTrack_FastSim();}
	int  isValid() const;
	SvtxTrack* Clone() const {return new SvtxTrack_FastSim(*this);}

private:

	unsigned int _truth_track_id;


	ClassDef(SvtxTrack_FastSim,1)
};

#endif /* __SVTXTRACK_FAST_SIM_H__ */
