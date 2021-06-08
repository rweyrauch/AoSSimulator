/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/Loonboss.h>
#include <UnitFactory.h>
#include <Board.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 70;

    bool Loonboss::s_registered = false;

    Loonboss::Loonboss(Allegiance allegiance,CommandTrait trait, Artefact artefact, bool isGeneral) :
            GloomspiteGitzBase(allegiance, "Loonboss", 5, g_wounds, 5, 5, false, g_pointsPerUnit),
            m_moonslicer(Weapon::Type::Melee, "Moon-slicer", 1, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, LOONBOSS};
        m_weapons = {&m_moonslicer};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_moonslicer);
        addModel(model);

        m_commandAbilities.push_back(std::unique_ptr<CommandAbility>(CreateImDaBossNoStabEmGood(this)));

        m_points = g_pointsPerUnit;
    }

    Unit *Loonboss::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_blessingsOfTheBadMoon[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_troglodyticTreasures[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Loonboss(allegiance, trait, artefact, general);
    }

    void Loonboss::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Loonboss::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    Loonboss::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_blessingsOfTheBadMoon[0], g_blessingsOfTheBadMoon),
                            EnumParameter("Artefact", g_troglodyticTreasures[0], g_troglodyticTreasures),
                            BoolParameter("General"),
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Loonboss", factoryMethod);
        }
    }

    int Loonboss::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);
        // Dead Tricksy
        modifier -= 1;
        return modifier;
    }

    int Loonboss::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace GloomspiteGitz
