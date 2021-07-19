# rpgDemo

Goal: Implementation of an automatic turn-based RPG mechanics including multiple actions during combat, leveling up, and equipment. See Godville's combat for example.

Features:
  Character Generation: Random values of HP, MP, attack, and defense. Random amount of skills and skill slots a character may have. Randomly generated name from a list.
  
  Basic Combat: Each turn a character will randomly either attack or use an active skill. Characters alternate turns. Random chance determines which character starts. Battle ends when a character's HP reaches 0. 
  Active Skills: Damaging skills, healing skills. Uses MP. 
    Examples: A fire spell that damages and burns the other character or a healing spell to restore the user's lost health.
  Passive Skills: Affect stats, give buffs.
  Buffs/Debuffs: Modify stats of a character. May have a duration. Current duration is reduced when each character ends their turn. 
    Example: include an increase to the attack value by %25 for 3 turns.
  
  After combat: TBD
