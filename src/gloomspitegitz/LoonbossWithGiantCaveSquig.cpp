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
    static const int BASESIZE = 50;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    bool LoonbossWithGiantCaveSquig::s_registered = false;

    LoonbossWithGiantCaveSquig::LoonbossWithGiantCaveSquig() :
            GloomspiteGitzBase("Loonboss with Giant Cave Squig", RAND_2D6, WOUNDS, 6, 4, true),
            m_moonProdderMissile(Weapon::Type::Missile, "Moon-prodder", 14, RAND_D6, 4, 3, -1, 1),
            m_moonProdder(Weapon::Type::Melee, "Moon-prodder", 2, 4, 4, 3, -1, 1),
            m_massiveFangFilledGob(Weapon::Type::Melee, "Massive Fang-filled Gob", 1, 4, 4, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, LOONBOSS};
        m_weapons = {&m_moonProdderMissile, &m_moonProdder, &m_massiveFangFilledGob};
        m_battleFieldRole = Leader;
    }

    bool LoonbossWithGiantCaveSquig::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_moonProdderMissile);
        model->addMeleeWeapon(&m_moonProdder);
        model->addMeleeWeapon(&m_massiveFangFilledGob);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *LoonbossWithGiantCaveSquig::Create(const ParameterList &parameters) {
        auto unit = new LoonbossWithGiantCaveSquig();

        auto trait = (CommandTrait)GetEnumParam("Command Trait", parameters, g_blessingsOfTheBadMoon[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact)GetEnumParam("Artefact", parameters, g_troglodyticTreasures[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
                            BoolParameter("General")
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

    int LoonbossWithGiantCaveSquig::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace GloomspiteGitz

