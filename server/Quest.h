#pragma once
class Quest
{
	unsigned short giver_id;
	unsigned short type; // ¹øÈ£

public:
	Quest();
	~Quest();
	Quest(unsigned short gid, unsigned short n) : giver_id{ gid }, type{ n } {}


	Quest& GetQuestInfo() { return *this; }

};

