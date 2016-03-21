/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRCOMPOSITEDISPLAYFACTORY_H_
#define VRCOMPOSITEDISPLAYFACTORY_H_

#include "display/factories/VRDisplayFactory.h"
#include <vector>

namespace MinVR {

class VRCompositeDisplayFactory : public VRDisplayFactory {
public:
	VRCompositeDisplayFactory();
	virtual ~VRCompositeDisplayFactory();

	VRDisplay* create(VRDataIndex& config, const std::string nameSpace);
	void addFactory(VRDisplayFactory* factory);

private:
	std::vector<VRDisplayFactory*> m_displayFactories;
};

} /* namespace MinVR */

#endif /* VRCOMPOSITEDISPLAYFACTORY_H_ */