/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SA.cpp
 * Author: BrunoVinicius
 * 
 * Created on November 22, 2018, 11:36 PM
 */

#include "../../include/ResourceAllocation/SA.h"
#include "../../include/ResourceAllocation/CSA.h"
#include "../../include/Calls/Call.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/Structure/Topology.h"
#include "../../include/GeneralClasses/Def.h"
#include "../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../include/Structure/Node.h"

SA::SA(ResourceAlloc* rsa, SpectrumAllocationOption option, Topology* topology) 
:resourceAlloc(rsa), specAllOption(option), topology(topology) {
    
}

SA::~SA() {
    
}

void SA::SpecAllocation(Call* call) {
    switch(this->specAllOption){
        case SpecAllRandom:
            this->Random(call);
            break;
        case SpecAllFF:
            this->FirstFit(call);
            break;
        case SpecAllMSCL:
            this->MSCL(call);
            break;
        default:
            std::cerr << "Invalid spectrum allocation option" << std::endl;
    }
}

std::vector<unsigned int> SA::SpecAllocation(unsigned int lastSlot) {
    std::vector<unsigned int> vec(0);
    
    switch(this->specAllOption){
        case SpecAllRandom:
            return this->RandomSlots(lastSlot);
        case SpecAllFF:
            return this->FirstFitSlots(lastSlot);
        default:
            std::cerr << "Invalid spectrum allocation option" << std::endl;
            return vec;
    }
}

void SA::Random(Call* call) {
    std::vector<unsigned int> vecSlots(0);
    
    vecSlots = this->FirstFitSlots(call);
    std::shuffle(vecSlots.begin(), vecSlots.end(), Def::pseudoRandomEngine);
    
    call->SetFirstSlot(vecSlots.back());
    call->SetLastSlot(vecSlots.back() + call->GetNumberSlots() - 1);
}

void SA::FirstFit(Call* call) {
    Route* route = call->GetRoute();
    unsigned int numSlotsReq = call->GetNumberSlots();
    unsigned int maxSlotIndex = this->topology->GetNumSlots() - 
                                 numSlotsReq;
    
    for(unsigned int slot = 0; slot <= maxSlotIndex; slot++){
        
        if(this->topology->CheckSlotsDisp(route, slot, slot + numSlotsReq - 1)){
            call->SetFirstSlot(slot);
            call->SetLastSlot(slot + numSlotsReq - 1);
            break;
        }
    }
}

void SA::MSCL(Call* call) {
    unsigned int numSlotsReq = call->GetNumberSlots();
    unsigned int numSlotsTop = this->topology->GetNumSlots();
    Route* route = call->GetRoute();
    unsigned int orNode = route->GetOrNode()->GetNodeId();
    unsigned int deNode = route->GetDeNode()->GetNodeId();
    std::vector<std::shared_ptr<Route>> intRoutes = 
    this->resourceAlloc->GetInterRoutes(orNode, deNode, route);
    
    unsigned int vetCapInic[intRoutes.size()];
    unsigned int vetCapFin[intRoutes.size()];
    bool vetDispInt[numSlotsTop];
    double perda, perdaMin = Def::Max_Double;
    unsigned int si = Def::Max_UnInt;
    bool DispFitSi = false;
    
    for(unsigned int s = 0; s < (numSlotsTop - numSlotsReq + 1); s++){
        DispFitSi = this->topology->CheckSlotsDisp(route, s, 
                                                   s + numSlotsReq - 1);
        
        if(DispFitSi){
            perda = 0.0;
            
            for(unsigned int r = 0; r < intRoutes.size(); r++){
                
                for(unsigned int se = 0; se < numSlotsTop; se++){
                    if(!(this->topology->CheckSlotDisp(intRoutes.at(r).get(), 
                                                       se))){
                        vetDispInt[se] = false;
                    }
                    else{
                        vetDispInt[se] = true;
                    }
                }
                
                vetCapInic[r] = 0;
                vetCapFin[r] = 0;
                
                //Calculates the initial capacity based on the number of 
                //allocation forms.
                for(unsigned int i = 2; i <= 5; i++)
                    vetCapInic[r] += this->CalcNumFormAloc(i, vetDispInt);
                
                //Calculates the requisition allocation impact in the 
                //interfering routes for each set of slots
                for(unsigned int i = s; i < s + numSlotsReq; i++)
                    vetDispInt[i] = false;
                
                for(unsigned int i = 2; i <= 5; i++)
                    vetCapFin[r] += this->CalcNumFormAloc(i, vetDispInt);
                
                perda += vetCapFin[r] - vetCapInic[r];
            }
            
            if(perda < perdaMin){
                perdaMin = perda;
                si = s;
            }
        }
    }
    
    if(perdaMin < Def::Max_Double){
        call->SetFirstSlot(si);
        call->SetLastSlot(si + numSlotsReq - 1);
    }
}

Topology* SA::GetTopology() {
    return topology;
}

unsigned int SA::CalcNumFormAloc(unsigned int reqSize, bool* dispVec) {
    unsigned int sum = 0;
    unsigned int si, se;
    unsigned int numNodes = this->topology->GetNumNodes();
    
    for(si = 0; si < numNodes - reqSize; si++){
        for(se = si; se < si + reqSize; se++){
            
            if(dispVec[se] == false)
                break;
        }
        
        if(se == si + reqSize)
            sum++;
    }
    
    return sum;
}

int SA::CalcNumFormAloc(int L, bool* Disp,int tam) { 
//L indica a largura da requisicao e Disp o vetor de disponibilidade
/*
	int sum = 0, si, se; //si eh o slot inicial da alocacao, que vai de 0 ate SE-L
	for(si = 0; si <= tam-L; si++){
		for(se = si; se < si+L; se++) //se checa se todos os slots de si ate si+L-1 estao disponiveis
			if(Disp[se] == false)
				break;
		if(se == si+L) // Os slots si,si+1,...,si+Lf-1 estao disponiveis
			sum++;
	}
	return sum;
}
*/
    int sum = 0, si;//si eh o slot inicial da alocacao, que vai de 0 ate SE-L
    int cont = 0;
    for(si = 0; si < tam; si++){
        if(Disp[si] == false){
            if(cont >= L){
                 sum += cont - L + 1;
                 //sum += floor (cont/L);
            }
                cont = 0;
        }
            else{
                cont++;
            }
    }
    if(cont >= L){
       sum += cont - L + 1;
    }
    return sum;
}

ResourceAlloc* SA::GetResourceAlloc(){
    return this->resourceAlloc;
}

std::vector<unsigned int> SA::RandomSlots(Call* call) {
    std::vector<unsigned int> vecSlots(0);
    vecSlots = this->FirstFitSlots(call);
    
    std::shuffle(vecSlots.begin(), vecSlots.end(), Def::pseudoRandomEngine);
    
    return vecSlots;
}

std::vector<unsigned int> SA::RandomSlots(unsigned int lastSlot) {
    std::vector<unsigned int> vecSlots = this->FirstFitSlots(lastSlot);
    
    std::shuffle(vecSlots.begin(), vecSlots.end(), Def::pseudoRandomEngine);
    
    return vecSlots;
}

std::vector<unsigned int> SA::FirstFitSlots(Call* call) {
    Route* route = call->GetRoute();
    unsigned int numSlotsReq = call->GetNumberSlots();
    unsigned int maxSlotIndex = this->topology->GetNumSlots() - numSlotsReq;
    std::vector<unsigned int> slots(0);
    
    for(unsigned int a = 0; a <= maxSlotIndex; a++){
        if(this->topology->CheckSlotsDisp(route, a, a + numSlotsReq - 1)){
            slots.push_back(a);
        }
    }
    
    return slots;
}

std::vector<unsigned int> SA::FirstFitSlots(unsigned int lastSlot) {
    std::vector<unsigned int> vecSlots(0);
    
    for(unsigned int a = 0; a <= lastSlot; a++)
        vecSlots.push_back(a);
    
    return vecSlots;
}
