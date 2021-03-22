/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "UnitFactory.h"
#include "spells/MysticShield.h"
#include "slavestodarkness/KhagrasRavagers.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_pointsPerUnit = 125;

    bool KhagrasRavagers::s_registered = false;

    Unit *KhagrasRavagers::Create(const ParameterList &parameters) {
        auto unit = new KhagrasRavagers();

        unit->setDamnedLegion(DamnedLegion::Ravagers);
        unit->setMarkOfChaos(MarkOfChaos::Undivided);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KhagrasRavagers::Init() {
        if (!s_registered) {

            static FactoryMethod factoryMethod = {
                    KhagrasRavagers::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    KhagrasRavagers::ComputePoints,
                    {
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Khagra's Ravagers", factoryMethod);
        }
    }

    KhagrasRavagers::KhagrasRavagers() :
            SlavesToDarknessBase("Khagra's Ravagers", 5, g_wounds, 7, 4, false),
            m_handWeapons(Weapon::Type::Melee, "Chaos Hand Weapons", 1, 3, 3, 3, 0, 1),
            m_mace(Weapon::Type::Melee, "Daemonbound Mace", 1, 3, 3, 3, -2, 1),
            m_staff(Weapon::Type::Melee, "Sorcerer Staff", 3, 1, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, UNDIVIDED, RAVAGERS, CHAOS_WARRIORS, KHAGRAS_RAVAGERS};
        m_weapons = {&m_handWeapons, &m_staff, &m_mace};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool KhagrasRavagers::configure() {

        auto khagra = new Model(g_basesize, wounds());
        khagra->addMeleeWeapon(&m_mace);
        khagra->setName("Khagra the Usurper");
        addModel(khagra);

        // TODO: handle Wizard
        auto zarshia = new Model(g_basesize, wounds());
        zarshia->setName("Zarshia Bittersoul");
        zarshia->addMeleeWeapon(&m_staff);
        addModel(zarshia);

        auto dour = new Model(g_basesize, wounds());
        dour->setName("Dour Cragen");
        dour->addMeleeWeapon(&m_handWeapons);
        addModel(dour);

        auto razek = new Model(g_basesize, wounds());
        razek->setName("Razek Godblessed");
        razek->addMeleeWeapon(&m_handWeapons);
        addModel(razek);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

    Wounds KhagrasRavagers::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Check for alive model with shield
        if (isNamedModelAlive("Khagra the Usuper") || isNamedModelAlive("Dour Cragen") ||
            isNamedModelAlive("Razek Godblessed")) {
            // Chaos Runeshield
            Dice::RollResult mortalSaves;
            Dice::RollD6(wounds.mortal, mortalSaves);

            Wounds totalWounds = wounds;
            totalWounds.mortal -= mortalSaves.rollsGE(5);
            totalWounds.mortal = std::max(totalWounds.mortal, 0);

            return totalWounds;
        }
        return SlavesToDarknessBase::applyWoundSave(wounds, attackingUnit);
    }

    int KhagrasRavagers::ComputePoints(int) {
        return g_pointsPerUnit;
    }

} //SlavesToDarkness