#include "HatchRelatedAPIData.hpp"

#include "ACAPinc.h"

namespace PhotoshopLikePlans {

GS::Array<API_HatchType> GetHatchData ()
{
	GS::Array<API_Guid> hatchGuidList;
	hatchGuidList.Clear ();

	API_SelectionInfo    selectionInfo;
	GS::Array<API_Neig> selNeigs;
	if (ACAPI_Selection_Get (&selectionInfo, &selNeigs, true) == NoError && selectionInfo.typeID != API_SelEmpty) {
		for (auto& selNeig : selNeigs) {
			API_ElemTypeID	typeID;
			if (ACAPI_Goodies (APIAny_NeigIDToElemTypeID, &selNeig.neigID, &typeID) == NoError && typeID == API_HatchID) {
				hatchGuidList.Push (selNeig.guid);
			}
		}
	}

	GS::Array<API_HatchType> hatchList;

	for (auto& hatchGuid : hatchGuidList) {
		API_Element 		element = {};
		element.header.guid = hatchGuid;
		if (ACAPI_Element_Get (&element) == NoError) {
			hatchList.Push (element.hatch);
		}
	}

	return hatchList;
}

}

