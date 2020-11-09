/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/FrostlordStonehorn.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 13;
    static const int g_pointsPerUnit = 400;

    struct TableEntry {
        int m_move;
        int m_hornsAttacks;
        int m_hoovesToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 5, 8, 10, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 6, 2},
                    {10, 5, 3},
                    {8,  4, 3},
                    {6,  3, 4},
                    {4,  2, 4}
            };

    bool FrostlordOnStonehorn::s_registered = false;

    Unit *FrostlordOnStonehorn::Create(const ParameterList &parameters) {
        auto unit = new FrostlordOnStonehorn();

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_frostlordTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_frostlordArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, g_stonehornTraits[0]);

        bool ok = unit->configure(mountTrait);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string FrostlordOnStonehorn::ValueToString(const Parameter &parameter) {
        return MawtribesBase::ValueToString(parameter);
    }

    int FrostlordOnStonehorn::EnumStringToInt(const std::string &enumString) {
        return MawtribesBase::EnumStringToInt(enumString);
    }

    void FrostlordOnStonehorn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FrostlordOnStonehorn::Create,
                    FrostlordOnStonehorn::ValueToString,
                    FrostlordOnStonehorn::EnumStringToInt,
                    FrostlordOnStonehorn::ComputePoints,
                    {
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe),
                            EnumParameter("Command Trait", g_frostlordTraits[0], g_frostlordTraits),
                            EnumParameter("Artefact", g_frostlordArtefacts[0], g_frostlordArtefacts),
                            EnumParameter("Mount Trait", g_stonehornTraits[0], g_stonehornTraits),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Frostlord on Stonehorn", factoryMethod);
        }
    }

    FrostlordOnStonehorn::FrostlordOnStonehorn() :
            MawtribesBase("Frostlord on Stonehorn", 12, g_wounds, 9, 3, false),
            m_spear(Weapon::Type::Melee, "Frost Spear", 2, 4, 3, 3, -1, 3),
            m_kicks(Weapon::Type::Melee, "Punches and Kicks", 1, 3, 3, 3, 0, 1),
            m_horns(Weapon::Type::Melee, "Rock-hard Horns", 2, 6, 4, 3, -2, 3),
            m_hooves(Weapon::Type::Melee, "Crushing Hooves", 2, RAND_D6, 3, 2, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, OGOR, STONEHORN, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, FROSTLORD};
        m_weapons = {&m_spear, &m_kicks, &m_horns, &m_hooves};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;
    }

    bool FrostlordOnStonehorn::configure(MountTrait mountTrait) {
        auto model = new Model(g_basesize, wounds());

        m_mountTrait = mountTrait;

        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_kicks);
        model->addMeleeWeapon(&m_horns);
        model->addMeleeWeapon(&m_hooves);

        addModel(model);

        m_points = FrostlordOnStonehorn::ComputePoints(1);

        return true;
    }

    void FrostlordOnStonehorn::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    int FrostlordOnStonehorn::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void FrostlordOnStonehorn::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_horns.setAttacks(g_damageTable[damageIndex].m_hornsAttacks);
        m_hooves.setToWound(g_damageTable[damageIndex].m_hoovesToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        MawtribesBase::onWounded();
    }

    Wounds
    FrostlordOnStonehorn::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Earth-shattering Charge
        if (m_charged && (weapon->name() == m_horns.name() || weapon->name() == m_hooves.name())) {
            return {weapon->damage() + 1, 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Wounds FrostlordOnStonehorn::applyWoundSave(const Wounds &wounds) {
        // Stone Skeleton
        Dice::RollResult woundSaves, mortalSaves;
        Dice::RollD6(wounds.normal, woundSaves);
        Dice::RollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int FrostlordOnStonehorn::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
