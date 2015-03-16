#ifndef SOLIDOBJECT_H
#define SOLIDOBJECT_H

class SolidObject
{
public:
	virtual ~SolidObject() {}
	
	virtual Vec3 pos() const = 0;
	virtual float boundingRadius() const = 0;
};

#endif
