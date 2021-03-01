/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/LuminarkOfHysh.h"
#include "CitiesOfSigmarPrivate.h"
#include "CoSLore.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 105;
    static const int g_wounds = 11;
    static const int g_pointsPerUnit = 210;
    static const int g_pointsPerUnitWithBattlemage = 270;

    struct TableEntry {
        int m_move;
        int m_auraRange;
        int m_beamOfLight;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 10, 2},
                    {9,  8,  3},
                    {8,  6,  4},
                    {7,  4,  5},
                    {6,  2,  6}
            };

    bool LuminarkOfHysh::s_registered = false;

    Unit *LuminarkOfHysh::Create(const ParameterList &parameters) {
        auto unit = new LuminarkOfHysh();

        auto battlemage = GetBoolParam("Battlemage", parameters, true);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto drug = (Narcotic)GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        unit->setNarcotic(drug);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(battlemage, lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string LuminarkOfHysh::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int LuminarkOfHysh::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void LuminarkOfHysh::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LuminarkOfHysh::Create,
                    LuminarkOfHysh::ValueToString,
                    LuminarkOfHysh::EnumStringToInt,
                    LuminarkOfHysh::ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                            BoolParameter("Battlemage"),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Luminark of Hysh", factoryMethod);
        }
    }

    LuminarkOfHysh::LuminarkOfHysh() :
            CitizenOfSigmar("Luminark of Hysh", 10, g_wounds, 6, 4, false),
            m_beamOfLight(Weapon::Type::Missile, "Searing Beam of Light", 30, 1, 0, 0, 0, 0),
            m_wizardsStaff(Weapon::Type::Melee, "Wizard's Staff", 2, 1, 4, 3, -1, RAND_D3),
            m_arcaneTools(Weapon::Type::Melee, "Arcane Tools", 1, 4, 5, 5, 0, 1),
            m_hooves(Weapon::Type::Melee, "Steel-shod Hooves", 1, 4, 4, 4, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, LUMINARK_OF_HYSH};
        m_weapons = {&m_beamOfLight, &m_wizardsStaff, &m_arcaneTools, &m_hooves};
        m_hasMount = true;
        m_hooves.setMount(true);
        m_battleFieldRole = Behemoth;

        s_globalUnbindMod.connect(this, &LuminarkOfHysh::locusOfHysh, &m_locusSlot);
    }

    LuminarkOfHysh::~LuminarkOfHysh() {
        m_locusSlot.disconnect();
    }

    bool LuminarkOfHysh::configure(bool battlemage, Lore lore) {
        if (battlemage) {
            addKeyword(WIZARD);
            addKeyword(HERO);
            m_battleFieldRole = Leader_Behemoth;
            m_totalSpells = 1;
            m_totalUnbinds = 1;
        }

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_beamOfLight);
        model->addMeleeWeapon(&m_arcaneTools);
        model->addMeleeWeapon(&m_hooves);
        if (battlemage) {
            model->addMeleeWeapon(&m_wizardsStaff);
        }
        addModel(model);

        if (battlemage) {
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
            m_knownSpells.push_back(std::make_unique<MysticShield>(this));
            m_points = g_pointsPerUnitWithBattlemage;
        } else {
            m_points = g_pointsPerUnit;
        }

        return true;
    }

    void LuminarkOfHysh::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    void LuminarkOfHysh::onWounded() {
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    int LuminarkOfHysh::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int LuminarkOfHysh::castingModifier() const {
        auto mod = Unit::castingModifier();

        // White Battlemage
        if (Board::Instance()->getRealm() == Hysh) mod++;

        return mod;
    }

    int LuminarkOfHysh::locusOfHysh(const Unit *caster) {
        if (caster->hasKeyword(COLLEGIATE_ARCANE) && caster->hasKeyword(WIZARD) &&
            ((caster->owningPlayer() == owningPlayer()) && (distanceTo(caster) <= 12.0))) {
            return 1;
        }
        return 0;
    }

    int LuminarkOfHysh::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar