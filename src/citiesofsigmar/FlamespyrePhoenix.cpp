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
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto anointed = GetBoolParam("Anointed", parameters, true);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new FlamespyrePhoenix(city, anointed, drug, trait, artefact, general);
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
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Flamespyre Phoenix", factoryMethod);
        }
    }

    FlamespyrePhoenix::FlamespyrePhoenix(City city, bool anointed, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Flamespyre Phoenix", 16, g_wounds, 8, 4, true, anointed ? g_pointsPerUnitWithAnointed : g_pointsPerUnit),
            m_talons(Weapon::Type::Melee, "Flaming Talons", 2, 6, 4, 3, -1, 2),
            m_halberd(Weapon::Type::Melee, "Great Phoenix Halberd", 2, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, MONSTER, FLAMESPYRE_PHOENIX};
        m_weapons = {&m_talons, &m_halberd};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_talons.setMount(true);

        if (anointed) {
            addKeyword(HERO);
            m_battleFieldRole = Role::Leader_Behemoth;

            setCommandTrait(trait);
            setArtefact(artefact);
            setGeneral(isGeneral);
            setNarcotic(narcotic);
        }

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_talons);
        if (anointed) {
            model->addMeleeWeapon(&m_halberd);
        }
        addModel(model);

        if (anointed) {
            m_commandAbilities.push_back(
                    std::make_unique<BuffRerollCommandAbility>(this, "Captain of the Phoenix Guard", 12, 12,
                                                               GamePhase::Combat, Attribute::To_Wound_Melee,
                                                               Rerolls::Failed,
                                                               Abilities::Target::SelfAndFriendly,
                                                               std::vector<Keyword>{PHOENIX_TEMPLE}));
        }
    }

    void FlamespyrePhoenix::onRestore() {
        CitizenOfSigmar::onRestore();
        // Restore table-driven attributes
        onWounded();
    }

    void FlamespyrePhoenix::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_talons.setAttacks(g_damageTable[damageIndex].m_talonAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        CitizenOfSigmar::onWounded();
    }

    size_t FlamespyrePhoenix::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds FlamespyrePhoenix::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = CitizenOfSigmar::applyWoundSave(wounds, attackingUnit);
        if (hasKeyword(HERO)) {
            // Witness to Destiny
            int numSixes = 0;
            totalWounds = ignoreWounds(totalWounds, 4, numSixes);
        }
        return totalWounds;
    }

    int FlamespyrePhoenix::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int FlamespyrePhoenix::woundModifier() const {
        auto mod = CitizenOfSigmar::woundModifier();

        // Blood of the Ur-Phoenix
        if (m_city == City::Phoenicium) mod++;

        return mod;
    }

} // namespace CitiesOfSigmar