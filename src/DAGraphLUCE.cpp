#include "DAGraphLUCE.h"
#include "Error.h"
#include "StarNetwork.h"
#include "Derivative.h"
#include "StarNode.h"
#include "StarLink.h"

#include <limits>
#include <cmath>
#include <cassert>
#include <iostream>

LineSearch* DAGraphLUCE::lineSearch_ = nullptr;

DAGraphLUCE::DAGraphLUCE(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol,
					const int originIndex,
					LineSearch* lineSearch) : DAGraph(net, mat, zeroFlow, originIndex), 
					nbNodes_(net->getNbNodes()), nbLinks_(net->getNbLinks()), 
					nodeFlowsInv_(net->getNbNodes(), 0.0), 
					flowPortionsInv_(net->getNbLinks(), 0.0) {
	lineSearch_  = lineSearch;
}; 

DAGraphLUCE::~DAGraphLUCE() = default; 

bool DAGraphLUCE::moveFlow() {
	std::vector<FPType> C(nbNodes_, 0.0);
	std::vector<FPType> G(nbNodes_, 0.0);
	computeAvCosts(C, G);
	
	std::vector<FPType> e_links(nbLinks_, 0.0);
	computeDirection(e_links, C, G);

	FPType stepSize = 1.0; 
	stepSize = computeStepSize(e_links); 
	if (stepSize != 0.0) {
		assignNewFlow(stepSize, e_links);
	}
	return false;
}; 

void DAGraphLUCE::computeAvCosts(std::vector<FPType> &C, std::vector<FPType> &G){

	const StarLink* link = nullptr;
	FPType flowPor = 0.0;
	int linkIndex = -1;
	int nodeFromIndex = -1;
	int i = beginAscPass();
  	i = getNextAscPass(); // 跳过起点
  	for (; i != -1; i = getNextAscPass()){
  		if (nodeFlowsInv_[i] > 0.0) {
  			std::list<StarLink*> inLinks;
  			getInLinks(i, inLinks);
	      	assert(inLinks.size()); // 如果节点流量为正，则意味着存在出向链路！ (原注释有误，应指入向链路或节点有流量来源)
							      	// there are out-going links!
	      	for (const auto & inLink : inLinks){
	      		link = inLink;
	      		linkIndex = link->getIndex();
	      		nodeFromIndex = link->getNodeFromIndex();
	      		flowPor = flowPortionsInv_[linkIndex];
	      		C[i] +=  flowPor * (link->getTime() + C[nodeFromIndex]);
	      		G[i] += flowPor * flowPor * (link->getDerivative() + G[nodeFromIndex]);
	      		if (G[i] != G[i]) {
	      			std::cout << "Flow is positive G[i] = " << G[i] << " flowPor = " << flowPor << 
	      			" der = " << link->getDerivative() << " G_prev = " <<  G[nodeFromIndex] << 
	      			" from = " << nodeFromIndex << std::endl;
	      			throw Error("Something gone wrong in LUCE");
	      		}
	      	}
	      } else {

	      	std::list<StarLink*> inLinks;

	      	getInLinks(i, inLinks);
		    if (inLinks.empty()) { //可能只对起点成立
		    	C[i] = 0.0;
		    	G[i] = 0.0;
		    } else {
		    	FPType minVal = std::numeric_limits<FPType>::infinity();
		    	FPType val = 0.0;
		    	FPType sum1 = 0.0;
		    	FPType sum2 = 0.0;
		    	for (std::list<StarLink*>::const_iterator it = inLinks.begin();
		    			it != inLinks.end(); ++it){
		    		link = *it;
		    		linkIndex = link->getIndex();
		    		nodeFromIndex = link->getNodeFromIndex();
		    		val = link->getTime() + C[nodeFromIndex];
		    		if (val < minVal) minVal = val;
		    		sum1 += val * (link->getDerivative() + G[nodeFromIndex]);
		    		sum2 += val;
		    	}
		    	C[i] = minVal;
		    	if (sum2 == 0.0) {
			  		G[i] = 0.0; // TODO: 仔细考虑这种情况 - 再次出现零导数
			  	} else {
			  		G[i] = sum1 / sum2;
			  	}

			  	if (G[i] != G[i]) {
			  		std::cout << "Flow is zero G[i] = " << G[i] << " sum1 = " 
			  		<< sum1 << " sum2 = " << sum2 << std::endl;
			  		throw Error("Something gone wrong in LUCE");
			  	}
			}

		}
	}
};

void DAGraphLUCE::computeDirection(std::vector<FPType> &e_links, const std::vector<FPType> &C, 
									const std::vector<FPType> &G){

	// 加载需求
	std::vector<FPType> e_nodes(nbNodes_, 0.0);
	std::vector<FPType> V(nbNodes_, 0.0);
	for (int i =  beginAscPass(); i != -1; i = getNextAscPass()) {
		e_nodes[i] = getDemand(i);
	}

	bool lambda = false;
	FPType sum1 = 0.0;
	FPType sum2 = 0.0;
	FPType tmp = 0.0;
	const StarLink *link = nullptr;
	int nodeFromIndex = -1;
	int linkIndex = -1;
	std::list<StarLink*> inLinks;
	const int originIndex = getOriginIndex();
	for (int i =  beginDescPass(); i != -1; i = getNextDescPass()) {
   		if (i != originIndex) { // 如果不是起点
	    	inLinks = getInLinksCopy(i);
	    	if (e_nodes[i] > zeroFlow_) {
	    		assert(!inLinks.empty());
	    		lambda = true;

	    		while (lambda) {
	    			lambda = false;
	    			sum1 = 0.0;
	    			sum2 = 0.0;
	    			StarLink *link = nullptr;
	    			for (std::list<StarLink*>::const_iterator it = inLinks.begin();
	    					it != inLinks.end(); ++it){
	    				link = *it;
	    				nodeFromIndex = link->getNodeFromIndex();
	    				linkIndex = link->getIndex();
	    				tmp = link->getDerivative() + G[nodeFromIndex];
	    				if (fabs(tmp) <= zeroFlow_){ 
		      				// TODO: 仔细考虑这种情况
	    					tmp = 1.0;
	    				}
	    				sum1 += (link->getTime() +  C[nodeFromIndex]) / tmp - e_nodes[i] * 
	    						flowPortionsInv_[linkIndex];
	    				sum2 += 1.0 / tmp;

	    			}

	    			V[i] = (e_nodes[i] + sum1) / sum2;
	    			
	    			unsigned int count = 0;
	    			FPType sumDir = 0.0;
	    			for (auto it = inLinks.begin();
	    					it != inLinks.end(); ++it){
	    				++count;
	    				
	    				link = *it;
	    				linkIndex = link->getIndex();
	    				nodeFromIndex = link->getNodeFromIndex();
	    				tmp = link->getDerivative() + G[nodeFromIndex];
	    				if (fabs(tmp) <= zeroFlow_){
		      				// TODO: 仔细考虑这种情况
	    					tmp = 1.0;
	    				}  
	    				if (count < inLinks.size()) { 
	    					e_links[linkIndex] = (V[i] - (link->getTime() + 
		    					C[nodeFromIndex])) / tmp + e_nodes[i] * flowPortionsInv_[linkIndex];
	    					sumDir += e_links[linkIndex];
	    				} else {
	    					e_links[linkIndex] = e_nodes[i] - sumDir; // 这允许保持流量可行性。
	    					// 可行性。然而，下降方向不正确，无法达到高精度。
	    					// precision cannot be achieved
	    				}	

	    				if (e_links[linkIndex] < 0.0) {
	    					e_links[linkIndex] = 0.0;
	    					it = inLinks.erase(it); 
	    					--it;
	    					lambda = true;
	    				} 
			    			
		    				
		    			}

	    		}

	    		for (const auto & inLink : inLinks){
	    			link = inLink;
	    			e_nodes[link->getNodeFromIndex()] += e_links[link->getIndex()];
	    		}
	    	} else {
				// 重置为零
	    		for (const auto & inLink : inLinks){
	    			link = inLink;
	    			e_links[link->getIndex()] = 0.0;
	    		}
    		}	
    	}
	}
};

FPType DAGraphLUCE::computeStepSize(const std::vector<FPType> &e_links){
	
	int linkIndex = -1;
	FPType x[nbLinks_];
	FPType y[nbLinks_];
	int indexes[nbLinks_];
	int size = 0;
	StarLink *link = nullptr;
	for (int i =  beginAscPass(); i != -1; i = getNextAscPass()) {
		std::list<StarLink*> outLinks;
		getOutLinks(i, outLinks);
		for (std::list<StarLink*>::const_iterator it = outLinks.begin(); it != outLinks.end(); ++it){
			link = *it;

			linkIndex = link->getIndex();
			y[linkIndex] = e_links[linkIndex] - getOriginFlow(linkIndex);
			x[linkIndex] = link->getFlow();
			indexes[size] = linkIndex;
			++size;
		}
	}
	
	(lineSearch_->getDerivative())->setDataPointers(size, x, y, indexes);
	return lineSearch_->execute(0.0, 1.0);;
	
};

void DAGraphLUCE::assignNewFlow(FPType stepSize, const std::vector<FPType> &e_links){
	StarLink *link = nullptr;
	int linkIndex = -1;
	FPType dFlow = 0.0;
	for (int i =  beginAscPass(); i != -1; i = getNextAscPass()) {
		std::list<StarLink*> outLinks;
		getOutLinks(i, outLinks);
		for (std::list<StarLink*>::const_iterator it = outLinks.begin(); it != outLinks.end(); ++it){
			link = *it;
			linkIndex = link->getIndex();
			dFlow = stepSize * (e_links[linkIndex] - getOriginFlow(linkIndex));
			if (fabs(dFlow) >= zeroFlow_) {
				if (dFlow + link->getFlow() <= zeroFlow_) {
					link->setFlow(0.0);
					setOriginFlow(linkIndex, 0.0);
				} else {
					addOriginFlow(linkIndex, dFlow);
					link->addFlow(dFlow);
				}
				link->updateTime();
			}

		}
	}
};

void DAGraphLUCE::prepareData(){

    // 将 nodeFlows 设置为零
	for (int j = 0; j < nbNodes_; ++j){
		nodeFlowsInv_[j] = 0.0;
	}

  	// 计算节点流量 - 针对当前BUSH（丛）
	FPType nodeFlow = 0.0;
	StarLink* link = nullptr;
	for (int i =  beginDescPass(); i != -1; i = getNextDescPass()) {
		std::list<StarLink*> inLinks;
		getInLinks(i, inLinks);
		nodeFlow = 0.0;
		for (std::list<StarLink*>::const_iterator it = inLinks.begin(); it != inLinks.end(); ++it){
			link = *it;
			nodeFlow += getOriginFlow(link->getIndex());
		}
		nodeFlowsInv_[i] = nodeFlow;
	}
  	calcFlowPortions();
};

void DAGraphLUCE::calcFlowPortions(){

	const StarLink *link = nullptr;
	FPType nodeFlow = 0.0;
	int index = -1;
	for (int i =  beginDescPass(); i != -1; i = getNextDescPass()) {
		std::list<StarLink*> inLinks;
		getInLinks(i, inLinks);
		nodeFlow =  nodeFlowsInv_[i];
		for (std::list<StarLink*>::const_iterator it = inLinks.begin(); it != inLinks.end(); ++it){
			link = *it;
			index = link->getIndex();
			if (nodeFlow > 0.0) {
				flowPortionsInv_[index] = getOriginFlow(index) / nodeFlow;
			} else {
				flowPortionsInv_[index] = 0.0;
			}
		}
	}

};