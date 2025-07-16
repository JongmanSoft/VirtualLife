#pragma once


class ObjectBase
{
protected:
	PlayerInfo pinfo; // obj id, 위치/회전정보

public:
	ObjectBase() {};
	~ObjectBase() {};
};

class HumanObject : public ObjectBase
{
protected:
	std::wstring name = L""; // 이름
	Customizing custom; // 커스텀

public:
	HumanObject() {};
	~HumanObject() {};
};