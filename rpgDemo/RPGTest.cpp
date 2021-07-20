//melvin su
//RPGTest

#include <iostream>
#include <random>
#include <ctime>
#include <iomanip> 

#include <vector>

using namespace std;

//damage variance is between variance_percentage and -(variance_percentage)
double variance_percentage = 0.25;
uniform_real_distribution<double> unif(-variance_percentage, variance_percentage);

struct effect_template {
private:
	string name;
public:
	effect_template() { name = "No Name"; }
	effect_template(string name) { this->name = name; }
	string getName() { return name; }
	void setName(string name) { this->name = name; }
};

//statuses to implement: 1.Burn - deal 5% HP, increase by another 5% if attack action is used, last 3 turns
//2. Frost - reduce atk/mag by %50, last 1 turn
//3. Shock - reduce DEF by %25, increase MAG by %25, infinite duration
//4. Guard - reduce final damage from attacks by 90%, last 1 turn
//5. Protection - reduce final damage from attacks and skills by 10%, prevent negative status, last 3 turns

//damaging, stat-change, special

struct status : public effect_template {
private:
	int duration = 0;
public:
	status() { setName("No Name"), duration = 99; }
	status(string name, int duration) { setName(name); this->duration = duration; }
	void effect() {};
};

status burn_st("Burn", 3), frost_st("Frost", 1), shock_st("Shock", 99), guard_st("Guard", 1), protection_st("Protection", 3);
vector <status> global_status_list = { burn_st, frost_st, shock_st, guard_st, protection_st };

struct skill : public effect_template {
private:
	pair<double, int> skill_target;
	vector <pair<status, int>> status_target;
public:
	skill(string name) : effect_template(name) {}
	skill(string name, pair<double, int>, vector <pair<status, int>>) { setName(name); this->skill_target = skill_target; this->status_target = status_target; }
};

//skills to implement: 1. Magic Shot
//2. Flame Blast
//3. Frost Spines
//4. Shockwave
//5. Guard
//6. Heal Up
//7. Protection
//8. ATK UP
//9. DEF UP

//damaging, stat-change, special

skill magic_shot_sk("Magic Shot"),
flame_blast_sk("Flame Blast"),
frost_spines_sk("Frost Spines"),
shockwave_sk("Shockwave"),
guard_sk("Guard"),
heal_up_sk("Heal Up"),
protection_sk("Protection"),
atk_up_sk("ATK Up"),
def_up_sk("DEF Up");

vector <skill> global_skill_list = { magic_shot_sk, flame_blast_sk, frost_spines_sk, shockwave_sk, guard_sk, heal_up_sk,protection_sk, atk_up_sk, def_up_sk };
uniform_int_distribution<int> skill_num_distributor(0, global_skill_list.size() - 1);

struct character {
private:
	string name;
	double hp = 100.0;
	double max_hp = 100.0;
	double base_atk = 10.0;
	double base_def = 5.0;

	double mp = 50;
	double max_mp = 50;
	double base_mag = 5;

	vector <skill> skill_list;
	int skill_count_limit = 3;
public:
	character(string name) { this->name = name; }
	double getCurrentHp() { return hp; }
	double getAtk() { return base_atk; }
	double getDef() { return base_def; }
	string getName() { return name; }
	void setHp(double hp) { this->hp = hp; }

	void addSkill(skill s) { skill_list.push_back(s); }
	void printSkills() { for (int i = 0; i < skill_list.size(); i++) { cout << skill_list[i].getName() << endl; } }
	void distribute_skills()
	{
		while (skill_list.size() < 3)
		{
			random_device r;
			default_random_engine re(r());;
			int iter = skill_num_distributor(re);
			skill_list.push_back(global_skill_list[iter]);
		}
	}
};

double attackCal(double atk, double def)
{
	random_device r;
	default_random_engine re(r());
	double variance = unif(re) + 1.0;
	double mod_atk = atk * variance;
	double damage = mod_atk - def;
	cout << "atk:" << atk << "\tvariance:" << variance << "\tmod_atk:" << mod_atk << "\tdef:" << def  << "\tfinal damage:" << damage << endl;
	if (damage > 0) {
		return damage;
	}
	else { return 0; }
}

double magCal(double mag1, double skill_pow, double mag2, double def2)
{
	double damage = (mag1 * skill_pow) - (mag2/2 + def2/2);
	if (damage > 0) {
		return damage;
	}
	else { return 0; }
}

//pre-battle effects, damage calculations, post-battle effects
void battle_phase(character p1, character p2) {
	;
}

void combat(character p1, character p2) {
	if (p1.getCurrentHp() <= 0 || p2.getCurrentHp() <= 0) {
		return;
	}
	while (p1.getCurrentHp() > 0 && p2.getCurrentHp() > 0) {
		//p1 attacks p2
		p2.setHp(p2.getCurrentHp() - attackCal(p1.getAtk(), p2.getDef()));
		cout << p2.getName() << " current hp is " << p2.getCurrentHp() << endl;
		if (p2.getCurrentHp() >= 0) {
			p1.setHp(p1.getCurrentHp() - attackCal(p2.getAtk(), p1.getDef()));
			cout << p1.getName() << " current hp is " << p1.getCurrentHp() << endl;
		}
		cin.ignore();
	}
}

int main()
{
	srand(time(NULL));

	character sheep("Warrior1");
	character wolf("Warrior2");
	
	//sheep.distribute_skills();
	//wolf.distribute_skills();

	//sheep.printSkills();
	//wolf.printSkills();

	combat(sheep, wolf);
	return 0;
}