/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Dice.h>
#include <sylvaneth/SpiritOfDurthu.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {
    static const int g_basesize = 105; // x70 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 300;

    bool SpiritOfDurthu::s_registered = false;

    struct TableEntry {
        int m_verdantBlastAttacks;
        int m_guardianSwordDamage;
        int m_talonsToWound;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {6, 6,       2},
                    {5, RAND_D6, 2},
                    {4, RAND_D6, 3},
                    {3, RAND_D6, 3},
                    {2, RAND_D3, 4}
            };

    SpiritOfDurthu::SpiritOfDurthu() :
            SylvanethBase("Spirit of Durthu", 5, g_wounds, 9, 3, false),
            m_verdantBlast(Weapon::Type::Missile, "Verdant Blast", 15, 6, 4, 3, -1, RAND_D3),
            m_guardianSword(Weapon::Type::Melee, "Guardian Sword", 3, 3, 3, 3, -2, 6),
            m_massiveImpalingTalons(Weapon::Type::Melee, "Massive Impaling Talons", 1, 1, 3, 2, -2, 1) {
        m_keywords = {ORDER, SYLVANETH, FREE_SPIRITS, MONSTER, HERO, SPIRIT_OF_DURTHU};
        m_weapons = {&m_verdantBlast, &m_guardianSword, &m_massiveImpalingTalons};
        m_battleFieldRole = Role::Leader_Behemoth;

        s_globalBraveryMod.connect(this, &SpiritOfDurthu::championOfTheEverqueensWill, &m_connection);
    }

    SpiritOfDurthu::~SpiritOfDurthu() {
        m_connection.disconnect();
    }

    bool SpiritOfDurthu::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_verdantBlast);
        model->addMeleeWeapon(&m_guardianSword);
        model->addMeleeWeapon(&m_massiveImpalingTalons);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void SpiritOfDurthu::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_verdantBlast.setAttacks(g_damageTable[damageIndex].m_verdantBlastAttacks);
        m_guardianSword.setDamage(g_damageTable[damageIndex].m_guardianSwordDamage);
        m_massiveImpalingTalons.setToWound(g_damageTable[damageIndex].m_talonsToWound);
    }

    int SpiritOfDurthu::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();

        // Our Roots Run Deep
        if (hasKeyword(OAKENBROW)) woundsInflicted += 2;

        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *SpiritOfDurthu::Create(const ParameterList &parameters) {
        auto unit = new SpiritOfDurthu();

        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        unit->setGlade(glade);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_aspectsOfWar[0]);

        unit->setCommandTrait(trait);
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

    void SpiritOfDurthu::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SpiritOfDurthu::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    SpiritOfDurthu::ComputePoints,
                    {
                            EnumParameter("Glade", g_glade[0], g_glade),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Command Trait", g_aspectsOfWar[0], g_aspectsOfWar),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Spirit of Durthu", factoryMethod);
        }
    }

    Wounds SpiritOfDurthu::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Impale
        if ((hitRoll == 6) && (weapon->name() == m_massiveImpalingTalons.name())) {
            return {0, Dice::RollD6()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void SpiritOfDurthu::onStartCombat(PlayerId id) {
        Unit::onStartCombat(id);

        // Groundshaking Stomp
        if (m_meleeTarget && distanceTo(m_meleeTarget) <= 3.0) {
            if (Dice::RollD6() >= 4) {
                // TODO: Make m_meleeTarget fight last
            }
        }
    }

    int SpiritOfDurthu::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);

        // Wrathful Guardian
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), AWAKENED_WYLDWOOD, 8.0);
        if (unit != nullptr) {
            attacks += 2;
        }

        return attacks;
    }

    int SpiritOfDurthu::championOfTheEverqueensWill(const Unit *target) {
        // Champion of the Everqueen's Will
        if (target->hasKeyword(SYLVANETH) && (target->owningPlayer() == owningPlayer()) &&
            (distanceTo(target) <= 12.0)) {
            return 1;
        }

        return 0;
    }

    int SpiritOfDurthu::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Sylvaneth
