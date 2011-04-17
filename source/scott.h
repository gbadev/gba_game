//speed
#define ZOMBIE_S 6
#define HEAL_S 5
#define RANGE_S 4
#define TANK_S 3
#define GRENADE_S 2

//range
#define ZOMBIE_M 8
#define HEAL_M 6
#define RANGE_M 8
#define TANK_M 5
#define GRENADE_M 6

//life
#define ZOMBIE_L 1
#define HEAL_L 50
#define RANGE_L 75
#define TANK_L 150
#define GRENADE_L 100

//power
#define ZOMBIE_P 10
#define HEAL_P 1
#define RANGE_P 1
#define TANK_P 1
#define GRENADE_P 1

//special
#define SPECIAL 1
#define IDLE 0

//status
#define UNDEAD 0
#define DEAD -1
#define ALIVE 1

//flipped
#define FLIPPED -1
#define NOT_FLIPPED 1

typedef struct turn_s{
	int index;
	int speed;
}t_s;

int turn[128];
int turn_copy[128];

int turn_counter(int t[], int t_c[], t_s s)
{
	if(s.index == 0)
	{
		t[0] = s.speed;
	}

}