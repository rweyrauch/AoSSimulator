/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AGEOFSIGMARSIMJS_H
#define AGEOFSIGMARSIMJS_H

#include "AgeOfSigmarSim.h"

struct ParameterJS
{
    ParamType paramType;
    char* name;
    int intValue;
    int minValue;
    int maxValue;
    int increment;
};

class JSInterface 
{
public:
    static void Initialize();
    static int GrandAllianceStringToKeyword(const char* allianceName);
    static int FactionStringToKeyword(const char* factionName);
    static const char* FactionKeywordToString(int faction);

    static const char* ParameterValueToString(const ParameterJS& param);
};

/*
struct JSFactoryMethod
{
public:
    std::vector<ParameterJS> parameters;
    int grandAlliance;
    int faction;
};

class JSUnitFactory
{
public:

    static Unit* Create(const char* name, const std::vector<ParameterJS>& parameters);
 
    static int GetNumRegisteredUnits();
    static const JSFactoryMethod* LookupUnit(const char* name);
};
*/

#endif //AGEOFSIGMARSIMJS_H
