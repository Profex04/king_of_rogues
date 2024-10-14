#ifdef _WIN32
#include <conio.h>
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif
#include <stdlib.h>
#define NOOFSPELLS 3
#define NOOFWEAPONS 5

struct spell_t {
    int id;
    int hp;
    char name[100];
    char description[100];
};
typedef struct spell_t spell;

struct weapon_t {
    int id;
    int hp;
    char name[100];
    char description[100];
};
typedef struct weapon_t weapon;

struct hero_t {
    int id;
    int hp;
    int coin;
    int weaponslot1;
    int weaponslot2;
    int spellslot1;
    int spellslot2;
    weapon *char_weapons;
    spell *char_spells;
    char name[100];
};
typedef struct hero_t hero;

struct monster_t {
    int id;
    int hp;
    int cutoff;
    char name[100];
};
typedef struct monster_t monster;
hero *heroes;
monster *monsters;
spell *spells;
weapon *weapons;

void attack(hero hero, monster monster, int roll, int weapon, int spell, int monstersturn) {
    int damage;
    if(monstersturn == 0) {
        if(spell == 0) damage = weapons[weapon].hp + roll;
        else damage = spells[spell].hp + roll;
        if(damage >= monster.cutoff) {
            monster.hp -= damage;
            printf("Damage Dealt=%d\nMonster's HP:%d\n",damage,monster.hp);
        }
        else printf("%s missed..\n", hero.name);
        if(roll == 1) {
            printf("%s hurt themself in confusion!", hero.name);
            hero.hp -= damage;
        }
        if(monster.hp < 10 && monster.hp > 0) printf("%s is at critical health..", monster.name);
        else if(monster.hp <= 0) printf("%s is dead.", monster.name);
    }
    else {
        damage = roll;
        //if(damage >= hero.armor) {
            hero.hp -= damage;
            printf("Damage Dealt=%d\nHero's HP:%d\n",damage,hero.hp);
        //}
        if(hero.hp < 20 && hero.hp > 0) printf("%s is at critical health..", hero.name);
        else if(hero.hp <= 0) printf("%s is dead.", hero.name);
    }
}

int read_herofile(const char *fname) {
  FILE *fp = fopen(fname, "r");
  if (!fp) {
    printf("not found %s\n", fname);
    return 0;
  }
  int num;
  fscanf(fp, "%d", &num); /* read the 1st line (# of data) */
  heroes = (hero*)malloc(num * sizeof(hero));
  for (int i = 0; i < num; i++) {
    heroes[i].char_weapons = (weapon*)malloc(2 * sizeof(weapon));
    heroes[i].char_spells = (spell*)malloc(2 * sizeof(spell));
    fscanf(fp, "%d %d %d %d %d %d %s", &heroes[i].hp,&heroes[i].coin,&heroes[i].weaponslot1,&heroes[i].weaponslot2,&heroes[i].spellslot1,&heroes[i].spellslot2,heroes[i].name);
    heroes[i].id = i;
    heroes[i].char_weapons[0] = weapons[heroes[i].weaponslot1];
    heroes[i].char_weapons[1] = weapons[heroes[i].weaponslot2];
    heroes[i].char_spells[0] = spells[heroes[i].spellslot1];
    heroes[i].char_spells[1] = spells[heroes[i].spellslot2];
  }
  fclose(fp);
  return num;
}

int read_monsterfile(const char *fname) {
  FILE *fp = fopen(fname, "r");
  if (!fp) {
    printf("not found %s\n", fname);
    return 0;
  }
  int num;
  fscanf(fp, "%d", &num); /* read the 1st line (# of data) */
  monsters = (monster*)malloc(num * sizeof(monster));
  for (int i = 0; i < num; i++) {
    fscanf(fp, "%d %d %s", &monsters[i].hp, &monsters[i].cutoff, monsters[i].name);
    monsters[i].id = i;
  }
  fclose(fp);
  return num;
}

void read_spellfile(const char *fname) {
  FILE *fp = fopen(fname, "r");
  if (!fp) {
    printf("not found %s\n", fname);
  }
  spells = (spell*)malloc(NOOFSPELLS * sizeof(spell));
  for (int i = 0; i < NOOFSPELLS; i++) {
    fscanf(fp, "%d %s", &spells[i].hp, spells[i].name);
    spells[i].id = i;
  }
  fclose(fp);
}

void read_weaponfile(const char *fname) {
  FILE *fp = fopen(fname, "r");
  if (!fp) {
    printf("not found %s\n", fname);
  }
  weapons = (weapon*)malloc(NOOFWEAPONS * sizeof(weapon));
  for (int i = 0; i < NOOFWEAPONS; i++) {
    fscanf(fp, "%d %s", &weapons[i].hp, weapons[i].name);
    weapons[i].id = i;
  }
  fclose(fp);
}

void print_spellslist(int detailed, hero hero) {
    int no_spells = sizeof(hero.char_spells) / sizeof(spell);
    printf("\n********************************\n");
    if(detailed == 1) {
        for (int i = 0; i < no_spells; i++) {
            printf("\nNo:%d HP:%d Name:%s\nDescription:%s\n", hero.char_spells[i].id, hero.char_spells[i].hp, hero.char_spells[i].name, hero.char_spells[i].description);
        }
    }
    else if(detailed == 0) {
        for (int i = 0; i < no_spells; i++) {
            printf("No:%d Name:%s\n", hero.char_spells[i].id, hero.char_spells[i].name);
        }
    }
    printf("********************************\n");
}

void print_weaponslist(int detailed, hero hero) {
    printf("\n********************************\n");
    int no_weapons = sizeof(hero.char_weapons) / sizeof(weapon);
    printf("%s no of weapons: %d", hero.name, no_weapons);
    if(detailed == 1) {
        for (int i = 0; i < no_weapons; i++) {
            printf("\nNo:%d HP:%d Name:%s\nDescription:%s\n", hero.char_weapons[i].id, hero.char_weapons[i].hp, hero.char_weapons[i].name, hero.char_weapons[i].description);
        }
    }
    else if(detailed == 0) {
        for (int i = 0; i < no_weapons; i++) {
            printf("No:%d Name:%s\n", hero.char_weapons[i].id, hero.char_weapons[i].name);
        }
    }
    printf("********************************\n");
}

void print_monsterslist(int detailed, int noOfMonsters) {
    printf("\n********************************\n");
    if(detailed == 1) {
        for (int i = 0; i < noOfMonsters; i++) {
            printf("\nNo:%d HP:%d Name:%s\n", monsters[i].id, monsters[i].hp, monsters[i].name);
        }
    }
    else if(detailed == 0) {
        for (int i = 0; i < noOfMonsters; i++) {
            printf("No:%d Name:%s\n", monsters[i].id, monsters[i].name);
        }
    }
    printf("********************************\n");
}

void print_playerslist(int detailed, int noOfPlayers) {
    printf("\n********************************\n");
    if(detailed == 1) {
        for (int i = 0; i < noOfPlayers; i++) {
            printf("\nNo:%d HP:%d Doubloons:%d Name:%s\n", heroes[i].id, heroes[i].hp, heroes[i].coin, heroes[i].name);
            //printf("********************************\n");
            printf("Weapon Slot 1:%s\nWeapon Slot 2:%s\n", weapons[heroes[i].weaponslot1].name,weapons[heroes[i].weaponslot2].name);
            printf("Spell Slot 1:%s\nSpell Slot 2:%s\n", spells[heroes[i].spellslot1].name,spells[heroes[i].spellslot2].name);            
            //printf("********************************\n");
        }
    }
    else if(detailed == 0) {
        for (int i = 0; i < noOfPlayers; i++) {
            printf("No:%d Name:%s\n", heroes[i].id, heroes[i].name);
        }
    }
    printf("********************************\n");
}

void write_herofile(const char *fname, int noOfPlayers) {
  FILE *fp = fopen(fname, "w");
  if (!fp) {
    printf("not found %s\n", fname);
  }
  fprintf(fp, "%d\n", noOfPlayers);
  for (int i = 0; i < noOfPlayers; i++) {
    fprintf(fp, "%d %d %d %d %d %d %s\n", heroes[i].hp, heroes[i].coin, heroes[i].weaponslot1, heroes[i].weaponslot2, heroes[i].spellslot1, heroes[i].spellslot2, heroes[i].name);
  }
  fclose(fp);
}

int menu() {
    int choice;
    printf("\n\n***Welcome Dungeon Master***\n1. Hero Attack\n2. Monster Attack\n3. Finances\n4. Player Stats\n5. Set New Monster\n6. Exit\n");
	scanf("%d", &choice);
	return choice;
}

void asciiart();

int main()
{
    read_weaponfile("weapons.txt");
    read_spellfile("spells.txt");
    int noOfPlayers = read_herofile("heroes.txt");
    int noOfMonsters = read_monsterfile("monsters.txt");
    int detailed;
    int choice; 
    int monsterno=0;
    asciiart();
    while(1) {
            choice = menu();
            if(choice == 1) {
	            int playerno,roll; 
	            int weapon, spell = 0;
	            print_playerslist(detailed=0, noOfPlayers);
	            printf("Player No= ");
	            scanf("%d", &playerno);
	            printf("Roll= ");
	            scanf("%d", &roll);
	            if(heroes[playerno].weaponslot1 != 0 && heroes[playerno].weaponslot2 != 0) {
	                print_weaponslist(detailed=0, heroes[playerno]);
	                printf("Select Weapon= ");
	                scanf("%d", &weapon);
	            }
	            if(heroes[playerno].spellslot1 != 0 || heroes[playerno].spellslot2 != 0) {
	                print_spellslist(detailed=0, heroes[playerno]);
	                printf("Select Spell= ");
	                scanf("%d", &spell);
	            }
	            attack(heroes[playerno],monsters[monsterno],roll, weapon, spell, 0);
	        }
	        else if(choice == 2) {
	            int playerno,roll; 
	            print_playerslist(detailed=0, noOfPlayers);
	            printf("Player No= ");
	            scanf("%d", &playerno);
	            printf("Monster Rolls= ");
	            scanf("%d", &roll);
	            attack(heroes[playerno],monsters[monsterno],roll, 0, 0, 1);
	        }
	        else if(choice == 3) {
	            int playerno, coin;
	            print_playerslist(detailed=0, noOfPlayers);
	            printf("Player No= ");
	            scanf("%d", &playerno);
	            printf("Doubloons [Enter negative or positive number]= ");
	            scanf("%d", &coin);
	            heroes[playerno].coin += coin;
	        }
	        else if(choice == 4) {
	            print_playerslist(detailed=1, noOfPlayers);
	        }
	        else if(choice == 5) {
	            print_monsterslist(detailed=0, noOfMonsters);
	            printf("Monster No= ");
	            scanf("%d", &monsterno);
	        }
	        else if(choice == 6) break;
	        else ;//clrscr(); 
        }
        //write_herofile("heroesupdated.txt", noOfPlayers);
        return 0;
}

void asciiart() {
printf("                            ,--.\n                          {    }\n                          K,   }\n                         /  `Y`\n                    _   /   /\n                   {_'-K.__/\n                     `/-.__L._\n                     /  ' /`\\_}\n                    /  ' /\n            ____   /  ' /\n     ,-'~~~~    ~~/  ' /_\n   ,'             ``~~~%%%%%%%%',\n  (                     %%  Y\n {                      %%%% I\n{      -                 %%  `.\n|       ',                %%  )\n|        |   ,..__      __. Y\n|    .,_./  Y ' / ^Y   J   )|\n\\           |' /   |   |   ||\n \\          L_/    . _ (_,.'(\n  \\,   ,      ^^^^' / |      )\n    \\_  \\          /,L]     /\n      '-_`-,       ` `   ./`\n         `-(_            )\n             ^^\\..___,.--``)\n");
}