/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "nurgle/LordOfAfflictions.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 60;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 190;

    bool LordOfAfflictions::s_registered = false;

    Unit *LordOfAfflictions::Create(const ParameterList &parameters) {
        auto unit = new LordOfAfflictions();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto legion = (PlagueLegion)GetEnumParam("Plague Legion", parameters, (int)PlagueLegion::None);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordOfAfflictions::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LordOfAfflictions::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    LordOfAfflictions::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Lord of Afflictions", factoryMethod);
        }
    }

    LordOfAfflictions::LordOfAfflictions() :
            NurgleBase("Lord of Afflictions", 8, g_wounds, 10, 4, true),
            m_festerspike(Weapon::Type::Melee, "Festerspike", 2, 3, 3, 3, -1, RAND_D3),
            m_mouthparts(Weapon::Type::Melee, "Foul Mouthparts", 1, 2, 3, 3, 0, 1),
            m_sting(Weapon::Type::Melee, "Venomous String", 1, 1, 4, 3, -1, RAND_D3),
            m_tocsin(Weapon::Type::Melee, "Dolorous Tocsin", 1, 1, 4, 3, -2, 2) {
        m_keywords = {CHAOS, MORTAL, DAEMON, NURGLE, ROTBRINGER, HERO, LORD_OF_AFFLICTIONS};
        m_weapons = {&m_festerspike, &m_mouthparts, &m_sting, &m_tocsin};
        m_battleFieldRole = Leader;
        m_hasMount = true;
        m_mouthparts.setMount(true);
        m_sting.setMount(true);
        s_globalToHitReroll.connect(this, &LordOfAfflictions::plagueVectorToHitRerolls, &m_plagueVectorSlot);
    }

    LordOfAfflictions::~LordOfAfflictions() {
        m_plagueVectorSlot.disconnect();
    }

    bool LordOfAfflictions::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_festerspike);
        model->addMeleeWeapon(&m_mouthparts);
        model->addMeleeWeapon(&m_sting);
        model->addMeleeWeapon(&m_tocsin);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds LordOfAfflictions::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        // Disgustingly Resilient
        return ignoreWounds(wounds, 5);
    }

    void LordOfAfflictions::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            // Rotten Regeneration
            heal(1);

            auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0);

            // Incubatch
            for (auto unit : units) {
                auto roll = Dice::RollD6();
                if (unit->hasKeyword(NURGLE) && (roll >= 6)) unit->applyDamage({0, 1}, this);
                else if (roll >= 2) unit->applyDamage({0, 1}, this);
            }

            // Virulent Discharge
            for (auto unit : units) {
                if (Dice::RollD6() >= 6) {
                    if (unit->hasKeyword(NURGLE)) unit->heal(Dice::RollD3());
                    else unit->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        }

    }

    Rerolls
    LordOfAfflictions::plagueVectorToHitRerolls(const Unit *attacker, const Weapon * /*weapon*/,
                                                const Unit * /*target*/) {
        if (isFriendly(attacker) && attacker->hasKeyword(ROTBRINGER) && (distanceTo(attacker) <= 7.0))
            return Reroll_Ones;

        return No_Rerolls;
    }

    int LordOfAfflictions::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
