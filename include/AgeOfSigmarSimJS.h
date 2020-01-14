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
#include "Unit.h"

struct JSUnitInfo
{
public:
    const char* name;
    int numberOfParameters;
    void getParameter(int which, Parameter& param) { param = this->parameters[which]; }
    int grandAlliance;
    int numberOfFactions;
    int getFaction(int which) { return this->factions[which]; }
    const Parameter* parameters;
    const int* factions;
};

class JSInterface 
{
public:
    static void Initialize();
    static void SetVerbosity(Verbosity verbosity);
    static int GrandAllianceStringToKeyword(const char* allianceName);
    static int FactionStringToKeyword(const char* factionName);
    static const char* FactionKeywordToString(int faction);
    static const char* GrandAllianceKeywordToString(int ga);

    static Unit* CreateUnit(const char* name, const Parameter* parameters, int numParams);
    static int GetNumberOfAvailableUnits();
    static void GetUnitInfo(int which, JSUnitInfo& info);
    static void GetUnitInfoByName(const char* name, JSUnitInfo& info);
    static const char* UnitParameterValueToString(const char* unitName, const char* paramName, int value);
    static int UnitEnumStringToInt(const char* name, const char* enumString);
};



#endif //AGEOFSIGMARSIMJS_H
