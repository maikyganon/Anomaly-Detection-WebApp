//Tom Magdaci 316603604

#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector(): SimpleAnomalyDetector() {
	// TODO Auto-generated constructor stub
	simpleMode=false;

}

HybridAnomalyDetector::HybridAnomalyDetector(bool is_simpleMode): SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub
    simpleMode=is_simpleMode;

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

bool HybridAnomalyDetector::checkInCorrFea(const char* f, const std::vector<correlatedFeatures>& cf) {
    if (cf.empty()) {
        return false;
    }
    std::vector<correlatedFeatures>::const_iterator it;
    for(it = cf.begin(); it != cf.end(); it++) {
        if ((strcmp(it->feature1.c_str(), f) == 0) || strcmp(it->feature2.c_str(), f) == 0) {
            return true;
        }
    }
    return false;
}



void HybridAnomalyDetector::learnHelper(const TimeSeries& ts,float p/*pearson*/,const char* f1, const char* f2,Point** ps){
    int psSize = ts.getFeatureByName(f1).getVecFloatsSize();
    if(p>threshold && !(checkInCorrFea(f1, this->getNormalModel()))){
        Circle minC = findMinCircle(ps,psSize);
        correlatedFeatures c;
        c.feature1=f1;
        c.feature2=f2;
        c.corrlation=p;
        c.lin_reg;
        c.threshold=minC.radius*1.1; // 10% increase
        c.center= new Point(minC.center.x,minC.center.y);
        cf.push_back(c);
    }
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    SimpleAnomalyDetector* sAD = new SimpleAnomalyDetector();
    sAD->learnNormal(ts);
    vector<correlatedFeatures> cf1 = sAD->getNormalModel();
    this->cf.insert(this->cf.end(),cf1.begin(),cf1.end());
    if (this->simpleMode==false) {
        setCorrelationThreshold(0.5);
        SimpleAnomalyDetector::learnNormal(ts);
    }
    delete sAD;
}

bool HybridAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c){
    if (c.center == nullptr) {
        return (abs(y - c.lin_reg.f(x)) > c.threshold);
    } else {
        return (distanceBetweenTwoPoints(*(c.center),*(new Point(x,y))) > c.threshold); //c.threshold = radious
    }
}

