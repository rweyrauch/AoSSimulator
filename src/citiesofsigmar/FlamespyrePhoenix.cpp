/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FlamespyrePhoenix.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 105;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 200;
    static const int g_pointsPerUnitWithAnointed = 300;

    struct TableEntry {
        int m_move;
        int m_talonAttacks;
        int m_wakeOfFire;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 7, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {16, 6, 5},
                    {14, 5, 4},
                    {12, 4, 3},
                    {10, 3, 2},
                    {8,  2, 61}
            };

    bool FlamespyrePhoenix::s_registered = false;

    Unit *FlamespyrePhoenix::Create(const ParameterList &parameters) {
        auto unit = new FlamespyrePhoenix();

        auto anointed = GetBoolParam("Anointed", parameters, true);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(anointed);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string FlamespyrePhoenix::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FlamespyrePhoenix::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FlamespyrePhoenix::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FlamespyrePhoenix::Create,
                    FlamespyrePhoenix::ValueToString,
                    FlamespyrePhoenix::EnumStringToInt,
                    FlamespyrePhoenix::ComputePoints,
                    {
                        BoolParameter("Anointed"),
                            EnumParameter("City", g_city[0], g_city),
                        EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                        EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                        BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Flamespyre Phoenix", factoryMethod);
        }
    }

    FlamespyrePhoenix::FlamespyrePhoenix() :
            CitizenOfSigmar("Flamespyre Phoenix", 16, g_wounds, 8, 4, true),
            m_talons(Weapon::Type::Melee, "Flaming Talons", 2, 6, 4, 3, -1, 2),
            m_halberd(Weapon::Type::Melee, "Great Phoenix Halberd", 2, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, MONSTER, FLAMESPYRE_PHOENIX};
        m_weapons = {&m_talons, &m_halberd};
        m_battleFieldRole = Behemoth;
    }

    bool FlamespyrePhoenix::configure(bool anointed) {
        if (anointed) {
            addKeyword(HERO);
            m_battleFieldRole = Leader_Behemoth;
        }

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_talons);
        if (anointed) {
            model->addMeleeWeapon(&m_halberd);
        }
        addModel(model);

        if (anointed) {
            m_points = g_pointsPerUnitWithAnointed;
        } else {
            m_points = g_pointsPerUnit;
        }

        return true;
    }

    void FlamespyrePhoenix::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    void FlamespyrePhoenix::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_talons.setAttacks(g_damageTable[damageIndex].m_talonAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    int FlamespyrePhoenix::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds FlamespyrePhoenix::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        if (hasKeyword(HERO)) {
            // Witness to Destiny
            Dice::RollResult woundSaves, mortalSaves;
            Dice::RollD6(wounds.normal, woundSaves);
            Dice::RollD6(wounds.mortal, mortalSaves);

            Wounds totalWounds = wounds;
            totalWounds.normal -= woundSaves.rollsGE(4);
            totalWounds.normal = std::max(totalWounds.normal, 0);
            totalWounds.mortal -= mortalSaves.rollsGE(4);
            totalWounds.mortal = std::max(totalWounds.mortal, 0);

            return totalWounds;
        }
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    int FlamespyrePhoenix::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int FlamespyrePhoenix::woundModifier() const {
        auto mod = Unit::woundModifier();

        // Blood of the Ur-Phoenix
        if (m_city == City::Phoenicium) mod++;

        return mod;
    }

} // namespace CitiesOfSigmar