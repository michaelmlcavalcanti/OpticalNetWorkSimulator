/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Link.h
 * Author: bruno
 *
 * Created on August 4, 2018, 1:54 AM
 */

#ifndef LINK_H
#define LINK_H

#include <iostream>
#include <vector>
#include <limits>
#include <iostream>
#include <memory>

class Topology;
class Signal;
class Core;

#include "../GeneralClasses/Def.h"

/**
 * @brief Class Link represents a link inside a topology. 
 */
class Link {
    
    friend std::ostream& operator<<(std::ostream& ostream,
    Link* link);
public:
    /**
     * @brief Standard constructor for a Link object.
     * @param topPointer Pointer to a Topology object that owns this link.
     * @param origimNode Source node of the link.
     * @param destinationNode Destination node of the link.
     * @param length Length of the link (meters).
     * @param numberSections Total number of sections of the link.
     * @param numberCores Total number of cores in the link.
     * @param numberSlots Total number of slots of each core in the link.
     */
    Link(Topology* topPointer, NodeIndex origimNode, NodeIndex destinationNode, 
    double length, unsigned int numberSections, unsigned int numberCores,
    unsigned int numberSlots);
    /**
     * @brief Virtual destructor of a Link object.
     */
    virtual ~Link();
    /**
     * @brief Operator used to compare two link pointers.
     * @param right Link pointer to be compared.
     * @return True if the links are the same.
     */
    bool operator==(const Link& right) const;
    /**
     * @brief Initialize the link, setting the start values
     * contained in it to free. 
     */
    virtual void Initialize();
    
    /**
     * @brief Get the origin node of the link.
     * @return Origin node index of this link.
     */
    NodeIndex GetOrigimNode() const;
    /**
     * @brief Get the destination node of the link.
     * @return Destination node index of this link.
     */
    NodeIndex GetDestinationNode() const;
    /**
     * @brief Get the length of the link.
     * @return Length of the link.
     */
    double GetLength() const;
    /**
     * @brief Get the number of sections of the link.
     * @return Number of sections of the link.
     */
    unsigned int GetNumberSections() const;
    /**
     * @brief Get the cost of the link.
     * @return Cost of the link.
     */
    double GetCost() const;
    /**
     * @brief Set the cost of the link.
     * @param cost cost of the link.
     */
    void SetCost(double cost);
    /**
     * @brief Checks if the link is functional.
     * @return True if the link is working.
     */
    bool IsLinkWorking() const;
    /**
     * @brief Set the link state (working or not).
     * @param NodeWorking Link state.
     */
    void SetLinkState(bool linkWorking);
    
    unsigned int GetUtilization() const;

    void SetUtilization(unsigned int utilization);
    
    unsigned int GetNumberCores() const;
    
    /**
     * @brief Update the signal power, ASE power and
     * nonlinear power crossing this link.
     * @param signal Signal pointer that will be updated.
     */
    void CalcSignal(Signal* signal) const;
    /**
     * @brief Occupy an specified slot in this link.
     * @param index Slot index.
     */
    void OccupySlot(const SlotIndex index);
    
    void OccupySlot(const CoreIndex coreId, const SlotIndex slotId);
    /**
     * @brief Release an specified slot in this link.
     * @param index Slot index.
     */
    void ReleaseSlot(const SlotIndex index);
    
    void ReleaseSlot(const CoreIndex coreId, const SlotIndex slotId);
    /**
     * @brief Check if an specified slot is occupied in
     * the link.
     * @param index Slot index.
     * @return True if the slot is occupied.
     */
    bool IsSlotOccupied(const SlotIndex index) const;
    
    bool IsSlotOccupied(const CoreIndex coreId, const SlotIndex slotId);
    /**
     * @brief Check if an specified slot is free in
     * the link.
     * @param index Slot index.
     * @return True if the slot is free.
     */
    bool IsSlotFree(const SlotIndex index) const;
    
    bool IsSlotFree(const CoreIndex coreId, const SlotIndex slotId);
    
    unsigned int GetNumSlots() const;
    
    unsigned int GetNumSlots(const CoreIndex coreId) const;
    /**
     * @brief Return the number of free slots in the link.
     * @return Number of free slots.
     */
    unsigned int GetNumberFreeSlots() const;
    
    unsigned int GetNumberFreeSlots(const CoreIndex coreId) const;
    /**
     * @brief Return the number of occupied slots in the link.
     * @return Number of occupied slots.
     */
    unsigned int GetNumberOccupiedSlots() const;
    
    unsigned int GetNumberOccupiedSlots(const CoreIndex coreId) const;
    /**
     * @brief Function to return pointer of the topology
     * @return Pointer to topology
     */
    Topology* GetTopology() const;
private:
    /**
     * @brief Pointer to a Topology object that
     * this link is contained
     */
    Topology* topPointer;
    /**
     * @brief Origin node of the link
     */
    const NodeIndex origimNode;
    /**
     * @brief Destination node of the link
     */
    const NodeIndex destinationNode;
    /**
     * @brief Length of this link (meters)
     */
    const double length;
    /**
     * @brief Number of sections of this link
     */
    const unsigned int numberSections;
    /**
     * @brief Cost of this link, based in the 
     * selected metric
     */
    double cost;
    
    std::vector<std::shared_ptr<Core>> cores;
    /**
     * @brief Boolean variable to indicate the  link state.
     */
    bool linkWorking;
    
    unsigned int utilization;
};

#endif /* LINK_H */

