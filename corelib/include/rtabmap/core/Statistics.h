/*
 * Copyright (C) 2010-2011, Mathieu Labbe and IntRoLab - Universite de Sherbrooke
 *
 * This file is part of RTAB-Map.
 *
 * RTAB-Map is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RTAB-Map is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RTAB-Map.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include "rtabmap/core/RtabmapExp.h" // DLL export/import defines

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <list>
#include <vector>

namespace rtabmap {

#define RTABMAP_STATS(PREFIX, NAME, UNIT) \
	public: \
		static std::string k##PREFIX##NAME() {return #PREFIX "/" #NAME "/" #UNIT;} \
	private: \
		class Dummy##PREFIX##NAME { \
		public: \
			Dummy##PREFIX##NAME() {if(!_defaultDataInitialized)_defaultData.insert(std::pair<std::string, float>(#PREFIX "/" #NAME "/" #UNIT, 0.0f));} \
		}; \
		Dummy##PREFIX##NAME dummy##PREFIX##NAME;

class RTABMAP_EXP Statistics
{
	RTABMAP_STATS(Loop, RejectedHypothesis,);
	RTABMAP_STATS(Loop, Highest_hypothesis_id,);
	RTABMAP_STATS(Loop, Highest_hypothesis_value,);
	RTABMAP_STATS(Loop, Vp_hypothesis,);
	RTABMAP_STATS(Loop, ReactivateId,);
	RTABMAP_STATS(Loop, Hypothesis_ratio,);

	RTABMAP_STATS(Memory, Working_memory_size,);
	RTABMAP_STATS(Memory, Short_time_memory_size,);
	RTABMAP_STATS(Memory, Signatures_removed,);
	RTABMAP_STATS(Memory, Signatures_retrieved,);
	RTABMAP_STATS(Memory, Images_buffered,);

	RTABMAP_STATS(Timing, Memory_update, ms);
	RTABMAP_STATS(Timing, Cleaning_neighbors, ms);
	RTABMAP_STATS(Timing, Reactivation, ms);
	RTABMAP_STATS(Timing, Add_loop_closure_link, ms);
	RTABMAP_STATS(Timing, Likelihood_computation, ms);
	RTABMAP_STATS(Timing, Posterior_computation, ms);
	RTABMAP_STATS(Timing, Hypotheses_creation, ms);
	RTABMAP_STATS(Timing, Hypotheses_validation, ms);
	RTABMAP_STATS(Timing, Statistics_creation, ms);
	RTABMAP_STATS(Timing, Memory_cleanup, ms);
	RTABMAP_STATS(Timing, Total, ms);
	RTABMAP_STATS(Timing, Forgetting, ms);
	RTABMAP_STATS(Timing, Joining_trash, ms);
	RTABMAP_STATS(Timing, Emptying_trash, ms);

	RTABMAP_STATS(, Hypothesis_reactivated,);

	RTABMAP_STATS(Keypoint, Dictionary_size, words);
	RTABMAP_STATS(Keypoint, Response_threshold,);

public:
	static const std::map<std::string, float> & defaultData();

public:
	Statistics();
	virtual ~Statistics();

	// name format = "Grp/Name/unit"
	void addStatistic(const std::string & name, float value);

	// setters
	void setExtended(bool extended) {_extended = extended;}
	void setRefImageId(int refImageId) {_refImageId = refImageId;}
	void setLoopClosureId(int loopClosureId) {_loopClosureId = loopClosureId;}
	void setLocalLoopClosureId(int localLoopClosureId) {_localLoopClosureId = localLoopClosureId;}
	void setRefImage(const cv::Mat & image);
	void setLoopImage(const cv::Mat & image);
	void setWeights(const std::map<int, int> & weights) {_weights = weights;}
	void setPosterior(const std::map<int, float> & posterior) {_posterior = posterior;}
	void setLikelihood(const std::map<int, float> & likelihood) {_likelihood = likelihood;}
	void setRawLikelihood(const std::map<int, float> & rawLikelihood) {_rawLikelihood = rawLikelihood;}
	void setRefWords(const std::multimap<int, cv::KeyPoint> & refWords) {_refWords = refWords;}
	void setLoopWords(const std::multimap<int, cv::KeyPoint> & loopWords) {_loopWords = loopWords;}

	// getters
	bool extended() const {return _extended;}
	int refImageId() const {return _refImageId;}
	int loopClosureId() const {return _loopClosureId;}
	int localLoopClosureId() const {return _localLoopClosureId;}
	const cv::Mat & refImage() const {return _refImage;}
	const cv::Mat & loopImage() const {return _loopImage;}
	const std::map<int, int> & weights() const {return _weights;}
	const std::map<int, float> & posterior() const {return _posterior;}
	const std::map<int, float> & likelihood() const {return _likelihood;}
	const std::map<int, float> & rawLikelihood() const {return _rawLikelihood;}
	const std::multimap<int, cv::KeyPoint> & refWords() const {return _refWords;}
	const std::multimap<int, cv::KeyPoint> & loopWords() const {return _loopWords;}

	const std::map<std::string, float> & data() const {return _data;}

private:
	int _extended; // 0 -> only loop closure and last signature ID fields are filled

	int _refImageId;
	int _loopClosureId;
	int _localLoopClosureId; // Note: used by VSLAM

	// extended data start here...
	cv::Mat _refImage;
	cv::Mat _loopImage;

	std::map<int, int> _weights;
	std::map<int, float> _posterior;
	std::map<int, float> _likelihood;
	std::map<int, float> _rawLikelihood;

	//keypoint memory
	std::multimap<int, cv::KeyPoint> _refWords;
	std::multimap<int, cv::KeyPoint> _loopWords;

	// Format for statistics (Plottable statistics must go in that map) :
	// {"Group/Name/Unit", value}
	// Example : {"Timing/Total time/ms", 500.0f}
	std::map<std::string, float> _data;
	static std::map<std::string, float> _defaultData;
	static bool _defaultDataInitialized;
	// end extended data
};

}// end namespace rtabmap

#endif /* STATISTICS_H_ */
