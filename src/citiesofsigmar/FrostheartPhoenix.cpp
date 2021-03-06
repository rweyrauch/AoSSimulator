/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FrostheartPhoenix.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 105;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 220;
    static const auto g_pointsPerUnitWithAnointed = 320;

    struct TableEntry {
        int m_move;
        int m_talonAttacks;
        double m_blizzazdAura;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 7, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {16, 8, 9},
                    {14, 6, 6},
                    {12, 5, 3},
                    {10, 4, 2},
                    {8,  2, 1}
            };

    bool FrostheartPhoenix::s_registered = false;

    Unit *FrostheartPhoenix::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto anointed = GetBoolParam("Anointed", parameters, true);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new FrostheartPhoenix(city, anointed, drug, trait, artefact, general);
    }

    std::string FrostheartPhoenix::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FrostheartPhoenix::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FrostheartPhoenix::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                            BoolParameter("Anointed"),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };

            s_registered = UnitFactory::Register("Frostheart Phoenix", factoryMethod);
        }
    }

    FrostheartPhoenix::FrostheartPhoenix(City city, bool anointed, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Frostheart Phoenix", 16, g_wounds, 9, 4, true, anointed ? g_pointsPerUnitWithAnointed : g_pointsPerUnit),
            m_talons(Weapon::Type::Melee, "Ice-cold Talons", 2, 8, 3, 3, -1, 2),
            m_halberd(Weapon::Type::Melee, "Great Phoenix Halberd", 2, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, MONSTER, FROSTHEART_PHOENIX};
        m_weapons = {&m_talons, &m_halberd};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_talons.setMount(true);
        s_globalToWoundMod.connect(this, &FrostheartPhoenix::blizzardAura, &m_connection);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_talons);
        if (anointed) {
            model->addMeleeWeapon(&m_halberd);
            setCommandTrait(trait);
            setArtefact(artefact);
            setGeneral(isGeneral);
            setNarcotic(narcotic);
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

    FrostheartPhoenix::~FrostheartPhoenix() {
        m_connection.disconnect();
    }

    void FrostheartPhoenix::onRestore() {
        CitizenOfSigmar::onRestore();
        // Restore table-driven attributes
        onWounded();
    }

    void FrostheartPhoenix::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_talons.setAttacks(g_damageTable[damageIndex].m_talonAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        CitizenOfSigmar::onWounded();
    }

    size_t FrostheartPhoenix::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int FrostheartPhoenix::blizzardAura(const Unit * /*attacker*/, const Weapon *weapon, const Unit *target) {
        // Blizzard Aura
        if (!weapon->isMissile() && (target->owningPlayer() != owningPlayer()) &&
            (distanceTo(target) <= g_damageTable[getDamageTableIndex()].m_blizzazdAura)) {
            return -1;
        }
        return 0;
    }

    Wounds FrostheartPhoenix::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = CitizenOfSigmar::applyWoundSave(wounds, attackingUnit);
        if (hasKeyword(HERO)) {
            // Witness to Destiny
            int numSixes = 0;
            totalWounds = ignoreWounds(totalWounds, 4, numSixes);
        }
        return totalWounds;
    }

    int FrostheartPhoenix::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int FrostheartPhoenix::woundModifier() const {
        auto mod = CitizenOfSigmar::woundModifier();

        // Blood of the Ur-Phoenix
        if (m_city == City::Phoenicium) mod++;

        return mod;
    }

} // namespace CitiesOfSigmar