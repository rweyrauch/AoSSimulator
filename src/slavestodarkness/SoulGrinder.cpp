/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/SoulGrinder.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 160;
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 210;

    struct TableEntry {
        int m_move;
        int m_cannonAttacks;
        int m_legAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 10, 13, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 6, 6},
                    {10, 5, 5},
                    {8,  4, 4},
                    {7,  3, 3},
                    {6,  2, 2}
            };

    bool SoulGrinder::s_registered = false;

    Unit *SoulGrinder::Create(const ParameterList &parameters) {
        auto unit = new SoulGrinder();

        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Warpmetal_Blade);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure(weapon);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string SoulGrinder::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            switch (parameter.intValue) {
                case Warpmetal_Blade:
                    return "Warpmetal Blade";
                case Daemonbone_Talon:
                    return "Daemonbone Talon";
                default:
                    break;
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int SoulGrinder::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Warpmetal Blade") return Warpmetal_Blade;
        else if (enumString == "Daemonbone Talon") return Daemonbone_Talon;

        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    int SoulGrinder::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void SoulGrinder::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Warpmetal_Blade, Daemonbone_Talon};
            static FactoryMethod factoryMethod = {
                    SoulGrinder::Create,
                    SoulGrinder::ValueToString,
                    SoulGrinder::EnumStringToInt,
                    SoulGrinder::ComputePoints,
                    {
                            EnumParameter("Weapon", Warpmetal_Blade, weapons),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Soul Grinder", factoryMethod);
        }
    }

    SoulGrinder::SoulGrinder() :
            SlavesToDarknessBase("Soul Grinder", 12, g_wounds, 10, 4, false),
            m_cannon(Weapon::Type::Missile, "Harvester Cannon", 16, 6, 4, 3, -1, 1),
            m_phlegm(Weapon::Type::Missile, "Phlegm Bombardment", 20, 1, 4, 3, -2, 3),
            m_legs(Weapon::Type::Melee, "Piston-driven Legs", 1, 6, 4, 3, -1, 1),
            m_claw(Weapon::Type::Melee, "Hellforged Claw", 2, 1, 4, 3, -2, RAND_D6),
            m_blade(Weapon::Type::Melee, "Warpmetal Blade", 2, 2, 4, 3, -2, 3),
            m_talon(Weapon::Type::Melee, "Daemonbone Talon", 2, 4, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, SLAVES_TO_DARKNESS, MONSTER, MARK_OF_CHAOS, SOUL_GRINDER};
        m_weapons = {&m_cannon, &m_phlegm, &m_legs, &m_claw, &m_blade, &m_talon};
        m_battleFieldRole = Behemoth;

        // Implacable Advance
        m_runAndShoot = true;
    }

    bool SoulGrinder::configure(WeaponOption option) {
        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_cannon);
        model->addMissileWeapon(&m_phlegm);
        model->addMeleeWeapon(&m_legs);
        model->addMeleeWeapon(&m_claw);
        if (option == Warpmetal_Blade)
            model->addMeleeWeapon(&m_blade);
        else if (option == Daemonbone_Talon)
            model->addMeleeWeapon(&m_talon);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds SoulGrinder::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {

        // Hellforged Claw
        if ((weapon->name() == m_claw.name()) && (hitRoll == 6)) {
            return {0, Dice::RollD6()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void SoulGrinder::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_cannon.setAttacks(g_damageTable[damageIndex].m_cannonAttacks);
        m_legs.setAttacks(g_damageTable[damageIndex].m_legAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void SoulGrinder::onRestore() {
        Unit::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    int SoulGrinder::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

} // namespace SlavesToDarkness


