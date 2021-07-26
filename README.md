# rpgDemo

Goal: Implementation of an automatic turn-based RPG mechanics including multiple actions during combat, leveling up, and equipment. See Godville's combat for example.


Features:
  Character Generation: Random values of HP (health points), MP (magic points), ATK (attack), MAG(magic attack), and DEF (defense). Random amount of skills and skill slots a character may have. Randomly generated name from a list.
  
  
  Basic Combat: Each turn a character will randomly either attack or use an active skill. Characters alternate turns. Random chance determines which character starts. Battle ends when a character's HP reaches 0. 
  
  Attack: Deals damage based off character's ATK and other character's DEF.
  
  Active Skills: Damaging magic skills, healing skills, skills that give buffs or debuffs. Uses MP. 
  
    Examples: A fire spell that deals damage based of MAG and inflicts Burn to the other character.
    
    A healing spell that restores the user's lost health based of MAG.
    
   MAG based damage ignore DEF stat of other character.
    
  Passive Skills: Affect stats or give buffs. May either active conditionally or at the start of the battle. No MP used.
  
    Example: An increase in HP by +25.
    
    When damaged by an active skill, restore HP by 5% of max.
  
  Statuses: Applies an effect that impacts the character in varying ways. May have a duration. Current duration is reduced when each character ends their own turn. 
  
    Example: Burn: Deal 5% of max HP at the end of each turn. For each attack used, add an additional +5% for Burn. Last 3 turns.
  
    Muscle: An increase to ATK by %25 of the base power for 3 turns.
    
    Rage: Force the next turn's action to be attack. Increase ATK by %75 of the base power, set DEF to 0. 
    
    
  Equipment: TBD
  
  Outside combat events: TBD
  
  Game/ Level Progression: TBD
