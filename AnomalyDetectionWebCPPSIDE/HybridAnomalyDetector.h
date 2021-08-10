//Tom Magdaci 316603604


#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
private:
    bool simpleMode;
public:
	HybridAnomalyDetector();
    HybridAnomalyDetector(bool is_simpleMode);
	virtual ~HybridAnomalyDetector();
    void learnNormal(const TimeSeries &ts);
protected:
    void learnHelper(const TimeSeries& ts,float p/*pearson*/,const char* f1, const char* f2,Point** ps);
    bool isAnomalous(float x, float y,correlatedFeatures c);
    bool checkInCorrFea(const char* f, const std::vector<correlatedFeatures>& cf);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
