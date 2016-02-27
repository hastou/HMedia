#pragma once

#include <vector>


class HObject
{
public:
	HObject(HObject *parent = NULL);
	~HObject();
	void addChild(HObject *child);


private:
	std::vector<HObject*> children;
};

