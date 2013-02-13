/*
 * @file test/dynamics/Perturb.cpp
 *
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems
 * http://www.vle-project.org
 *
 * Copyright (c) 2011 INRA http://www.inra.fr
 *
 * See the AUTHORS or Authors.txt file for copyright owners and contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <vle/devs/Dynamics.hpp>
#include <vle/value/Value.hpp>
#include <vle/value/Map.hpp>
#include <vle/utils/Exception.hpp>
#include <vle/devs/DynamicsDbg.hpp>

#include <iostream>

namespace differentialEquation { namespace test { namespace dynamics  {

namespace vv = vle::value;
namespace vd = vle::devs;
namespace vu = vle::utils;

/**
 * @brief This dynamic simulates a perturbation by sending a message
 * at a given time + a given number of bags
 *
 * parameters :
 * - message: the map value message to send
 * - sendTime: the time to send the message
 * - nbBags (default = 0): the number of bags to wait at sendTime before
 * sending the message
 */
class Perturb: public vd::Dynamics
{
private:
    enum STATE {
        BEFORE_PERT,
        DURING_PERT,
        AFTER_PERT
    };

    STATE mstate;
    vv::Value* message;
    vd::Time sendTime;
    unsigned int nbBags;

    unsigned int currentBag;
public:
    Perturb(const vd::DynamicsInit& init, const vd::InitEventList& events):
        vd::Dynamics(init, events), mstate(BEFORE_PERT),
        message(0), sendTime(events.getDouble("sendTime")),
        nbBags(0), currentBag(0)
    {
        if(events.exist("nbBags")){
            nbBags = events.getInt("nbBags");
        }
        message = events.get("message")->clone();
    }

    virtual ~Perturb()
    {
        delete message;
    }

    /**
     * @brief Implementation of Dynamics::init
     */
    vd::Time init(const vd::Time& /*time*/)
    {
        mstate = BEFORE_PERT;
        return sendTime;
    }
    /**
     * @brief Implementation of Dynamics::timeAdvance
     */
    vd::Time timeAdvance() const
    {
        switch(mstate){
        case BEFORE_PERT: {
            return sendTime;
            break;
        } case DURING_PERT: {
            return 0;
            break;
        } case AFTER_PERT: {
            return vd::infinity;
            break;
        }}
        return vd::infinity;
    }
    /**
     * @brief Implementation of Dynamics::internalTransition
     */
    void internalTransition(const vd::Time& /* time */)
    {
        switch(mstate){
        case BEFORE_PERT: {
            if(nbBags == 0){
                mstate = AFTER_PERT;
            } else {
                mstate = DURING_PERT;
            }
            break;
        } case DURING_PERT: {
            currentBag++;
            if(currentBag == nbBags){
                mstate = AFTER_PERT;
            } else {
                mstate = DURING_PERT;
            }
            break;
        } case AFTER_PERT: {
            mstate = AFTER_PERT;
            break;
        }}
    }
    /**
     * @brief Implementation of Dynamics::output
     */
    void output(const vd::Time& /* time */, vd::ExternalEventList& output) const
    {
        switch(mstate){
        case BEFORE_PERT: {
            if(nbBags == 0){
                vd::ExternalEvent* ee = new vd::ExternalEvent("p");
                ee->putAttribute("p",message->clone());
                output.addEvent(ee);
            }
            break;
        } case DURING_PERT: {
            if(currentBag == nbBags){
                vd::ExternalEvent* ee = new vd::ExternalEvent("p");
                ee->putAttribute("p", message->clone());
                output.addEvent(ee);
            }
            break;
        } case AFTER_PERT: {
            break;
        }}
    }

    /**
     * @brief Implementation of Dynamics::externalTransition
     */
    void externalTransition(const vd::ExternalEventList& /*event*/,
        const vd::Time& /* time */)
    {
        throw vu::ArgError(vle::fmt(_(
                "[%1%] Model that does not handle external events "))
        % getModelName());
    }
    /**
     * @brief Implementation of Dynamics::observation
     */
    vv::Value* observation(const vd::ObservationEvent& /*event*/) const
    {
        switch(mstate){
        case BEFORE_PERT: {
            return new vv::Double(0);
            break;
        } case DURING_PERT: {
            return new vv::Double(0);
            break;
        } case AFTER_PERT: {
            return message->clone();
        }}
        return 0;
    }
};


}}} // namespace differentialEquation test dynamics

DECLARE_DYNAMICS(differentialEquation::test::dynamics::Perturb)
//DECLARE_DYNAMICS_DBG(differentialEquation::test::dynamics::Perturb)
