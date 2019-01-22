# AoSManoAMano
AosManoAMano simple Warhammer Age of Sigmar&trade; battle simulator.
The application permits two units to conduct a simulator Age of Sigmar battle.
The battle is conducted following the Age of Sigmar 2.0 core rules.  The battle starts
with an initiative roll for first turn and then progresses through a multiturn simulation
of an Age of Sigmar battle.  The application aims to simulate each phase of the
game; Hero, Movement, Shooting, Charge, Combat and Battleshock.

## Why Bother?
This simulation attempts to answer questions like are 10 Liberators better than
10 Sequitors and by how much?  The simulations can run thousands of times to 
collect a set of statistics on battle results.  These statistics can then be used
to determine the strength or weaknesses of various matchups.  Experiments can
be done quickly with various weapon configurations to find the strongest setups for a
particular enemy unit.

## What Works?
Age of Sigmar is complex and rich game with a myriad of rules.  Not of all those rules have been
captured by the simulation.  The goal of this application is to conduct simple 1 vs 1 battles.
To support this goal the simulation supports initial unit deployment on a small(ish) battle
field.  Units support basic hero phase abilities, movement towards the enemy,

## What Does Not Work?
The short answer is quite a lot.  The simulation is very much a work in progress.  The big items
that are missing include spells in general, many many units have not been implemented and per-model
movement.  The 'intelligence' of the simulated units is quite primitive and in need of more
work to make them 'think' strategically.  Currently units attempt to move or run toward their
opponent if they prefer melee combat.  Shooting unit move to try to get in range for shooting.

## Building
AoSManoAMano is written in modern C++ (C++14) and intended to be highly portable.  Currently only 
Linux (or Unix-like) platforms are supported.  The build system uses CMake and follows the 
normal CMake build pattern.

    % mkdir build
    % cd build
    % cmake ..
    % make
  
## Running
The build produces an executable called AoSManoAMano.  The simulation supports a command 
line only interface currently.

    Age of Sigmar: Mano a Mano simulation.
    Usage: 
      ./AoSManoAMano [OPTION...]

      -h, --help            Print help
      -l, --list            List supported units
      -r, --rounds arg      Number of battle rounds
      -v, --verbose         Enable verbose logging
      -1, --red arg         Player 1 (Red) Unit
      -2, --blue arg        Player 2 (Blue) Unit
      -s, --save            Save battlemaps
          --mapname arg     Battlemap basename
      -i, --iterations arg  Number of battle iterations

Player units are specified using the following format:

    "<unit name>,<param1=value1>,<param2=value2>..."
    
The following specifies the parameters needed to create a unit of 10 Liberators with 2 Grandhammers.

    "Liberators,numModels=10,numGrandhammers=2"    
    
To simulate a battle between a unit of 10 Liberators and 20 Bloodreavers use the following command:

    % AosManoAMano -v -1 "Liberators,numModels=10,numGrandhammers=2" -2 "Bloodreavers,numModels=20,iconBearer=true,hornblowers=true"
    
## Future Plans

1. Windows and Android ports.
2. Proper GUI wrapper.
3. More units.
4. Improve simulation to include spells, prayers and other buffs/de-buffs.
5. Implement per-model positioning and pile in movement.
6. ????

    