/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/LordRegent.h>
#include <UnitFactory.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {

    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 0;

    bool VanariLordRegent::s_registered = false;

    Unit *VanariLordRegent::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraitsVanari[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsVanari[0]);
        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        return new VanariLordRegent(nation, trait, artefact, general);
    }

    int VanariLordRegent::ComputePoints(int numModels) {
        return g_pointsPerUnit;
    }

    void VanariLordRegent::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Command Trait", g_commandTraitsVanari[0], g_commandTraitsVanari),
                            EnumParameter("Artefact", g_artefactsVanari[0], g_artefactsVanari),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Vanari Lord Regent", factoryMethod);
        }
    }

    VanariLordRegent::VanariLordRegent(GreatNation nation, CommandTrait trait, Artefact artefact, bool isGeneral) :
        LuminethBase("Vanari Lord Regent", 14, g_wounds, 8, 3, false) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, HERO, WIZARD, LORD_REGENT, Sunmetal_Weapons};
        m_weapons = {&m_sword, &m_hornsAndClaws};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_hornsAndClaws.setMount(true);

        setNation(nation);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_hornsAndClaws);
        addModel(model);

        m_points = ComputePoints(1);
    }
}