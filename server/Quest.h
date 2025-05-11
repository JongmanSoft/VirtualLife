#pragma once
class Quest
{
	short giver_id;
	short type; // ¹øÈ£

public:
	Quest() {};
	~Quest() {};
	Quest(short gid, short n) : giver_id{ gid }, type{ n } {}


	Quest& GetQuestInfo() { return *this; }
	const short GetGID() { return giver_id; }
	const short GetNUM() { return type; }

};

