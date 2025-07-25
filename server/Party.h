#pragma once

class Player; 
class Party
{
private:
	std::string partyID;
	std::vector<Player*> members;
	std::mutex party_lock;

public:
	void set_partyID(std::string& str) { partyID = str; }
	std::string get_partyID() { return partyID; }
	std::vector<Player*>& get_members() { return members; }
	int get_member_count() { return static_cast<int>(members.size()); }
	void add_member(Player* player)
	{
		std::lock_guard<std::mutex> lock(party_lock);
		members.push_back(player);
	}
	void remove_member(Player* player)
	{
		std::lock_guard<std::mutex> lock(party_lock);
		auto it = std::remove(members.begin(), members.end(), player);
		if (it != members.end()) {
			members.erase(it, members.end());
		}
	}	
};

