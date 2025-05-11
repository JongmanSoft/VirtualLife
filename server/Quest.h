#pragma once
class Quest
{
	unsigned short giver_id;
	unsigned short type; // ¹øÈ£

public:
	Quest() {};
	~Quest() {};
	Quest(unsigned short gid, unsigned short n) : giver_id{ gid }, type{ n } {}


	Quest& GetQuestInfo() { return *this; }
	const unsigned short GetGID() { return giver_id; }
	const unsigned short GetNUM() { return type; }

};

