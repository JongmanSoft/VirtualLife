#pragma once

class Player; 
class Party
{
private:
	std::vector<Player*> members;
	std::mutex party_lock;

public:
	std::vector<Player*>& get_members() { return members; }
	int get_member_count() { return members.size(); }
	void add_member(Player* player)
	{
		std::lock_guard<std::mutex> lock(party_lock);
		members.push_back(player);
	}
};

