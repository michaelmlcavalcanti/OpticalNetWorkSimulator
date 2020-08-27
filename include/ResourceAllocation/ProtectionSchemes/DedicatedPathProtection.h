/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DedicatedPathProtection.h
 * Author: henrique
 *
 * Created on October 26, 2019, 9:36 AM
 */

#ifndef DEDICATEDPATHPROTECTION_H
#define DEDICATEDPATHPROTECTION_H

class Call;


#include "ProtectionScheme.h"



class DedicatedPathProtection : public ProtectionScheme {
public:
    
    DedicatedPathProtection(ResourceDeviceAlloc* rsa);

    virtual ~DedicatedPathProtection();
    
    
      /**
     * @brief Function which creates protection disjoint routes for each 
     * working route defined by routing algorithm and stored in allRoutes vector.
     */
    void CreateProtectionRoutes() override;
      /**
     * @brief Function which create a container of calls for working and backup
     * lightpaths.
     * 
     */
    void CreateProtectionCalls(Call* call) override;
       /**
     * @brief Function which perform RSA for Working and protection paths 
     * according with DPP scheme.
     * @param Call vector which contain working and protection paths.
     */
    void ResourceAlloc(CallDevices* call) override;
      
   
private:

   
};

#endif /* DEDICATEDPATHPROTECTION_H */

    