/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "citiesofsigmar/BattlemageOnGriffon.h"
#include "CitiesOfSigmarPrivate.h"
#include "CoSLore.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 90;
    static const int g_wounds = 13;
    static const int g_pointsPerUnit = 300;

    struct TableEntry {
        int m_move;
        int m_beaksDamage;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 11, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {15, 3, 6},
                    {13, 2, 5},
                    {11, 2, 4},
                    {9,  1, 3},
                    {7,  1, 2}
            };

    bool BattlemageOnGriffon::s_registered = false;

    Unit *BattlemageOnGriffon::Create(const ParameterList &parameters) {
        auto unit = new BattlemageOnGriffon();

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string BattlemageOnGriffon::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int BattlemageOnGriffon::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void BattlemageOnGriffon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BattlemageOnGriffon::Create,
                    BattlemageOnGriffon::ValueToString,
                    BattlemageOnGriffon::EnumStringToInt,
                    BattlemageOnGriffon::ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Battlemage on Griffon", factoryMethod);
        }
    }

    BattlemageOnGriffon::BattlemageOnGriffon() :
            CitizenOfSigmar("Battlemage on Griffon", 15, g_wounds, 6, 5, true),
            m_beastStaff(Weapon::Type::Melee, "Beaststaff", 2, 1, 4, 3, -1, RAND_D3),
            m_twinBeaks(Weapon::Type::Melee, "Twin Beaks", 2, 4, 3, 3, -1, 3),
            m_razorClaws(Weapon::Type::Melee, "Razor Claws", 2, 6, 4, 3, -1, 2) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, MONSTER, HERO, WIZARD, BATTLEMAGE};
        m_weapons = {&m_beastStaff, &m_twinBeaks, &m_razorClaws};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;
        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool BattlemageOnGriffon::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());

        model->addMeleeWeapon(&m_beastStaff);
        model->addMeleeWeapon(&m_twinBeaks);
        model->addMeleeWeapon(&m_razorClaws);

        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    void BattlemageOnGriffon::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    void BattlemageOnGriffon::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_twinBeaks.setDamage(g_damageTable[damageIndex].m_beaksDamage);
        m_razorClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    int BattlemageOnGriffon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int BattlemageOnGriffon::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Two-headed
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_twinBeaks.name())) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int BattlemageOnGriffon::castingModifier() const {
        auto mod = Unit::castingModifier();

        // Amber Battlemage
        if (Board::Instance()->getRealm() == Ghur) mod++;

        return mod;
    }

    int BattlemageOnGriffon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace CitiesOfSigmar