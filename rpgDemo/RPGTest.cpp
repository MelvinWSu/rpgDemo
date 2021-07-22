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
uniform_real_distribution<double> skill_selection(-variance_percentage, variance_percentage);
uniform_real_distribution<double> choice(0, 3);

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
	status() { setName("No Status Name"), duration = 99; }
	status(string name, int duration = 99) { setName(name); this->duration = duration; }
	void effect() {};
};

status empty_st("No Status Name",99), burn_st("Burn", 3), frost_st("Frost", 1), shock_st("Shock", 99), guard_st("Guard", 1), protection_st("Protection", 3);
vector <status> global_status_list = { burn_st, frost_st, shock_st, guard_st, protection_st };

//targeting: 0 = self, 1 = other

struct skill : public effect_template {
private:
	pair<double, int> skill_target;
	vector <pair<status, int>> status_targets;
public:
	skill(string name) : effect_template(name) {}
	skill(string name, pair<double, int> skill_target) { setName(name); this->skill_target = skill_target; }
	skill(string name, vector <pair<status, int>> status_targets) { setName(name); this->status_targets = status_targets; }
	skill(string name, pair<double, int> skill_target, vector <pair<status, int>> status_targets) {
		setName(name); this->skill_target = skill_target; this->status_targets = status_targets; 
	}
	
	double getSkillTarget_damage() { return skill_target.first; }
	int getSkillTarget_target() { return skill_target.second; }
	vector <pair<status, int>> getStatusTargets() { return status_targets; }
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

skill magic_shot_sk("Magic Shot", {1.5, 1}),
flame_blast_sk("Flame Blast", { 0.75, 1 }, vector<pair<status, int>>(1, make_pair(burn_st, 1))),
frost_spines_sk("Frost Spines", { 1.0, 1 }, vector<pair<status, int>>(1, make_pair(frost_st, 1))),
shockwave_sk("Shockwave", { 1.0, 1 }, vector<pair<status, int>>(1, make_pair(shock_st, 1))),
guard_sk("Guard", vector<pair<status, int>>(1, make_pair(guard_st, 0))),
heal_up_sk("Heal Up", { 1.5, 0 }),
protection_sk("Protection", vector<pair<status, int>>(1, make_pair(protection_st, 0))),
atk_up_sk("ATK Up"),
def_up_sk("DEF Up");

//vector <skill> global_skill_list = { magic_shot_sk, flame_blast_sk, frost_spines_sk, shockwave_sk, guard_sk, heal_up_sk, protection_sk, atk_up_sk, def_up_sk };
vector <skill> global_skill_list = { magic_shot_sk, flame_blast_sk, frost_spines_sk, shockwave_sk, heal_up_sk };
uniform_int_distribution<int> skill_num_distributor(0, global_skill_list.size() - 1);

struct character {
private:
	string name;
	double hp = 100.0;
	double max_hp = 100.0;
	double base_atk = 12.0;
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

	double getMag() { return base_mag; }
	double getMaxHP() { return max_hp; }
	double getMp() { return mp; }
	double getMaxMp() { return max_mp; }
	void addSkill(skill s) { skill_list.push_back(s); }
	vector <skill> getSkills() { return skill_list; }
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
	double variance = skill_selection(re) + 1.0;
	double mod_atk = atk * variance;
	double damage = mod_atk - def;
	//cout << "atk:" << atk << "\tvariance:" << variance << "\tmod_atk:" << mod_atk << "\tdef:" << def  << "\tfinal damage:" << damage << endl;
	//cout << "attack damage" << endl;
	if (damage > 0) {
		return damage;
	}
	else { return 0; }
}

double magCal(double mag1, double skill_power, double mag2 = 0, double def2 = 0)
{
	double damage = (mag1 * skill_power) - (mag2/2);
	//cout << "magic damage:" << damage << endl;
	if (damage > 0) {
		return damage;
	}
	else { return 0; }
}

void battle_phase(character p1, character p2) {
	;
}

//todo: split up code into another function
void turn(character p1, character p2) {
	random_device r;
	default_random_engine re(r());
	
	while (p1.getCurrentHp() > 0 && p2.getCurrentHp() > 0) {
		//p1 acts first against p2
		int use = choice(re);
		double damage;
		//use = 0 indicates normal attack, a non-zero value indicates a skill use
		if (use == 0) {
			cout << p1.getName() << " attacks " << endl;
			damage = attackCal(p1.getAtk(), p2.getDef());
			p2.setHp(p2.getCurrentHp() - damage);
			cout << p1.getName() << " hits for " << damage << endl << p2.getName()  << " current hp is " << p2.getCurrentHp() << endl;
		}
		else {
			int target = p1.getSkills()[use - 1].getSkillTarget_target();
			double power = p1.getSkills()[use - 1].getSkillTarget_damage();
			string name = p1.getSkills()[use - 1].getName();

			//target indicates if spell selected is damaging to opponent or helpful to user
			cout << p1.getName() << " used " << name << endl;
			if (target == 1) {
				damage = magCal(p1.getMag(), power, p2.getMag(), p2.getDef());
				p2.setHp(p2.getCurrentHp() - damage);
				cout << p1.getName() << " hits for " << damage << endl << p2.getName() << " current hp is " << p2.getCurrentHp() << endl;
			}
			else if (target == 0 && power != 0) {
				damage = magCal(p1.getMag(), power);
				if (damage + p1.getCurrentHp() > p1.getMaxHP()) {
					damage = p1.getMaxHP() - p1.getCurrentHp();
				}
				p1.setHp(p1.getCurrentHp() + damage);
				cout << p1.getName() << " has healed by " << damage << " to " << p1.getCurrentHp() << endl;
			}
		}
		if (p2.getCurrentHp() <= 0) {
			cout << p2.getName() << " is defeated!" << endl;
			return;
		}

		cout << endl;
		use = choice(re);
		//use = 0 indicates normal attack, a non-zero value indicates a skill use
		if (use == 0) {
			cout << p2.getName() << " attacks " << endl;
			damage = attackCal(p2.getAtk(), p1.getDef());
			p1.setHp(p1.getCurrentHp() - damage);
			cout << p2.getName() << " hits for " << damage << endl << p1.getName() << " current hp is " << p1.getCurrentHp() << endl;
		}
		else {
			int target = p2.getSkills()[use - 1].getSkillTarget_target();
			double power = p2.getSkills()[use - 1].getSkillTarget_damage();
			string name = p2.getSkills()[use - 1].getName();

			//target indicates if spell selected is damaging to opponent or helpful to user
			cout << p2.getName() << " used " << name << endl;
			if (target == 1) {
				damage = magCal(p2.getMag(), power, p1.getMag(), p1.getDef());
				p1.setHp(p1.getCurrentHp() - damage);
				cout << p2.getName() << " hits for " << damage << endl << p1.getName() << " current hp is " << p1.getCurrentHp() << endl;
			}
			else if (target == 0 && power != 0) {
				damage = magCal(p2.getMag(), power);
				if (damage + p2.getCurrentHp() > p2.getMaxHP()) {
					damage = p2.getMaxHP() - p2.getCurrentHp();
				}
				p2.setHp(p2.getCurrentHp() + damage);
				cout << p2.getName() << " has healed by " << damage << " to " << p2.getCurrentHp() << endl;
			}
		}
		if (p1.getCurrentHp() <= 0) {
			cout << p1.getName() << " is defeated!" << endl;
			return;
		}
		cin.ignore();
	}
}

int main()
{
	srand(time(NULL));

	character person1("Warrior");
	character person2("Soldier");
	
	person1.distribute_skills();
	person2.distribute_skills();

	turn(person1, person2);
	return 0;
}