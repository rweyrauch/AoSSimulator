/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/LoonbossWithGiantCaveSquig.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {

    static const int g_basesize = 50;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 100;

    bool LoonbossWithGiantCaveSquig::s_registered = false;

    LoonbossWithGiantCaveSquig::LoonbossWithGiantCaveSquig(Allegiance allegiance, CommandTrait trait, Artefact artefact, bool isGeneral) :
            GloomspiteGitzBase(allegiance, "Loonboss with Giant Cave Squig", RAND_2D6, g_wounds, 6, 4, true, g_pointsPerUnit),
            m_moonProdderMissile(Weapon::Type::Missile, "Moon-prodder", 14, RAND_D6, 4, 3, -1, 1),
            m_moonProdder(Weapon::Type::Melee, "Moon-prodder", 2, 4, 4, 3, -1, 1),
            m_massiveFangFilledGob(Weapon::Type::Melee, "Massive Fang-filled Gob", 1, 4, 4, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, LOONBOSS};
        m_weapons = {&m_moonProdderMissile, &m_moonProdder, &m_massiveFangFilledGob};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_massiveFangFilledGob.setMount(true);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_moonProdderMissile);
        model->addMeleeWeapon(&m_moonProdder);
        model->addMeleeWeapon(&m_massiveFangFilledGob);
        addModel(model);

        m_commandAbilities.push_back(std::unique_ptr<CommandAbility>(CreateImDaBossNoStabEmGood(this)));
    }

    Unit *LoonbossWithGiantCaveSquig::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_blessingsOfTheBadMoon[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_troglodyticTreasures[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new LoonbossWithGiantCaveSquig(allegiance, trait, artefact, general);
    }

    void LoonbossWithGiantCaveSquig::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LoonbossWithGiantCaveSquig::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    LoonbossWithGiantCaveSquig::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_blessingsOfTheBadMoon[0], g_blessingsOfTheBadMoon),
                            EnumParameter("Artefact", g_troglodyticTreasures[0], g_troglodyticTreasures),
                            BoolParameter("General"),
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Loonboss with Giant Cave Squig", factoryMethod);
        }
    }

    int LoonbossWithGiantCaveSquig::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);
        // Dead Tricksy
        modifier -= 1;
        return modifier;
    }

    int LoonbossWithGiantCaveSquig::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace GloomspiteGitz

