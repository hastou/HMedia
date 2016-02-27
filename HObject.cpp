#include "stdafx.h"
#include "HObject.h"


HObject::HObject(HObject * parent)
{
	if (parent)
	{
		parent->addChild(this);
	}
}

HObject::~HObject()
{
	for (int i = 0; i < this->children.size(); i++)
	{
		if (this->children[i])
		{
			delete children[i];
		}
	}
}

void HObject::addChild(HObject * child)
{
	this->children.push_back(child);
}

